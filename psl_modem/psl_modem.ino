/*
PSL modem is a gourd that applauds or boos whenever someone
tweets about pumpkin spice lattes.

Code can be found at github.com/joshsucher/pslmodem
Tutorial can be found at thingswemake.com/psl-modem

2014/10/19 - josh sucher - josh at thingswemake dot com

Heavily adopts from the Yun bridge library's MailboxReadMessage example,
Adafruit's VS1053 player_simple example, and
Adafruit / Arduino forum member Aero98's random track code.

http://arduino.cc/en/Tutorial/MailboxReadMessage
https://learn.adafruit.com/adafruit-vs1053-mp3-aac-ogg-midi-wav-play-and-record-codec-tutorial/software
http://forums.adafruit.com/viewtopic.php?f=31&p=307431

 */

#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

#include <Mailbox.h>

String tweetID = "";
//bool inUse = false;

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  //Adafruit_VS1053_FilePlayer(SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

File path;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  // Initialize Bridge and Mailbox
  Bridge.begin();
  Mailbox.begin();
  digitalWrite(13, HIGH);

  // Initialize Serial
 
  Serial.begin(9600);

randomSeed(analogRead(0));

  // Wait until a Serial Monitor is connected.
  //while (!Serial);

  Serial.println("YOLO PSL YOLO\n");
  
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  Serial.println("SD OK!");
  
  // list files
  printDirectory(SD.open("/"), 0);

path = SD.open("/");

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(5,5);

  if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT))
    Serial.println(F("DREQ pin is not an interrupt pin"));
}


void loop() {

//Serial.println(freeRam());
  
String message;
File results;
//File path = SD.open("/");
//char* MP3 = selectRandomFileFrom( path, results );
//Serial.println(MP3);
//delay(1000);

  // if there is a message in the Mailbox
  if (Mailbox.messageAvailable())
  {
      Mailbox.readMessage(message);
     
      //Serial.println("msg:" + message);
                  
      if(message != tweetID) {
        
       //Serial.println("id:" + message);

         musicPlayer.stopPlaying();     
         char* MP3 = selectRandomFileFrom(path, results);
         musicPlayer.startPlayingFile(MP3);
         //Serial.println("mp3" + String(MP3));
         tweetID = message;
         return;

    while (musicPlayer.playingMusic) {

    if (Mailbox.messageAvailable())
  {
    // read all the messages present in the queue
      Mailbox.readMessage(message);
      //Serial.println(message);
      
        if(message != tweetID) {
          //Serial.println("startover");
          tweetID = message;
          return;
        }
      }
      delay(250);
      }
     tweetID = message;
      }
    
}
delay(250);
}

/// File listing helper
void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      //Serial.println("**nomorefiles**");
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

char* selectRandomFileFrom( File dir, File result ) {
  File entry;
  int count = 0;

  dir.rewindDirectory();

  while ( entry = dir.openNextFile() ) {
    if ( random( count ) == 0 ) {
      result = entry;
    }
    entry.close();
    count++;
  }
  
  return result.name();   // returns the randomly selected file name
}

