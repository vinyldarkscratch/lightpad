# Lightpad
Software Designed, Developed, and Published by [Nightwave Studios](https://www.nightwave.co) (Vinyl Darkscratch, Light Apacha), © 2017 under a GNU General Public License.
Additional support from [LaunchpadFun](http://www.launchpadfun.com/en/).

# This software is almost ready for use.  Please check back later!

## Build and Installation
Lightpad has four executables: _main_, _timeline_, _midiprobe_, and _midiout_.  _Main_ is just what it sounds like, it's the main application, bringing together all of it's features.  _Timeline_ is a teast program to demo the timeline on it's own.  _Midiprobe_ and _midiout_ focus specifically on MIDI connectivity, with the first providing a list of available MIDI I/O ports, and the second playing a test file to the Launchpad, while also telling a pressed button to pulse a random color.

```bash
mkdir build
cd build
cmake .. && make -j8 && make install
../bin/lightpad[.exe]
../bin/timeline[.exe]
../bin/midiprobe[.exe]
../bin/midiout[.exe]
```

## Features
(Work In Progress List)

- Fast switching between projects and files
- Looping animation playback
- On-screen Launchpad to emulate animations
- Connectivity with a physical Launchpad
- RGB color to closest velocity match
- Automatic saving
- Integration with [Exige's MIDI Extension](http://forum.launchpad-pro.com/viewtopic.php?pid=35098)
- Export to a new Ableton Live project
- *More to come!*

## Compatibility
The software is mainly developed on a mid-2016 MacBook Pro, 2.7GHz Intel i7 with Integrated Graphics and a Radeon Pro 460 (4GB VRAM).

Supported Operating Systems:

* Mac OS [![Build Status](https://travis-ci.org/vinyldarkscratch/lightpad.svg?branch=master)](https://travis-ci.org/vinyldarkscratch/lightpad)
* Windows (broken at the moment)
* Linux/Unix (implementing)
