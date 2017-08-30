## **BPI-M64-bsp**
Banana Pi BPI-M64 bsp (u-boot 2014.7 & Kernel 3.10)

ref.from https://github.com/longsleep Thanks Simon Eisenmann.

### **Compile**


----------


`#./build.sh 1` 

Target download packages in SD/bpi-m64 after build. Please check the build.sh and Makefile for detail compile options.


### **Install to bpi image SD card**

----------


Get the image from [bpi](www.banana-pi.org/m64-download.html) and download it to the SD card. After finish, insert the SD card to PC

    # ./build.sh 6

Choose the type, enter the SD dev, and confirm yes, all the build packages will be installed to target SD card.

![Install](https://github.com/Dangku/readme/raw/master/m64/bpi-install.png)


### **Install to customer SD card**


----------


**Partitioning**

Partition the SD card with a 64MB boot partition starting at 1MB, and the rest as root partition.

**Format the partitions**

    # mkfs.vfat -n BOOT /dev/sdX1
    # mkfs.ext4 -L ROOT /dev/sdX2

**Bootloader**

Defaut support 5 types base on the display, 480p/720p/1080p/lcd5/lcd7

    # sudo gunzip -c SD/bpi-m64/100MB/u-boot-with-dtb-bpi-m64-{type}-8k.img.gz | dd of=/dev/sdX bs=1024 seek=8

**BOOT Partition**

    # mount -t vfat /dev/sdX1 /mnt/
    # cp SD/bpi-m64/BPI-BOOT/bananapi/bpi-m64/linux/Image /mnt/
    # cp SD/bpi-m64/BPI-BOOT/bananapi/bpi-m64/linux/initrd.img /mnt/

Defaut support 5 types base on the display, 480p/720p/1080p/lcd5/lcd7

    # cp SD/bpi-m64/BPI-BOOT/bananapi/bpi-m64/linux/${type}/* /mnt/

You must edit the uEnv.txt for u-boot to load related files with right direction.

![uEnv.txt](https://github.com/Dangku/readme/raw/master/m64/uenv.png)

    # umount /mnt/

Another way to create the BOOT partition files if you don't care the file location

    # tar xf SD/bpi-m64/BPI-BOOT-bpi-m64.tgz --keep-directory-symlink -C /mnt/

If you want to switch the display type in one of the five during the development cycle, this way is suggested, only download different type bootloader without any boot files changed.  

**ROOT Partition**

Using rootfs tarball

    # mount -t ext4 /dev/sdX2 /mnt/
    # tar -c /mnt/ -xvf rootfs.tar.gz
    # tar xf SD/bpi-m64/3.10.105-BPI-M64-Kernel.tgz --keep-directory-symlink -C /mnt/
    # tar xf SD/bpi-m64/BOOTLOADER-bpi-m64.tgz --keep-directory-symlink -C /mnt/

Get the ap6212 firmware from https://github.com/BPI-SINOVOIP/BPI_WiFi_Firmware/tree/master/ap6212 and place ap6212 folder in /mnt/lib/firmware/

    # umount /mnt/


