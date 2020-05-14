/*
 * mannaz.c
 *
 *  Created on: May 12, 2020
 *      Author: karl
 */

#include <stdio.h>
#include "mannaz.h"

#include "bt.h"

int btDiscoveredDevices = -1;
int bleDiscoveredDevices = -1;

BTDevice *btDevice = NULL;
BTDevice *bleDevice = NULL;


int btScan()
{
	BTDevice *devices = cmd_scan(0, &btDiscoveredDevices);
	for (int i = 0; i < btDiscoveredDevices; i++)
	{
		printf("\n\t%s\t%s\n", devices[i].address, devices[i].name);
	}

	return btDiscoveredDevices;
}

int bleScan()
{
	cmd_lescan(0);
	return bleDiscoveredDevices;
}
