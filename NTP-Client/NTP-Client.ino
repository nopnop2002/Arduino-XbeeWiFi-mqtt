/*
 * NTP Client using XBee WiFi radio module
 *  
 * for ATmega328
 * XBeeWiFi----------ATmega328
 * TX      ----------RX(D2)
 * RX      ----------TX(D3)
 * RESET   ----------D4
 * 
 * for ATmega2560
 * XBeeWiFi----------ATmega2560
 * TX      ----------RX(D19)
 * RX      ----------TX(D18)
 * RESET   ----------D4
 * 
 * for STM32 F103 MAPLE Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA3)
 * RX      ----------TX(PA2)
 * RESET   ----------PA4
 * 
 * for STM32 BlackPill/BluePill ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA3)
 * RX      ----------TX(PA2)
 * RESET   ----------PA4
 * 
 * for STM32 Generic F103 ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA10)
 * RX      ----------TX(PA9)
 * RESET   ----------PA11
 * 
 * for STM32 F303 BLACKPILL ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA3)
 * RX      ----------TX(PA2)
 * RESET   ----------PA4
 * 
 * for STM32 F401 BLACKPILL ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA3)
 * RX      ----------TX(PA2)
 * RESET   ----------PA4
 * 
 * for STM32 F4DISC1 ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PB6)
 * RX      ----------TX(PB7)
 * RESET   ----------PB8
 * 
 * for STM32 F407VE/F407VG ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA3)
 * RX      ----------TX(PA2)
 * RESET   ----------PA4
 * 
 * for STM32 NUCLEO64 ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA10)
 * RX      ----------TX(PA9)
 * RESET   ----------PA11
 * 
 */

// https://github.com/PaulStoffregen/Time
#include <TimeLib.h>
 
//for Arduino UNO(ATmega328)
#if defined(__AVR_ATmega328__)  || defined(__AVR_ATmega328P__)
#include <SoftwareSerial.h>
#define _SERIAL_RX      2
#define _SERIAL_TX      3
#define _RESET          4
SoftwareSerial Serial2(_SERIAL_RX, _SERIAL_TX); // RX, TX
#define _SERIAL_        Serial2
#define _MODEL_         "ATmega328"

//for Arduino MEGA(ATmega2560)
#elif defined(__AVR_ATmega2560__)
#define _RESET          4
#define _SERIAL_        Serial1
#define _MODEL_         "ATmega2560"

//for STM32F103(MAPLE Core)
#elif defined(__STM32F1__)
#define _RESET          PA4
#define _SERIAL_        Serial2
#define _MODEL_         "STM32F103 MAPLE Core"

//for STM32F103(ST Core)
#elif defined(ARDUINO_BLUEPILL_F103C8) || defined(ARDUINO_BLUEPILL_F103CB)
HardwareSerial Serial2(PA3, PA2); // RX, TX
#define _RESET          PA4
#define _SERIAL_        Serial2
#define _MODEL_         "BluePill ST Core"

//for STM32F103(ST Core)
#elif defined(ARDUINO_BLACKPILL_F103C8) || defined(ARDUINO_BLACKPILL_F103CB)
HardwareSerial Serial2(PA3, PA2); // RX, TX
#define _RESET          PA4
#define _SERIAL_        Serial2
#define _MODEL_         "BlackPill ST Core"

//for STM32F103(ST Core)
#elif defined(ARDUINO_GENERIC_F103C8TX) || defined(ARDUINO_GENERIC_F103CBTX)
HardwareSerial Serial1(PA10, PA9); // RX, TX
#define _RESET          PA11
#define _SERIAL_        Serial1
#define _MODEL_         "STM32F103 ST Core"

//for STM32F303(ST Core)
#elif defined(ARDUINO_BLACKPILL_F303CC)
HardwareSerial Serial2(PA3, PA2); // RX, TX
#define _RESET          PA4
#define _SERIAL_        Serial2
#define _MODEL_         "STM32F303 ST Core"

//for STM32F401(ST Core)
#elif defined(ARDUINO_BLACKPILL_F401CC)
HardwareSerial Serial1(PA10, PA9); // RX, TX 
#define _RESET          PA11
#define _SERIAL_        Serial1
#define _MODEL_         "STM32F401 ST Core"

