# node-embed

## Setup

### Mac

* Set Environment Variables:
    - CMAKE_PREFIX_PATH must point to your Qt directory (e.g. /Users/[USRNAME]/Qt/[QT-VERSION]/clang_64)

### Windows

Make sure you have Qt installed for the Visual Studio compiler.

### Linux (Ubuntu 17.04)

* install the required Node modules in the project root with: `npm install feedparser request`
* then either just open CMakeLists.txt with QtCreator, configure and press run
* or start from terminal with the following commands:

```
# build project:
CMAKE_PREFIX_PATH=~/Qt/5.10.0/gcc_64/lib/cmake/Qt5 cmake .
make -j6

# run executable:
./node-embed source/demo-app/hello_world.js
```
