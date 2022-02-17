import os
import json
import sys
import shutil
import urllib.request
import zipfile
import tarfile
import subprocess
import ssl

THIS_SCRIPT_PATH = os.path.dirname(os.path.realpath(__file__))


def unzip_file(src_zip_file: str, dst_folder: str):
    if src_zip_file.endswith('.tar') or src_zip_file.endswith('.gz'):
        with tarfile.open(src_zip_file, 'r:gz') as f:
            f.extractall(dst_folder)
    elif src_zip_file.endswith('.zip'):
        if sys.platform == 'win32':
            with zipfile.ZipFile(src_zip_file, 'r') as f:
                f.extractall(dst_folder)
        else:
            subprocess.check_call(['unzip', '-o', '-q', src_zip_file, '-d', dst_folder])


if __name__ == '__main__':

    deps_path = os.path.join(THIS_SCRIPT_PATH, 'deps')
    dst_include_path = os.path.join(deps_path, 'include')
    dst_libs_path = os.path.join(deps_path, 'lib')

    if os.path.exists(dst_include_path) and os.path.exists(dst_libs_path):
        print('SDK has exists, ignore download')
        exit(0)

    try:
        with open(os.path.join(deps_path, 'deps.json')) as json_reader:
            json_str = json_reader.read()
            json_config = json.loads(json_str)

            sdk_version = json_config['version']

    except Exception as e:
        raise Exception("No deps json, throw error")

    oss_url = 'https://storage.zego.im/express/video/windows/zego-express-video-windows-{}.zip'.format(sdk_version)
    artifact_name = oss_url.split('/')[-1]

    request = urllib.request.Request(oss_url)
    print('\n --> Request: "{}"'.format(oss_url))
    context = ssl._create_unverified_context()
    u = urllib.request.urlopen(request, context=context)
    print(' <-- Response: "{}"'.format(u.status))

    artifact_path = os.path.join(deps_path, artifact_name)
    with open(artifact_path, 'wb') as fw:
        fw.write(u.read())

    tmp_dst_unzip_folder = os.path.join(deps_path, '__tmp__')
    if os.path.exists(tmp_dst_unzip_folder):
        shutil.rmtree(tmp_dst_unzip_folder, ignore_errors=True)

    unzip_file(artifact_path, tmp_dst_unzip_folder)

    for folder in os.listdir(tmp_dst_unzip_folder):
        product_folder = os.path.join(tmp_dst_unzip_folder, folder)
        for f in os.listdir(product_folder):
            if os.path.isdir(os.path.join(product_folder, f)):
                shutil.copytree(os.path.join(product_folder, f), os.path.join(deps_path, f))
            if os.path.isfile(os.path.join(product_folder, f)):
                shutil.copyfile(os.path.join(product_folder, f), os.path.join(deps_path, f))
    print("Download SDK success")

    shutil.rmtree(tmp_dst_unzip_folder, ignore_errors=True)
    os.remove(artifact_path)














