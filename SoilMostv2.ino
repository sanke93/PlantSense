#include <AvvisoUSB.h>

#define DEBUG 0

int counterValue;
int counting;
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
int led = 13;
int photoCell=A1;
int photoRead=0;


void setup() {
  counterValue = 10;
  counting=0;
  Serial.begin(115200);

  
  Avviso.begin();
  Avviso.setApiKey("5c58051c41f47bc0d1d12f15459a35a5d6a4c105d7901e0f");
  Avviso.setApplicationName("PlantSense");
  pinMode(led, OUTPUT); 
}

void loop() {
  sensorValue = analogRead(sensorPin);    
  photoRead=analogRead(photoCell);
  delay(3000);  
  Serial.print("sensor = " );                       
  Serial.println(sensorValue);  
  Serial.print(" | photocell = " );                       
  Serial.println(photoRead); 
  Serial.print(" | Temperature (C)= " );  
  Serial.println(GetTemp(),1);
  Serial.print(" ");
  Serial.println();
  if (counterValue > 0) {
    if (DEBUG) Serial.println(counterValue);
    counterValue--;
    delay(200);
  } 
  else if(sensorValue>600 && sensorValue<1000 && counting<22){
  Avviso.push("Water level is kinda low", "Oops, better get on that!", 2);
  counting=23;
  digitalWrite(led, HIGH); 
  
  }  
  
  else if(sensorValue<601 || sensorValue>999 ){
    digitalWrite(led, LOW); 
  }
  else if (counting==90) {
    counterValue--;
    if (DEBUG) Serial.print("Sending push notification...");
    Avviso.push("Water level is low", "Oops, better get on that!", 0);
    if (DEBUG) Serial.println("OK.");
  }  
}

double GetTemp(void)
{
  unsigned int wADC;
  double t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  // The returned temperature is in degrees Celcius.
  return (t);
}
