# node-embed [![Build Status](https://travis-ci.org/hpicgs/node-embed.svg?branch=master)](https://travis-ci.org/hpicgs/node-embed)

## Purpose of this repository

This repository aims at demonstrating the usage of the shared library API in Node.js' implementation as of version 9.0.0, as well as the new, extended implementation of said API proposed in [this Node.js fork](https://github.com/hpicgs/node). This repository is a work in progress in the "Advanced Development in C++" project seminar at Hasso Platter Institute's chair of Computer Graphics Systems.

## Dependency Setup

### Qt

Make sure you have the Qt Version 5.6 or newer installed from [here](https://download.qt.io/official_releases/qt/).

### Node.js

Clone this repository:
```
git clone git@github.com:hpicgs/node-embed.git
```

For all of the examples to work, you will need to clone node.js from [here](https://github.com/hpicgs/node) into the ```externals``` folder in this repo's root directory and build it:
```
mkdir externals
cd externals
git clone git@github.com:hpicgs/node.git node
cd node
./configure --shared
make -j4
cd ../..
```

### Node.js modules

Install the required Node.js modules by running: ```npm install```.

### cpplocate

Install [cpplocate](https://github.com/cginternals/cpplocate.git). Here's how it could look like:

```
cd /tmp
git clone https://github.com/cginternals/cpplocate.git
cd cpplocate
mkdir build
cd build
cmake ..
cmake --build .
sudo make install
```

## Building the project

We use cmake to build this project. If you want, you can build it in a dedicated ```build``` directory, but beware that the cli-app has to be run from the repo's root directory to work completely.

### Windows

Make sure you have Qt installed for the Visual Studio compiler.

```
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_PREFIX_PATH='C:\Qt\Qt[VERSION]\[VERSION]\msvc2017_64\lib\cmake' .
cmake --build .
```

### Linux

```
cmake . -DCMAKE_PREFIX_PATH=[QT INSTALL DIR]/[VERSION]/gcc_64/lib/cmake/Qt5
make
```

Alternatively, use the provided `./configure` script.

### Mac

```
cmake . -DCMAKE_PREFIX_PATH=[QT INSTALL DIR]/[QT_VERSION]/clang_64
make
```

## Running the application

From the repository's root directory, run:

### Windows
```
./node-qt-rss.exe
./node-lib-qt-rss.exe
./node-lib-cli.exe
```

### Linux
```
./node-qt-rss
./node-lib-qt-rss
./node-lib-cli
```

### MacOS
```
./node-qt-rss.app/Contents/MacOS/node-qt-rss
./node-lib-qt-rss.app/Contents/MacOS/node-lib-qt-rss
./node-lib-cli.app/Contents/MacOS/node-lib-cli
```
