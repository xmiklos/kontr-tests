# Installation

There are two types of installation: **full** and **editing-only**. 

The difference is that **editing-only** is intended to be installed on developers' machine (and provide hinting in the editor), while the **full** version requires a bit more and can provide you with additional features (such as backtraces). Therefore you may edit on a machine with hinting (and not compile it there) and compile it somewhere else (where you have the full version).

Modern compiler is required (tested with gcc 4.8, known not to work properly with gcc 4.7). A editor with good **macros support** is recomended (I develop in QtCreator - current version is 3.0.0, but it has some [drawbacks](https://bugreports.qt-project.org/browse/QTCREATORBUG-11243), so the version [2.8.1](http://download.qt-project.org/official_releases/qtcreator/2.8/2.8.1/) is a good way to go). For a full version [CMake](http://www.cmake.org/) with version is at least 2.8 is required.

## Editing-only

To use the editing-only version *just clone the repository*:
``` git clone https://github.com/xbrukner/kontr-tests.git```

Then you just need to ```#include "kontr.h"``` in your project (see [Usage](usage.md)).

In order to update this installation just update the repository (```git pull```);

## Full version

To install the full version:
```Bash
git clone --recursive https://github.com/xbrukner/kontr-tests.git
cd kontr-tests
cmake .
make
make test
```

If the tests runs correctly, you have a supported environment!

To update this installation, issue these commands:
```Bash
git pull
git submodule update
```

Installation complete? Continue to [Usage](usage.md).
