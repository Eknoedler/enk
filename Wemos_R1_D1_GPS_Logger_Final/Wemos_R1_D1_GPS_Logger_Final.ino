#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include "SPI.h"
#include "SD.h"
File dataFile;
File AllData;
SoftwareSerial serial_connection(D1, D3); //RX=pin 1, TX=pin 3
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
// process and flow related variables
int updates;
int failedUpdates;
int pos;
int stringplace = 0;
int TimeZone = 4;
int GPSRead;
int TimeEST;
int AltAdj;
int GPSAlt;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);//This opens up communications to the Serial monitor in the Arduino IDE
  serial_connection.begin(9600);//This opens up communications to the GPS
  Serial.println("GPS Started");//Just show to the monitor that the sketch has started
  if (!SD.begin(4)) {
    Serial.println("No SD Card");
    // don't do anything more
  }
  else {
    Serial.println("Card Found");
  }
}

void loop()
{

  dataFile = SD.open("gps.txt", FILE_WRITE);
  AllData = SD.open("AllData.txt", FILE_WRITE);


  while (serial_connection.available()) //While there are characters to come from the GPS
  {
    gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }
  if (gps.location.isUpdated()) //This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    //Serial.print("Satellite Count: ");
    Serial.println(gps.satellites.value());
    //Serial.print("Latitude:        ");
    Serial.println(gps.location.lat(), 6);
    //Serial.print("Longitude:       ");
    Serial.println(gps.location.lng(), 6);
    //Serial.print("Speed MPH:       ");
    Serial.println(gps.speed.mph());
    //Serial.print("Altitude Feet:   ");
    GPSAlt = gps.altitude.feet();
    AltAdj = GPSAlt - 800;
    Serial.println(AltAdj);
    Serial.println("");
    GPSRead  = gps.time.hour();
    TimeEST = GPSRead - TimeZone;
    Serial.println(TimeEST);
    delay(100);

    Serial.println("writing");//When data has been collected write it to the SD
    digitalWrite(LED_BUILTIN, LOW);
    // longitude
    dataFile.print(gps.location.lng(), 6);
    dataFile.print(",");
    // latitude
    dataFile.print(gps.location.lat(), 6);
    dataFile.print(",");
    // Alt
    dataFile.print(AltAdj);
    dataFile.println("");
    Serial.println("done");

    AllData.print("Longitude: ");
    AllData.print(gps.location.lng(), 6);

    AllData.print(" Longitude: ");
    AllData.print(gps.location.lng(), 6);

    AllData.print(" Altitude: ");
    AllData.println(gps.altitude.feet());

    AllData.print("Time: ");
    AllData.print(TimeEST);
    AllData.print(":");
    AllData.print(gps.time.minute());
    AllData.print(" Number of Sats: ");
    AllData.print(gps.satellites.value());
    AllData.print(" Speed: ");
    AllData.println(gps.speed.mph());

  }
  dataFile.close();// close file on SD card
  AllData.close();
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
}
