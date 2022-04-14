/*
Alex Mitchell, Final Project RBE 1001, 12/6/2020
 */

#include <Arduino.h>
#include <RBE1001Lib.h>
#include <ESP32AnalogRead.h>
#include <ESP32Servo.h>
#include <ESP32Encoder.h>
#include <ESP32WifiManager.h>
#include <WebServer.h>
#include <ESP32PWM.h>

#include <Robot.h>

//robot object
Robot robot;

void setup()
{
  //must attach in setup or the code won't work

  Serial.begin(9600);
  ESP32PWM::allocateTimer(1);
  Motor::allocateTimer(0);

  robot.ultrasonic.rangefinder.attach(SIDE_ULTRASONIC_TRIG, SIDE_ULTRASONIC_ECHO);
  robot.arm.armServo.attach(SERVO_PIN);

  robot.linesensors.leftLine.attach(39);
  robot.linesensors.rightLine.attach(36);
  robot.linesensors.leftTrigger.attach(35);
  robot.linesensors.rightTrigger.attach(34);
}

// /**
//  * 1.5inch = 3.81cm
//  * 3inch = 7.62
//  * 3cm to pick up bag
//  * left direction is always -1
//  * right direction is always +1
//  * No direction is always 0
//  */

int DirectionRight = 1;
int DirectionLeft = -1;
int DirectionNone = 0;
float lowerPlatform = 3.81;
float higherPlatform = 7.62;


/**
 * 
 */
void loop()
{
  // higher tier is on the left side of the four way looking at it from the start location
  // lower tier is the middle
  // ground floor is next to free range zone
  // delay so i can get out of the way.
  delay(1000);

  //pickup bag
  robot.pickupBag();

  //drive to cross section then centers and turns left
  robot.followLineToCrossSection(DirectionRight, DirectionRight);

  //drives to platform and drops bag off
  robot.dropBag(higherPlatform);
  robot.arm.armTurn(60);

  //drives to cross section then centers then drives back to start to find second bag
  robot.followLineToCrossSection(DirectionRight, DirectionRight);
  robot.arm.armTurn(180);
  robot.followLineToCrossSection(DirectionLeft, DirectionNone);

  //finds and picks up second bag
  robot.turnWithLine(DirectionRight);
  robot.pickupBag();

  // drives to cross section and turns right then finds drop off area and drops bag off
  robot.followLineToCrossSection(DirectionRight, DirectionNone);
  robot.followLineToCrossSection(DirectionRight, DirectionNone);
  robot.chassis.forward(-3);
  robot.turnWithLine(DirectionRight);
  robot.dropBag(lowerPlatform);

  //drives to cross section and centers turning right
  robot.followLineToCrossSection(DirectionNone, DirectionLeft);
  robot.arm.armTurn(180);

  // //find a good place to look for free range bag
  robot.followLineToCrossSection(DirectionNone, DirectionNone);

  // //collect free range bag
  robot.FindObject();
  robot.pickupBag();

  // //find line
  robot.findLine();

  // //drop off bag
  robot.arm.armTurn(0);

  // // //implement win button

  // // //infinite loop so function only runs once
  while (true)
  {
  }
}
