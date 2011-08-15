#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <confuse.h>
#include <string.h>

void getSSID(char* ssid)
{
  cfg_opt_t opts[] =
  {
    CFG_SIMPLE_STR("ssid", &ssid), CFG_END()
  };
  cfg_t *cfg;

  cfg = cfg_init(opts, CFGF_NONE);
  if(cfg_parse(cfg, "/etc/reconnect-wifi-daemon/reconnect-wifi-daemon.conf") == CFG_PARSE_ERROR)
  {
    syslog(LOG_ERR, "Error reading config file. Daemon exiting.");
    exit(1);
  }

  cfg_free(cfg);
}

int main()
{
  //daemon(int, int) handles forking the proc
  if(daemon(0,0) == -1)
    err(1, NULL);

  openlog("reconnect-wifi-daemon", LOG_PID, LOG_DAEMON);
  syslog(LOG_INFO, "Started");

  char *ssid = strdup("NETGEAR");
  getSSID(ssid);
  int ssidLen = strlen(ssid);

  char scanForNetworks [strlen("sudo iwlist wlan0 scan | grep -q ") + ssidLen];
  sprintf(scanForNetworks, "sudo iwlist wlan0 scan | grep -q %s", ssid);

  //log what the SSID is
  syslog(LOG_INFO, "SSID: %s", ssid);

  free(ssid);
  ssid = NULL;

  while(1)
  {
    int retCode = system("iwconfig wlan0 | grep Not-Associated -q");

    //not connected
    if(retCode == 0)
    {
      //check if the network is available
      retCode = system(scanForNetworks);

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

  return 0;
}
