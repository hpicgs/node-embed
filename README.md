# node-embed

## Setup

* download the Node.js headers:
```
mkdir externals
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


### Building the project

#### Windows

Make sure you have Qt installed for the Visual Studio compiler.

```
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_PREFIX_PATH='C:\Qt\Qt5.9.3\5.9.3\msvc2017_64\lib\cmake' .
cmake --build .
```

#### Linux

```
CMAKE_PREFIX_PATH=~/Qt/5.10.0/gcc_64/lib/cmake/Qt5 cmake .
make -j6
```

#### Mac

* Set Environment Variables:
    - CMAKE_PREFIX_PATH must point to your Qt directory (e.g. /Users/[USRNAME]/Qt/[QT-VERSION]/clang_64)

### Running the application

```
./node-embedd.exe ../source/demo-app/rss_feed.js
```
