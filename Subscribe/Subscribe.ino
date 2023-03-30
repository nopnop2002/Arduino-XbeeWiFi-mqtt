/*
 * MQTT Publisher using ESP8266 AT Instruction Set
 *  
 * for ATmega328
 * XBeeWiFi----------ATmega328
 * TX      ----------RX(D2)
 * RX      ----------TX(D3)
 * 
 * for ATmega2560
 * XBeeWiFi----------ATmega2560
 * TX      ----------RX(D19)
 * RX      ----------TX(D18)
 * 
 * for STM32 F103 MAPLE Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA3)
 * RX      ----------TX(PA2)
 * 
 * for STM32 BlackPill/BluePill ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA3)
 * RX      ----------TX(PA2)
 * 
 * for STM32 Generic F103 ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA10)
 * RX      ----------TX(PA9)
 * 
 * for STM32 F303 BLACKPILL ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA3)
 * RX      ----------TX(PA2)
 * 
 * for STM32 F401 BLACKPILL ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA3)
 * RX      ----------TX(PA2)
 * 
 * for STM32 F4DISC1 ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PD9)
 * RX      ----------TX(PD8)
 * 
 * for STM32 F407VE/F407VG ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA3)
 * RX      ----------TX(PA2)
 * 
 * for STM32 NUCLEO64 ST Core
 * XBeeWiFi----------STM32
 * TX      ----------RX(PA10)
 * RX      ----------TX(PA9)
 * 
 */
 
//for Arduino UNO(ATmega328)
#if defined(__AVR_ATmega328__)  || defined(__AVR_ATmega328P__)
#include <SoftwareSerial.h>
#define SERIAL_RX       2
#define SERIAL_TX       3
SoftwareSerial Serial2(SERIAL_RX, SERIAL_TX); // RX, TX
#define _SERIAL_        Serial2
#define _MODEL_         "ATmega328"

//for Arduino MEGA(ATmega2560)
#elif defined(__AVR_ATmega2560__)
#define _SERIAL_        Serial1
#define _MODEL_         "ATmega2560"

//for STM32F103(MAPLE Core)
#elif defined(__STM32F1__)
#define _SERIAL_        Serial2
#define _MODEL_         "STM32F103 MAPLE Core"

//for STM32F103(ST Core)
#elif defined(ARDUINO_BLUEPILL_F103C8) || defined(ARDUINO_BLACKPILL_F103C8) || defined(ARDUINO_MAPLEMINI_F103CB)
HardwareSerial Serial2(PA3, PA2); // RX, TX
#define _SERIAL_        Serial2
#define _MODEL_         "STM32F103 ST Core"

//for STM32F303(ST Core)
#elif defined(ARDUINO_BLACKPILL_F303CC)
HardwareSerial Serial2(PA3, PA2); // RX, TX
#define _SERIAL_        Serial2
#define _MODEL_         "STM32F303 ST Core"

//for STM32F401(ST Core)
#elif defined(ARDUINO_BLACKPILL_F401CC)
HardwareSerial Serial1(PA10, PA9); // RX, TX
#define _SERIAL_        Serial1
#define _MODEL_         "STM32F401 ST Core"

//for STM32F4DISC1(ST Core)
#elif defined(ARDUINO_DISCO_F407VG)
HardwareSerial Serial1(PB7, PB6); // RX, TX
#define _SERIAL_        Serial1
#define _MODEL_         "DISC_F407VG ST Core"

//for STM32F407(ST Core)
#elif defined(ARDUINO_DIYMORE_F407VGT) || defined(ARDUINO_BLACK_F407VE) || defined(ARDUINO_BLACK_F407VG)
HardwareSerial Serial2(PA3, PA2); // RX, TX
#define _SERIAL_        Serial2
#define _MODEL_         "STM32F407 ST Core"

//for STM32 NUCLEO64(ST Core)
#else
HardwareSerial Serial1(PA10, PA9); // RX, TX
#define _SERIAL_        Serial1
#define _MODEL_         "STM32 NUCLEO64 ST Core"
#endif

