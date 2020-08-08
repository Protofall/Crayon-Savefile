//For the controller and mouse
#ifdef _arch_dreamcast
#include <dc/maple.h>
#include <dc/maple/controller.h>
#endif

#include "setup.h"
#include "graphics.h"

int main(){
	crayon_savefile_details_t savefile_details;

	uint8_t setup_res = setup_savefile(&savefile_details);

	#if defined(_arch_dreamcast)
	
	pvr_init_defaults();	//Init kos
	font_init();
	
	#endif

	//Try and load savefile
	int8_t load_error = crayon_savefile_load_savedata(&savefile_details);	//If a savefile DNE this fails

	//Change vars here
	// sf_var1[0] = 2997;
	// sf_var2[0] += 5.25;
	// sf_name[2][3] = '1';

	int8_t save_error = 1;
	if(savefile_details.present_devices){
		save_error = crayon_savefile_save_savedata(&savefile_details);
	}

	#if defined(_arch_dreamcast)

	// char buffer[70];
	// if(!setup_res){
	// 	sprintf(buffer, "Save created\nUses %d blocks and has %d frames of\nanimation",
	// 	crayon_savefile_bytes_to_blocks(crayon_savefile_get_savefile_size(&savefile_details)),
	// 	savefile_details.icon_anim_count);
	// }
	// else{
	// 	sprintf(buffer, "It failed with code %d", setup_res);
	// }

	// sprintf(buffer, "It failed with code %d", setup_res);

	char buffer[400];
	sprintf(buffer, "Res %d %d %d. Uses %d blocks and has\n%d frames of animation.\n%d %d %d", setup_res, save_error,
		load_error, crayon_savefile_bytes_to_blocks(crayon_savefile_get_savefile_size(&savefile_details)),
		savefile_details.icon_anim_count, savefile_details.present_devices, savefile_details.present_savefiles,
		savefile_details.current_savefiles);

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

	#else

	char buffer[70];
	if(!setup_res){
		sprintf(buffer, "Save initialised\nUses %d bytes", CRAYON_SF_HDR_SIZE + savefile_details.savedata.size);
	}
	else{
		sprintf(buffer, "It failed with code %d", setup_res);
	}

	char buffer2[32];
	char buffer3[32];
	sprintf(buffer2, "save_error: %d. load_error %d\n", save_error, load_error);
	sprintf(buffer3, "bitmaps: %d, %d, %d\n", savefile_details.present_devices,
		savefile_details.present_savefiles, savefile_details.current_savefiles);
	draw_string(0, 0, 0, 0, 0, 0, 0, buffer, 0, 0);
	draw_string(0, 0, 0, 0, 0, 0, 0, buffer2, 0, 0);
	draw_string(0, 0, 0, 0, 0, 0, 0, buffer3, 0, 0);

	// __crayon_savefile_print_savedata(&savefile_details.savedata);

	// sf_var1[0] = 2997;
	// sf_name[2][3] = '1';
	// //Each sf_name is 16 chars and we have 20 names. So this is the 4th char of the 3rd name
	// //(2 * 16) + 3 = 35-th index of the dest array

	// print_all_vars(&savefile_details);

	#endif

	crayon_savefile_free(&savefile_details);
	crayon_savefile_free_base_path();
	#ifdef _arch_dreamcast
	pvr_mem_free(font_tex);
	#endif

	return 0;
}