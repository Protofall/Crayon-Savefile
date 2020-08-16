#include "setup.h"

void savefile_defaults(){
	uint16_t i;
	for(i = 0; i < sf_vars_size; i++){
		sf_vars[i] = 0;
	}

	return;
}

int8_t upgrade_savefile(void **loaded_variables, crayon_savefile_version_t loaded_version,
	crayon_savefile_version_t latest_version){

	;

	return 0;
}

uint8_t setup_savefile(crayon_savefile_details_t * details, uint32_t size){
	uint8_t i, error;

	#if defined(_arch_pc)

	crayon_savefile_set_base_path("saves/");

	#else
	
	crayon_savefile_set_base_path(NULL);	//Dreamcast ignores the parameter anyways
											//(Assumes "/vmu/") so its still fine to
											//do the method above for all platforms
	#endif

	error = crayon_savefile_init_savefile_details(details, "BLOAT.s", SFV_CURRENT,
		savefile_defaults, upgrade_savefile);
	if(error){return 1;}

	error += crayon_savefile_set_app_id(details, "ProtoBloat");
	error += crayon_savefile_set_short_desc(details, "Bloat");
	error += crayon_savefile_set_long_desc(details, "Only purpose is to waste space");
	
	if(error){return 1;}

	#if defined(_arch_dreamcast)

	size *= 512;

	#endif

	//Remove the version number and hdr size
	if(size <= sizeof(crayon_savefile_version_t) + CRAYON_SF_HDR_SIZE){
		sf_vars_size = size - sizeof(crayon_savefile_version_t) - CRAYON_SF_HDR_SIZE;
	}
	sf_vars_size = 1;	//Just so we don't make a MASSIVE file

	//Now lets construct our history
	crayon_savefile_add_variable(details, &sf_vars, sf_vars_type, sf_vars_size, SFV_INITIAL, VAR_STILL_PRESENT);

	//Set the savefile
	if(crayon_savefile_solidify(details)){return 1;}

	return 0;
}
