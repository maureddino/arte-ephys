#How to install the openCV dev libraries for tracker.

# This page gives simple instructions #

[Ubuntu OpenCV installation](https://help.ubuntu.com/community/OpenCV)


# Details #

Copied from there, in case that site goes down?

OpenCV is a software —better yet, a collection of libraries— that provide image processing routines and algorithms for software development. Many people are having problems with installing OpenCV from source.

To install it using the terminal, type the following sequence of commands:

```
sudo su
sudo apt-get install build-essential
sudo apt-get install libavformat-dev
sudo apt-get install ffmpeg
sudo apt-get install libcv2.1 libcvaux2.1 libhighgui2.1 python-opencv opencv-doc libcv-dev libcvaux-dev libhighgui-dev
```

To compile and view the examples, type:

```
cp -r /usr/share/doc/opencv-doc/examples .
cd examples
cd c/c/
chmod 755 build_all.sh
./build_all.sh
```

This will build most/all of the examples. You will also get the python examples, which can be compiled with python filename.py

Haar training
Other advanced tasks such as haartraining and such are also available. To access them type:
```
opencv-haartraining
opencv-createsamples
opencv-performance (checking performance of the classifier)
```
The actual training procedure can be found here:

http://note.sonots.com/SciSoftware/haartraining.html