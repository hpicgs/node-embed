# node-embed [![Build Status](https://travis-ci.org/hpicgs/node-embed.svg?branch=master)](https://travis-ci.org/hpicgs/node-embed)

## Purpose of this repository

This repository aims at demonstrating the usage of the shared library API in Node.js' implementation as of version 9.0.0, as well as the new, extended implementation of said API proposed in [this Node.js fork](https://github.com/hpicgs/node). This repository is a work in progress in the "Advanced Development in C++" project seminar at Hasso Platter Institute's Chair of Computer Graphics Systems.

## Dependency Setup

Make sure you have the Qt Version 5.6 or newer installed from [here](https://download.qt.io/official_releases/qt/).

Clone this repository:
```
git clone git@github.com:hpicgs/node-embed.git
```

For all of the examples to work, you will need to clone node.js from [here](https://github.com/hpicgs/node) into the ```externals``` folder in this repo's root directory and build it:
```
mkdir externals
cd externals
git clone git@github.com:hpicgs/node.git
cd node-v9.0.0
./configure --shared
make -j4
cd ../..
```
Alternatively, if you have built the shared libraries before, you can copy them into ```externals/node-v9.0.0/Release/```. If you only want to try out the old API example, you can also build node from [here](http://nodejs.org/dist/v9.0.0/node-v9.0.0.tar.gz).

Next, download the headers needed to include the Node.js shared libraries and place them in the node directory:
```
cd externals
# MacOS: use curl instead of wget
wget http://nodejs.org/dist/v9.0.0/node-v9.0.0-headers.tar.gz
tar -xzvf node-v9.0.0-headers.tar.gz
cd ..
```
Install the required Node.js modules by running: ```npm install```.

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

### Mac

```
cmake . -DCMAKE_PREFIX_PATH=[QT INSTALL DIR]/[QT_VERSION]/clang_64
make
```

## Running the application

### Windows
```
./node-embedd-qt.exe ../source/qt-app/rss_feed.js
./node-embedd-cli.exe
```

### Linux
```
./node-embed-qt ../source/qt-app/rss_feed.js
./node-embed-cli
```

### MacOS
```
./node-embed-qt.app/Contents/MacOS/node-embed-qt  PATH/TO/node-embed/source/qt-app/rss_feed.js
./node-embed-cli.app/Contents/MacOS/node-embed-cli
```
