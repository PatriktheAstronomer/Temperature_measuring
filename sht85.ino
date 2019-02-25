/*
 * Temperature and humidity measurement station sending data via ethernet
 * Made by Patrik Novotny in 2019 - open source licence
 * Schema - SHT85 to data pin A4, clock pin A5 + 3,3V, I2C address 0x44
 * Arduino Ethernet Shield 2 with controller W5500 runnign on 5V (not recommended to use PoE due to overheating)
 */

#include <Sensirion.h>    // All libraries are available on GitHub or in default IDE settings
#include <Wire.h>
#include <SPI.h>
#include <Ethernet2.h>
#include "cactus_io_SHT31.h"
cactus_io_SHT31 sht31;

float temperature = 0;
float humidity = 0;

                                    // Ethernet connection variables setting
char server[] = "75.45.120.125";    // IP Address (or name) of server to dump data to - for name use char, for IP Address byte, as you can see it below
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x05 };    // Enter a MAC address and IP address for your controller below.
IPAddress ip(72, 27, 210, 218);    // The IP address will be dependent on your local network.
IPAddress myDns(72, 27, 210, 1);
IPAddress gateway(72, 27, 210, 1);    // Gateway and subnet are optional
IPAddress subnet(72, 27, 210, 0);
EthernetClient client;
int ID = 5;

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);    //Setup of ethernet shield  pinMode(ledPin, OUTPUT);
  delay(15);                           // Wait at least 11 ms before first cmd
  Serial.println("Please wait a while");
  if (!sht31.begin()) {
  Serial.println("Count not find sensor. Check wiring and I2C address");
  while(1) delay(1);
  }
  Serial.println("Setup done");
}

void loop(){
  temperature = sht31.getTemperature_C();
  Serial.println(temperature);
  humidity = sht31.getHumidity();
  sendData();
} 

void sendData()
{
    if (client.connect(server, 80)) {    // Arduino get a connection to the server
    client.print( "GET http://75.45.120.125/add_data.php?");
    client.print("temperature=");
    client.print(temperature);
    client.print("&&");
    client.print("therperature=");
    client.print(0);
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
