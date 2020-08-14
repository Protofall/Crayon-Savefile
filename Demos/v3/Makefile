TARGET = Crayon-Savefile-Demo-v3
IPBIN = ./IP.BIN

# directories
#------------------------------------------------------------------------------

# CD ROM contents will be placed here for packaging into a cdi
CDFS_DIR = cdfs

# list of directories to be automatically created
SUBDIRS = $(CDFS_DIR)

# files
#------------------------------------------------------------------------------

OFILES = $(patsubst code/%.c,%.o,$(wildcard code/*.c)) romdisk_boot.o
SCRAMBLED = $(CDFS_DIR)/1st_read.bin

# list of files to be removed by make clean
INTERMEDIATES = $(OFILES) $(SCRAMBLED)

# flags
#------------------------------------------------------------------------------

SAVE_INCLUDE=-I../../libCrayonSavefile/include
SAVE_LINK=-L../../libCrayonSavefile/lib/dreamcast

CFLAGS =
CFLAGS += $(SAVE_INCLUDE)
# CFLAGS += -I$(KOS_BASE)/addons/zlib

LIBS=-lCrayonSavefile -lz -lm $(KOS_LIBS)

# main targets
# ------------------------------------------------------------------------------

all: $(TARGET).elf

cdi: $(TARGET).cdi

elf: $(TARGET).elf

clean: rm-elf rm-cdi rm-bin rm-iso
	-rm -f $(INTERMEDIATES)
	-rm -f romdisk_boot.*

rm-%:
	-rm -f $(TARGET).$*

echo-%:
	@echo $($*)

# build rules
# ------------------------------------------------------------------------------

include $(KOS_BASE)/Makefile.rules

# create cdi image from CDFS_DIR
$(TARGET).cdi: $(SCRAMBLED)
	mkisofs -G $(IPBIN) -C 0,11702 -J -l -r -o $(TARGET).iso $(CDFS_DIR)
	cdi4dc $(TARGET).iso $(TARGET).cdi

# create SCRAMBLED executable from elf
$(SCRAMBLED): $(CDFS_DIR) $(TARGET).elf
	sh-elf-objcopy -R .stack -O binary $(TARGET).elf $(TARGET).bin
	$(KOS_BASE)/utils/scramble/scramble $(TARGET).bin $@

# link objects into elf
$(TARGET).elf: $(OFILES)
	kos-cc $(SAVE_LINK) -o $@ $(KOS_START) $^ $(LIBS)

%.o: code/%.c
	kos-cc -o $@ -c $(SAVE_INCLUDE) $^

romdisk_boot.img:
	$(KOS_GENROMFS) -f $@ -d romdisk_boot

romdisk_boot.o: romdisk_boot.img
	$(KOS_BASE)/utils/bin2o/bin2o $< romdisk_boot $@

# create output directories if not present
$(SUBDIRS):
	mkdir -p $@
