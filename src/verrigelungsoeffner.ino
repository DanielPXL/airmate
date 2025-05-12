#include <Stepper.h>

// Anzahl der Schritte pro Umdrehung
#define STEPS 1024

// Motorgeschwindigkeit in RPM
#define MOTOR_RPM 10

const int onButtonPin = 4;

int on = 1;

boolean open = false;

Stepper stepper(STEPS, 8, 10, 9, 11); // Motor Pins

void setup()
{
  pinMode(onButtonPin, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop()
{
    on = digitalRead(onButtonPin);
    Serial.println(on);
    Serial.println(open);
    if (on == LOW && open){

      stepper.setSpeed(MOTOR_RPM);
      stepper.step(-STEPS);
      
      Serial.println("on is pushed!");
      delay(1000);
      open = false;

    }else if ( on == LOW && !open){

      stepper.setSpeed(MOTOR_RPM);
      stepper.step(STEPS);
      
      Serial.println("off is pushed!");
      delay(1000);
      open = true;

    }
  
}
