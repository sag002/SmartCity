
#define BLYNK_PRINT SwSerial

#define limit_max  400  // 500cm
#define limit_min  20  // 100cm
#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11); // RX, TX

#include <BlynkSimpleStream.h>
#include <DHT.h>
#include "MQ135.h"

char auth[] = "KiaLapIT9SxWQfAwd6PUKPQ79QjYACMD";

#define DHTPIN 2   // What digital pin we're connected to
#define DHTTYPE DHT11// DHT 11
#define R_Pin A0
#define echoPin 11 // Echo Pin of Ultrasonic Sensor
#define pingPin 12 // Trigger Pin of Ultrasonic Sensor
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

int led_1;
int led_2;
int led_3;

int ir1 = 10;
int led1 = 6;

int ir2 = 9;
int led2 = 5;

int ir3 = 8;
int led3 = 4;

int ldr = A4;
int val =0;
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  long duration, inches, cm;
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  
  MQ135 gasSensor = MQ135(A2);
  float air_quality = gasSensor.getPPM();
  
  bool IsRain=digitalRead(R_Pin);
  
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(pingPin, LOW);
  
  duration = pulseIn(echoPin, HIGH); // using pulsin function to determine total time
  inches = microsecondsToInches(duration); // calling method
  cm = microsecondsToCentimeters(duration); // calling method
  Blynk.virtualWrite(V7, inches);
  if(inches < limit_min){
    Blynk.virtualWrite(V8, 255);
  }else{
    Blynk.virtualWrite(V8, 0);
  }

  if (isnan(h) || isnan(t)) {
    SwSerial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  
   if (isnan(air_quality)) {
    SwSerial.println("Failed to read from MQ135 sensor!");
    return;
  }
  Blynk.virtualWrite(V1, air_quality);
  
  Blynk.virtualWrite(V3, IsRain);
}

void setup()
{
  // Debug console
  SwSerial.begin(9600);
  pinMode(pingPin, OUTPUT); // initialising pin 3 as output
  pinMode(echoPin, INPUT); // initialising pin 2 as input

  pinMode(ir1,INPUT);
  pinMode(led1,OUTPUT);

  pinMode(ir2,INPUT);
  pinMode(led2,OUTPUT);

  pinMode(ir3,INPUT);
  pinMode(led3,OUTPUT);
  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(9600);
  Blynk.begin(Serial, auth);

  dht.begin();
  
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  
  int s1 = digitalRead(ir1);
  int s2 = digitalRead(ir2);
  int s3 = digitalRead(ir3);

  val = analogRead(ldr);

  if(val<800)
  {
    if(s1==0)
    {
      digitalWrite(led1,LOW);
    }
    else
    {
      digitalWrite(led1,HIGH);
    }
    if(s2==0)
    {
      digitalWrite(led2,LOW);
    }
    else
    {
      digitalWrite(led2,HIGH);
    }

    if(s3==0)
    {
      digitalWrite(led3,LOW);
    }
    else
    {
      digitalWrite(led3,HIGH);
    }
  }
  else
  {
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
  }
  
  Blynk.run();
  timer.run();
}

long microsecondsToInches(long microseconds) // method to covert microsec to inches 
{
 return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds) // method to covert microsec to centimeters
{
   return microseconds / 29 / 2;
}
