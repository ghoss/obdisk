#=====================================================
# obdisk
# Oberon Ceres Disk Utility
#
# Guido Hoss, 12.03.2022
#
# Published by Guido Hoss under GNU Public License V3.
#=====================================================

CFLAGS += -DVERSION_BUILD_DATE=\""$(shell date +'%F')"\"
CFLAGS += -DPKG=\"obdisk\"

bin_PROGRAMS = obdisk

obdisk_SOURCES = \
	ob_disk.c \
	obdisk.c \
	obdisk.h \
	ob_extract.c \
	ob_listdir.c \
	ob_usage.c
