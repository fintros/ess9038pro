KERNEL_SRC = /lib/modules/$(shell uname -r)/source
BUILD_DIR := $(shell pwd)
DTC_DIR = /lib/modules/$(shell uname -r)/build/scripts/dtc/
VERBOSE = 0

OBJS    = ess9038pro.o

obj-m := $(OBJS)

all:
	make -C $(KERNEL_SRC) M=$(BUILD_DIR)

clean:
	make -C $(KERNEL_SRC) SUBDIRS=$(BUILD_DIR) clean
	rm -f ess9038pro.dtbo

dtbs:
	$(DTC_DIR)/dtc -@ -H epapr -I dts -O dtb -o ess9038pro.dtbo ess9038pro-overlay.dts

modules_install:
	cp ess9038pro.ko /lib/modules/$(shell uname -r)/kernel/sound/soc/codecs/
	depmod -a

modules_remove:
	rm /lib/modules/$(shell uname -r)/kernel/sound/soc/codecs/ess9038pro.ko
	depmod -a

install:
	modprobe ess9038pro

remove:
	modprobe -r ess9038pro

# Kernel >4.4.y
install_dtbo:
	cp ess9038pro.dtbo /boot/overlays/

remove_dtb:
	rm -f /boot/overlays/ess9038pro.dtbo
