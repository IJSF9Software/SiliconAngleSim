# SiliconAngleSim

SiliconAngleSim is a simple software package to simulate silicon detectors for
tracks traversing at different angles. It was written as a part of my Master's
thesis. Currently it supports strip detectors which are easily configured by a
configuration file (see [examples](examples) and below for more information).


## Usage
The only mandatory argument is a config file. By default, each run will
calculate initial signal, generate events and make basic plots in a `root` file.
```
$ SiliconAngleSim examples/angle-thickness.yaml
```
For details see `SiliconAngleSim --help`.

### lxplus
SiliconAngleSim is prebuilt on lxplus. Run the following setup script which will
setup proper paths. You need to setup ROOT 6 manually!
```
$ source /afs/cern.ch/work/t/tadej/public/root/bin/setup_SiliconAngleSim.sh
```

### Configuration
All configuration is done in `yaml` files. Properties should be mostly self
explanatory. All default values can be found in
[examples/default.yaml](examples/default.yaml).

The main nodes are:
 * `name` - configuration name
 * `demo` - run in demo mode to plot some initial plots (will force one event)
 * `detector` - detector parameters
   (pitch, strip-width, thickness, noise, fluence, average-field, neff, diffusion)
 * `simulation` - simulation parameters (steps, threshold, angle, offset)
 * `variations` - specify variations of parameters by list or equidistant steps
   (overrides previously set initial parameters)
   ```
   variations:
     - type: angle
       start: 0
       end: 1.0471975512
       steps: 7

     - type: fluence
       list:
         - 5e14
         - 1e15
         - 2e15
         - 5e15
   ```
 * `plot` - plot settings (min/max on y-axis, legend position)


## Building
### Requirements
 * [CMake](http://cmake.org) (3.1 or later)
 * [yaml-cpp](https://github.com/jbeder/yaml-cpp) (current master)
 * [ROOT 6](https://root.cern)
 * [KDetSim](https://github.com/IJSF9Software/KDetSim) (master CMake build from GitHub)

### Building
It is recommended to build SiliconAngleSim in a separate build directory.
ROOT location will be detected automatically if `ROOTSYS` environment variable
is set or by defining `-DROOTSYS=/some/path` when configuring with CMake.
Other dependencies (yaml-cpp, KDetSim) should be accesible in `PATH` to be
successfully found.
```
$ mkdir build && cd build
$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
$ make
$ make install
```

### Building on lxplus
I provide latest builds on lxplus at
```
/afs/cern.ch/work/t/tadej/public/root/bin
```

If you need to build it on your own, a newer version of CMake and ROOT needs
to be selected. Also compiler might not be detected properly. The whole process
is summarised below:
```
$ lsetup "cmake 3.7.0"
$ lsetup "root"
$ export PATH=$PATH:/afs/cern.ch/work/t/tadej/public/root/bin
$ mkdir build && cd build
$ CC=`which gcc` CXX=`which g++` cmake ..
$ make
$ make install
```


## Copyright info

Copyright (C) 2016 Tadej Novak

SiliconAngleSim is free (libre) software. This means that the application
source code is available to public, anyone is welcome to research how
the application works, participate in its development, freely distribute
the application and spread the word!

This project may be used under the terms of the
GNU General Public License version 3.0 as published by the
Free Software Foundation and appearing in the file [LICENSE.md](LICENSE.md).
