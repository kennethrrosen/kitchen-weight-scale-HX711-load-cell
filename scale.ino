/* 
TKTKKT
by: Questionable Mechanics
contact: questionable.mechanics@gmail.com
last rev: 2/2021
This sketch is in the public domain
Note: Tested using Arduino IDE ver 1.8.9 via VS Code
*/

#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>
#include <HX711.h>
#include <Wire.h>

#define I2C_ADDR    0x27

LiquidCrystal_I2C lcd(0x27,20,4); //I2C SCLK=A4, SDATA=A5 depends on board
HX711 LoadCell(4, 5);

void setup() {
  Serial.begin(115200);
  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(999.0); // calibration factor for load cell => dependent on your setup
  lcd.begin(16, 2); // begins connection to the LCD module
  lcd.backlight(); // turns on the backlight
}

void loop() {
  LoadCell.update(); // retrieves data from the load cell
  float i = LoadCell.getData(); // get output value
  lcd.setCursor(0, 0); // set cursor to first row
  lcd.print("Weight[g]:"); // print out to LCD
  lcd.setCursor(0, 1); // set cursor to secon row
  lcd.print(i); // print out the retrieved value to the second row
}


//*********************//

#include "HX711.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int IN1 = A0;
int IN2 = A1;  

int over_val;
int data;
int g_weight;
int Weight;

void setup()
{       
  pinMode(IN1, INPUT);
  pinMode(IN2, INPUT);
  Init_Hx711();     
  Serial.begin(9600);
  Serial.print("Ready!\n");
  Get_Maopi();
  lcd.begin(16, 2); 
  delay(1000);
}

void loop()
{       

Weight = Get_Weight();
g_weight = Weight-data;  
        
 lcd.setCursor(0,0);       
 lcd.print("Weight:");
 lcd.print(g_weight);
 lcd.print("g    ");
digitalRead(IN2) == LOW){data=Weight;}
digitalRead(IN1) == LOW){over_val=g_weight;}
  
delay(50);     
}



//*********************//

long sample=0;
float val=0;
long count=0;

unsigned long readCount(void)
{
  unsigned long Count;
  unsigned char i;
  pinMode(DT, OUTPUT);
  digitalWrite(DT,HIGH);
  digitalWrite(SCK,LOW);
  Count=0;
  pinMode(DT, INPUT);
  while(digitalRead(DT));
  for (i=0;i<24;i++)
  {
    digitalWrite(SCK,HIGH);
    Count=Count<<1;
    digitalWrite(SCK,LOW);
    if(digitalRead(DT)) 
    Count++;
  }
  digitalWrite(SCK,HIGH);
  Count=Count^0x800000;
  digitalWrite(SCK,LOW);
  return(Count);
}

void setup()
{
  pinMode(SCK, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  lcd.begin(16, 2);
  lcd.print("    Weight ");
  lcd.setCursor(0,1);
  lcd.print(" Measurement ");
  delay(1000);
  lcd.clear();
  calibrate();
}

void loop()
{
  count= readCount();
  int w=(((count-sample)/val)-2*((count-sample)/val));
  lcd.setCursor(0,0);
  lcd.print("Measured Weight");
  lcd.setCursor(0,1);
  lcd.print(w);
  lcd.print("g             ");

  if(digitalRead(sw)==0)
  {
    val=0;
    sample=0;
    w=0;
    count=0;
    calibrate();
  }
}

void calibrate()
{
    lcd.clear();
  lcd.print("Calibrating...");
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");
  for(int i=0;i<100;i++)
  {
    count=readCount();
    sample+=count;
  }
  sample/=100;
  lcd.clear();
  lcd.print("Put 100g & wait");
  count=0;
  while(count<1000)
  {
    count=readCount();
    count=sample-count;
  }
  lcd.clear();
  lcd.print("Please Wait....");
  delay(2000);
  for(int i=0;i<100;i++)
  {
    count=readCount();
    val+=sample-count;
  }
  val=val/100.0;
  val=val/100.0;        // put here your calibrating weight
  lcd.clear();
}