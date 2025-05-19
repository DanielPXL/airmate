#include <Stepper.h>

// Anzahl der Schritte pro Umdrehung
#define STEPS 1024

// Motorgeschwindigkeit in RPM
#define MOTOR_RPM 10

const int ButtonPin = 4;

int on = 1;

boolean open = false;

Stepper lockStepper(STEPS, 8, 10, 9, 11); // Motor Pins

void setup()
{
  pinMode(ButtonPin, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop()
{
    on = digitalRead(ButtonPin);
    Serial.println(on);
    Serial.println(open);
    if (on == LOW && open){

      lockStepper.setSpeed(MOTOR_RPM);
      lockStepper.step(-STEPS);
      
      Serial.println("on is pushed!");
      delay(1000);
      open = false;

    }else if ( on == LOW && !open){

      lockStepper.setSpeed(MOTOR_RPM);
      lockStepper.step(STEPS);
      
      Serial.println("off is pushed!");
      delay(1000);
      open = true;

    }
  
}

