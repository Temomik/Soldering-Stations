
/* I2C LCD with Arduino example code. More info: https://www.makerguides.com */

// Include the libraries:
// LiquidCrystal_I2C.h: https://github.com/johnrickman/LiquidCrystal_I2C
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <EEPROM.h>   

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 20, 4); // Change to (0x27,16,2) for 16x2 LCD.

int presetTemp[3];
void readDataEeprom()
{
  for(int i = 0; i < 3; i++)
  {
    presetTemp[i] = EEPROM.read(i); 
  }
}

void writeDataEeprom()
{
  for(int i = 0; i < 3; i++)
  {
    EEPROM.update(i,presetTemp[i]); 
  }
}

void setup() {
  // Initiate the LCD:
  lcd.init();
  lcd.backlight();
  pinMode(A1,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,OUTPUT);
  pinMode(9,INPUT);
  pinMode(11,INPUT);
  pinMode(12,INPUT);
  readDataEeprom();
}
int PWM = 0;
int out = 0;
int presetNum = -1;
void loop() {
  if(digitalRead(3) == HIGH)
    PWM++;
  if(digitalRead(4) == HIGH)
    PWM--;
  if(digitalRead(9) == HIGH) {
    presetNum = 0;
    PWM = presetTemp[0];
  }
  if(digitalRead(5) == HIGH) {
    presetNum = 1;
    PWM = presetTemp[1];
  }
  if(digitalRead(11) == HIGH)
  { 
    presetNum = 2;
    PWM = presetTemp[2];
  }  
  if(digitalRead(12) == HIGH)
  {  
    if(presetNum >= 0)
    {
      presetTemp[presetNum] = PWM;
    }
    writeDataEeprom();
  }
  
  if(PWM > 400 || PWM < 0 )
    PWM = 0; 
  lcd.setCursor(0, 1);
  lcd.print("selected:   ");  
  lcd.setCursor(9, 1); 
  //int PWM = map(analogRead(A0),0,1023,0,400);
  lcd.print(PWM);
  lcd.setCursor(0, 0); 
  lcd.print("current:     ");
  lcd.setCursor(8, 0);
  if(out == 1){
    digitalWrite(6,0);  
  delay(10);
  }
  int temperature = (int)analogRead(A1);
  digitalWrite(6,out);
  int tmp = (temperature < 137)?0:temperature - 137;
  lcd.print(tmp); 
  if(tmp < PWM) {
    out = 1; 
  } else { 
    out = 0;  ;
  }
}
