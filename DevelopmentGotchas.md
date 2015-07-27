## Rough listing of problems that come up when coding, and solutions in some cases ##


### Prob 1 ###
Programs that try to operate on big arte data files segfault, while they work on smaller files.
### Solution ###
Sln: compile with this flag: `-D_FILE_OFFSET_BITS=64`  This seems to work sometimes even when the define line (#define _FILE\_OFFSET\_BITS 64) doesn't work.  See build.sh for arte\_to\_tt for example._

### Prob 2 ###
My thing that includes global\_defs.h won't build, it complains about UINT16\_MAX being not declared.

### Solution ###
they're declared in <stdint.h> but only if you pass this flag to gcc:  `-D__STDC_LIMIT_MACROS`   (see build.sh for arte, or for arte\_to\_tt, for an example).

### Prob 3 ###
Trying to simultaneously use the analog in lines and a counter line, you get daqmx error num: -89137 with message: Specified route cannot be satisfied, because it requires resources that are currently in use by another route.  /  Property: DAQmx\_RefClk\_Src  /  Source Device: Dev1  /  Source Terminal: 10MHzRefClock  /  Required Resources in Use by  /  Task Name: Counter Task / Source Device: Dev1 / Source Terminal: None /  Destination Device: Dev1  /  Destination Terminal: RefClock / Task Name: Dev1 / Status Code: -89137
### Solution ###
The clock source of the counter must be explicitly set to match that on the analog in lines.  Add this before starting the counter task:    daq\_err\_check( DAQmxSetRefClkSrc( counterTask, "OnboardClock") );

### Prob4 ###
Python scripts that recvfrom for broadcast packets don't see any packets, even when I know the packets are getting to the specified port on that host.
### Solution ###
Is socket.bind( str host, int port ) the host field actually seems to be important.  Setting it to "127.0.0.1" or "1.1.2.54" (localhost, in each case), the socket won't see packets.  Setting to "" (empty string - as used in some python sockets tutorials) does seem to work.  I wonder if I have it wrong this way in the c code of backend, too, and it only works because we got lucky?.