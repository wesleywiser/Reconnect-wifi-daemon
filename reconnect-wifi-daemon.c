#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>

int main()
{
  //daemon(int, int) handles forking the proc
  if(daemon(0,0) == -1)
    err(1, NULL);

  openlog("reconnect-wifi-daemon", LOG_PID, LOG_DAEMON);
  syslog(LOG_INFO, "Started");

  while(1)
  {
    int retCode = system("iwconfig wlan0 | grep Not-Associated -q");

    //not connected
    if(retCode == 0)
    {
      //check if the network is available
      retCode = system("sudo iwlist wlan0 scan | grep WesleyPhone -q");

      //network available
      if(retCode == 0)
      {
        syslog(LOG_INFO, "Detected network, restarting wlan0");

        //restart the network
        system("sudo restart network-manager");

        sleep(60);
      }
      //network not available
      else if (retCode > 0)
      {
        syslog(LOG_INFO, "Network not detected");
            
        sleep(30);        
      }
      //other error
      else
      {
        syslog(LOG_INFO, "Scanning Error");
      
        sleep(10);
      }
    }
    else
      sleep(60);
  }
}
