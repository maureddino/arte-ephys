#I'm thinking of using [0mq](http://www.zeromq.org/) for our networking.  Here I'm documenting my attempts to install it under RHEL5.

## Instructions ##

Go [here](http://www.zeromq.org/intro:get-the-software) and get the POSIX tarball & read the instructions.  I'm trying latest stable 2.1 version.


## Details ##

My steps:

If you don't already have epel repo set up, you likely have to:
```
sudo rpm -Uvh http://download.fedora.redhat.com/pub/epel/5/i386/epel-release-5-4.noarch.rpm
```

```
sudo yum install libtools
```

libtools package seems to be sometimes found, sometimes not. If not just

```
sudo yum install automake autoconf
```

This included automake & autoconf as dependencies, which surprisingly weren't already installed on the system.  Instructions want uuid-dev package, but under RHEL5 there's only uuid-devel.

```
sudo yum install uuid-devel
sudo yum install e2fsprogs-devel
sudo yum install e2fsprogs-libs
```

With dependencies gotten, grab the zeromq source & install.

```
cd ~/Downloads/
wget http://download.zeromq.org/zeromq-2.1.7.tar.gz
tar -xf zeromq-2.1.7.tar.gz
cd zeromq-2.1.7
./configure
make
```

Watch ./configure and make steps.  They shouldn't give any errors.  Then install and run ldconfig.

```
sudo make install
sudo /sbin/ldconfig
```

Installation worked for me on first try.  Let's make a test program.

```
cd ~
mkdir test_zmq
cd test_zmq
wget https://raw.github.com/imatix/zguide/master/examples/C/hwclient.c
wget https://raw.github.com/imatix/zguide/master/examples/C/hwserver.c
gcc hwclient.c -lzmq -o cli
gcc hwserver.c -lzmq -o serv
./cli &
./serv &
```

And it works!  Woot!  (don't forget the -lzmq flag for gcc.  I didn't use that and got stuck for a little while.

After changing the hostname in hwclient.c so that it looks for a server on devel3, and removed the sleep(1) command from the loop, the two passed 10,000 messages (5 bytes each) through the switch per second.  This may be too slow for the data.  Of course faster than necessary for commands.