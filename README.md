# Crayon-Savefile
A savefile system designed to take up little space and support version control. Works on PC and Dreamcast

The new version of the savefile system supports multiple versions of your savedata, aka backwards compatibility. This means you can add new variables, remove old ones and control what happens to the values from old variables when loading an older savefile.

If you make a savefile in version X, then it will load in version X and beyond, but a savefile from version X won't load in a program who believes a version before X is the latest (ie. Using a savefile from the latest build with an older version of the program). Also note the first savefile version is 1, not 0 since 0 is internally used for no or corrupted savefile.

### Repository structure

- Demo folder contains some demos of the program at each version of its life.
- The libCrayonSavefile folder contains the source code for the savefile system and allows you to build a library file that can be linked into any project

### Savefile structure

All versions have an app_id in the header as a way to identify what program/game the savefile belongs to.

The body is the version number (32 bit unsigned value by default) followed by the user's variables. There isn't any padding or wasted space, only the user's variables.

The program will know how to handle older savefiles using its history data list that is created when the user "add"s variables to the savefile and later solidify it.

### Other

Special thanks to BlueCrab for showing me a more detailed example of savefiles on the Dreamcast than other examples I had seen, MrNeo240 for helping me understand the Dreamcast Savefile icon and eyecatcher bitmap formats and thanks to MrNeo240 and Lerabot for using the old savefile system and giving advice for improvements.
