#if defined(_arch_dreamcast)

//For the controller and mouse
#include <dc/maple.h>
#include <dc/maple/controller.h>

#endif

#include "setup.h"
#include "graphics.h"


#if defined(_arch_dreamcast)

//Baked Romdisk
extern uint8 romdisk_boot[];
KOS_INIT_ROMDISK(romdisk_boot);

#endif

int main(){
	char buffer[400];	//Currently around 254 chars are used
	int dev_id = 0;

	#if defined(_arch_dreamcast)
	
	pvr_init_defaults();	//Init kos
	font_init();
	
	uint32_t previous[4] = {0};
	uint8_t end = 0;
	while(!end){
		sprintf(buffer, "What save device do you want to use?\nPress up or down on D-PAD: %d", dev_id);

		pvr_wait_ready();
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			if((st->buttons & CONT_A) && !(previous[__dev->port] & CONT_A)){
				end = 1;
				break;
			}
			if((st->buttons & CONT_DPAD_UP) && !(previous[__dev->port] & CONT_DPAD_UP)){
				if(dev_id < CRAYON_SF_NUM_SAVE_DEVICES - 1){
					dev_id++;
				}
			}
			if((st->buttons & CONT_DPAD_DOWN) && !(previous[__dev->port] & CONT_DPAD_DOWN)){
				if(dev_id > 0){
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

	printf("Which device do you want to save to (Max is %d)?\n", CRAYON_SF_NUM_SAVE_DEVICES - 1);
	scanf("%d", &dev_id);

	#endif

	crayon_savefile_details_t savefile_details;

	uint8_t setup_res = setup_savefile(&savefile_details);

	//Try and load savefile
	int8_t save_error = -1;
	int8_t load_error = -1;
	if(!setup_res){
		crayon_savefile_set_device(&savefile_details, dev_id);
		load_error = crayon_savefile_load_savedata(&savefile_details);	//If a savefile DNE this fails
		save_error = crayon_savefile_save_savedata(&savefile_details);
	}

	uint32_t bytes = crayon_savefile_get_savefile_size(&savefile_details);
	if(!setup_res){
		#if defined(_arch_dreamcast)

		sprintf(buffer, "Save initialised. %d blocks\n", crayon_savefile_convert_bytes_to_blocks(bytes));

		#else

		sprintf(buffer, "Save initialised. %"PRIu32" bytes\n", bytes);

		#endif
	}
	else{
		sprintf(buffer, "It failed with code %d\n", setup_res);
	}

	char buffer2[50];
	sprintf(buffer2, "Save_error: %d. Load_error %d\n", save_error, load_error);
	strcat(buffer, buffer2);
	sprintf(buffer2, "Bitmaps: %d, %d, %d. Dev ID: %d\n", savefile_details.present_devices,
		savefile_details.present_savefiles, savefile_details.upgradable_to_current, savefile_details.save_device_id);
	strcat(buffer, buffer2);
	
	sprintf(buffer2, "Versions: ");
	strcat(buffer, buffer2);
	uint8_t i;
	for(i = 0; i < CRAYON_SF_NUM_SAVE_DEVICES; i++){
		sprintf(buffer2, "%"PRIu32", ", savefile_details.savefile_versions[i]);
		strcat(buffer, buffer2);
	}
	strcat(buffer, "\n");

	sprintf(buffer2, "Status: ");
	strcat(buffer, buffer2);
	for(i = 0; i < CRAYON_SF_NUM_SAVE_DEVICES; i++){
		sprintf(buffer2, "%d, ", crayon_savefile_save_device_status(&savefile_details, i));
		strcat(buffer, buffer2);
	}
	strcat(buffer, "\n");

	printf("%s", buffer);

	#if defined(_arch_dreamcast)

	end = 0;
	while(!end){
		pvr_wait_ready();
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			if(st->buttons & CONT_START){
				end = 1;
			}
		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

			draw_string(30, 30, 1, 255, 255, 216, 0, buffer, 2, 2);
		
		pvr_list_finish();


		pvr_scene_finish();
	}

	#endif

	crayon_savefile_free_details(&savefile_details);
	crayon_savefile_free_base_path();
	#ifdef _arch_dreamcast
	pvr_mem_free(font_tex);
	#endif

	return 0;
}
