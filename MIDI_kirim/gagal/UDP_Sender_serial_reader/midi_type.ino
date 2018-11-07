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
      _flagSysExclusive = 0;
      _dataCount = 0;
      _midiMessage[x+1]= '\n';
      break;
      
    case SystemExclusive:
      // 3 byte identifikasi
      // tidak terbatas
      _dataCount = SYS_EX_MAX_SIZE+3;
      _flagSysExclusive = 1;
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
      _midiMessage[0] = _buff;
      _midiMessage[_dataCount + 1] = '\n' ;
      x = 1;
    }
    //Serial.println(_dataCount);
    //Serial.println(_buff);
  } else {
    //data
    if (_dataCount) {
      _midiMessage[x] = _buff;
      x++;
      _dataCount--;
    } else {
      kirim();
      kosong();
    }
  }
}

