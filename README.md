# Crayon-Savefile
A savefile system designed to take up little space and support version control. Works on PC and Dreamcast

The new version of the savefile system supports multiple versions of your savedata, aka backwards compatibility. This means you can add new variables, remove old ones and control what happens to the values from old variables when loading an older savefiles.

If you make a savefile in version X, then it will load in version X and beyond, but a savefile from version X won't load in a program who believes a version before X is the latest (ie. Using a savefile from the latest build with an older version of the program). Also note the first savefile version is 1, not 0 since 0 is internally used for no or corrupted savefiles.

### Repository structure

- The libCrayonSavefile folder contains the source code for the savefile system and allows you to build a library file that can be linked into any project
- Demo folder contains some demos of the program at each version of its life. You can also build the library from there

### Savefile structure

All versions have an `app_id` in the header as a way to identify what program/game the savefile belongs to.

The body is the version number (32 bit unsigned value by default) followed by the user's variables. There isn't any padding or wasted space, only the user's variables.

The program will know how to handle older savefiles using its history data list that is created when the user "add"s variables to the savefile and later "solidify"s it.

### Dependencies

Every platform has scons as a dependancy, installing it is decribed below.

Dreamcast depends on "KallistiOS". Setup tutorials can be found here
- http://gamedev.allusion.net/softprj/kos/setup-wsl.php
- http://gamedev.allusion.net/softprj/kos/setup-linux.php
- http://gamedev.allusion.net/softprj/kos/setup-osx.php

For Dreamcast you'll also need cdi4dc. Download and build this program
- https://github.com/Kazade/img4dc/tree/master/cdi4dc

Copy the executable into one of your path folders, this will allow the cdi4dc program to be callable from anywhere on your system.


### Building/SConstruct

In order to build the library and the Demos, you need to use the SConstruct files. If you're familiar with Makefiles, then they are similar in the sense that they are both build languages. If you have the apt package manager use the following command/s to install SConstruct and the necessary Python stuff:

`sudo apt-get install python3 python3-pip && pip3 install scons`

To build the library file go to "libCrayonSavefile" and type `scons PLATFORM=all DEBUG=0` to build both Dreamcast and one of the PC builds (Depending on your compiler it will build either the Windows, Mac or Linux files). If you only want to build for one of the platforms, you can instead choose `PLATFORM=dreamcast` or `PLATFORM=pc`. The SConstruct file in the demos are called the same way too.

### Related Software

For the Dreamcast side, I have some tools for creating eyecatchers, savefile icons and the VMU LCD icons. You can find them at https://github.com/Protofall/Crayon-Utilities.git as well as more information on their size requirements and other details.

### Thanks

Special thanks to BlueCrab for showing me a more detailed example of savefiles on the Dreamcast than other examples I had seen, MrNeo240 for helping me understand the Dreamcast Savefile icon and eyecatcher bitmap formats and thanks to MrNeo240 and Lerabot for using the old savefile system and giving advice for improvements.
