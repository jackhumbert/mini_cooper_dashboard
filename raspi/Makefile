#
# Makefile
#

CIRCLEHOME = ../lib/circle
DASHHOME = ../src

INCLUDE = -I $(CIRCLEHOME)/../../.pio/libdeps/esp32-s3-devkitc-1-myboard/tiny_ttf/include \
	-I $(CIRCLEHOME)/addon/lvgl/lvgl \
	-I $(DASHHOME)

OBJS = main.o kernel.o \
	# $(CIRCLEHOME)/../../.pio/libdeps/esp32-s3-devkitc-1-myboard/tiny_ttf/src/tiny_ttf.o

LIBS	= $(CIRCLEHOME)/addon/lvgl/liblvgl.a \
	  $(CIRCLEHOME)/lib/usb/libusb.a \
	  $(CIRCLEHOME)/lib/input/libinput.a \
	  $(CIRCLEHOME)/lib/fs/libfs.a \
	  $(CIRCLEHOME)/lib/libcircle.a

include $(DASHHOME)/dash.mk

include $(CIRCLEHOME)/Rules.mk

-include $(DEPS)