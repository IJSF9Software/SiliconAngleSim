# SiliconAngleSim

## Building
### Requirements
 * [CMake](http://cmake.org) (3.0 or later)
 * [yaml-cpp](https://github.com/jbeder/yaml-cpp) (current master)
 * [ROOT 6](https://root.cern)
 * [KDetSim](https://github.com/IJSF9Software/KDetSim) (master CMake build from GitHub)

### Building on lxplus
```
lsetup "cmake 3.7.0"
lsetup "root"
export PATH=$PATH:/afs/cern.ch/work/t/tadej/public/root/bin
mkdir build && cd build
CC=`which gcc` CXX=`which g++` cmake ..
make
make install
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
