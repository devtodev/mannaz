/*
 * mannaz.c
 *
 *  Created on: May 12, 2020
 *      Author: karl
 */

#include <stdio.h>
#include "mannaz.h"
#include "bluez.h"

int btDiscoveredDevices = -1;
int bleDiscoveredDevices = -1;

struct btDevices *btDevice = NULL;
struct btDevices *bleDevice = NULL;


int btScan()
{
	cmd_scan(0);
	// todo: keep developing here!
	return btDiscoveredDevices;
}

int bleScan()
{
	return bleDiscoveredDevices;
}
