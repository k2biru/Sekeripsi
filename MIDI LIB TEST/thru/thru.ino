#include <MIDI.h>

static const unsigned ledPin = 2;
MIDI_CREATE_DEFAULT_INSTANCE();
void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  MIDI.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (MIDI.read()) {
    MIDI.send (MIDI.getType(), MIDI.getData1(), MIDI.getData2(), MIDI.getChannel());
  }
}
