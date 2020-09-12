#ifndef CRAYON_H
#define CRAYON_H

// This file contains functions and defines normally found in crayon

#if defined(__APPLE__) || defined(__linux__) || defined(_WIN32)
	#define _arch_pc
#endif

#if !(defined(_arch_dreamcast) || defined(_arch_pc))
	#error "UNSUPPORTED ARCHITECTURE/PLATFORM"
#endif

#ifndef CRAYON_DEBUG

#define CRAYON_DEBUG 0

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> // For the uintX_t types

#ifdef _arch_dreamcast

#include <dc/maple/vmu.h>
#include <dc/vmu_pkg.h>
#include <dc/vmufs.h>
#include <kos/fs.h>

#endif

// Checks if the computer running this code is big endian or not
uint8_t crayon_misc_is_big_endian();

void crayon_misc_endian_correction(uint8_t *buffer, size_t bytes);	// UNFINISHED. WIP

void crayon_misc_encode_to_buffer(uint8_t *buffer, uint8_t *data, size_t bytes);

// Only used for port/slot in dreamcast. x is port, y is slot
typedef struct vec2_s8{
	int8_t x, y;
} vec2_s8_t;

vec2_s8_t crayon_peripheral_dreamcast_get_port_and_slot(int8_t savefile_device_id);

// Returns a bitmap of all vmus with a screen (a1a2b1b2c1c2d1d2)
uint8_t crayon_peripheral_dreamcast_get_screens();

void crayon_peripheral_vmu_display_icon(uint8_t vmu_bitmap, void *icon);

// 0 if the peripheral doesn't have the function, 1 if it does
// Kind of a Dreamcast-specific function, but for later console ports this might be reusable
uint8_t crayon_peripheral_has_function(uint32_t function, int8_t save_device_id);

#endif
