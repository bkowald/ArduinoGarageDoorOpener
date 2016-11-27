# ArduinoGarageDoorOpener
Garage door control using an Arduino and ethernet shield

This program usings an Arduino Uno and an ethernet shield to trigger a garage door opener. It's a webserver with a single page with a button labeled "Activite". When clicked, it turns on a relay for a short pulse. The relay contacts can be connected to the button terminals of a garage door opener, or it can be connected across a button of a remote opener. I choose to connect to a wireless remote so I could mount my auduino in my basement where my network hub is.  

I can only access the opener once I get home and connect to the wifi in my house. That is my security mechansism. I could forward request for port 80 to the opener, but then my opener would be exposed to the whole world. 

Also, I did not connect a sensor to indicate if the door is currently open or closed, but there is logic in the code to read one and display the state to the user, but it is currently commented out.




