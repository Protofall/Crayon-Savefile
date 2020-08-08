# Version differences

Here we have a copy of the savefile program for each version of our savefile.

### v1

The first version. Adds a bunch of variables:

- `uint16_t coins`
- `float var2`
- `uint8_t dunno`
- `var4[10]`
	- `uint8_t lol`
	- `int32_t hi[2]`
	- `char name[16]`

Note that `var4` doesn't exist in the normal sense, think of it as a struct that contains the following 3 variables and we have 10 of that struct. So all together we have

- 1 float
- 20 signed 32-bit numbers
- 1 unsigned 16-bit number
- 11 unsigned 8-bit numbers
- 160 chars

### v2

This version adds the following variables

- `uint8_t myspace`
- `double speedrun_times[2]`
- `float garbage[128]`

Note that `garbage` is 512 bytes long or 1 block on Dreamcast so the savefile will now be bigger.

So now our total vars are

- 2 doubles
- 129 float
- 20 signed 32-bit numbers
- 1 unsigned 16-bit number
- 12 unsigned 8-bit numbers
- 160 chars

### v3

A few variables are removed, and one variable is added. We forget the value of `myspace`, use the values from `hi` as a part of `lol` and the old `coins` is now stored in our new coins variable so we can store more coins (Former was `uint16_t`, latter is `uint32_t`). `garbage` is removed. It took 512 bytes which is 1 Block exactly on Dreamcast so now the savefile will be a block smaller.

We also add 5 new variables all of length one with types `float`, `int32_t`, `uint16_t`, `int16_t`, `int8_t` in that order. The new float is to show we can delete a variable (`garbage`) from any position in history, not just the first or last added of that type. The rest are to show that those types can be handled in general

Total vars
- 2 doubles
- 2 float
- 1 unsigned 32-bit number
- 1 signed 32-bit number
- 1 unsigned 16-bit number
- 1 signed 16-bit number
- 11 unsigned 8-bit numbers
- 1 signed 8-bit number
- 160 chars

### v4

We just add a new char called `sf_fave_letter`. Nothing special

Total vars
- 2 doubles
- 2 float
- 1 unsigned 32-bit number
- 1 signed 32-bit number
- 1 unsigned 16-bit number
- 1 signed 16-bit number
- 11 unsigned 8-bit numbers
- 1 signed 8-bit number
- 161 chars