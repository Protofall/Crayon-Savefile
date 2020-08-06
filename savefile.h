#ifndef SAVEFILE_CRAYON_H
#define SAVEFILE_CRAYON_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //For the uintX_t types

#ifdef _arch_dreamcast

#include <dc/maple/vmu.h>
#include <dc/vmu_pkg.h>
#include <dc/vmufs.h>

#endif

#include "crayon.h"

//Var types the user passes into functions
enum {
	CRAY_TYPE_DOUBLE = 0,
	CRAY_TYPE_FLOAT,
	CRAY_TYPE_UINT32,
	CRAY_TYPE_SINT32,
	CRAY_TYPE_UINT16,
	CRAY_TYPE_SINT16,
	CRAY_TYPE_UINT8,
	CRAY_TYPE_SINT8,
	CRAY_TYPE_CHAR,
	CRAY_NUM_TYPES	//This will be the number of types we have available
};

//This is never accessed directly by the user, but it will contain all of you variables that will get saved
typedef struct crayon_savefile_data{
	double *doubles;
	float *floats;
	uint32_t *u32;
	int32_t *s32;
	uint16_t *u16;
	int16_t *s16;
	uint8_t *u8;
	int8_t *s8;
	char *chars;

	uint32_t lengths[CRAY_NUM_TYPES];	//The lengths are in the order they appear above
	uint32_t size;
} crayon_savefile_data_t;

//If you know you won't have many versions, change this to a uint8_t or uint16_t
//But don't change it mid way through a project
#define crayon_savefile_version_t uint32_t

typedef struct crayon_savefile_history{
	uint32_t id;

	uint8_t data_type;
	uint32_t data_length;
	crayon_savefile_version_t version_added;
	crayon_savefile_version_t version_removed;	//if equal to latest version + 1, it hasn't been removed

	//Contains the address of the user's pointer so we can re-assign it in solidify()
	union{
		double **t_double;
		float **t_float;
		uint32_t **t_u32;
		int32_t **t_s32;
		uint16_t **t_u16;
		int16_t **t_s16;
		uint8_t **t_u8;
		int8_t **t_s8;
		char **t_char;
	} data_ptr;

	struct crayon_savefile_history *next;
} crayon_savefile_history_t;

//Only Dreamcast has the descriptions
enum {
	CRAY_SF_STRING_FILENAME = 0,
	CRAY_SF_STRING_APP_ID,
	#if defined( _arch_dreamcast)
	CRAY_SF_STRING_SHORT_DESC,
	CRAY_SF_STRING_LONG_DESC,
	#endif
	CRAY_SF_NUM_DETAIL_STRINGS	// 4 on Dreamcast, 2 on PC
};

#if defined( _arch_dreamcast)

	#define CRAY_SF_NUM_SAVE_DEVICES 8
	#define CRAY_SF_HDR_SIZE sizeof(vmu_hdr_t)

#elif defined(_arch_pc)

	#define CRAY_SF_NUM_SAVE_DEVICES 1

	#define CRAY_SF_HDR_SIZE (16 + 16)	//Must be a multiple of 4 bytes long
	typedef struct crayon_savefile_hdr{
		char name[16];	//Not actually used, but useful if you read a savefile with a hex editor
						//It should always be set to "CRAYON SAVEFILE" with a null terminator at the end
		char app_id[16];
	} crayon_savefile_hdr_t;

#endif

//The struct that contains all of a save file info. This is useful for passing
//by reference into a function and if you want to modify the save file data easily
//or even use different save files for one game
typedef struct crayon_savefile_details{
	//The version of the most recent savefile version known to the program
	crayon_savefile_version_t latest_version;

	//The struct that contains the true data the user will indirectly manipulate
	crayon_savefile_data_t savedata;

	//All the strings we have. Different systems might have a different number of strings
	char *strings[CRAY_SF_NUM_DETAIL_STRINGS];

	//The following 3 variables are bitmaps
	uint8_t present_devices;	//Shows any device thats present that has enough space to save too
								//(Unless there's a savefile newer than latest present)
	uint8_t present_savefiles;	//Shows any device with any savefile from any version
	uint8_t current_savefiles;	//Only shows savefiles in the current version
	crayon_savefile_version_t savefile_versions[CRAY_SF_NUM_SAVE_DEVICES];	//Stores the versions of savefiles detected

	//This tells us what storage device we save to
	//On Dreamcast this corresponds to one of the 8 memory cards (slots 1 or 2, ports 0 to 3)
	//On PC there is only one, the savefile folder
	int8_t save_device_id;

	crayon_savefile_history_t *history;
	crayon_savefile_history_t *history_tail;	//Just used to speed stuff the history building process
	uint32_t num_vars;	//The length of the linked list

	//The functions the user provides for setting the default values of a new savefile and
	//how to handle older savefiles in the new system
	void (*default_values_func)();
	int8_t (*update_savefile_func)(void**, crayon_savefile_version_t, crayon_savefile_version_t);

	//Dreamcast exclusive variables
	#if defined(_arch_dreamcast)

	//Savefile icon
	uint8_t *icon_data;
	uint16_t *icon_palette;
	uint8_t icon_anim_count;	//Decided to not go with a uint16_t since the Dreamcast BIOS
								//will only work with up to 3 icons
	uint16_t icon_anim_speed;	//Set to "0" for no animation (1st frame only)

	//Appears when selecting a save in the BIOS
	uint8_t *eyecatcher_data;
	uint8_t eyecatcher_type;		//PAL4BPP (4 Blocks), PAL8BPP (8.875 Blocks), bitmap ARGB4444 (15.75 Blocks)

	#endif
} crayon_savefile_details_t;


