#include "DHT.h"
#include <DallasTemperature.h>
#include <OneWire.h>

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define ONE_WIRE_BUS 3
#define IN1 2
#define IN2 4
#define IN3 12
#define IN4 13
#define EN1 5
#define EN2 6



uint8_t DHTin = 11; 
uint8_t DHTout = 10;
float humidity_in, humidity_out, temp_in, temp_out, inwater, dewpoint,alpha;

DHT dhtinside(DHTin, DHTTYPE);                
DHT dhtoutside(DHTout, DHTTYPE); 

OneWire oneWire(ONE_WIRE_BUS);               
DallasTemperature dsb_in(&oneWire);
//DeviceAddress deviceAddress;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(DHTin, INPUT);
  pinMode(DHTout, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  dhtinside.begin();
  delay(50);
  dhtoutside.begin();
  delay(50);
  dsb_in.begin();
  delay(50);

}

void loop() {
  // put your main code here, to run repeatedly:
    dsb_in.requestTemperatures();
    delay(100);
    float inwater = dsb_in.getTempCByIndex(0); 
    temp_in = dhtinside.readTemperature(); // Gets the values of the temperature
    humidity_in = dhtinside.readHumidity();
    delay(100);// Gets the values of the humidity  
    temp_out = dhtoutside.readTemperature();
    humidity_out = dhtoutside.readHumidity();
    delay(100);
    // Right Fomula Dewpoint 
    // Dewpoint = (bα(T,RH)) / (a - α(T,RH))
    // b = 243.12
    // a = 17.62 
    // α(T,RH) = ln(RH/100) + aT/(b+T)

    
    alpha = (log10(humidity_out)-2.0)/0.4343+(17.62*temp_out)/(243.12+temp_out);
//    dewpoint = ((temp_out - ((100 - humidity_out)) / 5.0));
    dewpoint = 243.12*alpha/(17.62-alpha);
    Serial.print("inh-");
    Serial.print(humidity_in);
    Serial.print(" ");
    Serial.print("int-");
    Serial.print(temp_in);
    Serial.print(" ");
    Serial.print("outh-");
    Serial.print(humidity_out);
    Serial.print(" ");
    Serial.print("outt-");
    Serial.print(temp_out);
    Serial.print(" ");
    Serial.print("wint");
    Serial.print(inwater);
    Serial.print(" ");
    Serial.print("dp-");
    Serial.println(dewpoint);
    
    if (temp_in >= dewpoint-5.0){
      coldpump(500);
    }
    else{
      hotpump(500);
    }
   
  stoppump(1000);
    
    
} 
void coldpump(int delays){
    analogWrite(EN1, 254);
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2, LOW);
    delay(delays);
    
}
void hotpump(int delays){
    analogWrite(EN2, 254);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4, LOW);
    delay(delays);
}
void stoppump(int delays){
  analogWrite(EN1, 0);
  analogWrite(EN2, 0);
  delay(delays);
}
