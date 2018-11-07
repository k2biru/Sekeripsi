#include <ESP8266WiFi.h>
#include "ESPAsyncUDP.h"

#include "midi_type.h"


#define LED_INDIKATOR 2

const char * ssid = "MIDI";
const char * password = "MIDIMIDI";

AsyncUDP udp;

void setup() {
  Serial.begin(31250);
  //Serial.begin (115200);
  // Koneksi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  }
  if (udp.connect(IPAddress(192, 168, 43, 255), 1112)) {
    Serial.println("\n UDP connected : 192,168,43,255");
  }
  //Send unicast
  //udp.print("Aku Siap /n");
///////////////

  pinMode(LED_INDIKATOR, OUTPUT);
  delay(1000);
}

void loop() {
  while (Serial.available() ) {
    readSerial_(Serial.read()); // parsing data
  }
}

void kirim() {
  digitalWrite(LED_INDIKATOR, HIGH);

  //for (int i = 0; i < panjangPesanTersimpan; i++) {
  //  Serial.write(_midiMessage[i]);
  //}
  
  udp.write(_midiMessage, panjangPesanTersimpan);

  yield();
  //digitalWrite(LED_INDIKATOR, LOW);
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
      _dataCount = SYS_EX_MAX_SIZE + 3;
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
      yield();
    }
  } else {
    //data
    if (_dataCount) {
      _midiMessage[panjangPesanTersimpan] = _buff;
      panjangPesanTersimpan++;
      _dataCount--;
    }
    yield();
  }
  if (!_dataCount) {
    kirim();
    kosongkan();
  }
}

