#=====================================================
# obtext
# Oberon Ceres Text File Conversion
#
# Guido Hoss, 12.03.2022
#
# Published by Guido Hoss under GNU Public License V3.
#=====================================================

CFLAGS += -DVERSION_BUILD_DATE=\""$(shell date +'%F')"\"
CFLAGS += -DPKG=\"obtext\"

bin_PROGRAMS = obtext

obtext_SOURCES = \
	obtext.c \
	obtext.h \
	ob_usage.c \
	ob_convert.c