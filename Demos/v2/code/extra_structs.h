#ifndef MY_SAVEFILE_VARS_H
#define MY_SAVEFILE_VARS_H

#include <crayon_savefile/savefile.h>


//WARNING. Don't manually modify the pointers. Crayon savefile functions will set them itself


//v1 VARS

uint16_t *sf_coins;
#define sf_coins_type CRAYON_TYPE_UINT16
#define sf_coins_length 1	//If you want, these length defines could be unsigned int consts.

float *sf_var2;
#define sf_var2_type CRAYON_TYPE_FLOAT
#define sf_var2_length 1

uint8_t *sf_dunno;
#define sf_dunno_type CRAYON_TYPE_UINT8
#define sf_dunno_length 1

//Now the "other_struct"
#define sf_var4_length 10

uint8_t *sf_lol[sf_var4_length];
#define sf_lol_type CRAYON_TYPE_UINT8
#define sf_lol_length 1

int32_t *sf_hi[sf_var4_length];
#define sf_hi_type CRAYON_TYPE_SINT32
#define sf_hi_length 2

char *sf_name[sf_var4_length];	// "sf_var4_length" strings with 16 chars each
#define sf_name_type CRAYON_TYPE_CHAR
#define sf_name_length 16


//v2 VARS

uint8_t *sf_myspace;
#define sf_myspace_type CRAYON_TYPE_UINT8
#define sf_myspace_length 1

double *sf_speedrun_times;
#define sf_speedrun_times_type CRAYON_TYPE_DOUBLE
#define sf_speedrun_times_length 2

float *sf_garbage;
#define sf_garbage_type CRAYON_TYPE_FLOAT
#define sf_garbage_length 128


//For those unfamiliar with enum, a value with no assigned number is equal to the previous value plus 1
//Also you just use the variable name like a constant, not "savefile_version.sf_initial" or something
enum savefile_version{
	SFV_INITIAL = 1,
	SFV_SPEEDRUNNER,
	//
	//Add new versions here
	//
	SFV_LATEST_PLUS_ONE	//DON'T REMOVE
};

#define VAR_STILL_PRESENT SFV_LATEST_PLUS_ONE

#define SFV_CURRENT (SFV_LATEST_PLUS_ONE - 1)

#endif