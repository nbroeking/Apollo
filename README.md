# Project Apollo
A bluetooth interactive speaker that knows what notes are being played and lights up leds to the music. 


## Installation
1. sudo apt-get update
2. sudo apt-get dist-upgrade
3. sudo apt-get install ssh g++ make vim git libpulse-dev fftw3-dev <any other tool you need>
4. sudo apt-get install pulseaudio bluez espeak bluetooth alsa linux-firmware pulseaudio-module-bluetooth pavucontrol blueman libao-common bluez-utils bluez-alsa

5. cp upstart/* script /etc/init/
6. cp apollo /usr/share/apollo
7. Change permissions for dbus

## Intro 
This is project Apollo. A wireless bluetooth speaker that processes
audio and gives great visualization. Right now I am currently done with Phase
2. The speaker system is compromized of several sections that I will go into
   depth on how they work.

## System Daemons
There are three system daemons that are critical for getting the system running.

### Pulse Audio 
Pulse aupdio provides the framework to allow the raspberry pi to
play audio. Pulse audio needs to be run at startup in daemon mode as well as in
system mode to allow every user access to it. For a bluetooth server there isnt
such a thing as users though so you need to set up the bluetooth server. (More
on this next). To get pulseaudio working you need to move the file in
upstart/pulse-audio to /etc/init/. THis starts and configures the pulseaudio
daemon. 

### Bluetooth 
The other part to the bluetooth audio daemon is the actual
bluetooth server. Ubuntu uses bluez or bluetoothd as the main daemon.
Installing this will start up the bluetooth daemon but we need to configure it
after everything starts. To do this copy the init.d/bluetooth-agent file to
/etc/init.d/ and the raspberry pi will then work as a bluetooth server. Don't
forget to run sudo update-rc.d bluetooth-agent defaults and sudo update-rc.d
bluetooth-agent enable. Note if you insalled with the deb package these should
be set for you. In addition to the default bluetooth rules we also added
85-apollo.rules to the udev config. This allows Apollo to start and stop
scanning depending on wether or not someone is connected. Also note that if you
want to change the name of the device you need to do this under
/var/lib/bluetooth/DEVICEMAC/config. 

#### Config
Change /etc/libao.conf= libao to use pulse instead of alsa

#### Bluetooth settings
add Disable=Media
and Enable=Socket to the /etc/bluetooth/audio.conf file


### DBus 
Dbus is started by ubuntu by default so not technically one of the
three subsystems. However we need to modify pulse and bluetooth permissions in
order to allow everything to work together. To do this copy the files in dbus/
to /etc/dbus-1/system.d Like above the deb package should do this for you. 
 
### Lightsd
The lights daemon is the daemon that is in charge of processing the audio and deciding which lights to turn off and on. Create a folder /usr/share/apollo and put the light daemon inside. Then the upstart script (the one you already copied) will start the daemon at boot.


## Audio Processing

Once the bluetooth server was in place. I could then start trying to do frequency detection. Pulse audio provided default sources and sinks that allow any c++ application to pull off the audio stream. To do this I use the Pulse Audio Simple API. The API provides a blocking call that once the buffer is full of audio will return and give an array of audio data. The audio data I read is sampled at 44100 times a second and I chunk the data into arrays of size 4096. The audio data is read in chucks of 16 bits of Left, Right, Left, Right data in little endian format. The first step to doing to audio function is converting this amplitude into a format that I can run an FFT on. So I take each left right pair and convert it to a float between -1 and 1. This leaves me with a waveform of size 2048 floats. Then I run a hamming function on the wave to ensure that my frequency bins are extremely accurate. Next I actually take the fft. This returns an array of size about 1024. Each index contains the aptitude of a bin that is about the size of 21hz wide. I take the square of the absolute value of the complex result (FFT returns a complex value), and this represents the frequency at each bin. I then iterate through every bin and decide the max value for several categories. These categories end up representing different lights. If each category ends up with a max higher than 52 I turn on its corresponding light. The actual frequencies for each of these categories changes depending on the version, I can't quite make up my mind) but they have a width of about 100 hz. 

## Lights Daemon

I replaced the java light daemon with this c++ daemon to control the lights. Im not sure what version Ill use for the final but for now the c++ daemon works great. Currently the lights daemon streams off the music bytes, processes them and then passes them to the arduino for vizualizations. 

## Arduino

The arduino acts as the light card for the system. The raspberry pi can detect the arduino over serial and then can send it a coorisponding bit mask to tell it all the notes. The raspberry pi calculates the threasholds and then creates a bit mask in the format of :XXXXXXX
where the : marks the start of a command and each X is an 8 bit mask matching if the bin has bet that threshold. The arduino then reads the mask and depending on what bits have been set to 1 decides what lights to turn on. 

## Enclosure
The last major part to this project was the enclosure. I designed it using adobe illustrator. It is a simple enclosure that uses mounts on inside plates so the outside looks smooth and clean. These files are all included in the enclosure folder. 

## Bluetooth Connection
The last piece that was put into place was the bluetooth interaction. I added two scripts connect and disconect that live in /usr/share/apollo. By adding udev rules they will get called when a device connects or disconects. They are responsible for managing the bluetooth discoverable state as well as turning on an internal light to make the enclosure seem like it is glowing.  

## Notes
Pulse audio needs to be started by root and in service mode. 
http://community.linuxmint.com/tutorial/view/1137 was the best way to do that. I spent probably 30+ hours getting damn pulse audio to work. Hopefully that link saves some time. 
