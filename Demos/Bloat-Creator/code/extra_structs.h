#ifndef MY_SAVEFILE_VARS_H
#define MY_SAVEFILE_VARS_H

#include <crayon_savefile/savefile.h>

uint8_t *sf_vars;
#define sf_vars_type CRAYON_TYPE_UINT8
uint32_t sf_vars_size;

//For those unfamiliar with enum, a value with no assigned number is equal to the previous value plus 1
//Also you just use the variable name like a constant, not "savefile_version.sf_initial" or something
enum savefile_version{
	SFV_INITIAL = 1,
	//
	//Add new versions here
	//
	SFV_LATEST_PLUS_ONE	//DON'T REMOVE
};

#define VAR_STILL_PRESENT SFV_LATEST_PLUS_ONE

#define SFV_CURRENT (SFV_LATEST_PLUS_ONE - 1)

#endif
