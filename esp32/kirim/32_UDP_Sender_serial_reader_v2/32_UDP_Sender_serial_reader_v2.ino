#include "WiFi.h"
#include <WiFiUDP.h>
#include "midi_type.h"


#define LED_INDIKATOR 2

unsigned int UDPPort = 1112;

WiFiUDP Udp;

void setup() {
  Serial.begin(31250);
  //Serial.begin (115200);
  pinMode(LED_INDIKATOR, OUTPUT);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin("MIDI", "MIDIMIDI");
  Serial.println ("Connect to MIDI" );
  while (WiFi.status() != WL_CONNECTED) {
    delay (500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP : " + WiFi.localIP().toString());

  Udp.begin(UDPPort);
}

void loop() {
  while (Serial.available() ) {
    readSerial_(Serial.read());
  }
}

void kirim() {
  digitalWrite(LED_INDIKATOR, HIGH);
  Udp.beginPacket("192.168.43.255", UDPPort);
  Udp.write(_midiMessage, panjangPesanTersimpan);
  Udp.endPacket();
  /* thru mode
  for (int i = 0; i < panjangPesanTersimpan; i++) {
    Serial.print(_midiMessage[i]);
  }
  */
  yield;
  digitalWrite(LED_INDIKATOR, LOW);
}
void kosongkan() {
  for (unsigned int i = 0; i < (MIDI_BUFFER_MAX); i++) {
    _midiMessage[i] = NULL;
  }
}

MidiType getStatusType(byte status_) {
  if ((status_  < 0x80) ||
      (status_ == 0xf4) ||
      (status_ == 0xf5) ||
      (status_ == 0xf9) ||
      (status_ == 0xfD))
  {
    // KOSONG / bukan apa-apa
    return InvalidType;
  }
  if (status_ < 0xf0)
  {
    // hapus kanal
    return MidiType(status_ & 0xf0);
  }
  return MidiType(status_);
}

int getStatus_dataCount (byte inStatus) {
  switch (getStatusType(inStatus))
  {
    // 1 byte messages
    case Start:
    case Continue:
    case Stop:
    case Clock:
    case ActiveSensing:
    case SystemReset:
    case TuneRequest:
      // Tidak ada data, hanya pesan status saja
      _dataCount = 0;
      return true;
      break;

    // 2 bytes messages
    case ProgramChange:
    case AfterTouchChannel:
    case TimeCodeQuarterFrame:
    case SongSelect:
      // Ada 1 byte data dan 1 byte status
      _dataCount = 1;
      break;

    // 3 bytes messages
    case NoteOn:
    case NoteOff:
    case ControlChange:
    case PitchBend:
    case AfterTouchPoly:
    case SongPosition:
      // Ada 2 byte data dan 1 byte status
      _dataCount = 2;
      break;

    // Akhir SystemExclusive
    case EndOfSystemExclusive:
      _flagEndSysExclusive = true;
      _dataCount = 0;
      //_midiMessage[panjangPesanTersimpan+1]= '\n';
      break;

    case SystemExclusive:
      // 3 byte identifikasi
      // tidak terbatas
      _dataCount = SYS_EX_MAX_SIZE + 3; // 3 byte identitas
      _flagEndSysExclusive = false;
      break;

    case InvalidType:
    default:
      return false;
      break;
  }
}

void readSerial_(byte _buff) {
  if (_buff & FLAG_BIT_STATUS) {
    //status
    if (getStatus_dataCount(_buff)) {
      if (_flagEndSysExclusive) {
        _midiMessage[panjangPesanTersimpan] = _buff;
        panjangPesanTersimpan++;
        _flagEndSysExclusive = false;
      } else {
        _midiMessage[0] = _buff;
        _midiMessage[_dataCount + 1] = '\n' ;
        panjangPesanTersimpan = 1;
      }
    }
  } else {
    //data
    if (_dataCount) {
      _midiMessage[panjangPesanTersimpan] = _buff;
      panjangPesanTersimpan++;
      _dataCount--;
    }
  }
  if (!_dataCount) {
    kirim();
    kosongkan();
  }
}

