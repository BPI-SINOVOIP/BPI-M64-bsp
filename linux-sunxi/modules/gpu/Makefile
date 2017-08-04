ifneq ($(LICHEE_KDIR),)
KERNEL_DIR = $(LICHEE_KDIR)
GPU_BUILD_TYPE = release
endif

include $(KERNEL_DIR)/.config

##############################################################
##################   Set ARCH and CROSS_COMPILE  #############
##############################################################
ifdef CONFIG_ARM
export ARCH ?= arm
export CROSS_COMPILE ?= arm-linux-gnueabi-
endif
	
ifdef CONFIG_ARM64
export ARCH ?= arm64
export CROSS_COMPILE ?= aarch64-linux-gnu-
endif

##############################################################
##################     Set GPU_TYPE         ##################
##############################################################

#####################################
#For mali400 Platform
#####################################
ifdef CONFIG_ARCH_SUN8IW3P1
GPU_TYPE = mali400
endif

ifdef CONFIG_ARCH_SUN8IW5P1
GPU_TYPE = mali400
endif

ifdef CONFIG_ARCH_SUN8IW7P1
GPU_TYPE = mali400
endif

ifdef CONFIG_ARCH_SUN50IW1P1
GPU_TYPE = mali400
endif

##############################################################
#############  Set GPU_BUILD_DIR and GPU_KO_NAME ##############
##############################################################

#####################################
#For mali400 Platform
#####################################
ifeq ($(GPU_TYPE),mali400)
GPU_BUILD_DIR = mali400/kernel_mode/driver/src/devicedrv/mali
GPU_KO_NAME   = $(GPU_BUILD_DIR)/mali.ko

ifdef CONFIG_OF
export USING_DT = 1
endif

export USING_UMP=0
export KDIR=$(KERNEL_DIR)
endif

##############################################################

all: build install

build:
	$(MAKE) -j -C $(GPU_BUILD_DIR) BUILD=$(GPU_BUILD_TYPE) KERNELDIR=$(KERNEL_DIR)

install:
	if [ $(LICHEE_MOD_DIR) ]; then \
		cp -u $(GPU_KO_NAME) $(LICHEE_MOD_DIR); \
	fi
	#if [ -z $(LICHEE_MOD_DIR) ]; then \
	#	cp -u $(GPU_KO_NAME) $(GPU_TARGET_OUT_DIR)/module; \
	#fi

clean:
	$(MAKE) -C $(GPU_BUILD_DIR) clean
	if [ $(GPU_CLEAN_DIR) ]; then \
		rm -rf $(GPU_CLEAN_DIR); \
	fi
