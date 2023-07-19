#include <Stepper.h>

const int Steps = 200;
const int DirA = 12;
const int DirB = 13;
const int BrakeA = 9;
const int BrakeB = 8;
const int PWMA = 3; 
const int PWMB = 11; 
const int LimitPlus = 4;
const int LimitMinus = 2; 
int ExtrudeAllow = 0; 
int incomingByte = 0;
char Distancedigits[3];
char Speeddigits[2];
int arrayTick = 0;
int direction = 0;
float distance_mm = 0;
float speed_mm_s = 0;
const int RPMconv = 7.52468305;
const int MinimumSpeed = 5;
const int MaximumSpeed = 35;

Stepper extrudeMotor = Stepper(Steps, DirA, DirB);  //Initialize the stepper library

void home(float speed_mm_s){

    extrudeMotor.setSpeed(speed_mm_s*RPMconv);

    int LimitMinusValue = HIGH;

    while(LimitMinusValue == HIGH){

    LimitMinusValue = digitalRead(LimitMinus);  

    extrudeMotor.step(1);
    
    }

    digitalWrite(PWMA, LOW);
    digitalWrite(PWMB, LOW);

}

void extrude(float distance_mm, float speed_mm_s, int direction){

  digitalWrite(PWMA, HIGH);
  digitalWrite(PWMB, HIGH);

  //home(speed_mm_s);

  delay(500);

  int StepCnt = 0;

  while(distance_mm > 0 && speed_mm_s > 0 && StepCnt < distance_mm * 25 + 1){

    extrudeMotor.setSpeed(speed_mm_s * RPMconv);

    int LimitPlusValue = digitalRead(LimitPlus);

    int LimitMinusValue = digitalRead(LimitMinus);

    extrudeMotor.step(-1 * direction);

    StepCnt = StepCnt + 1;

    if(LimitPlusValue == LOW && StepCnt > 2){

      delay(500);

      home(speed_mm_s);

      distance_mm = 0;
        
      speed_mm_s = 0;

    }

    if(LimitMinusValue == LOW) {
      
      distance_mm = 0;
        
      speed_mm_s = 0;

    }
  }
        digitalWrite(PWMA, LOW);
        digitalWrite(PWMB, LOW);
        distance_mm = 0;
        speed_mm_s = 0;
}


void setup() {

  Serial.begin(9600);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BrakeA, OUTPUT);
  pinMode(BrakeB, OUTPUT);
  pinMode(LimitPlus, INPUT_PULLUP);
  pinMode(LimitMinus, INPUT_PULLUP);
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(BrakeA, LOW);
  digitalWrite(BrakeB, LOW);

}




void loop() {

  if (Serial.available() > 0) {

    char incomingByte = Serial.read();

    if(incomingByte == 69){ //ASCII 69 = "E" for Extrude

      arrayTick = 0;

      while(arrayTick < 2){

        delay(100);
        char digit = Serial.read();
        delay(500);

        if (digit >= '0' && digit <= '9'){
          delay(100);
          Serial.print(digit);
          Distancedigits[arrayTick] = digit;
          arrayTick = arrayTick + 1;
        }
      }
    }

    incomingByte = Serial.read();

    if(incomingByte == 83){ //ASCII 83 = "S" for Speed

      arrayTick = 0;

      while(arrayTick < 3){

        delay(100);
        char digit = Serial.read();
        delay(500);

        //if(48 <= digit <= 57){
        if(digit >= '0' && digit <= '9'){
          Serial.print(digit);
          Speeddigits[arrayTick] = digit;
          arrayTick = arrayTick + 1;
        }
      }
    }

   incomingByte = Serial.read();

   if(incomingByte == 68){ //ASCII 68 = "D" for Direction

    char digit = Serial.read();

    if(digit == 43){ //ASCII 43 = "+"
      direction = 1;
    } else if(digit == 45){ //ASCII 45 = "-"
      direction = -1;
    }
  }

    delay(1000);

    Serial.println('\n');
    Serial.print("Command registered");
    Serial.println('\n');

    distance_mm = 10*(Distancedigits[0]-'0') + Distancedigits[1]-'0';

    speed_mm_s = 10*(Speeddigits[0]-'0') + 1*(Speeddigits[1]-'0') + 0.1*(Speeddigits[2]-'0');

    Serial.print("Speed and distance calculated");
    Serial.println('\n');
    Serial.print("Distance = ");
    Serial.print(distance_mm);
    Serial.println('\n');
    Serial.print("Speed = ");
    Serial.print(speed_mm_s);
    Serial.println('\n');

    if(speed_mm_s > 0 && distance_mm > 0){

      Serial.println('\n');
      Serial.print("Distance = ");
      Serial.print(distance_mm);
      Serial.print('\n');
      Serial.print("Speed = ");
      Serial.print(speed_mm_s);
      Serial.print('\n');
      Serial.print("Direction = ");
      Serial.print(direction);
      Serial.println('\n');

      extrude(distance_mm, speed_mm_s, direction);
            for(int i = 0; i < 3; i++){
        Speeddigits[i] = 0;
      }
      for(int i = 0; i < 2; i++){
        Distancedigits[i] = 0;
      }
    }
  }
}


