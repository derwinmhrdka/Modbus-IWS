#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ModbusRtu.h>
#define ID 1
#define DHTTYPE DHT11

LiquidCrystal_I2C lcd(0x3f ,16,2);
Modbus slave(ID,0,0);

int8_t state = 0;
uint16_t data[6];
uint16_t h,t,s,statusPump;
const int soil=A0;
const int relay = 16;
const int DHTPin = 2;
DHT dht(DHTPin, DHTTYPE);

void setup()
{
  slave.begin(9600);
  Serial.begin(9600);
  dht.begin();
  lcd.backlight();
  lcd.begin(); 
  pinMode(relay,OUTPUT);
  digitalWrite(relay,LOW);
}
  
void loop()
{
      h = dht.readHumidity();
      t = dht.readTemperature();  
      s = analogRead(soil);
      
      data[3]=h;
      data[4]=t;
      data[5]=s;
      bitWrite(data[0],2, statusPump);  
      int dataTerima =bitRead(data[0],0);
      int dataMan =bitRead(data[0],1);
      state=slave.poll(data,6);
      lcd.clear();

      if (dataMan==0){
            lcd.setCursor(15,1);lcd.print("A");
            if     (s>550 || dataTerima > 0) {digitalWrite(relay,HIGH); statusPump=1; lcd.setCursor(9,1);lcd.print("P_On");
            }
            else                             {digitalWrite(relay,LOW); statusPump=0; lcd.setCursor(9,1);lcd.print("P_Off");}
      }
      else {
            lcd.setCursor(15,1);lcd.print("M");
            if     (dataTerima > 0)          {digitalWrite(relay,HIGH); statusPump=1; lcd.setCursor(9,1);lcd.print("P_On");
            }
            else                             {digitalWrite(relay,LOW); statusPump=0; lcd.setCursor(9,1);lcd.print("P_Off");}
      }
      lcd.setCursor(0,0);lcd.print("T: ");lcd.print(t);lcd.setCursor(6,0);lcd.print((char)223);lcd.setCursor(7,0);lcd.print("C");
      lcd.setCursor(9,0);lcd.print("H: ");lcd.print(h);lcd.setCursor(15,0);lcd.println("%");       
      lcd.setCursor(0,1);lcd.print("S: ");lcd.print(s);lcd.setCursor(7,1);lcd.print("%");
}
