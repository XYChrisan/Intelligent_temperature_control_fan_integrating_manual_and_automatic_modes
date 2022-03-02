#include <Robojax_L298N_DC_motor.h>
#include <dht.h>
#include <U8x8lib.h>
U8X8_SSD1306_128X64_ALT0_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

dht DHT;
#define IN1 12
#define IN2 13
#define ENA 11
#define NTD1 294
int ledPin = 4;
int tonepin=5;

const int CCW = 2; 
const int CW  = 1; 
const int motorDirection = CW;
#define motor1 1
Robojax_L298N_DC_motor motor(IN1, IN2, ENA, true);

#define dataPin 3
float t; 

const int voltMin = 0; 
const int voltMax = 12; 

const float tempMin = 30.0;  
const float tempMid = 40.0;
const int speedMin = 70;  
const float tempMax = 50.0;  
const float extreme = 60.0; 
const int speedMax = 100;
const int buttonPin = 6;
const int rotaryPin = A0;
int rotaryValue = 0;

int selectedMode = 0; 

int motorSpeed; 

int buttonState = 0;

void setup() {
  motor.begin();
  u8x8.begin();
  u8x8.setFlipMode(1);
  pinMode(rotaryPin, INPUT);
  pinMode(buttonPin, INPUT);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 1);
  u8x8.print("             ");
  u8x8.setCursor(0, 1);
  u8x8.print("Automatic");
  pinMode(ledPin, OUTPUT);
  pinMode(tonepin,OUTPUT);
}

void loop() {

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    if (selectedMode == 0) {
      selectedMode = 1;
      u8x8.setFont(u8x8_font_chroma48medium8_r);
      u8x8.setCursor(0, 1);
      u8x8.print("             ");
      u8x8.setCursor(0, 1);
      u8x8.print("Manual");
    } else {
      selectedMode = 0;
      u8x8.setFont(u8x8_font_chroma48medium8_r);
      u8x8.setCursor(0, 1);
      u8x8.print("             ");
      u8x8.setCursor(0, 1);
      u8x8.print("Automatic");
    }
  }

  if (selectedMode == 0) {
    tempToSpeed();
  } else {
    rotaryToSpeed();
  }

  motor.rotate(motor1, motorSpeed, motorDirection);
  delay(100);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  u8x8.print("Temp:");
  u8x8.setCursor(6, 0);
  u8x8.print(t);
}

void tempToSpeed()
{
  float readData = DHT.read11(dataPin);
  t = DHT.temperature;
  motorSpeed = map(t, tempMin, tempMax, speedMin, speedMax);
  if (t < tempMin)
  {
    motorSpeed = 0;
    motor.brake(1); 
    noTone(tonepin);
  }
  if (t >= tempMin && t <= tempMid)
  {
    motorSpeed = 50;
    noTone(tonepin);
  }
  if (t > tempMid && t <= tempMax)
  {
    motorSpeed = 70;
    noTone(tonepin);
  }
  if (t > tempMax && t < extreme)
  {
    motorSpeed = 100;
    noTone(tonepin);
  }
  if (t >= extreme)
  {
    motorSpeed = 100;
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
    tone(tonepin,294);
    delay(100);
    noTone(tonepin);
  }
}


void rotaryToSpeed()
{
  rotaryValue = analogRead(rotaryPin);
  motorSpeed = map(rotaryValue, voltMin, voltMax, speedMin, speedMax);
  motorSpeed = motorSpeed / 26.08;
}
