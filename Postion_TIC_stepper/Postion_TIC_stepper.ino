// This example shows how to send I2C commands to the Tic
// Stepper Motor Controller to control the position of a Stepper
// Motor.
//
// The Tic's control mode must be set to "Serial/I2C/USB".  The
// serial device number must be set to its default value of 14.
//
// If you have sent a De-energize command to the Tic, for example
// by clicking "De-energize" in the Tic Control Center, you will
// need to undo that by clicking "Resume" or power-cycling the
// Tic.
//
// Please see https://github.com/pololu/tic-arduino for details
// on how to make the connections between the Arduino and theerffrtgetgdsgsdfgsdfgdsfgsdfgsdfgsdfgsdfgsdf
// Tic.

#include <Tic.h>
#include <Wire.h>

TicI2C tic;


/* Motor: Target position global variable */
int targetposition = 200;
int count=0;
int stepcounter = 10;
int jumpstep = 100;

void setup()
{
  // print function
  Serial.begin(115200);
  Serial.println("Setup");
  
  // Set up I2C.
  Wire.begin();

  // Give the Tic some time to start up.
  delay(20);

  // Set the Tic's current position to 0, so that when we command
  // it to move later, it will move a predictable amount.
  tic.haltAndSetPosition(0);

  // Tells the Tic that it is OK to start driving the motor.  The
  // Tic's safe-start feature helps avoid unexpected, accidental
  // movement of the motor: if an error happens, the Tic will not
  // drive the motor again until it receives the Exit Safe Start
  // command.  The safe-start feature can be disbled in the Tic
  // Control Center.
  tic.exitSafeStart();
}

// Sends a "Reset command timeout" command to the Tic.  We must
// call this at least once per second, or else a command timeout
// error will happen.  The Tic's default command timeout period
// is 1000 ms, but it can be changed or disabled in the Tic
// Control Center.
void resetCommandTimeout()
{
  tic.resetCommandTimeout();
}

// Delays for the specified number of milliseconds while
// resetting the Tic's command timeout so that its movement does
// not get interrupted by errors.
void delayWhileResettingCommandTimeout(uint32_t ms)
{
  uint32_t start = millis();
  do
  {
    resetCommandTimeout();
  } while ((uint32_t)(millis() - start) <= ms);
}

// Polls the Tic, waiting for it to reach the specified target
// position.  Note that if the Tic detects an error, the Tic will
// probably go into safe-start mode and never reach its target
// position, so this function will loop infinitely.  If that
// happens, you will need to reset your Arduino.
void waitForPosition(int32_t targetPosition)
{
  do
  {
    resetCommandTimeout();
  } while (tic.getCurrentPosition() != targetPosition);
}

// Main loop for all motion and delays
void loop()
{
  // Tell the Tic to move to position 100, and wait until it gets there.
  //tic.setTargetPosition(targetposition);
  //waitForPosition(targetposition);
  Serial.println(targetposition);       // prints a current count
  Serial.print("\t");

   if (count >= stepcounter) /* if the maximum count reached, move*/ {
      count = 0;
      /*Ready for the next jump*/
      tic.setTargetPosition(targetposition);
      waitForPosition(targetposition);
      targetposition = targetposition+jumpstep;
      Serial.println("Motion: Jump to the next position");       // jump now
      Serial.print("\t");
   }else {
      /*wait time prior the next jump*/
      count++;
   }

  // delay variable
  delayWhileResettingCommandTimeout(1000);
}
