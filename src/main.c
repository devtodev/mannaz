/*
 *
 * Simple Bluetooth scanner
 * Based on hcitools from Bluetooth protocol stack for Linux
 * version library BlueZ 5.18
 * Tested on Beaglebone Blue
 * Developed for IoT Robot Tank
 * 	https://github.com/devtodev/robot
 *
 * main.c
 *
 *  Created on: Apr 21, 2020
 *      Author: Carlos Miguens
 */

#include "bt.h"
#include "stdint.h"

#define MAXBUFFERSIZE 100

int main(int argc, char **argv) {
	// joystick N64 00:12:12:24:14:56
	//btScan();
	char buffer[MAXBUFFERSIZE];
	char *address = "00:12:12:24:14:56";
	int status = 0;
	status = cmd_rfcomm_connect(address);
	while(status != -1)
	{
		status = cmd_rfcomm_read(buffer, MAXBUFFERSIZE);
		printf("%s\n", buffer);
	}
	cmd_rfcomm_disconnect();
	//bleScan();
	return 0;
}



