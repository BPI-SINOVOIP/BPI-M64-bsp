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
	gzip -k ${files}
	mv ${files}.gz $U/  
  done
}

pack_boot()
{
  echo "pack boot"

  mkdir -p $B/bananapi/${TARGET_PRODUCT}/linux
  cp -a $DTB/*.dtb $B/bananapi/${TARGET_PRODUCT}/linux/
  cp -a $ENV/* $B/bananapi/${TARGET_PRODUCT}/linux/
  cp -a $IMAGE/Image $B/bananapi/${TARGET_PRODUCT}/linux/Image
}

pack_root()
{
  echo "pack root"

  mkdir -p $R/usr/lib/u-boot/bananapi/${TARGET_PRODUCT}
  cp -a $U/*.gz $R/usr/lib/u-boot/bananapi/${TARGET_PRODUCT}/
  rm -rf $R/lib/modules
  mkdir -p $R/lib/modules
  cp -a $MODULES/3.10.105-${BOARD}-Kernel $R/lib/modules/
}

# pack 100M
pack_bootloader

# pack BPI_BOOT
pack_boot

# pack BPI_ROOT
pack_root

(cd $B ; tar czvf $TOPDIR/SD/${TARGET_PRODUCT}/BPI-BOOT-${TARGET_PRODUCT}.tgz .)
(cd $R ; tar czvf $TOPDIR/SD/${TARGET_PRODUCT}/3.10.105-${BOARD}-Kernel.tgz lib/modules)
(cd $R ; tar czvf $TOPDIR/SD/${TARGET_PRODUCT}/BOOTLOADER-${TARGET_PRODUCT}.tgz usr/lib/u-boot/bananapi)

echo "pack finish"
