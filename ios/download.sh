#!/bin/bash

echo "[*] Downloading native dependency"

WORKSPACE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $WORKSPACE

DEPSURL=$(<$WORKSPACE/DEPS)
DEPSVER=$(echo $DEPSURL | cut -d'?' -f2 | cut -d'=' -f2)

LIBSDIR=$WORKSPACE/libs
mkdir -p $LIBSDIR

if [ -f $LIBSDIR/VERSION.txt ] && [ -f $LIBSDIR/ZegoExpressEngine.xcframework/Info.plist ]; then
  VERSION=$(head -n 1 "$LIBSDIR/VERSION.txt" | tr -d '\n')
  if [ "$VERSION" = "$DEPSVER" ]; then
    echo "[*] The specific version SDK exists, no need to download it again, exit!"
    exit 0
  else
    echo "[*] SDK found, but the version $VERSION does not match the DEPS $DEPSVER, overwrite it!"
  fi
fi

curl -s "$DEPSURL" --output $LIBSDIR/sdk.zip
unzip -q -o $LIBSDIR/sdk.zip -d $LIBSDIR

rm -rf $LIBSDIR/ZegoExpressEngine.xcframework
mv $LIBSDIR/release/Library/ZegoExpressEngine.xcframework $LIBSDIR
mv -f $LIBSDIR/release/VERSION.txt $LIBSDIR

rm -rf $LIBSDIR/release
rm -f $LIBSDIR/sdk.zip

echo "[*] Done!"
exit 0
