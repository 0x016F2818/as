# as
automotive software and its tool-chain

[![Build Status](https://travis-ci.org/parai/as.svg?branch=master)](https://travis-ci.org/parai/as)

## why this repository
because I am not powerful so I decided to develop small but smart part of automotive software.

## Status
* 2016-July-8: Recently done AGL application framework(afbinder) study, I think somehow it behaves the same as AUTOSAR VFB, the main part of afbiner is for the purpose of cummunication and remote call, I think it was good, so integrate part of the code into as, base on that part, implement a remote GUI daemon assg.py, which shows a log of performance improvment than the original one C Sg. 
* 2016-May-27: Nowdays, almost no real work, most of my time is spent on study of AGL security feature(SMACK,Cynara,Security-Manager), that's good but feel boring, still hope to do something related a real product. And ddd trampoline os which is AUTOSAR compatible, the most amazimg part of it is that the management ready queue by bubble sort algorithm dynamicaly, but bring up failed, but it doesn't matter as that I just want to read the code of it, if necessary, it will not be a problem to bring it up.
* 2016-Dec-24: Moving from AUTOSAR platform to linux, so now I almost have no update on AUTOSAR software stack, but many research on linux platform, generally focus on the driver system study.
* 2016-Jan-24:  Now I have porting toppers-atk2-sc4 on arm920t(s3c2440a) and arm-cortex-m3, but now I feel confused about what to do for the next step. 

## Key Point

### 1. Integrated ArcCore AUTOSAR 3.1 BSW
* Communication: Can CanIf PduR Com SoAd
* Diagnostic: CanTp Dcm DoIP
* System: EcuM SchM
* Memory: Fls Fee Eep Ea NvM MemIf

### 2. OS 
* [toppers_osek](https://www.toppers.jp/osek-os.html)
* [FreeOSEK](http://opensek.sourceforge.net/)
* [FreeRTOS](http://www.freertos.org/)
* [toppers-atk2](https://www.toppers.jp/atk2.html)
* small : this OS is developed by me, OSEK BCC1 conformed
* [contiki](http://contiki-os.org/)
* [trampoline](https://github.com/TrampolineRTOS/trampoline)

## 3. BSW configuration tool
> This GUI tool is the one from [OpenSAR](https://github.com/parai/OpenSAR.git) which is also developed by me, the GUI can be automatically created from xml.

## 4. AUTOSAR & LINUX communication
* [virtio](http://docs.oasis-open.org/virtio/virtio/v1.0/csprd01/virtio-v1.0-csprd01.pdf)
* [remoteproc](https://www.kernel.org/doc/Documentation/remoteproc.txt)
* [rpmsg](https://www.kernel.org/doc/Documentation/rpmsg.txt)
* [virtio ring buffer](http://www.ibm.com/developerworks/cn/linux/1402_caobb_virtio/)

## 5. Lua Script Integrated
* support CAN device by lascanlib: CANcaseXL, [PeakCAN](http://www.peak-system.com/PCAN-USB.199.0.html?L=1), [SocketCAN](https://en.wikipedia.org/wiki/SocketCAN),also provide a kind of python library for CAN access.
* support miscellaneous device by lasdevlib with the smae API interface "open/read/write/ioctl/close": rs232

## 6. aslinux simulation environment on QEMU
* Now use one [makefile](https://github.com/parai/as/blob/master/release/aslinux/makefile) to bring up the QEMU [AGL](https://www.automotivelinux.org/) environment.
* very simple and easy, with one single [Makefile](https://github.com/parai/as/blob/master/release/aslinux/script/vexpress-ca9.mk) to build out the kernel and the rootfs, and a [shell script](https://github.com/parai/as/blob/master/release/aslinux/script/run-vexpress.sh) to kick off the qemu virtual machine to execute that kernel and mount that rootfs.(for details check my [github.io page](http://parai.github.io/as/navigations/categories.html) on category linux)
* aslinux build step

```sh
git clone https://github.com/parai/as.git
cd $(patch-to)/as/release/aslinux
mkdir build
cd build
ln -fs ../script/vexpress-ca9.mk Makefile
ln -fs ../script/run-vexpress.sh run.sh
make all
./run.sh
# if aslua want to be used
cd $(path-to)/as/release/aslua && make 31 && make 33 
cp out/arm.exe $(path-to)/as/release/aslinux/build/out/rootfs/usr/bin/aslua -v
cp /usr/arm-linux-gnueabi/lib/*.so* $(path-to)/as/release/aslinux/build/out/rootfs/lib -v
# then the sdcard need to be rebuilt
cd $(path-to)/as/release/aslinux/build && make sdcard
```

## 7. Other 3rd part package
* LWIP
* afbindef/websock

## Setup Environment

``` bash
sudo apt-get install git
sudo apt-get install libxext6 libxext-dev libqt4-dev libqt4-gui libqt4-sql qt4-dev-tools qt4-doc qt4-designer qt4-qtconfig "python-qt4-*" python-qt4
sudo apt-get install pyqt5-dev pyqt5-dev-tools
sudo apt-get install libreadline6 libreadline6-dev
sudo apt-get install openjdk-7-jre
sudo apt-get install build-essential
sudo apt-get install gcc-arm-none-eabi
sudo apt-get install gcc-arm-linux-gnueabi
sudo apt-get insatll g++-arm-linux-gnueabi
sudo apt-get install libncurses5-dev
sudo apt-get install gawk autoconf
sudo apt-get install zlib1g-dev libglib2.0-0 libglib2.0-dev
sudo apt-get install libsdl-dev
sudo apt-get install libgtk2.0-dev libgtk-3-dev
sudo apt-get install lib32z1 lib32ncurses5 lib32bz2-1.0 lib32stdc++6
sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf  tslib
sudo apt-get install automake autogen autoconf libtool
sudo apt-get install u-boot-tools
sudo apt-get install nfs-kernel-server
sudo apt-get install mtd-utils
sudo apt-get install Kolourpaint
sudo apt-get install cmake uuid-dev libboost-dev libwebsockets-dev libjson-c-dev libjson-glib-1.0-0 libjson0 libjson0-dev
sudo apt-get install gcc-aarch64-linux-gnu
sudo apt-get install libstdc++6:i386 # for 64bit machine run 32bit app
sudo add-apt-repository ppa:webupd8team/java && sudo apt-get update && sudo apt-get install oracle-java7-installer
sudo apt-get install gtk-doc-tools
```

## Clone
> git clone https://github.com/sics-sse/moped.git

> git clone https://github.com/openxc/bitfield-c.git

> git clone git://git.omapzoom.org/repo/sysbios-rpmsg.git

> git clone https://github.com/parai/freertos-multicore.git

> git clone https://github.com/parai/isotp-c.git

> git clone https://github.com/parai/GainOS.git

> git clone https://github.com/parai/GainOS_Studio.git

> git clone https://github.com/parai/OpenOSEK.git

> git clone https://github.com/parai/OpenSAR.git

> git clone https://github.com/parai/WhatsApp.git

> git clone https://github.com/parai/as.git

> git clone https://github.com/parai/GaInOS_Based_On_uTenux-tkernel.git

> git clone https://github.com/parai/gainos-tk.git

> git clone https://github.com/lua/lua.git

> git clone https://github.com/z2akhtar/PWC-Demonstrator.git

> git clone https://github.com/elupus/autosar-xcp.git