//for STM32F4DISC1(ST Core)
#elif defined(ARDUINO_DISCO_F407VG)
HardwareSerial Serial1(PB7, PB6); // RX, TX
#define _RESET          PB8
#define _SERIAL_        Serial1
#define _MODEL_         "DISC_F407VG ST Core"

//for STM32F407(ST Core)
#elif defined(ARDUINO_DIYMORE_F407VGT) || defined(ARDUINO_BLACK_F407VE) || defined(ARDUINO_BLACK_F407VG)
HardwareSerial Serial2(PA3, PA2); // RX, TX
#define _RESET          PA4
#define _SERIAL_        Serial2
#define _MODEL_         "STM32F407 ST Core"

//for STM32 NUCLEO64(ST Core)
#else
HardwareSerial Serial1(PA10, PA9); // RX, TX
#define _RESET          PA11
#define _SERIAL_        Serial1
#define _MODEL_         "STM32 NUCLEO64 ST Core"

#endif

#define NTP_PACKET_SIZE 48                     // NTP Packet Size
#define TIME_ZONE       9                      // Time Difference from GMT
#define _DEBUG_         0                      // for Debug

// Last Packet Send Time (MilliSecond)
unsigned long lastSendPacketTime = 0;

// send an NTP request to the time server at the given address
int buildPacket(char *buf)
{
  // set all bytes in the buffer to 0
  memset(buf, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)

  buf[0] = 0b11100011;   // LI, Version, Mode
  //buf[0] = 0b00011011;   // LI, Version, Mode
  buf[1] = 0;     // Stratum, or type of clock
  buf[2] = 6;     // Polling Interval
  buf[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  buf[12]  = 49;
  buf[13]  = 0x4E;
  buf[14]  = 49;
  buf[15]  = 52;

  return NTP_PACKET_SIZE;
}

int getResponse(unsigned char *buf, int timeout){
  int index = 0;
  unsigned char c;
  
  long int time = millis() + timeout;
  while( time > millis()) {
    if (_SERIAL_.available()) {
      c = _SERIAL_.read();
      if (_DEBUG_) Serial.println(c, HEX);
      buf[index++] = c;
    } // end if
  } // end while
  return index;
}


int executeAT(char * cmd, unsigned char *buf, int timeout) {
  unsigned char c;
  unsigned char wk[16];
  int wk_len;
  unsigned char ok[3];
  ok[0] = 0x4F; // O
  ok[1] = 0x4B; // K
  ok[2] = 0x0D;

  bool apmode = false;
  for (int retry=0;retry<3;retry++) {
    Serial.print("+++ ----> ");
    _SERIAL_.write("+++");
    delay(100);
    wk_len = getResponse(wk, timeout);
    if (wk_len == 3) {
      if (strncmp((char*)wk, (char*)ok, 3) == 0) {
        Serial.println("OK");
        apmode = true;
        break;
      }
    }
    Serial.println("NG");
    Serial.print("wk_len=");
    Serial.println(wk_len);
    delay(100);
  }
  if (apmode == false) return 0;

  Serial.print(cmd);
  Serial.print(" ----> ");
  int cmd_len = strlen(cmd);
  strcpy((char*)wk, (char*)cmd);
  wk[cmd_len] = 0x0d;
  _SERIAL_.write(wk, cmd_len+1);
  int ret_len = getResponse(buf, timeout);
  if (ret_len == 0) {
    Serial.println("NG");
    Serial.print("ret_len=");
    Serial.println(ret_len);
  } else {
    Serial.println("OK");
    buf[ret_len-1] = 0;
    Serial.println((char*)buf);
  }
    
  strcpy((char*)wk, "ATCN");
  wk[4] = 0x0d;
  _SERIAL_.write(wk, 5);
  wk_len = getResponse(wk, timeout);
  
  return ret_len; 
}

void hexDump(char *buf, int buf_size) {
  Serial.println();
  Serial.print("buf_size=");
  Serial.println(buf_size);
  for(int i=0;i<buf_size;i++) {
    if (buf[i] < 0x10) Serial.print("0");
    Serial.print(buf[i],HEX);
    Serial.print(" ");
    if ( ((i+1) % 20) == 0) Serial.println();
  }
  Serial.println();
}

bool isResponce(char *packet) {
  static int stat = 0;
  static char _packet[48];
  
  while (_SERIAL_.available()) {
    unsigned char c = _SERIAL_.read();
    if (_DEBUG_) Serial.print("c=0x");
    if (_DEBUG_) Serial.println(c, HEX);
    if (stat == 0) {
      if (c == 0x24) {
        //Serial.println("Start packet");
        _packet[0] = c;
        stat = 1;
      }
      
    } else if (stat != 0) {
      _packet[stat] = c;
      stat++;
      if (stat == NTP_PACKET_SIZE) {
        memcpy(packet, _packet, NTP_PACKET_SIZE);
        stat = 0;
        return true;
      }
    }

  }
  return false;
}

// dow() Return day of the week number[0-Sunday, 1-Monday etc.]
uint8_t dow(unsigned long t) {
  return ((t / 86400) + 4) % 7;
}

// dow_char() Return day of the week string(English) [Sun,Mon....]
char * dow_char(byte days) {
  char *you[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
  return you[days];
}

void showTime(char * title, time_t timet, char * dow) {
  if (_DEBUG_) Serial.print("[showTime] timet=");
  if (_DEBUG_) Serial.println(timet);
  Serial.print(title);
  Serial.print(year(timet));
  Serial.print("/");
  Serial.print(month(timet));
  Serial.print("/");
  Serial.print(day(timet));
  Serial.print(" ");
  Serial.print(hour(timet));
  Serial.print(":");
  Serial.print(minute(timet));
  Serial.print(":");
  Serial.print(second(timet));
  Serial.print(" [");
  Serial.print(dow);
  Serial.println("]");
}

void hardwareReset(int pin) {
  digitalWrite(5, HIGH);
  delay(10);
  digitalWrite(5, LOW);
  delay(10);
  digitalWrite(5, HIGH);
  delay(10);
}

void setup() {
  unsigned char resp[32];
  int resp_len;
  Serial.begin(115200);
  _SERIAL_.begin(9600);

  pinMode(_RESET, OUTPUT);
  hardwareReset(_RESET);
  
  //Query my IP address
  while(1) {
    Serial.println();
    resp_len = executeAT("ATMY", resp, 1000);
    if (resp_len == 0) {
      Serial.println("Can't get IP address. Check wiring.");
      while(1);
    }
    Serial.print("My IP Address is [");
    Serial.print((char *)resp);
    Serial.println("]");
    if (strcmp((char *)resp, "0.0.0.0")) break;
    hardwareReset(_RESET);
  }

  //Query my MAC Low address
  Serial.println();
  resp_len = executeAT("ATSL", resp, 1000);
  if (resp_len == 0) {
    Serial.println("Can't get MAC address. Check wiring.");
    while(1);
  }
  Serial.print("My MAC Low Address is [");
  Serial.print((char *)resp);
  Serial.println("]");

  lastSendPacketTime = millis() + 9000;
}

void loop() {
  unsigned char sendPacket[128];
  unsigned char recvPacket[128];
  int packet_size;
  bool obtainTime = true;

  unsigned long now = millis();
  if ( (now - lastSendPacketTime) < 0) {
    lastSendPacketTime = now;
  }

  if ( (now - lastSendPacketTime) > 10000 && obtainTime) {
    lastSendPacketTime = now;
    packet_size = buildPacket(sendPacket);
    if (_DEBUG_) hexDump(sendPacket, packet_size);
    _SERIAL_.write(sendPacket, packet_size);
  }

  if (isResponce(recvPacket)) {
    if (_DEBUG_) hexDump(recvPacket, NTP_PACKET_SIZE);
    Serial.println("\nNTP packet received");
    obtainTime = false;
    
    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:
    unsigned long highWord = word(recvPacket[40], recvPacket[41]);
    unsigned long lowWord = word(recvPacket[42], recvPacket[43]);
    
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long unixTime = secsSince1900 - seventyYears; 
    Serial.print("Unix time = ");
    Serial.println(unixTime);

    // Greenwich Mean Time(GMT)
    uint8_t DayOfWeek = dow(unixTime); 
    showTime("The UTC time is ", unixTime, dow_char(DayOfWeek));

    // Local Time
    if (TIME_ZONE != 0) {
      unsigned long localTime = unixTime + (TIME_ZONE * 60 * 60);
      DayOfWeek = dow(localTime);
      showTime("Local time is ", localTime, dow_char(DayOfWeek));
    }
  }
}
