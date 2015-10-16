# csci441-midterm
Author: Chris Butler, Nico Pampe

Midterm project. 3 heros. So many more hours.

Usage
=====

Mouse input:
    Right clicking brings up a menu. There are three main options {Switch
    Camera, Switch Hero, Quit}. The two switching option bring up a submenu.
    There are 4 options for the camera, free camera, fast free camera, arc
    ball camera, and first person. Finally, for the Switching hero option,
    selecting the heros name will change the active hero for the cameras 
    to follow.

    Left clicking will change the cameras view angle only for the free camera
    and arc ball camera.

    Left clicking + Ctrl zooms the arc ball camera in and out.

Keyboard input:
    Esc = closes the program
    W = moves either Incallids or the free camera forward.
    S = moves either Incallids or the free camera backward.
    A = moves either Incallids or the free camera left.
    D = moves either Incallids or the free camera right.
    Q = moves the free camera up.
    E = moves the free camera down.
    1-5 controls active camera
    SpaceBar channels the power of the shout.

File Format
===========

Our format uses [YAML](http://yaml.org/). See the website for a description of how it works. It's convient to think of it as white-space away JSON.

Our World files, located in `assets/world/*.yaml` are designed to be read and written by humans.
Labels:
        -WorldCPoints: Describes the surfaces making up our world
                -numberOfCurvers: (sic) How many curves are in this file. Our examples only have one.
                -numberofPoints: How many points we have total. They come in chucks of 16-and-overlapping-by-1. We never have more than 1 though.
                -points: A list of length-3 lists, representing x, y, and z. This length should be the same as above.

        -Trees:
                -numberOfTrees: A count for how many trees we have. Should be the same as the length of the next item.
                -points: A list of length-2 lists, representing the x and z coordinates of the trees. Their heights are determiend by the curve(s) described above.

        -FlagBanner: The proud flag of the Stormcloaks
                -numberOfFlags: Self explanatory at this point.
                -points: locations of the flags, again in x, z.
                -windPoints: ??? Nico, fill this out
                
        -Track: Describes the track for two heros to race around on (IN THE SKY!)
                -numberofPoints: Self explanatory at this point.
                -points: [x, y, z] of the control points describing the Bezier Curve that is the track.
                    
FMOD is pretty flexibile about loading files, so we don't have any kind of rigor for loading audio. Just some mp3s ripped from YouTube in `assets/audio/*.mp3`.

Notes
=====

3rd Party Code: All of our third party code is under ext. yaml-cpp and tinyformat as in there as git submodules. boost is dropped in (it's BIG).

1. yaml-cpp [github](https://github.com/jbeder/yaml-cpp/tree/03d6e7d67271be698d9a2fd51f749201649e0223)
2. Boost (needed by yaml-cpp) [boost](http://www.boost.org/)
3. tinyformat [github](https://github.com/c42f/tinyformat/tree/fd72f9cf17e4b12cbca932049a17badc7af6718b)

Running
-------

All of our paths are relative and expect the `assets` folder to be in the working directory of the executable.

Building
--------

Building is **slow**. We have almost 50 files, including yaml's. Unless you abuse `make`'s `-j` command, we certainly did, it should take you almost a minute to build. Sorry.

Our project has a rather involved build process. We have two systems: A Unix-friendly Makefile and a cross-platform CMake file. We had issues getting it to build on the CTLM machines - it was a struggle with things like 'make' not being there, instead listed as 'gmake' and it rejecting the library files we tried to use for fmod. It works fine in a Visual Studio project generated from CMake, provided you set all of the variables to place it correftly. (e.g. `BOOST_ROOT`, `FMOD_LIBRARIES`, etc. See further down for details.) Please, please let us know if you have any issues building. It's a fairly complicated project and building it is somewhat delicate.

Our **Makefile** has lots of hard-coded directories, but basically expects the following directories under ext: fmod, yaml, tinyformat, and boost. We'll run over each briefly.

fmod - Audio library. You're the one who told us about it, so you probably know more than we do. In particular, we use the lowlevel library so the Makefile expects ext/fmod/inc and ext/fmod/lib to exist and be populated with the appropriate files.

yaml - YAML is a file format designed to be used for configuration files. It's like XML, JSON, or INI, but easier to read. It delimits with whitespace. We use yaml-cpp from GitHub to parse files and saved a ton of time. Building this requires CMake. There is no way around it. Our makefile will call CMake for you, using your default generator. It should produce a lib_yaml-cpp.a under object/yaml which is thien linked against.

tinyformat - Header file in a GitHub repo which provides a printf-like interface to std::cout and std::cerr. We use it mostly for logging and debugging. It shouldn't cause problems.

boost - A major C++ library with over a third of a million lines of code. It sucks up something like 100 MB. We're sorry. yaml-cpp uses it and we can't really get around it. It's (mostly) a header-only library and doesn't require any compiling. It just needs to be there for yaml.

Our **CMake** file makes it easy to generate different types of projects for different computers. We used it to get Visual Studio and Sublime Text projects with  [Ninja](https://martine.github.io/ninja/) build scripts. Ninja was super helpful because it was *fast*. Ninja rarely broke 9 seconds even with 51 files (half of which came from yaml). Our Makefile took over 50 seconds without `-j` and 11 seconds with `-j32`. Here's a sample to get you rolling if you start in the root of our source directory. It's important for optimizations to be turned on - FPS drops sizeably without them. 

```
mkdir build
cd build
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DAUTO_FORMAT=ON \
  -DBOOST_ROOT=$PWD/../ext \
  -DFMOD_INCLUDE_DIRS=$PWD/../ext/fmod/inc \
  -DFMOD_LIBRARY=$PWD/../ext/fmod/lib/libfmod.a
```

Be sure to change the FMOD_LIBRARY option at the end if you build for something that's not gcc.

Bugs
====

Anything involving our Bezier Surface probably has a bug. Same for the curve too. We just didn't get a solid implementation done. It works *mostly*.


