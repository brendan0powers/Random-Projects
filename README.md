Random Projects
===============
This repository contains some projects I've worked on. Most of them are incomplete, or abandoned. However, I decided to post them here so that I would not loose track of them. Below is a list of the projects in this repository.

headphonectl
------------
Allows the use of the special keys on the Logitech G930 wireless headset in Linux. It has modules for controlling VLC, a UPnP media server, and a live audio stream from another sound card.

LaserTag5000
------------
A proof of concept Android application. It is designed to interface with a peice of custom hardware over the Bluetooth SPP protocol, and a CouchDB database server. The Bluetooth interface to the hardware works. However, the server side databse has yet to be fully implemented. This is the first real project in Java that I have ever done.

NetFS
-----
A network file system using the FUSE file system layer. It's designed to mount network file system (SMB, NFS, etc..) to a folder in a users home directory.

ogreproject
-----------
This project uses the Ogre graphics library, the ODE phycics library, the Torque network library and the Qt graphics library. It was an attempt to create a 3D multi player game with a physics element. The project got to the point where it could load a Quake 3 map with lighting and textures, and had simple client side physics. You could connect more than one client to the server, and see where the other players were in the map. There was also a Qt GUI overlay that let you do things like use a WebKit browser in game. I was responsible for writing the client side, physics, and UI code. Chris Vickery wrote the Torque library integration and server code.

opencvtest
----------
An test application for the OpenCV computer vision library. Among ather things, this application will capture images from two cameras and search the images for a face. Once a face has been found in both images, it uses the relative positions of the faces in each image to calculate the face's distance from the camera. The application also contains code to test some other algorithms, such as pattern matching, and edge detection.

QxtIrc
------
An IRC client library and bot framework writen using the Qt library. This project was a collaboration with several people. It was designed to be simple to use, and several interesting IRC bots were created using it.

MobileIRC
------
A Windows Mobile IRC client writen using the QxtIrc library. It worked fairly well. However, it only joined one channel.
