#ifndef _OTA_wifi_h
#define _OTA_wifi_h

void OTA_server_run(char* wifi_ssid, char* wifi_pwd);

void OTA_ide_loop(char *wifi_ssid, char *wifi_pwd);
void OTA_ide_run(char* wifi_ssid, char* wifi_pwd);

#endif