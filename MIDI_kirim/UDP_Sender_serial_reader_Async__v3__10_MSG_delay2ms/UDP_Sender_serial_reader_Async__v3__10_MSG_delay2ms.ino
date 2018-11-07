//====================================
// Versi ini belum mendukung SysEx
//====================================

// uncomment for debug
//#define DEBUG_ON
#define DELAY_MS_BEFORE 2
#define THRU_ON

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
  pinMode(LED_INDIKATOR, OUTPUT);
  delay(1000);

#ifdef DEBUG_ON
  Serial.println ("<+++++++++++++++ Debug ON +++++++++++++++++++++ >");
#endif

  // Koneksi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  }
  if (udp.connect(IPAddress(192, 168, 0, 255), 1112)) {
    Serial.println("\n UDP connected : 192,168,0,255");
  }


}


void loop() {
  //while (Serial.available() ) {
  //  readSerial_(Serial.read());
  //}
  if (Serial.available()) {
    readSerial_multy();
  }
}

void kirim() {
  digitalWrite(LED_INDIKATOR, HIGH);

#ifdef DEBUG_ON
  Serial.print ("panjangPesanTersimpan : "); Serial.println(panjangPesanTersimpan);
  Serial.print("jumlahPesan : "); Serial.println(jumlahPesan);
#endif

  udp.write(_midiMessage, panjangPesanTersimpan);

#ifdef THRU_ON
  for (int i = 0; i < panjangPesanTersimpan; i++) {
    //  Serial.write(_midiMessage[i]);
    Serial.print(_midiMessage[i], HEX);
  }
  Serial.println();
#endif

#ifdef DEBUG_ON
  Serial.println();
#endif
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
      _dataCount = SYS_EX_MAX_SIZE + 3;
      _flagEndSysExclusive = false;
      break;

    case InvalidType:
    default:
      return false;
      break;
  }
}

void readSerial_multy() {
  //unsigned int loop = 0;
  byte _buff;
  delay(DELAY_MS_BEFORE);
  while (1) {
    if (Serial.available()) {
      _buff = Serial.read();

#ifdef DEBUG_ON
      Serial.println("Masuk Serial multy");
#endif

      /// filtrasi
      if (_buff & FLAG_BIT_STATUS) {

#ifdef DEBUG_ON
        Serial.println("Masuk Status");
#endif

        /// Status
        if (getStatus_dataCount(_buff)) {
          if (_flagEndSysExclusive) {
            _midiMessage[panjangPesanTersimpan] = _buff;
            panjangPesanTersimpan++;
            _flagEndSysExclusive = false;
          } else {

#ifdef DEBUG_ON
            Serial.println("Status Biasa");
#endif

            _midiMessage[panjangPesanTersimpan] = _buff;
            //_midiMessage[_dataCount + 1] = '\n' ;
            panjangPesanTersimpan++;
          }
        }
        /// end Status
      } else {
        ///data

#ifdef DEBUG_ON
        Serial.println("Masuk Data");
#endif

        if (_dataCount) {
          _midiMessage[panjangPesanTersimpan] = _buff;
          panjangPesanTersimpan++;
          _dataCount--;
        }
      }
      /// end filtrasi
      if (!_dataCount) {
        //1 pesan MIDI terpenuhi.
        jumlahPesan++;
      }
      if (panjangPesanTersimpan > MIDI_BUFFER_MAX - 1) {
        // buffer kwebak

#ifdef DEBUG_ON
        Serial.println("Buffer Penuh");
#endif
        break;
      }
      if (jumlahPesan >= MIDI_MSG_MAX_UDP) {
        // pesan sudah maksimal
        break;
      }
    } else {
      // ora ono pesan lagi
      if (!_dataCount) {
        //Memastikan sudah tidak ada data (minimal 1 pesan midi terpenuhi)

#ifdef DEBUG_ON
        Serial.println("Tidak ada pesan lagi");
#endif

        break;
      }
    }
  }
  kirim();
  panjangPesanTersimpan = 0;
  jumlahPesan = 0;
}

