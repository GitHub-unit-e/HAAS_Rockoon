#include <SimpleTimer.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

#define PIEZO 13//Buzzer to pin 13
#define MOTOR 2//Gear Motor to pin 2

/**
 * bmp085 Sensor
 * Arduino A5 -> BMP180 SCL
 * Arduino A4 -> BMP180 SDA
 * Arduino +5V -> BMP180 VIN
 * Arduino GND -> BMP180 GND
 */
Adafruit_BMP085 bmp;
SimpleTimer timer;
boolean counter = true;
int a_start, a_current;

void setup() {

  Serial.begin(9600);

  //set pin modes
  pinMode(MOTOR, OUTPUT);
  pinMode(PIEZO, OUTPUT);

  //comment out unneeded method
  //setupTimerMethod();
  setupBMPMethod();

  tone(PIEZO, 1000, 200);//signal set up has been complete
}

void setupTimerMethod() {

  //set up initial timer
  timer.setTimeout(1000 * 60 * 60 * 3, spinMotor);//number in miliseconds - initially set to three hours
  timer.run();
}

void setupBMPMethod() {

  //check if BMP085 sensor exists
  if (!bmp.begin(BMP085_ULTRAHIGHRES)) {

    Serial.println("BMP sensor not found");
  } else {

    Serial.println("OK");
  }

  //set start altitute to initial reading
  a_start = (int) bmp.readAltitude();

  Serial.print("Start:");
  Serial.print(a_start);
  Serial.print("m");
}

void loop() {

  //comment out unneeded method
  //runTimerMethod();
  runBMPMethod();
}

void runTimerMethod() {

  //loop timer run
  timer.setTimeout(1000 * 5, spinMotor);//spin motor in 2 ~ 3 hours (or any time you want)
  timer.run();
}

void runBMPMethod() {

  //set current altitute to current reading
  a_current = (int) bmp.readAltitude();
  Serial.print("Altitude:");
  Serial.print(a_current);
  Serial.print("m");

  //if the difference is more than # meters, 
  if ((a_current - a_start) > 2000) {//change difference number as needed - initial value set at 2km

    Serial.print("difference:");
    Serial.print(a_current - a_start);
    Serial.print("m");

    //spin motor
    spinMotor();
  }

  delay(500);
}

void spinMotor() {

  //check counter value for single motor spins
  if (counter) {

    //signal motor spin
    tone(PIEZO, 200, 200);

    //spin motor 5 times
    for (int i = 0; i < 5; i++) {

      digitalWrite(MOTOR, HIGH);
      delay(250);
      digitalWrite(MOTOR, LOW);
      delay(500);
    }

    //set counter to false
    counter = false;
  }
}

