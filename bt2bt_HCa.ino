/*
By   : Phillipe Cantin
Desc.: Basic automatic connection between 2 HC-type bluetooth modules 

*/
#include <SoftwareSerial.h>
#include <Time.h>

int gLedPin = 2;
int gPowerPin = 12;
int gKeyPin = 9;
int gRxPin = 10;
int gTxPin = 11;
int gResetPin = 8;

int gIsMaster = 0
;

SoftwareSerial BTSerial(gRxPin, gTxPin);

void BtReset(void){
    Serial.println("BtReset");    
  BTSerial.flush();
  delay(500);
  
  //Method 1: Using the power
  digitalWrite(gPowerPin, LOW);  
  delay(2000); 
  digitalWrite(gPowerPin, HIGH);
  
/*
  //Method 2: Using the reset pin
  digitalWrite(gResetPin, LOW);
  delay(2000);
  digitalWrite(gResetPin, HIGH);
  */
  delay(500);
}

void BtSetCmdMode(int i_cmdMode = 1){
    Serial.print("BtSetCmdMode ");    
    Serial.println(i_cmdMode);    
  digitalWrite(gKeyPin, (1 == i_cmdMode)?HIGH:LOW);
  
  BtReset();
  
  BTSerial.begin((1 == i_cmdMode)?38400:57600);//57600
}

void BtSetupRole(int i_isMaster = 1){
  BTSerial.begin(38400);
  BtSetCmdMode(1);
  BTSerial.println("AT");
  delay(1000);
  BTSerial.println("AT+UART=57600,0,0");
  delay(1000);
  
  if(0 == i_isMaster){
    BTSerial.println("AT+ROLE=0");
    Serial.println("Set as Slave");
  } else {
    BTSerial.println("AT+ROLE=1");
    Serial.println("Set as Master");
  }
  delay(1000); 

  BtSetCmdMode(0); 
}

void BtStartMaster(void){
  BtSetCmdMode(1);
  
  BTSerial.println("AT+INIT");
  Serial.println("AT command: AT+INIT");
  delay(500); 
  BTSerial.println("AT+LINK=13,3,191407");
  Serial.println("AT command: AT+LINK=");
  delay(100); 

  BtSetCmdMode(0);
}

void setup() 
{
  pinMode(gLedPin, OUTPUT);
  pinMode(gPowerPin, OUTPUT);
  digitalWrite(gPowerPin, LOW);
  pinMode(gKeyPin, OUTPUT);
  digitalWrite(gKeyPin, LOW);
  
  Serial.begin(9600);
  Serial.println("bt2bt HC module - version a");
  
 if(1 == gIsMaster){
   BtSetupRole(1);
   BtStartMaster();
 }else{
   BtSetupRole(0);
 }  
}

void loop()
{
  if(1 == gIsMaster){
    BTSerial.print("1");
    delay(1000);
    BTSerial.print("0");
    delay(1000);
  }else{
    if (BTSerial.available()){
      char tmpChar = BTSerial.read();
      Serial.println(tmpChar);
      if(tmpChar == '1'){
        digitalWrite(gLedPin, HIGH);
      } else if(tmpChar == '0'){
        digitalWrite(gLedPin, LOW);
      }
    }
  }
}


