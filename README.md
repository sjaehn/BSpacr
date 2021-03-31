# B.Spacr

Description: LV2 sound effect plugin

B.Spacr is a unique LV2 effect plugin that enables a clear and brilliant audibility of your music production.
B.Spacr is suited for **any** kind of music, including rock, funk, pop, rap, and electronic music.
This plugin can be added to each track individually or to the master bus with **zero latency**.
The result is a space-clear sound **without any loss** of audio signal information and without
any artifacts only depending on the quality of the input signal. In contrast to many over-complex
audio plugins, the number of parameters have been reduced to the minimum for the **best user experience**.

## Installation

Build your own binaries in the following three steps.

Step 1: [Download the latest published version](https://github.com/sjaehn/BSpacr/releases) of B.Spacr. Or clone or
[download the master](https://github.com/sjaehn/BSpacr/archive/master.zip) of this repository.

Step 2: Install pkg-config and the development packages for x11, cairo, and lv2 if not done yet. If you
don't have already got the build tools (compilers, make, libraries) then install them too.

On Debian-based systems you may run:
```
sudo apt-get install build-essential
sudo apt-get install pkg-config libx11-dev libcairo2-dev lv2-dev
```

On Arch-based systems you may run:
```
sudo pacman -S base-devel
sudo pacman -S pkg-config libx11 cairo lv2
```

Step 3: Building and installing into the default lv2 directory (/usr/local/lib/lv2/) is easy using `make` and
`make install`. Simply call:
```
make
sudo make install
```

**Optional:** Standard `make` and `make install` parameters are supported. Compiling using `make CPPFLAGS+=-O3`
is recommended to improve the plugin performance. Alternatively, you may build a debugging version using
`make CPPFLAGS+=-g`. For installation into an alternative directory (e.g., /usr/lib/lv2/), change the
variable `PREFIX` while installing: `sudo make install PREFIX=/usr`. If you want to freely choose the
install target directory, change the variable `LV2DIR` (e.g., `make install LV2DIR=~/.lv2`).


## Running

After the installation Ardour, Reaper, Qtractor, Zrythm, Carla, and any other LV2 host should automatically
detect B.Spacr.

If jalv is installed, you can also call it using one of the graphical jalv executables (like
jalv.gtk, or jalv.gtk3, or jalv.qt4, or jalv.qt5, depending on what is installed), like

```
jalv.gtk https://www.jahnichen.de/plugins/lv2/BSpacr
```

to run it stand-alone and connect it to the JACK system.
