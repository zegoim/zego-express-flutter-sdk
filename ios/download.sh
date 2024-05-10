#!/bin/bash

echo "[ZEGO][PLUGIN] Download native dependency"

WORKSPACE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $WORKSPACE

if [ -f $WORKSPACE/../.debugging ]; then
  echo "[ZEGO][PLUGIN] Found the .debugging file in the project root, no need to download dependency, exit!"
  exit 0
fi

if [[ $DEPS == http* ]]; then
  DEPSURL=$DEPS
  echo "[ZEGO][PLUGIN][DEV] 'DEPS' env was found: $DEPSURL"
else
  DEPSURL=$(grep 'ios:' $WORKSPACE/../DEPS.yaml | cut -d ' ' -f 2)
fi

DEPSVER=$(echo $DEPSURL | cut -d'?' -f2 | cut -d'=' -f2)
echo "[ZEGO][PLUGIN] Native version: $DEPSVER"

LIBSDIR=$WORKSPACE/libs
mkdir -p $LIBSDIR

if [ -f $LIBSDIR/VERSION.txt ] && [ -f $LIBSDIR/ZegoExpressEngine.xcframework/Info.plist ]; then
  VERSION=$(head -n 1 "$LIBSDIR/VERSION.txt" | tr -d '\n')
  if [ "$VERSION" = "$DEPSVER" ]; then
    echo "[ZEGO][PLUGIN] The specified version SDK already exists!"
    exit 0
  else
    echo "[ZEGO][PLUGIN] SDK was found in cache, but the version ($VERSION) does not match the version specified in DEPS, overwrite!"
  fi
fi

curl -s "$DEPSURL" --output $LIBSDIR/sdk.zip
unzip -q -o $LIBSDIR/sdk.zip -d $LIBSDIR

#rm -rf $LIBSDIR/ZegoExpressEngine.xcframework
#mv $LIBSDIR/release/Library/ZegoExpressEngine.xcframework $LIBSDIR

rm -rf $LIBSDIR/*.xcframework
for file in $LIBSDIR/release/Library/*.xcframework
do
    mv "$file" $LIBSDIR
done

mv -f $LIBSDIR/release/VERSION.txt $LIBSDIR

rm -rf $LIBSDIR/release
rm -f $LIBSDIR/sdk.zip

echo "[ZEGO][PLUGIN] Done!"
exit 0
