// IR-Transponder v1.0 by Aether4E0A 6/23/2016
// This is meant to be compatible with I-Laps systems.
// Compiles using Arduino IDE 1.6.6.

// Hardware is an Arduino Pro Mini, 5v, 16MHz. It has two series diodes added to reduce
// the supply voltage, to prevent reverse-breakdown of the IR diode at 5v. A series diode
// could instead be added to the IR diode, but this has not yet been tested.

// IR diode is OSRAM SFH 4641, with 120 Ohm series resistor. Anode is connected to pin 3,
// while Cathode is connected to pin 8 using the resistor.    3 ---|>|---^^^--- 8
// Pin 8 has the UART 38400 signal, while pin 3 generates the carrier frequency of 455KHz.
// The IR diode only conducts in one direction, thus we can use fastPWM to modulate the
// carrier frequency on the UART signal, using the anode. This greatly simplifies things.

#include <avr/io.h>
#include <util/delay.h>
#include <SoftwareSerial.h> // Software serial allows use of almost any pin.

SoftwareSerial mySerial(7, 8); // RX, TX // 8 as TX is all we care about.

void setup() {
  mySerial.begin(38400);  // Begin software serial interface at 38400.
  pinMode(3, OUTPUT);     // Set output pin for OCR2B
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20); // Set registers for fast PWM.
  TCCR2B = _BV(WGM22) | _BV(CS20); // Read the 328p datasheet for more details on fast PWM.
  OCR2A = 35; // At 16MHz, these values produce roughly 455KHz signal. 16M / 35 = 457K.
  OCR2B = 17; // This should be half of above value. This can be adjusted slightly for taste.
}

void loop() { // Transponder code "1234567" (0E AD DC BA 98 D6)
  mySerial.write(0x0e); // transmit one byte at a time.
  mySerial.write(0xad);
  mySerial.write(0xdc);
  mySerial.write(0xba);
  mySerial.write(0x98);
  mySerial.write(0xd6);
  delay(5); // wait 5ms before transmitting again.
}
