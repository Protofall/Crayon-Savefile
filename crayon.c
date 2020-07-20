#include "crayon.h"

extern uint8_t crayon_misc_is_big_endian(){
	int a = 1;
	return !((char*)&a)[0];
}

extern void crayon_misc_endian_correction(uint8_t *buffer, size_t bytes){
	printf("ENDIANESS CORRECTOR INCOMPLETE\n");
	return;
}

extern void crayon_misc_encode_to_buffer(uint8_t *buffer, uint8_t *data, size_t bytes){
	while(bytes--){*buffer++ = *data++;}

	return;
}

#if defined(_arch_dreamcast)

#if CRAYON_BOOT_MODE == 1

void unmount_ext2_sd(){
	fs_ext2_unmount("/sd");
	fs_ext2_shutdown();
	sd_shutdown();
}

int mount_ext2_sd(){
	kos_blockdev_t sd_dev;
	uint8 partition_type;

	// Initialize the sd card if its present
	if(sd_init()){
		return 1;
	}

	// Grab the block device for the first partition on the SD card. Note that
	// you must have the SD card formatted with an MBR partitioning scheme
	if(sd_blockdev_for_partition(0, &sd_dev, &partition_type)){
		return 2;
	}

	// Check to see if the MBR says that we have a Linux partition
	if(partition_type != 0x83){
		return 3;
	}

	// Initialize fs_ext2 and attempt to mount the device
	if(fs_ext2_init()){
		return 4;
	}

	//Mount the SD card to the sd dir in the VFS
	if(fs_ext2_mount("/sd", &sd_dev, MNT_MODE)){
		return 5;
	}
	return 0;
}

#endif

uint8_t crayon_memory_mount_romdisk(char *filename, char *mountpoint){
	void *buffer;

	ssize_t size = fs_load(filename, &buffer); // Loads the file "filename" into RAM

	if(size == -1){
		return 1;
	}
	
	fs_romdisk_mount(mountpoint, buffer, 1); // Now mount that file as a romdisk, buffer will be freed when romdisk is unmounted
	return 0;
}

#endif


vec2_s8_t crayon_peripheral_dreamcast_get_port_and_slot(int8_t save_device_id){
	vec2_s8_t values = {-1,-1};
	if(save_device_id < 0 || save_device_id >= 8){return values;}
	
	if(save_device_id % 2 == 0){
		values.y = 1;
	}
	else{
		values.y = 2;
	}

	values.x = save_device_id / 2;

	return values;
}

uint8_t crayon_peripheral_dreamcast_get_screens(){
	#if defined(_arch_dreamcast)

	uint8_t screens = 0;	//a1a2b1b2c1c2d1d2

	int i;
	for(i = 0; i < 8; i++){	//8 because we can have 8 VMUs max
		//Check if device contains this function bitmap (Returns 0 on success)
		if(!crayon_peripheral_has_function(MAPLE_FUNC_LCD, i)){
			// crayon_savefile_set_device_bit(&screens, i);
			screens |= (1 << i);
		}
		
	}
	return screens;

	#else

	return 0;

	#endif
}

void crayon_peripheral_vmu_display_icon(uint8_t vmu_bitmap, void *icon){
	#ifdef _arch_dreamcast
	
	maple_device_t *vmu;
	uint8_t i, j;
	for(j = 0; j <= 3; j++){
		for(i = 1; i <= 2; i++){
			//a1a2b1b2c1c2d1d2
			if((vmu_bitmap >> ((2 * j) + (i - 1))) & 1){	//We want to display on this VMU
				if(!(vmu = maple_enum_dev(i, j))){	//Device not present
					continue;
				}
				vmu_draw_lcd(vmu, icon);
			}
		}
	
	}
	#endif

	return;
}



//Returns true if device has certain function/s
uint8_t crayon_peripheral_has_function(uint32_t function, int8_t save_device_id){
	#if defined(_arch_dreamcast)

	maple_device_t *vmu;

	vec2_s8_t port_and_slot = crayon_peripheral_dreamcast_get_port_and_slot(save_device_id);

	//Invalid controller/port
	if(port_and_slot.x < 0){
		return 1;
	}

	//Make sure there's a device in the port/slot
	if(!(vmu = maple_enum_dev(port_and_slot.x, port_and_slot.y))){
		return 1;
	}

	//Check the device is valid and it has a certain function
	if(!vmu->valid || !(vmu->info.functions & function)){
		return 1;
	}

	#endif

	return 0;
}
