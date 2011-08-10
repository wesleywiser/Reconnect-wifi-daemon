Readme for Reconnect-wifi-daemon
================================


What is this?
-------------
Reconnect-wifi-daemon is a simple, C-based daemon which polls a wireless adapter and automatically reconnects it to a specified wifi access point if the connection drops.



How do I set it up?
-------------------
Pull the latest version to somewhere convenient. Create the file `/etc/reconnect-wifi-daemon/reconnect-wifi-daemon.conf` and add the line 

    SSID="text"

where `text` is the SSID of the wifi access point that you want to automatically reconnect to. Run `./build` and add the output "reconnect-wifi-daemon" to your prefered startup-script launcher. This daemon **must** run as *root* user. This can also be run from the command line by typing `sudo ./reconnect-wifi-daemon`
