# Arduino-XbeeWiFi-mqtt
MQTT Applicaton for Arduino using XBee-WiFi Module. 

The XBee module has three wireless systems: ZigBee/WiFi/IEEE 802.15.4.   
This project uses Wi-Fi wireless method.   
![Xbee-Modue](https://user-images.githubusercontent.com/6020549/228780419-e6623881-1a44-4087-b9a6-f3be927e5c04.JPG)

# Memory usage
__Saves Flash and SRAM__   

_UNO+ENC28J60+UIPEthernet+PubSubClient_

```
Sketch uses 25090 bytes (77%) of program storage space. Maximum is 32256 bytes.
Global variables use 1736 bytes (84%) of dynamic memory, leaving 312 bytes for local variables. Maximum is 2048 bytes.
Low memory available, stability problems may occur.
```

_UNO+W5100+Ethernet+PubSubClient_

```
Sketch uses 19582 bytes (60%) of program storage space. Maximum is 32256 bytes.
Global variables use 1101 bytes (53%) of dynamic memory, leaving 947 bytes for local variables. Maximum is 2048 bytes.
```

_UNO+XBee WiFi+Software Serial_

```
Sketch uses 7908 bytes (24%) of program storage space. Maximum is 32256 bytes.
Global variables use 581 bytes (28%) of dynamic memory, leaving 1467 bytes for local variables. Maximum is 2048 bytes.
```

# Hardware requirements   
- ATmega328/ATmega2560/STM32   

- XBee WiFi module like XBee S6B   

- Base board like this   
![xbee-arduino-1](https://user-images.githubusercontent.com/6020549/228780907-5c8457e1-30cb-4bec-af40-a8a2353fc585.JPG)

# Software requirements   
- Arduino Environment   

- Core library is required when using STM32.   
 It supports Maple cores and ST cores.   


# Wiring   

### for ATmega328   

XBee WiFi(Tx) - ATmega328(D2)   
XBee WiFi(Rx) - ATmega328(D3)   


### for ATmega2560   

XBee WiFi(Tx) - ATmeag2560(D19)   
XBee WiFi(Rx) - ATmega2560(D18)   


### for STM32F103(MAPLE Core)   

XBee WiFi(Tx) - STM32F103(PA3)   
XBee WiFi(Rx) - STM32F103(PA2)   

MAPLE Core.    
https://github.com/rogerclarkmelbourne/Arduino_STM32   


### for STM32 NUCLEO(ST Core)   

XBee WiFi(Tx) - STM32F103(PA10)   
XBee WiFi(Rx) - STM32F103(PA9)   


ST Core.    
https://github.com/stm32duino/Arduino_Core_STM32   


### for STM32 F103 BluePill/BlackPill(ST Core)   

XBee WiFi(Tx) - STM32F103(PA3)   
XBee WiFi(Rx) - STM32F103(PA2)   

Serial printing goes to PA9.   


### for STM32 F103 Generic board(ST Core)   

XBee WiFi(Tx) - STM32F103(PA10)   
XBee WiFi(Rx) - STM32F103(PA9)   

Serial printing goes to PA9.   


### for STM32 F303CC BlackPill(ST Core)   

XBee WiFi(Tx) - STM32F303(PA3)   
XBee WiFi(Rx) - STM32F303(PA2)   

Serial printing goes to PA9.   


### for STM32 F401CC BlackPill(ST Core)    

XBee WiFi(Tx) - STM32F401(PA3)   
XBee WiFi(Rx) - STM32F401(PA2)   

Serial printing goes to PA9.   

### forSTM32 F407VG DISCOVERY(ST Core)   

XBee WiFi(Tx) - STM32F4DISC1(PD9)   
XBee WiFi(Rx) - STM32F4DISC1(PD8)   

Serial printing goes to PA2.   

### for STM32 F407 development board that like DIYMORE F407VGT   

XBee WiFi(Tx) - STM32F407(PA3)   
XBee WiFi(Rx) - STM32F407(PA2)   

Serial printing goes to PA9.   

# Setup XBee Module   
See here.   
