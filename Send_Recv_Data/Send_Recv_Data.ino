#include<string.h>
#define LASERPIN 13
#define TRIGPIN 10
#define ECHOPIN 11
#define PIRPIN 2
#define SOLARPIN A0
#define THRESHOLD 140
#define LP 2
#define READLED 6
#define WRITELED 8
long duration;
float dist;

int prevReadTime=millis();
int prevWriteTime=millis();
int readDelay=5000;
int writeDelay=5000;

String data="";

void setup() {
  // put your setup code here, to run once:
  pinMode (LASERPIN, OUTPUT) ;
  pinMode (TRIGPIN,OUTPUT);
  pinMode (ECHOPIN,INPUT);
  pinMode (PIRPIN,INPUT);
  pinMode(SOLARPIN, INPUT);
  pinMode(READLED,OUTPUT);
  pinMode(WRITELED,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int timeNow=millis();
  prevReadTime=timeNow;
  Serial.println("Reading");
  digitalWrite(READLED,HIGH);
  while(timeNow-prevReadTime<readDelay){
    int reading = analogRead(SOLARPIN);

    int bits[8];

    //Listening for the start bit
    if (reading > THRESHOLD) {
      for (int i =0; i<8 ; i++) {
        if (analogRead(SOLARPIN) > THRESHOLD) {
          bits[i] = 1 ;
        }
        else {
          bits[i] = 0;
        }
        delay(10);
      }

      int m = 0;
      for (int j =1; j <8; j++) {
        if (bits[j] ==1) {
          m = m + (1<<(7-j));
        }
      }
  
      char n=m;
      data+=n;
      Serial.print(n);
    }
    timeNow=millis();
  }
  digitalWrite(READLED,LOW);
  // Serial.println(data);
  timeNow=millis();
  prevWriteTime=timeNow;
  Serial.println("Writing");
  digitalWrite(WRITELED,HIGH);
  while(timeNow-prevWriteTime<writeDelay){
    // Serial.println(timeNow-prevWriteTime);
    int val = digitalRead(PIRPIN);
    if (val == HIGH) {
      digitalWrite(TRIGPIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIGPIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIGPIN, LOW);
      duration = pulseIn(ECHOPIN, HIGH);
      dist = duration * 0.034 / 2;

      String d =data+"_"+String(dist)+"_"+String(LP)+"_";

      Serial.println(d);

      int length = d.length();
      int ar[length+1];
      int m;
      int bits[8];

      for (int i =0 ; i<length ; i++ ) {
        ar[i]= int(d[i]);
      }
      for (int n =0 ; n<length ; n++ ){
        m=ar[n];
        int z;
        int bin[7];
        int newbin[7];
        for(z=0;z<8;z++){
          bin[z] = m%2;
          m = m /2 ; 
        }
      
        for (int j= 7 ; j>=0 ; j-- ){
          newbin[7-j] = bin[j] ;
        }

        for( int p=0 ; p<8 ; p++ ){  
          if (newbin[p] == 1){
            bits[p] = HIGH ;
          }
          if ( newbin[p] == 0){
            bits[p] = LOW ;
          }
        }

        bits[0] = HIGH;
      
        for (int i = 0; i < 8; i++) {
          digitalWrite(LASERPIN, bits[i]);
          delay(10);
        }

        digitalWrite(LASERPIN, LOW);

        delay(100);
      }
    }
    timeNow=millis();
  }
  // prevWriteTime=timeNow;
  digitalWrite(WRITELED,LOW);
  data="";
}