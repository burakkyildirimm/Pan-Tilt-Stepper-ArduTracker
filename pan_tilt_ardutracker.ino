// Sweep
// by ALGAN_UAV_TEAM

#include <Stepper.h>
#include <Servo.h> 

Stepper stepper_pan(200, 7,6, 3, 2);//pan stepper pins
Stepper stepper_tilt(200, 13,12, 11, 10);//tilt stepper pins

int pan_stepCount = 0;
int tilt_stepCount = 0;

long pan_previous = 48*5;
int pan_relacao=5;//// This is the calibration constant for the pan which has a gear ratio Fron stepper

long tilt_previous = 48*5;
int tilt_relacao=5;

long val_pan;
long val_tilt;

Servo pan;  // pan servo
Servo tilt; // tilt servo

 int pan_val=0;
 int tilt_val=0;
 
// Pin 13 has an LED connected on most Arduino boards.
int led = 13;
long lastTilt = 50L;
long lastPan = 50L;

int pan_dir = 1;
int pan_pos = 0;


int tilt_dir = 1;
int tilt_pos = 0;

long minPWM = 1000L;
long maxPWM = 2000L;
long panVal = 0L;
long tiltVal = 0L;

long pan_interpVal = 0L;
long tilt_interpVal = 0L;

String cmd = "";
String panStr = "";
String tiltStr = "";

void setup() 
{ 
  stepper_pan.setSpeed(30);
  stepper_tilt.setSpeed(30);
  pan.attach(9);  
  tilt.attach(10);
  
  Serial.begin(57600);

  pinMode(led, OUTPUT);     

  // Origin for both servos
  tilt.write(lastTilt);
  pan.write(lastPan);

} 
 
 
void loop() 
{ 
  if (Serial.available()) 
  {
    char ch = Serial.read();

    if (ch == 10)
    {
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      
      // Line feed is the command char
      if (cmd.startsWith("!!!PAN:"))
      {
        panStr = cmd.substring(7,11);
      
        if (cmd.substring(12,16) == "TLT:")
        {
          tiltStr = cmd.substring(16,20);
        }

        if (panStr != "")
        {
           panVal = panStr.toInt();
           if (panVal < minPWM)
           {
             panVal = minPWM;
           }        
         
           if (panVal > maxPWM)
           {
             panVal = maxPWM;
           }
       
           pan_interpVal = ((panVal-minPWM) * 360) / (maxPWM-minPWM);  
         
            if (pan_interpVal > 360)
            {
              pan_interpVal = 360;
            }
        
            if (pan_interpVal < 0)
            {
              pan_interpVal = 0;
            }

            Serial.print("Pan=");
            Serial.println(pan_interpVal);
            val_pan = ((pan_interpVal * 50) / 37);
            // This is the calibration constant for the pan which has a gear ratio
      
            pan_interpVal = ((pan_interpVal * 10) / 37) + 50;
            pan.write(pan_interpVal);
            
  // val = interpVal;
  // move a number of steps equal to the change in the
  // sensor reading
  stepper_pan.step((pan_val - pan_previous)*-1);
  // remember the previous value of the sensor
  pan_previous = val_pan;
            
            lastPan = pan_interpVal;
        }
      
        if (tiltStr != "")
        {
           tiltVal = tiltStr.toInt();
           if (tiltVal < minPWM)
           {
             tiltVal = minPWM;
           }        
         
           if (tiltVal > maxPWM)
           {
             tiltVal = maxPWM;
           }
         
           tilt_interpVal = ((tiltVal-minPWM) * 360) / (maxPWM-minPWM);  
           
            if (tilt_interpVal > 360)
            {
              tilt_interpVal = 360;
            }
          
            if (tilt_interpVal < 0)
            {
              tilt_interpVal = 0;
            }
  
            Serial.print("Tilt=");
            Serial.println(tilt_interpVal);
            val_tilt = ((tilt_interpVal * 50) / 37);
            // This is the calibration constant for the pan which has a gear ratio
      
            tilt_interpVal = ((tilt_interpVal * 10) / 37) + 50;
            tilt.write(tilt_interpVal);
            
  // val = interpVal;
  // move a number of steps equal to the change in the
  // sensor reading
  stepper_tilt.step((val_tilt - tilt_previous)*-1);
  // remember the previous value of the sensor
  tilt_previous = val_tilt;
            
            lastTilt = tilt_interpVal;
      }
      
      panStr = "";
      tiltStr == "";
      cmd = "";

      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW         
    }
    else
    {
      cmd += ch;
    }
    }
  }
} 
