#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include "SPI.h"
#include "SD.h"
File dataFile;
File AllData;
SoftwareSerial serial_connection(D1, D3); //RX=pin 10, TX=pin 11
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
// process and flow related variables
int updates;
int failedUpdates;
int pos;
int stringplace = 0;
int GPSTime = 0; 
int TimeAdjust = 0;
int TimeZone = 4;

void setup()
{
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
    Serial.println(gps.altitude.feet());
    Serial.println(TimeAdjust);
    Serial.println("");
    delay(50);

    Serial.println("writing");//When data has been collected write it to the SD
    // longitude
    dataFile.print(gps.location.lng(), 6);
    dataFile.print(",");
    // latitude
    dataFile.print(gps.location.lat(), 6);
    dataFile.print(",");
    // Alt
    dataFile.print(gps.altitude.feet());
    dataFile.println("");

    AllData.print("Longitude: ");
    AllData.print(gps.location.lng(), 6);

    AllData.print(" Latitude: ");
    AllData.print(gps.location.lat(), 6);

    AllData.print(" Altitude: ");
    AllData.println(gps.altitude.feet());

    GPSTime = gps.time.hour();
    TimeAdjust = (GPSTime - TimeZone);

    AllData.print("Time: ");
    AllData.print(TimeAdjust);
    AllData.print(":");
    AllData.print(gps.time.minute());
    AllData.print(" Number of Sats: ");
    AllData.print(gps.satellites.value());
    AllData.print(" Speed: ");
    AllData.println(gps.speed.mph());
    Serial.println("done");

  }
  dataFile.close();// close file on SD card
  AllData.close();
  delay(50);
}

/*
   $GPRMC,183729,A,3907.356,N,12102.482,W,000.0,360.0,080301,015.5,E*6F
  $GPRMB,A,,,,,,,,,,,,V*71
  $GPGGA,183730,3907.356,N,12102.482,W,1,05,1.6,646.4,M,-24.1,M,,*75
  $GPGSA,A,3,02,,,07,,09,24,26,,,,,1.6,1.6,1.0*3D
  $GPGSV,2,1,08,02,43,088,38,04,42,145,00,05,11,291,00,07,60,043,35*71
  $GPGSV,2,2,08,08,02,145,00,09,46,303,47,24,16,178,32,26,18,231,43*77
  $PGRME,22.0,M,52.9,M,51.0,M*14
  $GPGLL,3907.360,N,12102.481,W,183730,A*33
  $PGRMZ,2062,f,3*2D
  $PGRMM,WGS 84*06
  $GPBOD,,T,,M,,*47
  $GPRTE,1,1,c,0*07
  $GPRMC,183731,A,3907.482,N,12102.436,W,000.0,360.0,080301,015.5,E*67
  $GPRMB,A,,,,,,,,,,,,V*71
*/
