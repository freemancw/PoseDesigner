## PoseDesigner

*PoseDesigner* is an application designed to make pose generation and
recognition easy for developers working with the Xbox Kinect. While the
next milestone release will aim to robustly support only static poses,
the framework is designed to be easily extended to include dynamic gestures
or short movement clips.

The software will consist of two pieces: the GUI designer and a library that
can be linked into your program to expose the recognition functionality. This
repository contains the GUI editor files - a sister repository will be created
when the recognition library is mature enough.

Currently, only OpenNI driven programs are supported. It is possible that
future versions will also support the official Microsoft SDK.

### Current Features

* Take multiple samples of a pose to automatically generate a profile.
* Timed sampling for individual developers.
* Export data to XML based format.
* Each sample has corresponding image for easy recollection of what you were doing.
