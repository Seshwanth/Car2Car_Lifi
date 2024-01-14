#include<string.h>
#define LASERPIN 13
#define TRIGPIN 10
#define ECHOPIN 11
#define PIRPIN 2
#define ENA 9
#define IN1 8
#define IN2 7
#define ENB 5
#define IN3 4
#define IN4 3
#define LP 1

long duration;
float dist;

void setup() {
  // put your setup code here, to run once:
  pinMode (LASERPIN, OUTPUT) ;
  pinMode (TRIGPIN,OUTPUT);
  pinMode (ECHOPIN,INPUT);
  pinMode (PIRPIN,INPUT);
  Serial.begin(9600);

  analogWrite(ENA, 255); 
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW);

  analogWrite(ENB, 255); 
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(PIRPIN);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(2);
  // Sets the TRIGPIN on HIGH state for 10 micro seconds
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);
  // Reads the ECHOPIN, returns the sound wave travel time in microseconds
    duration = pulseIn(ECHOPIN, HIGH);
  // Calculating the distance
    dist = duration * 0.034 / 2;
  } 
  else {
    return;
  }

  String data = String(dist)+"_"+String(LP)+"_$";

  Serial.println(data);

  int length = data.length();
  int ar[length+1];
  int m;
  int bits[8];

  for (int i =0 ; i<length ; i++ ) {
    ar[i]= int(data[i]);
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