# Introduction #

The base system is a reasonably modern desktop computer with multiple PCI slots, a NIDaq acquisition card (NI-6254), and an nVidia graphics card (GeForce 7300?).

The OS is CentOS 5.4 (?), non-PAE

Nvidia drivers (in rpmforge repo) will be needed for the graphics card, and the NI card(s) is/are driven with the NIDaqmx driver (link).

A number of additionall development libraries need to be installed to build ArtE.  Most of these are in the rpmforge repo, but some are not.

# Details #

## Desktop ##
Our test machine:

> spec 1 (processor, link to newegg?)

> spec 2 (mobo, link to newegg?)

> spec 3


## NI-6254 ##
> drivers link

## GeForce 7300 (?) ##

nVidia driver: recommend that you do it using the nVidia binary installer, rather than a package (after Greg's experience)
Find your hardware: sudo lspci
Find the driver here: http://www.nvidia.com/Download/index.aspx?lang=en-us
Download it and chmod it to be runnable
change /etc/inittab
id:5:initdefault:
Change 5->3, reboot (into terminal-only mode)
Nav to installer and run it.  This worked fine for me. When prompted about running nvidia-xconfig, say 'yes'.
set /etc/inittab back to runlevel 5.
Reboot.  To change monitor settings, run as root nvidia-settings


Alternatively:

http://www.linwik.com/wiki/configuring+a+nvidia+graphics+chip+for+centos+and+rhel+5
(replacing uname-r with uname-r minus .mwlKernel part

## Additional libraries ##

> libXt-devel (and dependencies)
> plotutils-devel (and dependencies)
> gtk+-devel (... and dependencies for all the following...)
> gcc-c++
> libXaw-devel
> libpng-devel
> gtk2-devel  (this should also pull down atk, pango, cairo)
> mesa-libGLU-devel
> LINK to gtkglext-1.2.0 - instructions for how to configure and build
> gtkmm2-devel
> cairomm-devel
> gtkmm4-devel
> (this is the list of packages installed by yum on the test computer.  Of course the building gtkmm24 code would probably work with some omissions, but I haven't tested this yet.  Let's not pare this list down until we test the proposed new list on a fresh machine).