#ifndef MS_SETUP_H
#define MS_SETUP_H

#include "extra_structs.h"

#include <crayon_savefile/savefile.h>
#include <crayon_savefile/crayon.h>

#include <stdlib.h>
#include <string.h>


void savefile_defaults();
int8_t update_savefile(void **loaded_variables, crayon_savefile_version_t loaded_version,
	crayon_savefile_version_t latest_version);

uint8_t setup_savefile(crayon_savefile_details_t * details);

int16_t setup_vmu_icon_load(uint8_t ** vmu_lcd_icon, char * icon_path, uint8_t *vmu_bitmap);

#endif
