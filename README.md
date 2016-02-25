# Project Athena
Java App to Control Dancing Lights on Raspberry Pi.

##Installation
1.) sudo apt-get update
2.) sudo apt-get dist-upgrade
3.) sudo apt-get install pulseaudio
4.) cp upstart script to /etc/init
5.) UDev Rules and bluetooth script http://www.instructables.com/id/Turn-your-Raspberry-Pi-into-a-Portable-Bluetooth-A/step5/Setup-the-script-that-gets-executed-when-a-Bluetoo/

##Intro
The very first thing that I have updated was the project name. Now that I have a physical project I needed to choose a name and I settled with Athena. Now to the real update.

##Bluetooth

I turned the raspberry pi into a bluetooth server. To do this I first installed pulse audio. Pulse audio is the default audio daemon for ubuntu so it seemed like the best tool for the job. Then I had to create an upstart script to get pulse to run at startup. This is not an easy task because pulse audio is not designed to run as a system wide daemon ( more on this later). Once the upstart script was in place to run pulse audio in system mode I then needed to install the bluetooth daemon. Once both pieces were installed I needed to figure out how to get them to talk, ( this was the problem I mentioned before). Pulse Audio uses udev to talk to Bluetoothd. This however was not enough because udev rejects pulse audio in system mode. To fix this I needed to go into udev settings and force it to allow system pulse manually. This can be done by changing the rules. Once this was done I then needed to make bluetooth discoverable. I created a upstart script that then puts athena into discoverable mode and then runs the bluetooth tools to handle connections. After this I had a working Bluetooth server. 

##Audo Processing

Once the bluetooth server was in place. I could then start trying to do frequency detection. Pulse audio provided default sources and sinks that allow any c++ application to pull off the audio stream. To do this I use the Pulse Audio Simple API. The API provides a blocking call that once the buffer is full of audio will return and give an array of audio data. The audio data I read is sampled at 44100 times a second and I chunk the data into arrays of size 4096. The audio data is read in chucks of 16 bits of Left, Right, Left, Right data in little endian format. The first step to doing to audio function is converting this amplitude into a format that I can run an FFT on. So I take each left right pair and convert it to a float between -1 and 1. This leaves me with a waveform of size 2048 floats. Then I run a hamming function on the wave to ensure that my frequency bins are extremely accurate. Next I actually take the fft. This returns an array of size about 1024. Each index contains the aptitude of a bin that is about the size of 21hz wide. I take the square of the absolute value of the complex result (FFT returns a complex value), and this represents the frequency at each bin. I then iterate through every bin and decide the max value for several categories. These categories end up representing different lights. If each category ends up with a max higher than 25 I turn on its corresponding light. The actual frequencies for each of these categories changes depending on the version, I can't quite make up my mind) but they have a width of about 100 hz. 

##Lights Daemon

I replaced the java light daemon with this c++ daemon to control the lights. Im not sure what version Ill use for the final but for now the c++ daemon works great. 

 
##Future Work

To finish up Project 2 I plan to replace the lights with an LED matrix powered by an Arduino. This will give the lights more range and power. This will land me right at my stretch goals for a very successful project. 

##Future Future Work

There are plenty more things I can do to create a really cool light music player but this will have to extend into project 3 time. The main focus of project 3 would be to turn my proof of concept into a hardened product. Things I propose for project 3 are.

1.) Bug fixes: The bluetooth module is no where close to perfect and has plenty of bugs. Working these out would create a really clean bluetooth speaker.

2.) Add speakers, right now I need external ones to get the job done.

3.) Create an enclosure. I am a software engineer and thus never really get to do this so it gives me an opportunity to learn skills in areas I never get to play with.

4.) Create all cables connections and pieces so I can have a standalone box. 

5.) Harden the light daemon: The lights daemon seg faults every once in a while. I would rather this not happen.

6.) I would like to set up the box to ensure that only one person connects via bluetooth at a time or try and get it so it will mix audio signals. Either way right now it crashes when this happens and that is defiantly not ideal. 

##Notes
Pulse audio needs to be started by root and in service mode. 
http://community.linuxmint.com/tutorial/view/1137 was the best way to do that. I spent probably 20+ hours getting damn pulse audio to work. Hopefully that link saves some time. 

##Demo
[![View the Demo](https://www.youtube.com/watch?v=sox-BuRqf5I/0.jpg)](https://www.youtube.com/watch?v=sox-BuRqf5I)