//www.elegoo.com
//2016.06.13

#include <DallasTemperature.h>

#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
  // Start up the library
  sensors.begin();
}
void loop(void)
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  //Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.println("DONE");

  double celcius = sensors.getTempCByIndex(0);
  double fahrenheight = ((9.0 / 5) * celcius) + 32;

  //Serial.println("New reading");
  //Serial.print("Celcius: ");
  //Serial.println(celcius);
  //Serial.print("Fahrenheigh: ");
  Serial.println(fahrenheight);
}
