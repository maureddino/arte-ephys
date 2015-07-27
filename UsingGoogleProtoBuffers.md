We'll use Google's [Protocol Buffers](http://code.google.com/p/protobuf/) to build command messages into buffers that can go over the network.

Protobufs seems pretty easy to use.  You define a message in a .proto file, listing the names and datatypes of fields (they can be arrays).  Then you run your.proto file through google's protoc compiler, and it spits out c++ code that you include in your project.  Each message in the .proto gets its own class, with access and setting methods, and methods to convert from instance to byte-string.  The classes themselves take care of all endianness (and actually put little-endian data on the wire - they say this makes for better optimization b/c likely both ends of the communication will be little endian, so you avoid 2 byteswaps per piece of data).  I guess that's fine as long as protobufs is responsible both for the packing _and_ unpacking of the messages.

You'll need to do this install on any machine that builds arte code that uses protobufs.  Although most of the library seems to be about generating c++ classes from the .proro file, that generated code includes protobufs headers.

```
cd Downloads/
wget http://protobuf.googlecode.com/files/protobuf-2.4.1.tar.bz2
tar -xf protobuf-2.4.1.tar.bz2
cd protobuf-2.4.1
./configure
make
make check
sudo make install
sudo /sbin/ldconfig
```

NB: In one build case, I got a makefile in protobuf-2.4.1 that didn't make anything.  I had to cd gtest first (reading the output from ./configure, protobuf built there.  This message is sent from all ./configure runs though, even on computers where make works from within the base directory).

On RHEL5/CentOS 5.5 machines, pkg-config doesn't seem to work out of the box.  Before compiling, you have to issue:
```
PKG_CONFIG_PATH = /usr/local/lib/pkgconfig
```
Consider adding this to .profile or something like that, b/c setting the environment variable only lasts until restart.  Other option is to add -lprotobuf  to the g++ line; -lprotobuf seems to be the only essential flag set by `pkg-config --cflags --libs protobuf`.

Built and installed for me with no errors.  make check even gives a nice "all 4 tests passed" message when it finishes.

In the process of trying it out now (writing a little server & client to transmit protobuf messages over zmq pub/sub).

To build protobufs applications, you'll have to put
```
`pkg-config --cflags --libs protobuf` 
```
(backtics) on the g++ line.  You may also have to use zmq.hpp (the c++ zmq bindings) instead of the standard c bindings, as the c zmq API seems to do things with void pointers that c++ complains about. (hint if you want to see this - compile a .cpp file that includes zhelpers and uses send\_s or recv\_s.  I may be wrong).