from __future__ import print_function
import os
import json
import sys
import shutil
import zipfile

if (sys.version_info.major == 3):
    import urllib.request
else:
    import urllib


THIS_SCRIPT_PATH = os.path.dirname(os.path.realpath(__file__))

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

    oss_url = 'https://storage.zego.im/rtc/ZegoExpressVideo/win/{}/ZegoExpressVideo-win-shared-cpp.zip'.format(sdk_version)
    artifact_name = oss_url.split('/')[-1]
    artifact_path = os.path.join(deps_path, artifact_name)

    print('Download sdk from {}'.format(oss_url))
    if (sys.version_info.major == 3):
        urllib.request.urlretrieve(oss_url, artifact_path)
    else:
        urllib.urlretrieve(oss_url, artifact_path)

    tmp_dst_unzip_folder = os.path.join(deps_path, '__tmp__')
    if os.path.exists(tmp_dst_unzip_folder):
        shutil.rmtree(tmp_dst_unzip_folder, ignore_errors=True)

    with zipfile.ZipFile(artifact_path, 'r') as f:
        print('unzip sdk')
        f.extractall(tmp_dst_unzip_folder)

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














