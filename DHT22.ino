/*
 * Temperature and humidity measurement station sending data via ethernet
 * Made by Patrik Novotny in 2018 - open source licence
 * Schema - DHT22 to 2 digital pins + 3,3V,
 * NTC B57045K 1kOhm to 2 analog pin and 1k resistor collateraly;
 * Arduino Ethernet Shield 2 with controller W5500 runnign on 5V (not recommended to use PoE due to overheating)
 */

#include <Sensirion.h>    // All libraries are available on GitHub or in default IDE settings
#include <Wire.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <DHT.h>

                                    // Ethernet connection variables setting
char server[] = "10.26.210.125";    // IP Address (or name) of server to dump data to - for name use char, for IP Address byte, as you can see it below
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 };    // Enter a MAC address and IP address for your controller below.
IPAddress ip(10, 26, 210, 162);    // The IP address will be dependent on your local network.
IPAddress myDns(10, 26, 210, 1);
IPAddress gateway(10, 26, 210, 1);    // Gateway and subnet are optional
IPAddress subnet(255, 255, 255, 0);
EthernetClient client;
int ID = 1;    // To resolve between more Arduinos
                                      // Code variables setting
                                      
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz

const uint8_t ledPin  = 13;            // Arduino built-in LED
const uint32_t BLINKSTEP =  250UL;     // LED blink period
uint16_t rawData;
float temperature;    // [C]
float humidity;    // [%]
float Vin=5.0;     // [V]        
float Rt=1000;    // Resistor t [ohm]
float R0=1000;    // value of rct in T0 [ohm]
float T0=298.15;   // use T0 in Kelvin [K]
float Vout=0.0;    // Vout in A0 
float Rout=0.0;    // Rout in A0
float T1=273.15;      // [K] in datasheet 0º C
float T2=373.15;      // [K] in datasheet 100° C
float RT1=80.903;    // [ohms]   Resistence in T1
float RT2=3002.9;   // [ohms]  Resistence in T2
float beta=0.0;    // Initial parameters [K]
float Rinf=0.0;    // Initial parameters [ohm]   
float TempK=0.0;   // Variable output
float TempC=0.0;   // Variable output
byte ledState = 0;



void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);    //Setup of ethernet shield
  pinMode(ledPin, OUTPUT);
  dht.begin();
  delay(15);                           // Wait at least 11 ms before first cmd
  Serial.println("Please wait a while");
  beta=(log(RT1/RT2))/((1/T1)-(1/T2));    // Setup thermistor calculation
  Rinf=R0*exp(-beta/T0);
  digitalWrite(7, HIGH);    // I use it as power source how shielding, in case you do not use shielding, erase this line
  Serial.println("Setup done");
}

void loop()
{
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    sendData();
}

void Thermistor() {
  Vout=Vin*((float)(analogRead(0))/1024.0);    // NTC analog output
  Rout=(Rt*Vout/(Vin-Vout));

  TempK=(beta/log(Rout/Rinf));    // Calculation of temperature according to the resistivity of the thermistor
  TempC=TempK-273.15;
}

void sendData()
{
  if (client.connect(server, 80)) {    // Arduino get a connection to the server
    Thermistor();
    client.print( "GET http://10.26.210.125/add_data.php?");
    client.print("temperature=");
    client.print(temperature);
    client.print("&&");
    client.print("therperature=");
    client.print(TempC);
    client.print("&&");
    client.print("humidity=");
    client.print(humidity);
    client.print("&&");
    client.print("ID=");
    client.println(ID);
    client.println( "HTTP/1.1");
    client.print( "Host: " );
    client.println(server);
    client.println( "Connection: close" );
    client.println();
    Serial.println("Written in SQLdatabase");    // Data sucessfully saved
    client.stop();
    delay(60000);
  }
  else {
    Serial.println("--> connection failed");    // Arduino didn't get a connection to the server
  }
}
