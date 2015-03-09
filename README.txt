//############################################################################
//
// LaserBoy !!!
//
// by James Lehman
// Extra Stimulus Inc.
// james@akrobiz.com
//
// began: October 2003
//
// Copyright 2003, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 2015 James Lehman.
// This source is distributed under the terms of the GNU General Public License.
//
// README.txt (this file) is part of LaserBoy.
//
// LaserBoy is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// LaserBoy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with LaserBoy. If not, see <http://www.gnu.org/licenses/>.
//
//############################################################################

If your computer is Internet connected, running LaserBoy
will make a standard HTTP request to laserboy.org to check
for the latest version found here:

http://laserboy.org/forum/index.php?topic=10.0

//############################################################################

To run LaserBoy,

In Windows,

edit the file LaserBoy.bat so that it calls the executable
(LaserBoy.exe) with the two arguments following that are the
width and the height in pixels of the window that LaserBoy
will open and populate. Width can be the width of your display.
Height should be at least 30 pixels less than your display height,
to allow for the window title bar.

Start LaserBoy by double clicking on LaserBoy.bat inside of the
LaserBoy folder.

//############################################################################

In Linux,

unzip the LaserBoy distribution in the location of your choice.
navigate to inside of LaserBoy/src/ and issue the make command
to build LaserBoy from source files.

If you get the following linker error:

undefined reference to symbol 'pthread_getspecific...

Opened Makefile in src and changed this line:

LIBS = -lSDL -lboost_system -lboost_filesystem

by adding -lpthread to the end like this:
 
LIBS = -lSDL -lboost_system -lboost_filesystem -lpthread

Save Makefile and make again.

cd ../ to the root of the LaserBoy directory to find the
executable file, LaserBoy.

it is possible to run LaserBoy from a terminal, if the kernel
is compiled for Linux Frame Buffer support.

From inside the LaserBoy directory, issue the command:

[root@home ~]# fbset -depth 32 -s

To put the frame buffer in 32 bits per pixel color mode and
display the geometry; [width] [height] in pixels of the display.

Start LaserBoy:
[root@home ~]# ./LaserBoy [width] [height]

where [width] [height] are the actual values displayed by
calling fbset.

It is also possible to run LaserBoy in Xfree86 in much the
same way; by calling it from a terminal from the LaserBoy
directory:

[root@home ~]# ./LaserBoy [width] [height]

where [width] [height] are the dimensions of the window
that it will open.

//############################################################################

In Mac OSX,

1. Install xcode.
2. Install MacPorts.
3. Install SDL through MacPorts.
4. Install boost through MacPorts.
5. Go to the LaserBoy src folder in a terminal and type:

[root@home ~]# make -f Makefile.osx

cd ../ to the root of the LaserBoy directory to find the
executable file, LaserBoy. Issue the command:

[root@home ~]# ./LaserBoy [width] [height]

where [width] [height] are the dimensions of the window
that it will open in decimal.

//############################################################################

LaserBoy is compiled for Windows with:

MinGW with GCC 4.7.2
http://www.mingw.org/

libSDL version 1.2.15.0
http://www.libsdl.org/

boost C++ version 1.53.0
http://www.boost.org/

using Dev-C++ dev environment
http://www.bloodshed.net/devcpp.html

//############################################################################

The Simple DirectMedia Layer (SDL for short) is a cross-platform library
designed to make it easy to write multi-media software, such as games and
emulators.

The Simple DirectMedia Layer library source code is available from:
http://www.libsdl.org/

This library is distributed under the terms of the GNU LGPL license:
http://www.gnu.org/copyleft/lesser.html

//############################################################################

Enjoy!

