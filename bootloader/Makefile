# This program is licensed "as is" without any
# warranty of any kind, whether express or implied.

# Simple Makefile to create Pine64 u-boot in Armbian build script compatible way
# Made by makefile noob, so no facepalms please

include ../chosen_board.mk

VERSION= 1
PATCHLEVEL= 0
SUBLEVEL= 0
EXTRAVERSION=

SHELL := bash

# pass ccache to submodules make
ccache := $(findstring ccache,$(CROSS_COMPILE))

# uboot board model
export TARGET_PRODUCT ${TARGET_PRODUCT}

.PHONY: all
all: make_image

.PHONY: u_boot_sunxi
u_boot_sunxi:
	$(MAKE) -C u-boot-sunxi ARCH=arm CROSS_COMPILE="$(ccache) arm-linux-gnueabihf-" sun50iw1p1_config
	$(MAKE) -C u-boot-sunxi ARCH=arm CROSS_COMPILE="$(ccache) arm-linux-gnueabihf-"

.PHONY: arm_trusted_firmware
arm_trusted_firmware:
	$(MAKE) -C arm-trusted-firmware PLAT=sun50iw1p1 DEBUG=1 bl31

.PHONY: make_image
make_image: u_boot_sunxi
ifeq ($(TARGET_PRODUCT),bpi-m64)
	@for target in $$(ls -f bpi-m64*); \
	do \
		target=$${target%.*}; \
		dtc -I dts -O dtb -o sun50i-a64-$$target.dtb $$target.dts; \
		./boot0img -B boot0.bin -s blobs/scp.bin -d blobs/bl31.bin -u u-boot-sunxi/u-boot.bin -e -F sun50i-a64-$$target.dtb -o u-boot-with-dtb.bin; \
		cp -a u-boot-with-dtb.bin u-boot-with-dtb-$$target-8k.img; \
	done
else
	@dtc -I dts -O dtb -o sun50i-a64-$(TARGET_PRODUCT).dtb $(TARGET_PRODUCT).dts
	./boot0img -B boot0.bin -s blobs/scp.bin -d blobs/bl31.bin -u u-boot-sunxi/u-boot.bin -e -F sun50i-a64-$(TARGET_PRODUCT).dtb -o u-boot-with-dtb.bin
	cp -a u-boot-with-dtb.bin u-boot-with-dtb-$(TARGET_PRODUCT)-8k.img
endif

.PHONY: clean
clean:
	[ -f u-boot-sunxi/Makefile ] && $(MAKE) -C u-boot-sunxi ARCH=arm distclean
	[ -f arm-trusted-firmware/Makefile ] && $(MAKE) -C arm-trusted-firmware PLAT=sun50iw1p1 distclean
	@rm -f *.bin *.dts *.dtb *.img

.PHONY: $(TARGET_PRODUCT)_defconfig
$(TARGET_PRODUCT)_defconfig:
ifeq ($(TARGET_PRODUCT),bpi-m64)
	@for target in $$(ls -f blobs/bpi-m64*); \
	do \
		cp $$target . ; \
	done
else
	@cp blobs/$(TARGET_PRODUCT).dts $(TARGET_PRODUCT).dts
endif
	@cp blobs/boot0.bin boot0.bin


.PHONY: pine64_defconfig
pine64_defconfig:
	@cp blobs/pine64.dts dt.dts
	@cp blobs/boot0.bin boot0.bin

.PHONY: pine64_plus_defconfig
pine64_plus_defconfig:
	@cp blobs/pine64plus.dts dt.dts
	@cp blobs/boot0.bin boot0.bin

.PHONY: pine64_so_defconfig
pine64_so_defconfig:
	@cp blobs/pine64so.dts dt.dts
	@cp blobs/boot0so.bin boot0.bin

.PHONY: pine64_pinebook_defconfig
pine64_pinebook_defconfig:
	@cp blobs/pine64pinebook.dts dt.dts
	@cp blobs/boot0pinebook.bin boot0.bin
