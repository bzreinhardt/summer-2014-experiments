Motor Stuff
--------------------
Motor quickstart guide
https://www.sparkfun.com/tutorials/400

Motor Controller specs
http://www.schmalzhaus.com/EasyDriver/index.html#Q14
http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Robotics/EasyDriver_v44.pdf

Game Controller Stuff
----------------------
https://www.circuitsathome.com/mcu/hid-joystick-code-sample
USB Host Shield: http://www.circuitsathome.com/usb-host-shield-hardware-manual
Key information
* Digital I/O pins 8-13. In this group, the shield in its default configuration uses pins 9 and 10 for INT and SS interface signals. However, standard-sized Arduino boards, such as Duemilanove and UNO have SPI signals routed to pins 11-13 in addition to ICSP connector, therefore shields using pins 11-13 combined with standard-sized Arduinos will interfere with SPI. INT and SS signals can be re-assigned to other pins (see below); SPI signals can not.
__UPSHOT__ Don't use digital pins 9-13
Picture here: 
http://www.circuitsathome.com/wp/wp-content/uploads/2011/02/uhs20s_pin_layout.jpg

Using code based on PS3USB.ino in the host shield 2.0 library. 
Note that whatever .ino file you're using (I renamed my to PS3USB.ino) you need to put it in its own folder with the same name as the file. 

