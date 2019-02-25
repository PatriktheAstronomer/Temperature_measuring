/*
 * LDC tester for switch/router in UTIA Lab
 * Made by Patrik Novotny in 2019 - open source licence
 * Arduino Ethernet Shield 2 with controller W5500 runnign on 5V (not recommended to use PoE due to overheating)
 */
#include <Wire.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <LiquidCrystal.h>
                                    // Ethernet connection variables setting
char server[] = "75.45.120.125";    // IP Address (or name) of server to dump data to - for name use char, for IP Address byte, as you can see it below
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04 };    // Enter a MAC address and IP address for your controller below.
IPAddress ip(18, 26, 178, 264);    // The IP address will be dependent on your local network.
IPAddress myDns(18, 78, 210, 264);
IPAddress gateway(18, 26, 124, 264);    // Gateway and subnet are optional
IPAddress subnet(255, 255, 255, 0);
EthernetClient client;
int ID = 4;

LiquidCrystal lcd(8, 9, 5, 7, 3, 2);
// initialize the library with the numbers of the interface pins


void setup() {
  Ethernet.begin(mac, ip);    //Setup of ethernet shield
  delay(15);                           // Wait at least 11 ms before first cmd
  lcd.begin(16, 2);
  // set up the LCD's number of columns and rows: 
  // Print a message to the LCD.
  lcd.print("Router monitor");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  if (client.connect(server, 80)) {    // Arduino get a connection to the server
    client.print( "GET http://75.45.120.125/add_data.php?");
    client.print("temperature=");
    client.print(27.5);
    client.print("&&");
    client.print("therperature=");
    client.print(0);
    client.print("&&");
    client.print("humidity=");
    client.print(0);
    client.print("&&");
    client.print("ID=");
    client.println(ID);
    client.println( "HTTP/1.1");
    client.print( "Host: " );
    client.println(server);
    client.println( "Connection: close" );
    client.println();
    lcd.print("Running");    // Data sucessfully saved
    client.stop();
    delay(60000);
  }
  else {
    lcd.print("UNO disconnected");    // Arduino didn't get a connection to the server
  }
}
