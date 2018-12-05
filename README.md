# arduino-rfid-RDM6300
Code, wiring diagrams, and documentation describing how to interface an Arduino with RDM6300 module to read 125kHz EM4100 tags. When a card with the correct tag ID is read, as defined in the code, a relay is activated which can, for example, unlock a maglock.
![RDM6300 module](https://github.com/playfultechnology/arduino-rfid-RDM6300/raw/master/documentation/RDM6300.jpg)

The reader communicates via serial interface - since Arduino UNO/Nano only have a single hardware serial port (and that is already being used by the USB connection), the [AltSoftSerial](https://github.com/PaulStoffregen/AltSoftSerial) library is used to create a software serial connection on Pin 8. The RDM6300 transmits the data of any tags read via its TX pin to the Arduino on this single pin (there is no need to connect the RX pin, since no commands nor data need to be sent *to* the reader). 

The RDM6300 is a very cheap reader, and very easy to use, but it has only a limited read range of a few cm and there is no easy, reliable way to connect multiple readers to the same Arduino. Furthermore, there is no way to detect when a tag has been *removed* from the reader. For a comparison of alternative readers, see https://www.patreon.com/posts/rfid-roundup-23115452

# Wiring
![RDM6300 to Arduino using Serial interface](https://raw.githubusercontent.com/playfultechnology/arduino-rfid-RDM6300/864722c7aec461057b4b85dabd1add70258897b7/wiring/RDM6300_bb.jpg)
