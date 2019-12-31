# InternetTv-IPmulticast-Stream-radio

This repository contains a InternetTV/Radio application that is implemented using multicast IP `ASM(Any Source Multicast)` model in `C` language. 
This Live Streaming service can be implemented in any kind of LAN(WLAN/Ethernet) provided the server and clients are in the same network. 

The project uses two different `Transport Layer Protocols(TCP/IP & UDP)` for communicating different data.And uses two
way communication for fetching radio list(TCP) and other for streaming service(UDP).

## Installation

The Project includes many `GUI` and `VLC` plugins which will be needed for displaying the video file recieved continously.

-Server File : Only GCC version 8 or above is needed. 
 
 Before proceeding, mae sure you do `sudo apt-get update`
-Client File : It uses `Gtk2` for GUI and libvlc for integrating `vlc` window in the GUI 
 -Gtk2 : `sudo apt-get install libgtk-3-dev`
 -libvlc : `sudo apt-get install libvlc-dev`
