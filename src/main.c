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

#include "mannaz.h"

int main(int argc, char **argv) {
	//btScan();
	bleScan();
	return 0;
}



