#!/usr/bin/env bash

PWD=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PROJ_ROOT=$(dirname "$PWD")

IOS_IMPL_INTERNAL_DIR=$PROJ_ROOT/ios/Classes/internal
MAC_IMPL_INTERNAL_DIR=$PROJ_ROOT/macos/Classes/internal

if [ -d "$MAC_IMPL_INTERNAL_DIR" ]; then
  echo "[*] Remove '$MAC_IMPL_INTERNAL_DIR'"
  rm -rf $MAC_IMPL_INTERNAL_DIR
fi

echo "[*] Copy '$IOS_IMPL_INTERNAL_DIR' to '$MAC_IMPL_INTERNAL_DIR'"
cp -r $IOS_IMPL_INTERNAL_DIR $MAC_IMPL_INTERNAL_DIR
