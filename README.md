# Neurocid
#### A space ship simulation controlled by neural networks, evolved through a genetic algorithm

### Prerequisites
Installing **development** packages for following libraries is mandatory: 

    pkg-config, boost-system, boost-program-options, SDL-gfx, sdl, SDL-image, SDL-ttf

Following libraries can be disabled by prefixing make with WITHOUT_VIDEOENC=1 (e.g WITHOUT_VIDEOENC=1 make):

    libavformat, libavcodec, libswscale, libavutil

Following libraries can be disabled by prefixing make with WITHOUT_SERIALIZE=1:

    boost-serialization

On Debian/Ubuntu you can install required packages like this (Video encoding not supported):

   apt-get install pkg-config libsdl1.2-dev libsdl-gfx1.2-dev libsdl-image1.2-dev libsdl-ttf2.0-dev libboost-system-dev libboost-program-options-dev libboost-serialization-dev
 
On Opensuse 13.1

    sudo zypper install libSDL-devel libSDL_image-devel libSDL_gfx-devel boost-devel libffmpeg-devel

**NOTE:** Because of troubles with different ffmpeg implementations video encoding is not supported on debian/ubuntu. don't forget to use WITHOUT_VIDEOENC

### Build
if you have problems with the ffmpeg support you may disable video capturing functionality entirely by prefixing make with WITHOUT_VIDEOENC

    git clone https://github.com/kallaballa/Neurocid.git
    cd Neurocid
    ./buildDeps.sh
    make
    sudo make install
    
### Run
In the examples directory there are two shell scripts that are examples of how to run neurocid to train and watch space ship populations.
* basic_training.sh - attempts to produce a well fighting population in a short time.
* play.sh - loads a population and multiplies it 5 times to run it it in a big scenario.

Please have a look at Hotkeys.txt to learn how to control the simulation

**NOTE:** You might have to tweak hardcoded parameters in the scripts (e.g. resolution).

    
