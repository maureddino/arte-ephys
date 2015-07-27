We'll use the PCI-CTR05 counter card in machines needing to count the clock signal other than the backend (computer running tracker, for example).

The hope is that this card can be used more easily than the national instruments cards under Ubuntu.  (We don't want the tracker running under CentOS like the backend b/c we're limited to less fancy libraries under CentOS).

A driver can be found here
[ftp://lx10.tx.ncsu.edu/pub/Linux/drivers](ftp://lx10.tx.ncsu.edu/pub/Linux/drivers)

Then click PCI, and the ctr05.tgz

Alternatively (if you don't care about grabbing most recent):
```
ftp://lx10.tx.ncsu.edu/pub/Linux/drivers/PCI/PCI-CTR05.1.7.tgz
```


Also pick up 60-mcc.rules from there.  Read the readme file.  I'm going to try to install it on my workstation presently..

Install notes:
copied the rules file.
Updating rules: need to use new command because udevcontrol no longer exists.  instead:

```
cp 60-mcc.rules /etc/udev/rules.d/
udevadm control --reload-rules
```

The LKM will compile, but the driver itself will silently not-compile, unless you change a line in c9513.c  .  So open up c9513.c in a text editor, and within the last #if block, after the warning (line 54 as of Oct12-2011) add the line:

```
#include "c9513_2_26_29.c"
```

> These if blocks were put there to prevent people from installing the driver under a kernel too modern to be compatible with the calls made by the driver.  For now we will forgo this safety feature.  I tested the driver under 2.6.35-30 with no problems).


Now:

```
sudo make
sudo make install
```

make install should give you 1 error only (and only the first time you run it:

```
/sbin/rmmod ctr05
ERROR: Module ctr05 does not exist in /proc/modules
```

This is just due to a little thing in the Makefile, don't need to worry about it.

Now, assuming the card is plugged in, you should see something like this in your /proc/devices  file:

```
cat /proc/devices | grep "ctr"
TODO - find out exact output (working from home here)
```

For me, after all this, running the test program that came with the driver works!

Although it took a few days, due to me not noticing the kernel version restrictions.  If interested, I wrote up a [question for stackexchange](http://superuser.com/questions/344764/confusion-and-difficulty-using-a-pci-device-controlled-by-a-kernel-module) about troubleshooting the issue.  May be a good read if you're having problems and want to see what dead-ends I tried.