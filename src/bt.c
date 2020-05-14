/*
 * bluez.c
 *
 *  Created on: May 13, 2020
 *      Author: Carlos Miguens
 */
#include "bt.h"

#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

// Based on hcitool
BTDevice *cmd_scan(int dev_id, int *num_rsp)
{
	BTDevice *devices;
	inquiry_info *info = NULL;
	uint8_t lap[3] = { 0x33, 0x8b, 0x9e };
	int length, flags;
	char addr[18], name[249];
	int i, n, dd;

	length  = 8;	/* ~10 seconds */
	*num_rsp = 0;
	flags   = 0;

	printf("Scanning ...\n");
	*num_rsp = hci_inquiry(dev_id, length, 0, lap, &info, flags);
	if (num_rsp < 0) {
		perror("Inquiry failed");
		*num_rsp = -1;
		return NULL;
	}

	dd = hci_open_dev(dev_id);
	if (dd < 0) {
		perror("HCI device open failed");
		free(info);
		*num_rsp = -1;
		return NULL;
	}

	if (num_rsp == 0) {
		*num_rsp = 0;
		return NULL;
	}

	devices = malloc(sizeof(BTDevice) * (*num_rsp));

	for (i = 0; i < *num_rsp; i++) {
		ba2str(&(info+i)->bdaddr, addr);

		if (hci_read_remote_name_with_clock_offset(dd,
				&(info+i)->bdaddr,
				(info+i)->pscan_rep_mode,
				(info+i)->clock_offset | 0x8000,
				sizeof(name), name, 100000) < 0)
			strcpy(name, "n/a");

		for (n = 0; n < 248 && name[n]; n++) {
			if ((unsigned char) name[i] < 32 || name[i] == 127)
				name[i] = '.';
		}

		name[248] = '\0';
		// alloc and cpy name
		devices[i].name = malloc(strlen(name));
		strcpy(devices[i].name, name);
		// alloc and cpy address
		devices[i].address = malloc(strlen(addr));
		strcpy(devices[i].address, addr);
	}
	bt_free(info);
	hci_close_dev(dd);
	return devices;
}


