# pool_cleaner_robot
## Instructions
1. Create a ROS workspace
2. Clone this package onto the src folder of the ROS workspace:
```
cd ~/catkin_ws/src
git clone https://github.com/raultron/pool_cleaner_robot.git
cd ..
catkin build
source devel/setup.bash
```

3. Run ros_serial, (replace /dev/ttyS0 with the serial port the Xbee USB Explorer is connected to):

```
rosrun rosserial_python serial_node.py /dev/ttyS0
```

4. Run the joystick capture node
```
rosrun joy joy_node
```

5. Run the motor control python script node:
```
rosrun pool_cleaner_robot joy_control.py
```


TODO:
Consolidate all previous commands into a ROS launcher.
