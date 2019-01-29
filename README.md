# Implementation Project - Apollo20

This project aims to control a 5R-plan drawer robot through a GUI. To do so, we used a beaglebone black and 2 controlled joint, which drive the whole system.


Authors : Loïc Kerboriou, Toledo P. Bruna

Release : February 2019

Polytech Sorbonne Engineering School - Major in Robotics

Location : Paris, France

Teacher : Frédéric Plumet


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

ROS Melodic

```
http://wiki.ros.org/melodic/Installation/Ubuntu
```
OpenCV 

```
https://docs.opencv.org/3.0-beta/doc/tutorials/introduction/linux_install/linux_install.html
```


### Initializing and running

CAREFULL : Actually the whole project is not on this page, some package are not there yet. You can not now run the program as following. 

Open a terminal and place yourself on ROS directory (usually catkin_ws). Check that you got a controller (as a Xbox controller) connected to your computer.

Launch the simulation with the command
```
roslaunch minilab_simulation minilab_simulation.launch
```
It launches the simulation on Gazebo and connect the controller to the simulation.

In an other terminal, in the same directory. Launch the SLAM package with the command
```
roslaunch slam_karto karto_slam.launch
```
It permit the creation of a map that you can recover.

In an third terminal, launch the path follower package with the command
```
roslaunch my_services my_services.launch
```

Finally you can launch in a last terminal rviz with the command 
```
rviz
```

You are now able to move the robot in a simulation.

## To do

-  Simplify the launch, do an unique launchfile for all
-  Back to the loading base (where the robot started)
-  Autonomous exploration