//---------------------Internal stuff that the user should have to touch------------------------


//Takes a byte count and returns no. blocks needed to save it
uint16_t crayon_savefile_bytes_to_blocks(uint32_t bytes);

//Returns the number of bytes your save file will need
uint32_t crayon_savefile_get_savefile_size(crayon_savefile_details_t *details);

uint16_t crayon_savefile_detail_string_length(uint8_t string_id);

void crayon_savefile_serialise(crayon_savefile_details_t *details, uint8_t *buffer);
int8_t crayon_savefile_deserialise(crayon_savefile_details_t *details, uint8_t *data, uint32_t data_length);

uint32_t crayon_savefile_check_device_free_space(int8_t device_id);

//Returns a pointer on success, returns NULL if either the the save_device_id is OOB or failed malloc
char *crayon_savefile_get_full_path(crayon_savefile_details_t *details, int8_t save_device_id);

//Only really meant for debugging on PC. It just prints all the values in the savedata struct
void __crayon_savefile_print_savedata(crayon_savefile_data_t *savedata);

void crayon_savefile_buffer_to_savedata(crayon_savefile_data_t *data, uint8_t *buffer);

uint8_t crayon_savefile_set_string(crayon_savefile_details_t *details, const char *string, uint8_t string_id);

//Returns 0 if a savefile on that device exists with no issues, 1 if there's an error/DNE/version is newer than latest
int8_t crayon_savefile_check_savedata(crayon_savefile_details_t *details, int8_t save_device_id);

//This function will update the valid devices and/or the current savefile bitmaps
void crayon_savefile_update_all_device_infos(crayon_savefile_details_t *details);
int8_t crayon_savefile_update_device_info(crayon_savefile_details_t *details, int8_t save_device_id);

void crayon_savefile_free_icon(crayon_savefile_details_t *details);
void crayon_savefile_free_eyecatcher(crayon_savefile_details_t *details);
void crayon_savefile_free_savedata(crayon_savefile_data_t *savedata);

//Basically get/setting bits from a bitmap
uint8_t crayon_savefile_get_device_bit(uint8_t device_bitmap, uint8_t save_device_id);
void crayon_savefile_set_device_bit(uint8_t *device_bitmap, uint8_t save_device_id);


//---------------Stuff the user should be calling----------------


uint8_t crayon_savefile_set_base_path(char *path);	//On Dreamcast this is always "/vmu/" and it will ignore the param

//Make sure to call this on a new savefile details struct otherwise you can get strange results if
	//you use it without this. The last two parameters are function pointers to user defined default
	//values and how to handling going up a version
//Note that you should also add the icon/eyecatcher if you want and set all the strings as well as
	//the variable history.
uint8_t crayon_savefile_init_savefile_details(crayon_savefile_details_t *details, const char *save_name,
	crayon_savefile_version_t latest_version, void (*default_values_func)(),
	int8_t (*update_savefile_func)(void**, crayon_savefile_version_t, crayon_savefile_version_t));

#define crayon_savefile_set_app_id(details, string) \
crayon_savefile_set_string(details, string, CRAY_SF_STRING_APP_ID);

#if defined(_arch_dreamcast)

#define crayon_savefile_set_short_desc(details, string) \
crayon_savefile_set_string(details, string, CRAY_SF_STRING_SHORT_DESC);
#define crayon_savefile_set_long_desc(details, string) \
crayon_savefile_set_string(details, string, CRAY_SF_STRING_LONG_DESC);

#else

#define crayon_savefile_set_short_desc(details, string) 0;
#define crayon_savefile_set_long_desc(details, string) 0;

#endif

//The return value is 1 when the number of icons is greater than 3. The DC BIOS can't render icons
	//with 4 or more frames.
uint8_t crayon_savefile_set_icon(crayon_savefile_details_t *details, const char *image, const char *palette,
	uint8_t icon_anim_count, uint16_t icon_anim_speed);

uint8_t crayon_savefile_set_eyecatcher(crayon_savefile_details_t *details, const char *eyecatch_path);

//Return type is the id of the variable. Returns the id, but will also return 0 on error
//Note that if a variable still exists, for version_removed we set it to the latest version + 1
uint32_t crayon_savefile_add_variable(crayon_savefile_details_t *details, void *data_ptr, uint8_t data_type, 
	uint32_t length, crayon_savefile_version_t version_added, crayon_savefile_version_t version_removed);

//Once the history is fully constructed, we can then build our actual savefile with this fuction
uint8_t crayon_savefile_solidify(crayon_savefile_details_t *details);

//Returns 0 on success and 1 or more if failure. Handles loading and saving of uncompressed savefiles
int8_t crayon_savefile_load_savedata(crayon_savefile_details_t *details);
int8_t crayon_savefile_save_savedata(crayon_savefile_details_t *details);

//This will delete the saved savefile 
uint8_t crayon_savefile_delete_savedata(crayon_savefile_details_t *details);	//UNFINISHED. INCOMPLETE

void crayon_savefile_free(crayon_savefile_details_t *details);	//Calling this calls the other frees
void crayon_savefile_free_base_path();

#endif