#define MQTT_SUB_TOPIC "/XBEE-MQTT/#"          // Subscribe Topic
#define MQTT_KEEP_ALIVE 60                     // Pingreq Interval[Sec]
#define MQTT_WILL_TOPIC "/XBEE-MQTT/"          // Will Topic
#define MQTT_WILL_MSG   "I am leaving..."      // Will Payload
#define MQTT_CLIENT_ID  "Arduino"              // MQTT Client ID
#define _DEBUG_         0                      // for Debug

// Last Packet Send Time (MilliSecond)
unsigned long lastSendPacketTime = 0;

int buildConnect(char *buf, int keep_alive, char *client_id, char *will_topic, char *will_msg) {
  int rlen = 12;
  int pos = 14;

  int client_id_len = strlen(client_id);
  //Serial.println(client_id_len);
  buf[pos++] = 0x00;
  buf[pos++] = client_id_len;
  for(int i=0;i<client_id_len;i++) {
    buf[pos++] = client_id[i];
  }
  rlen = rlen + 2 + client_id_len;
  
  int will_topic_len = strlen(will_topic);
//  Serial.print("will_topic_len=");
//  Serial.println(will_topic_len);
  int will_msg_len = strlen(will_msg);
//  Serial.print("will_msg_len=");
//  Serial.println(will_msg_len);

  if (will_topic_len > 0 && will_msg_len > 0) {
    buf[pos++] = 0x00;
    buf[pos++] = will_topic_len;
    for(int i=0;i<will_topic_len;i++) {
      buf[pos++] = will_topic[i];
    }
    buf[pos++] = 0x00;
    buf[pos++] = will_msg_len;
    for(int i=0;i<will_msg_len;i++) {
      buf[pos++] = will_msg[i];
    }
    rlen = rlen + 2 + will_topic_len + 2 + will_msg_len;  
  }

  buf[0] = 0x10;
  buf[1] = rlen;
  buf[2] = 0x00;
  buf[3] = 0x06;
  buf[4] = 'M';
  buf[5] = 'Q';
  buf[6] = 'I';
  buf[7] = 's';
  buf[8] = 'd';
  buf[9] = 'p';
  buf[10] = 0x03;
  buf[11] = 0x02;
  if (will_topic_len > 0 && will_msg_len > 0) buf[11] = 0x06;
  buf[12] = 0x00;
  buf[13] = keep_alive;
  return buf[1] + 2;  
}

int buildSubscribe(char *buf, char *topic) {
  int tlen = strlen(topic);
  for(int i=0;i<tlen;i++) {
    buf[6+i] = topic[i];
  }
  buf[0] = 0x82;
  buf[1] = tlen + 5;
  buf[2] = 0x00;
  buf[3] = 0x01;
  buf[4] = 0x00;
  buf[5] = tlen;
  buf[tlen+6] = 0x00;
  return buf[1] + 2;   
}

void mqttDisconnect() {
  char disconnect[] = {0xe0,0x00};
  //Send to DISCONNECT
  Serial.println();
  Serial.println("Sending DISCONNECT");
  _SERIAL_.write(disconnect, 2);
}

