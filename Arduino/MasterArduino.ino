/* Author: Bethany Kon
 * Title: SR0 Pool Cleaner Master Arduino
 * Description:
 * 
 * Date:
 * Version: 1000.0
 */

//Required Libraries
#include <ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/UInt16.h>
#include <Wire.h>
#include <Servo.h>

//Initializing Node Handler to use Rosserial
ros::NodeHandle nh;

//Initializing Publisher with IMU message
sensor_msgs::Imu imu_msg;
ros::Publisher chatter("chatter", &imu_msg);
char frame_id[] = "imu";


//Global Variables
char buffer[35];
int i = 0;
int found = 0;

//Initializing Srvo motor classes
Servo rightservo;
Servo leftservo;
Servo frontbrush;
Servo rightbrush;
Servo leftbrush;


/* The following functions receive a subscribed message and 
 * actuate the appropriate Servos accordingly
 */
void rightservo_cb( const std_msgs::UInt16& cmd_msg){
  rightservo.write(cmd_msg.data); //set servo angle, should be from 0-180  
  digitalWrite(13, HIGH-digitalRead(13));  //toggle led  
}

void leftservo_cb( const std_msgs::UInt16& cmd_msg){
  leftservo.write(cmd_msg.data);
}

void frontbrush_cb( const std_msgs::UInt16& cmd_msg){
  frontbrush.write(cmd_msg.data);
}

void rightbrush_cb( const std_msgs::UInt16& cmd_msg){
  rightbrush.write(cmd_msg.data);
}

void leftbrush_cb( const std_msgs::UInt16& cmd_msg){
  leftbrush.write(cmd_msg.data);
}

//Initializing Subscriber for each Servo motor
ros::Subscriber<std_msgs::UInt16> sub1("rightservo", rightservo_cb);
ros::Subscriber<std_msgs::UInt16> sub2("leftservo", leftservo_cb);
ros::Subscriber<std_msgs::UInt16> brush1("frontbrush", frontbrush_cb);
ros::Subscriber<std_msgs::UInt16> brush2("rightbrush", rightbrush_cb);
ros::Subscriber<std_msgs::UInt16> brush3("leftbrush", leftbrush_cb);

void setup() {
  //Initializing pin modes for motors
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  //Set Baud Rate to communicate with Rosserial and Robot
  nh.getHardware()->setBaud(57600);
  //Initialize Publisher and Subscribers
  nh.initNode();
  nh.advertise(chatter);
  nh.subscribe(sub1);
  nh.subscribe(sub2);
  nh.subscribe(brush1);
  nh.subscribe(brush2);
  nh.subscribe(brush3);

  //Link Servo classes to appropriate pins
  rightservo.attach(2);
  leftservo.attach(3);
  frontbrush.attach(4);
  rightbrush.attach(5);
  leftbrush.attach(6);

  //Begin I2C communication with slave arduino
  Wire.begin();

}

/* This function takes char data acquired from the Slave Arduino,
 * splits it by commas, converts the char characters to integers,
 * and assigns it as part of the IMU msg.
 */
void printing(){
  //Acquire  time stamp
  imu_msg.header.stamp = nh.now();

  //Split buffer string by data and convert to integer
  i = 0;
  char *pt;
  int measure[9] = {0,0,0,0,0,0,0,0,0};
  pt = strtok(buffer,",");
  while(pt != NULL) {
    int a = atoi(pt);
    measure[i++] = a;
    pt = strtok(NULL, ",");
  }

  //Assign data to appropriate message output
  imu_msg.orientation.x = measure[6];
  imu_msg.orientation.y = measure[7];
  imu_msg.orientation.z = measure[8];

  imu_msg.angular_velocity.x = measure[3];
  imu_msg.angular_velocity.y = measure[4];
  imu_msg.angular_velocity.z = measure[5];

  imu_msg.linear_acceleration.x = measure[0];
  imu_msg.linear_acceleration.y = measure[1];
  imu_msg.linear_acceleration.z = measure[2];

  //Publish the IMU msg
  chatter.publish( &imu_msg );
  nh.spinOnce();

  //Reset to acquire new data
  i = 0;
  memset(buffer,0,35);
}

void loop() {
  //Get data from slave arduino
  Wire.requestFrom(8,1);

  if(Wire.available()){
    char c = Wire.read();

    //Identify relevant data
    if(c=='#'){
      found = 0;
      printing();
    }

    if(found==1) buffer[i++] = c;
    if(c=='$') found = 1;
 
  }
}
