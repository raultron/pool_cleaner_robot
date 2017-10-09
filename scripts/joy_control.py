#!/usr/bin/env python
import rospy
from std_msgs.msg import UInt16
from sensor_msgs.msg import Imu
from sensor_msgs.msg import Joy

pub_motor0 = rospy.Publisher('/rightservo', UInt16, queue_size=10)
pub_motor1 = rospy.Publisher('/leftservo', UInt16, queue_size=10)
pub_motor2 = rospy.Publisher('/frontbrush', UInt16, queue_size=10)
pub_motor3 = rospy.Publisher('/leftbrush', UInt16, queue_size=10)
pub_motor4 = rospy.Publisher('/rightbrush', UInt16, queue_size=10)


def joy_callback(data):
    # We read first the data from the joystick
    axe0 = data.axes[0]
    axe1 = data.axes[1]
    axe2 = data.axes[2]
    axe3 = data.axes[3]

    button0 = data.buttons[0]
    button1 = data.buttons[1]
    button2 = data.buttons[2]
    button3 = data.buttons[3]
    button4 = data.buttons[4]
    button5 = data.buttons[5]
    button6 = data.buttons[6]

    brush_msg = UInt16()
    motor_msg = UInt16()

    # Do something with the joystick data
    ## Brush Motors
    brush_msg.data = int((axe1 + 1)*40 + 50)
    if (axe1<0.01) and (axe1>(-0.01)):
	brush_msg.data = 95
    if button3:
	pub_motor2.publish(brush_msg)
    if button2:
	pub_motor4.publish(brush_msg)
    if button0:
	pub_motor3.publish(brush_msg)
    ## Wheel Motors
    alpha = 1
    beta = 1
    vr = alpha*axe3 + (beta*axe2)
    vl = alpha*axe3 - (beta*axe2)
    
    r = (2-(vr+1))*(60) + 30
    
    l = (2-(vl+1))*(60) + 30
    if (r<70):
	r = 70
    if (l<70):
	l = 70
    if (r>120):
	r = 120
    if (l>120):
	l = 120

    motor_msg.data = int(r)
    pub_motor0.publish(motor_msg)
    motor_msg.data = int(l)
    pub_motor1.publish(motor_msg)
    
    ## Stop All Motors
    if button1:
	brush_msg.data = 95
	motor_msg.data = 90
	pub_motor0.publish(motor_msg)
	pub_motor1.publish(motor_msg)
	pub_motor2.publish(brush_msg)
	pub_motor3.publish(brush_msg)
	pub_motor4.publish(brush_msg)
    ##

'''
    #Publish the commands to the motor topics
    motor_msg = UInt16()
    motor_msg.data = axe0
    pub_motor0.publish(motor_msg)
'''






if __name__ == '__main__':
    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('joy_control', anonymous=True)


    rospy.Subscriber("/joy", Joy, joy_callback)





    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

