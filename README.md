# InternetTv-IPmulticast-Stream-radio

This repository contains a InternetTV/Radio application that is implemented using multicast IP `ASM(Any Source Multicast)` model in `C` language. 
This Live Streaming service can be implemented in any kind of LAN(WLAN/Ethernet) provided the server and clients are in the same network. 

The project uses two different `Transport Layer Protocols(TCP/IP & UDP)` for communicating different data.And uses two
way communication for fetching radio list(TCP) and other for streaming service(UDP).

## Installation

The Project includes many `GUI` and `VLC` plugins which will be needed for displaying the video file recieved continously.
and before u install vlc plugin make sure u have installed it with `apt` repository and not with `snap` repository.
#### For Vlc media Player : `sudo apt install vlc` (if installed with snap, uninstall it and install with apt).

- Server File : `sudo apt install ffmpeg`
- Client File : It uses `Gtk2` for GUI and libvlc for integrating `vlc` window in the GUI 
   - Gtk2 : `sudo apt-get install libgtk-3-dev` (it will  install both gtk3 & gtk 2 versions)
   - libvlc : `sudo apt-get install libvlc-dev`

## Usage & Documentation


