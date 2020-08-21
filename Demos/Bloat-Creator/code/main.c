#if defined(_arch_dreamcast)

//For the controller and mouse
#include <dc/maple.h>
#include <dc/maple/controller.h>

#endif

#include "setup.h"
#include "graphics.h"

#include <math.h>


#if defined(_arch_dreamcast)

//Baked Romdisk
extern uint8 romdisk_boot[];
KOS_INIT_ROMDISK(romdisk_boot);

#endif

int main(){
	crayon_savefile_details_t savefile_details;

	#if defined(_arch_dreamcast)
	
	pvr_init_defaults();	//Init kos
	font_init();
	
	#endif

	uint8_t mode = 0;

	uint32_t size = 5 * 512;
	uint8_t dev_id = 0;

	char buffer[300];
	uint32_t previous[4] = {0};

	#if defined(_arch_dreamcast)

	while(mode < 2){
		if(mode == 0){
			sprintf(buffer, "How many blocks do you want?\n(Press A to continue)\nBlocks: %d", (uint32_t)ceil(size/512.0));
		}
		else if(mode == 1){
			sprintf(buffer, "Which device id? %d", dev_id);
		}

		pvr_wait_ready();
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			if((st->buttons & CONT_A) && !(previous[__dev->port] & CONT_A)){
				mode++;
			}
			if((st->buttons & CONT_DPAD_UP) && !(previous[__dev->port] & CONT_DPAD_UP)){
				if(mode == 0 && size < 200 * 512){
					size += 512;
				}
				else if(mode == 1 && dev_id < CRAYON_SF_NUM_SAVE_DEVICES - 1){
					dev_id++;
				}
			}
			if((st->buttons & CONT_DPAD_DOWN) && !(previous[__dev->port] & CONT_DPAD_DOWN)){
				if(mode == 0 && size > 0){
					size -= 512;
				}
				else if(mode == 1 && dev_id > 0){
					dev_id--;
				}
			}
			
			previous[__dev->port] = st->buttons;

		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

			draw_string(30, 30, 1, 255, 255, 216, 0, buffer, 2, 2);
		
		pvr_list_finish();


		pvr_scene_finish();
	}

	#else

	printf("How many bytes do you want the savefile to be?\n");
	scanf("%d", &size);

	#endif

	uint8_t setup_res = setup_savefile(&savefile_details, size);

	//Set the device id, even if we can't use that device
	// savefile_details.save_device_id = dev_id;
	if(crayon_savefile_set_device(&savefile_details, dev_id) == -1){
		sprintf(buffer, "Unable to save to the requested device\n");
	}
	else{

		int8_t save_error = 1;
		if(!setup_res){
			save_error = crayon_savefile_save_savedata(&savefile_details);
		}

		#if defined(_arch_dreamcast)

		uint32_t true_size = crayon_savefile_bytes_to_blocks(crayon_savefile_get_savefile_size(&savefile_details));

		#else

		uint32_t true_size = size;

		#endif

		sprintf(buffer, "Save error %d, %d. Size: %d\n Bitmaps: %d, %d, %d", save_error, setup_res, true_size,
			savefile_details.present_devices, savefile_details.present_savefiles, savefile_details.upgradable_to_current);
	}

	printf("%s", buffer);

	#if defined(_arch_dreamcast)

	mode = 0;
	while(!mode){
		pvr_wait_ready();
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			if(st->buttons & CONT_START){
				mode = 1;
			}
		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

			draw_string(30, 30, 1, 255, 255, 216, 0, buffer, 2, 2);
		
		pvr_list_finish();


		pvr_scene_finish();
	}

	#endif

	crayon_savefile_free(&savefile_details);
	crayon_savefile_free_base_path();
	#ifdef _arch_dreamcast
	pvr_mem_free(font_tex);
	#endif

	return 0;
}
