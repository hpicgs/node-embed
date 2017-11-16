# node-embed

## Setup

### Mac

* Set Environment Variables:
    - CMAKE_PREFIX_PATH must point to your Qt directory (e.g. /Users/[USRNAME]/Qt/[QT-VERSION]/clang_64)

### Windows

Make sure you have Qt installed for the Visual Studio compiler.

### Linux (Ubuntu 17.04)

* download the Node.js headers:
```
cd externals
wget http://nodejs.org/dist/v9.0.0/node-v9.0.0-headers.tar.gz
tar -xzvf node-v9.0.0-headers.tar.gz
cd ..
```
* either copy Node libs to externals or build it from source:
```
cd externals
wget http://nodejs.org/dist/v9.0.0/node-v9.0.0.tar.gz
tar -xzvf node-v9.0.0.tar.gz
cd node-v9.0.0
./configure --shared
make -j4
cp out/Release/lib.target/libnode.so.59 ../libnode.so.59
cp out/Release/lib.target/libnode.so.59 ../libnode.59.so
cd ../..
```
* install the required Node modules in the project root with: `npm install feedparser request`
* then either just open CMakeLists.txt with QtCreator, configure it and press run
* or start from terminal with the following commands:

```
# build project:
CMAKE_PREFIX_PATH=~/Qt/5.10.0/gcc_64/lib/cmake/Qt5 cmake .
make -j6

# run executable:
./node-embed source/demo-app/hello_world.js
```
