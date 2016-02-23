# DancingLights
Java App to Control Dancing Lights on Raspberry Pi.

##Installation
1.) sudo apt-get update
2.) sudo apt-get dist-upgrade
3.) sudo apt-get install pulseaudio
4.) cp upstart script to /etc/init
5.) UDev Rules and bluetooth script http://www.instructables.com/id/Turn-your-Raspberry-Pi-into-a-Portable-Bluetooth-A/step5/Setup-the-script-that-gets-executed-when-a-Bluetoo/

##Notes
Pulse audio needs to be started by root and in service mode. 
http://community.linuxmint.com/tutorial/view/1137 was the best way to do that. I spent probably 20+ hours getting damn pulse audio to work. Hopefully that link saves some time. 


