//For the controller and mouse
#ifdef _arch_dreamcast
#include <dc/maple.h>
#include <dc/maple/controller.h>
#endif

#include "setup.h"
#include "graphics.h"

int main(){
	return 0;
	crayon_savefile_details_t savefile_details;

	uint8_t setup_res = setup_savefile(&savefile_details);

	#if defined(_arch_dreamcast)
	
	pvr_init_defaults();	//Init kos
	font_init();
	
	#endif

	//Try and load savefile
	int8_t load_error = crayon_savefile_load_savedata(&savefile_details);	//If a savefile DNE this fails

	int8_t save_error = 1;
	if(savefile_details.present_devices){
		save_error = crayon_savefile_save_savedata(&savefile_details);
	}

	char buffer[500];
	uint32_t bytes = CRAYON_SF_HDR_SIZE + savefile_details.savedata.size;
	if(!setup_res){
		sprintf(buffer, "Save initialised. %d bytes, %d blocks (DC)\n", bytes, 
			crayon_savefile_bytes_to_blocks(bytes));
	}
	else{
		sprintf(buffer, "It failed with code %d\n", setup_res);
	}

	char buffer2[50];
	sprintf(buffer2, "save_error: %d. load_error %d\n", save_error, load_error);
	strcat(buffer, buffer2);
	sprintf(buffer2, "bitmaps: %d, %d, %d\n", savefile_details.present_devices,
		savefile_details.present_savefiles, savefile_details.current_savefiles);
	strcat(buffer, buffer2);

	#if defined(_arch_pc)

	printf("%s", buffer);

	#elif defined(_arch_dreamcast)

	uint8_t end = 0;
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

	crayon_savefile_free(&savefile_details);
	crayon_savefile_free_base_path();
	#ifdef _arch_dreamcast
	pvr_mem_free(font_tex);
	#endif

	return 0;
}
