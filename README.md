# Crayon-Savefile
A savefile system designed to take up little space and support version control. Works on PC and Dreamcast

The new version of the savefile system supports multiple versions of your savedata, aka backwards compatibility. This means you can add new variables, remove old ones and control what happens to the values from old variables when loading an older savefile.

If you make a savefile in version X, then it will load in version X + Y (Where Y is a positive number), but a savefile from version X won't load in version X - Y (Where Y is a positive number and X - Y >= 0).

### Savefile structure

All versions have an app_id in the header as a way to identify what program the savefile belongs to.

The body is the version number (32 bit unsigned value by default) followed by the user's variables. There isn't any padding or wasted space, only the user's variables.

The program will know how to handle older savefiles using its history data list, this is created by the user when they "add" variables to the savefile and later solidify it.

setup.c's `setup_savefile` function shows how the user might construct a savefile with backwards compatibility for older versions. You can find a copy of the pc savefiles for all versions of this savefile in `pc_savefile_backups` including an example of a savefile that's too new (When you try loading a newer savefile on an old version of your program)
