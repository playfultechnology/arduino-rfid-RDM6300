/***
 * RDM6300 reader
 * 
 * Demonstrates a basic RFID puzzle using a single RDM6300 reader. 
 * When any tag is held up to the reader, the UID is displayeed as both a series of HEX bytes
 * and a long integer (in many cases, this is the format as printed on an EM4100 card itself).
 * If the tag scanned matched the correctID defined at the top of the code, a relay connected
 * to the Arduino board is activated, which can power any load such as a maglock.
 **/

// DEFINES
#define DEBUG
#define startByte 2
#define endByte 3

// LIBRARIES
// Arduino UNO/Nano etc. only have one dedicated hardware port. 
// We'll use AltSoftSerial library (superior alternative to SoftwareSerial library)
// to define an additional software serial port
// Download from https://github.com/PaulStoffregen/AltSoftSerial
#include <AltSoftSerial.h>

// CONSTANTS
// Data structure returned by the RDM6300 is 14 bytes long, as follows:
// 1 byte head (always has value 0x02),
// 2 bytes version,
// 8 bytes data,
// 2 bytes checksum, 
// 1 byte tail (always has value 0x03)
const byte tagLength = 14;
// This pin will be driven LOW when reader detects the correct tag
const byte relayPin = 12;
// Define the "correct" tag to be identified
const byte correctID[tagLength] = {2,53,57,48,48,66,52,55,66,52,70,68,57,3};

// GLOBALS
// Define the reader object (AltSoftSerial always uses Pin 8 as RX)
AltSoftSerial RDM6300;
// Byte array of the ID of the current tag
byte currentTag[tagLength];

void setup() {
  // Set the relay as output and write HIGH value to it
  // (to e.g. lock any maglock attached to the load side) 
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  
  // Initialise serial interface to PC 
  Serial.begin(115200); 

  // Initialise serial interface to RDM6300 (always operates @9600 baud)
  RDM6300.begin(9600);
  
  Serial.println(F("RDM6300 Initialised"));
}
     
void loop() {
  // If the reader has sent data
  if (RDM6300.available() > 0){

    // Read the first byte
    byte incomingByte = RDM6300.read(); 

    // If this byte is the start of a tag
    if (incomingByte == startByte) {
      // Store it as the first byte in our currentTag array
      currentTag[0] = startByte; 
      // Read all remaining bytes in this tag
      RDM6300.readBytesUntil(endByte, currentTag+1, tagLength);
      // Append the endbyte onto the end of the currentTag array
      currentTag[tagLength-1] = endByte; 
      // Read (and discard) anything else that might be in the serial buffer to flush it out
      while(RDM6300.available()){ RDM6300.read(); }

      // Print out UID of tag
      Serial.print(F("Card Detected: "));
      // Print tag as HEX bytes
      printTagRaw(currentTag);
      Serial.print("|");
      // Print tag as Integer
      printTagFormatted(currentTag);
      Serial.println();

      // Compare tag against "correct" tag
      if(memcmp(currentTag, correctID, 14) == 0){
        Serial.print(F("CORRECT!"));
        digitalWrite(relayPin, LOW);
      }
    }
  }
  // Slight delay before checking for new card
  delay(100);
}

// Print each individual ASCII byte in the tag data received
void printTagRaw(byte currentTag[]){
  // Just loop through and print each byte individually
  for(int i=0; i<tagLength; i++){
    Serial.print(currentTag[i], HEX);
    Serial.print(F(" "));
  }
}

// Print card data as long integer (most likely as printed on card itself)
void printTagFormatted(byte currentTag[]){
  // Allocate memory for a new variable to hold 8 bytes of tag data + 1 byte char terminator
  char* strTag = malloc(9);
  // Copy 8 bytes of tag data from the tag (ignore the start byte and version number in the first 3 bytes)
  memcpy(strTag, currentTag+3, 8);
  // Append a trailing string terminator
  strTag[8] = '\0';
  // Convert null-terminated string to long integer
  long intTag = strtol(strTag, NULL, 16);
  // Clean up
  free(strTag);
  // Print the result
  Serial.print(intTag);
}
