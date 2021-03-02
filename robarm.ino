#include<Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial fingerPrint(2,3);

#include <Adafruit_Fingerprint.h>
uint8_t id;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);

#define echopin A1 // echo pin
#define trigpin A0 // trig pin

int M_A1 = 4; //motor a = +
int M_A2 = 5; //motor a = -
int M_B1 = 6; //motor b = -
int M_B2 = 7; //motor b = +

Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;
Servo servo_5;

int servo1 = 90;
int servo2 = 90;
int servo3 = 90;
int servo4 = 90;

char state;

int count=0, stop=0;
int  dist;
long duration;

int buz = 13;


void setup(){   
servo_1.attach(8);
servo_2.attach(9);
servo_3.attach(10);
servo_4.attach(11);
servo_5.attach(12); 
 
servo_1.write(servo1); 
servo_2.write(servo2); 
servo_3.write(servo3); 
servo_4.write(servo4); 
servo_5.write(90);

pinMode(M_B1, OUTPUT);
pinMode(M_B2, OUTPUT);
pinMode(M_A1, OUTPUT);
pinMode(M_A2, OUTPUT);

pinMode(buz, OUTPUT);

pinMode(trigpin, OUTPUT);
pinMode(echopin, INPUT);

Serial.begin(9600);
finger.begin(57600);
 delay(1000);
 if (finger.verifyPassword()) {
  Serial.println("Found fingerprint sensor!");
  delay(1000);
  }else {Serial.println("Did not find fingerprint sensor :(");
    while (1);
    }

delay(1000);
} 

void loop(){
if(stop==0){
int result=getFingerprintIDez();
if(result>0){Serial.print("ID Ok =");Serial.println(result); stop=1;
  return;
 }
}


if(Serial.available() > 0){     
  state = Serial.read();   
  Serial.println(state);
} 

if(stop==1){
dist = data();
//Serial.println(dist); 

if (state == 'd'){
digitalWrite(M_A1, HIGH);
digitalWrite(M_A2, LOW);
digitalWrite(M_B1, LOW);
digitalWrite(M_B2, HIGH);  
}
 
else if (state == 'b'){
digitalWrite(M_A1, HIGH);
digitalWrite(M_A2, LOW);
digitalWrite(M_B1, HIGH);
digitalWrite(M_B2, LOW);    
}

else if (state == 'c'){
digitalWrite(M_A1, LOW);
digitalWrite(M_A2, HIGH);
digitalWrite(M_B1, LOW);
digitalWrite(M_B2, HIGH);  
}

else if (state == 'a'){
if(dist>30){
digitalWrite(M_A1, LOW);
digitalWrite(M_A2, HIGH);
digitalWrite(M_B1, HIGH);
digitalWrite(M_B2, LOW);    
}else{
digitalWrite(M_A1, LOW);
digitalWrite(M_A2, LOW);
digitalWrite(M_B1, LOW);
digitalWrite(M_B2, LOW); 
  }
}
    
else if (state == 'e'){
digitalWrite(M_A1, LOW);
digitalWrite(M_A2, LOW);
digitalWrite(M_B1, LOW);
digitalWrite(M_B2, LOW);     
}


if (state == 'f'){
if(servo1<180){servo1 = servo1+1;}
servo_1.write(servo1);  
}

if (state == 'g'){
if(servo1>0){servo1 = servo1-1;} 
servo_1.write(servo1);  
}

if (state == 'i'){
if(servo2>0){servo2 = servo2-1;}
servo_2.write(servo2);
}

if (state == 'h'){
if(servo2<180){servo2 = servo2+1;}
servo_2.write(servo2);  
}

if (state == 'n'){
if(servo3>0){servo3 = servo3-1;}
servo_3.write(servo3);  
}

if(state == 'o'){
if(servo3<180){servo3 = servo3+1;}
servo_3.write(servo3);
}

if(state == 'o'){
if(servo4>0){servo4 = servo4-1;}
servo_4.write(servo4);
}

if (state == 'n'){
if(servo4<180){servo4 = servo4+1;}
servo_4.write(servo4);
}

if (state == 'q'){servo_5.write(90);}
if (state == 'p'){servo_5.write(150);}

delay(40);
}else{
digitalWrite(M_A1, LOW);
digitalWrite(M_A2, LOW);
digitalWrite(M_B1, LOW);
digitalWrite(M_B2, LOW);
state = "";
}

     if(state == '1'){count=1;}
else if(state == '2'){count=2;}
else if(state == '3'){count=3;}
else if(state == '4'){count=4;}
else if(state == '5'){count=5;}
else if(state == '6'){count=6;}
else if(state == '7'){count=7;}
else if(state == '8'){count=8;}
else if(state == '9'){count=9;}
else if(state == 'S'){stop=0;} 

if(count>0){
if(state == 'A'){Enroll();}   
if(state == 'D'){delet();}
}

}



void Enroll(){
while(1){
id=count;
getFingerprintEnroll();
count = 0;
return;
}
}

void delet(){
while(1){
id=count;
deleteFingerprint(id);
count = 0;
return;         
}
}

long data(){
 digitalWrite(trigpin,LOW);
 delayMicroseconds(2);
 digitalWrite(trigpin,HIGH);
 delayMicroseconds(10);
 duration=pulseIn (echopin,HIGH);
 return duration / 29 / 2; 
}

uint8_t getFingerprintEnroll() {
  int p = -1;
  delay(1000);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No Finger");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }
  // OK success!
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      return;
    }
  }
  // OK success!
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  }
  else {
    Serial.println("Unknown error");
    return p;
  }   
}
int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  
  if (p != FINGERPRINT_OK)  
  return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  
  return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
 digitalWrite(buz, HIGH);   
 delay(2000);  
 digitalWrite(buz, LOW);
  return -1;
  }
  // found a match!
  Serial.print("Found ID #"); 
  Serial.print(finger.fingerID); 
  return finger.fingerID; 
}
uint8_t deleteFingerprint(uint8_t id){
  uint8_t p = -1;  
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } 
  
  else {
    Serial.print("Something Wrong"); 
    delay(2000);
    return p;
  }   
}