void mqttPingreq() {
  static long lastKeepAlive = 0;
  char pingreq[] = {0xc0,0x00};
  long now = millis();
  if (lastKeepAlive  > now) return;

  //Send to PINGREQ
  Serial.println("Sending PINGREQ");
  _SERIAL_.write(pingreq, 2);

#if 0
  //Wait for PINGRESP
  unsigned char resp[16];
  int resp_len = getResponse(resp, 1000);
  if (resp_len != 2) {
    Serial.println("Illegal pingresp");
    Serial.print("resp_len=");
    Serial.println(resp_len);
    while(1);
  }
  if (resp[0] != 0xD0 || resp[1] != 0x00) {
    Serial.println("Illegal pingresp");
    Serial.print("resp[0]=0x");
    Serial.print(resp[0], HEX);
    Serial.print("resp[1]=0x");
    Serial.print(resp[1], HEX);
    Serial.println();
    while(1);
  }
#endif
  
  //ATmegaではlong=int*1000はマイナスになる
  //In ATmega, long = int * 1000 becomes negative
  //long=long(int)*1000は正しい値になる
  //long = long (int) * 1000 is the correct value
  lastKeepAlive = now + long(MQTT_KEEP_ALIVE) * 1000;
  if (lastKeepAlive < 0) lastKeepAlive = long(MQTT_KEEP_ALIVE) * 1000; // OverFlow  
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
  unsigned char ok[3];
  ok[0] = 0x4F; // O
  ok[1] = 0x4B; // K
  ok[2] = 0x0D;
  
  Serial.print("+++ ----> ");
  _SERIAL_.write("+++");
  int wk_len = getResponse(wk, timeout);
  if (wk_len != 3) {
    Serial.println("NG");
    Serial.print("wk_len=");
    Serial.println(wk_len);
    return 0;
  }
  if (strncmp((char*)wk, (char*)ok, 3) != 0) {
    Serial.println("NG");
    Serial.println((char*)wk);
    return 0;
  }
  Serial.println("OK");

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

bool isPublish(int *qos, int *retain, char *topic, char *payload) {
  static int stat = 0;
  static int remaining_length = 0;
  static int topic_length = 0;
  static int topic_index = 0;
  static int payload_length = 0;
  static int payload_index = 0;
  static int _qos = 0;
  static int _retain = 0;
  
  while (_SERIAL_.available()) {
    unsigned char c = _SERIAL_.read();
    if (_DEBUG_) Serial.print("c=0x");
    if (_DEBUG_) Serial.println(c, HEX);
    if (stat == 0) {
      unsigned char c_high = c & 0xF0;
      unsigned char c_low = c & 0x0F;
      if (_DEBUG_) Serial.print("c_high=0x");
      if (_DEBUG_) Serial.print(c_high, HEX);
      if (_DEBUG_) Serial.print(" c_low=0x");
      if (_DEBUG_) Serial.println(c_low, HEX);

      if (c_high == 0x30) { // PUBLISH packet
        Serial.println("PUBLISH packet");
        _qos = c_low & 0x06;
        _qos = _qos >> 1;
        _retain = c_low & 0x01;
#if 0
        Serial.print("c_low=0x");
        Serial.println(c_low, HEX);
        Serial.print("_qos=");
        Serial.print(_qos);
        Serial.print(" _retain=");
        Serial.println(_retain);
#endif
        stat = 1;
      } else if (c_high == 0xD0) { // PINGRESP packet
        Serial.println("PINGRESP packet");
        stat = 80;
      } else {
        stat = 90;
        Serial.println("Unknown packet");
        Serial.print("c=0x");
        Serial.println(c, HEX);
      }
    } else if (stat == 1) {
      remaining_length = c;
      if (_DEBUG_) Serial.print("remaining_length=");
      if (_DEBUG_) Serial.println(remaining_length);
      stat = 2;
    } else if (stat == 2) {
      topic_length = c * 256;
      stat = 3;
    } else if (stat == 3) {
      topic_length = topic_length + c;
      if (_DEBUG_) Serial.print("topic_length=");
      if (_DEBUG_) Serial.print(topic_length);
      payload_length = remaining_length - topic_length - 2;
      if (_DEBUG_) Serial.print(" payload_length=");
      if (_DEBUG_) Serial.println(payload_length);
      topic_index = 0;
      payload_index = 0;
      stat = 4;
    } else if (stat == 4) {
      topic[topic_index] = c;
      topic_index++;
      if (topic_index == topic_length) {
        topic[topic_index] = 0;
        stat = 5;
      }
    } else if (stat == 5) {
      payload[payload_index] = c;
      payload_index++;
      if (payload_index == payload_length) {
        payload[payload_index] = 0;
        stat = 0;
        *qos = _qos;
        *retain = _retain;
        return true;
      }
    } else if (stat == 80) { // Pingresp 2nd byte
      stat = 0;
    }

  }
  if (stat == 90) stat = 0;
  return false;
}

void setup() {
  char packet[128];
  int packet_size;
  unsigned char resp[32];
  Serial.begin(115200);
  _SERIAL_.begin(9600);

  //query my IP address
  Serial.println();
  int resp_len = executeAT("ATMY", resp, 1000);
  if (resp_len == 0) {
    Serial.println("Can't get ip address. Check wiring.");
    while(1);
  }
  Serial.print("My IP Address is [");
  Serial.print((char *)resp);
  Serial.println("]");

  //Client requests a connection to a server
  Serial.print("MQTT CONNECT.....");
  packet_size = buildConnect(packet, MQTT_KEEP_ALIVE, MQTT_CLIENT_ID, MQTT_WILL_TOPIC, MQTT_WILL_MSG);
  if (_DEBUG_) hexDump(packet, packet_size);
  _SERIAL_.write(packet, packet_size);

  //Wait for CONNACK
  resp_len = getResponse(resp, 1000);
  if (resp_len != 4) {
    Serial.println("NG. Can't connect server");
    Serial.print("resp_len=");
    Serial.println(resp_len);
    while(1);
  }
  
  if (resp[0] != 0x20 || resp[1] != 0x02 || resp[3] != 0x00) {
    Serial.println("NG. Illegal connack");
    Serial.print("resp[0]=0x");
    Serial.print(resp[0], HEX);
    Serial.print("resp[1]=0x");
    Serial.print(resp[1], HEX);
    Serial.print("resp[2]=0x");
    Serial.print(resp[2], HEX);
    Serial.print("resp[3]=0x");
    Serial.print(resp[3], HEX);
    Serial.println();
    while(1);
  }
  Serial.println("OK.");

  //Client requests a subscribe to a server
  Serial.print("MQTT SUBSCRIBE.....");
  packet_size = buildSubscribe(packet, MQTT_SUB_TOPIC);
  if (_DEBUG_) hexDump(packet, packet_size);
  _SERIAL_.write(packet, packet_size);

  //Wait for SUBACK
  resp_len = getResponse(resp, 1000);
  if (resp_len != 5) {
    Serial.println("NG. Can't connect server");
    Serial.print("resp_len=");
    Serial.println(resp_len);
    while(1);
  }

  if (resp[0] != 0x90) {
    Serial.println("NG. Illegal connack");
    Serial.print("resp[0]=0x");
    Serial.print(resp[0], HEX);
    Serial.print("resp[1]=0x");
    Serial.print(resp[1], HEX);
    Serial.print("resp[2]=0x");
    Serial.print(resp[2], HEX);
    Serial.print("resp[3]=0x");
    Serial.print(resp[3], HEX);
    Serial.print("resp[4]=0x");
    Serial.print(resp[4], HEX);
    Serial.print("resp[5]=0x");
    Serial.print(resp[5], HEX);
    Serial.println();
    while(1);
  }
  Serial.println("OK.");

  Serial.println("Start MQTT Subscribe [" + String(_MODEL_) + "]");
  lastSendPacketTime = millis();
}

void loop() {
  //static int timer = INTERVAL;
  //static int counter = 0;
  char topic[128];
  char payload[128];
  int qos;
  int retain;

/*
 * Publish Message Format
 * 
 * 3xyzzfffffffffggggg
 * 
 * x:a-b-c-d
 *   a:DUP flag
 *   b-c:QoS level
 *   d:RETAIN
 * y:Remaining Length 
 * zz:Topic Length
 * f:Topic
 * g:Payload
 * 
 * Example
 * 
 * [0x30] [0x19] [0x00] [0x0F] [0x2F] [0x58] ------- [0x73] [0x74] [0x30] [0x37] [0x3A] [0x33] [0x39] [0x3A] [0x35] [0x33]
 *                                                                 <-----------------------------------------------------> Payload
 *                             <---------------------------------> Topic
 *               <-----------> Topic Length(=15)
 *        <----> Remaining Length(=25) 
 * <----> Marker 
 * 
 */

  if (isPublish(&qos, &retain, topic, payload)) {
    Serial.print("qos=");
    Serial.print(qos);
    Serial.print(" retain=");
    Serial.print(retain);
    Serial.print(" topic=[");
    Serial.print(topic);
    Serial.print("] payload=[");
    Serial.print(payload);
    Serial.println("]");
  }

  mqttPingreq();
}
