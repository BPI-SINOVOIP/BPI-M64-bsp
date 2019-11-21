#!/bin/bash

die() {
        echo "$*" >&2
        exit 1
}

[ -s "./env.sh" ] || die "please run ./configure first."

set -e

. ./env.sh

DTB=$TOPDIR/bootloader
ENV=$TOPDIR/sunxi-pack/allwinner/${MACH}/configs/${BOARD}
IMAGE=$TOPDIR/linux-sunxi/arch/arm64/boot
MODULES=$TOPDIR/linux-sunxi/output/lib/modules
HEADER=$TOPDIR/linux-sunxi/output/usr/src
U=$TOPDIR/SD/${TARGET_PRODUCT}/100MB
B=$TOPDIR/SD/${TARGET_PRODUCT}/BPI-BOOT
R=$TOPDIR/SD/${TARGET_PRODUCT}/BPI-ROOT

if [ -d $TOPDIR/SD ]; then
  rm -rf $TOPDIR/SD
fi

mkdir -p $U
mkdir -p $B
mkdir -p $R

pack_bootloader()
{
  echo "pack bootloader"

  for files in $(ls -f $DTB/*.img)
  do
	echo "gzip ${files}"
	#gzip -k ${files} #fix for docker run --privileged -d -p 2222:22 -v /media:/media sinovoip/bpi-build:ubuntu12.04
	gzip ${files}
	mv ${files}.gz $U/  
  done
}

pack_boot()
{
  echo "pack boot"

  mkdir -p $B/bananapi/${TARGET_PRODUCT}/linux
  cp -a $ENV/* $B/bananapi/${TARGET_PRODUCT}/linux/
  cp -a $IMAGE/Image $B/bananapi/${TARGET_PRODUCT}/linux/Image

  # copy each board dtb to target dir
  if [ $TARGET_PRODUCT = "bpi-m64" ]; then
    for files in $(ls -f $DTB/*.dtb)
    do
	name=${files%.*}
	type=${name##*-}
  	cp -a ${files} $B/bananapi/${TARGET_PRODUCT}/linux/${type}/
    done
  else
	cp -a $ENV/* $B/bananapi/${TARGET_PRODUCT}/linux/
  fi
}

pack_root()
{
  echo "pack root"

  mkdir -p $R/usr/lib/u-boot/bananapi/${TARGET_PRODUCT}
  cp -a $U/*.gz $R/usr/lib/u-boot/bananapi/${TARGET_PRODUCT}/

  rm -rf $R/lib/modules
  mkdir -p $R/lib/modules
  cp -a $MODULES/3.10.105-${BOARD}-Kernel $R/lib/modules/

  rm -rf $R/usr/src
  mkdir -p $R/usr/src
  cp -a $HEADER/linux-headers-3.10.105-${BOARD}-Kernel $R/usr/src/
}

# pack 100M
pack_bootloader

# pack BPI_BOOT
pack_boot

# pack BPI_ROOT
pack_root

(cd $B ; tar czvf $TOPDIR/SD/${TARGET_PRODUCT}/BPI-BOOT-${TARGET_PRODUCT}.tgz .)
(cd $R ; tar czvf $TOPDIR/SD/${TARGET_PRODUCT}/3.10.105-${BOARD}-Kernel.tgz lib/modules)
(cd $R ; tar czvf $TOPDIR/SD/${TARGET_PRODUCT}/linux-headers-3.10.105-${BOARD}-Kernel.tgz usr/src)
(cd $R ; tar czvf $TOPDIR/SD/${TARGET_PRODUCT}/BOOTLOADER-${TARGET_PRODUCT}.tgz usr/lib/u-boot/bananapi)

echo "pack finish"
