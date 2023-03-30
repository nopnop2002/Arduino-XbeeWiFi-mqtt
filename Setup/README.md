# Xbee WiFi module setup

Start XCTU on your host.   
Click the magnifying glass icon (Discover devices) in the upper left corner to select a port.   
![xctu-1](https://user-images.githubusercontent.com/6020549/228783174-e5f93340-4200-496f-a8a2-ef3e91054a6d.jpg)

Select the COM port that the XBee is connected to and proceed with Next.   
![xctu-2](https://user-images.githubusercontent.com/6020549/228783183-04a712dc-f605-481a-b69b-200f150d643c.jpg)

Selecting a port will select the serial communication specification.   
Do not change anything at the time of factory shipment and proceed with Finish.   
When changing the baud rate of serial communication, etc., it is necessary to select the baud rate after the change.   
If you don't understand well, you can specify Select all to search for XBee modules under various conditions.   
![xctu-3](https://user-images.githubusercontent.com/6020549/228783189-9f5acef5-05d6-44c2-9c44-60deb0d808a9.jpg)

This screen will appear when the XBee module is found.   
Go ahead with Add selected devices.   
![xctu-4](https://user-images.githubusercontent.com/6020549/228783190-87af8ad0-f3a1-44a8-8210-d95e02c46bfb.jpg)

This is the main screen.   
Click the left side.   
![xctu-5](https://user-images.githubusercontent.com/6020549/228783191-538376a6-a35f-4243-a092-cdd1d5a6075e.jpg)

The current settings are displayed on the right.   
You can search for access points with the Active Scan button.   
![xctu-6](https://user-images.githubusercontent.com/6020549/228783197-82f1f875-747f-416f-b7e8-8ff1a7ca4e14.jpg)

The found access points are displayed.   
![xctu-7](https://user-images.githubusercontent.com/6020549/228783214-b8ffd228-7993-402f-96a9-9f10c4598044.jpg)

Select the access point to connect to and enter the password.   
Connect to the access point with the Connect button.   
![xctu-8](https://user-images.githubusercontent.com/6020549/228783221-c4c6e67f-158d-414b-88ba-26cc19f4ffd1.jpg)

When the connection to the access point is completed, the IP address issued by DHCP will be displayed.   
This will be the IP address for this module.   
![xctu-9](https://user-images.githubusercontent.com/6020549/228783225-348830f2-ace1-44e8-a55b-ba077de50861.jpg)

At this point, the XBee is running both a UDP server and a UDP client.   
The destination will be broadcast.   
![xctu-11](https://user-images.githubusercontent.com/6020549/228783232-2e8e05a7-3d91-404d-a19c-2de69a679b2a.jpg)

XCTU specifies the connection destination by Destination IP Address, but unfortunately it does not support name resolution by host name.   
Destination IP Address must always specify an IP address.   
So, first we need to find out the IP address of the MQTT Broker.   
This time, for MQTT Broker, I used broker.emqx.io which is a public server.   
You can find the IP address of broker.emqx.io with the following command.   
![broker](https://user-images.githubusercontent.com/6020549/228785173-0affe62a-a943-4778-a50f-7f427c7d907a.jpg)

Change:   
- IP Protocol-->TCP   
- TCP Client Connection Timeout-->FFFF   
- Destination IP Address-->54.87.92.106   

TCP ClientConnection Timeout is the idle time until the TCP client disconnects from the server. If it is not set long, the TCP connection cannot be maintained.   
Write changes to the module with the pen icon.   
![xctu-mqtt-1](https://user-images.githubusercontent.com/6020549/228785187-6f94f089-4c4b-4e74-86a5-36eea1be1812.jpg)
![xctu-mqtt-2](https://user-images.githubusercontent.com/6020549/228785193-37fde8de-b306-4f2d-9e1b-dabdb7a74cfd.jpg)

Exit XCTU.   

