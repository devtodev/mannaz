/*
 * bluez.c
 *
 *  Created on: May 13, 2020
 *      Author: Carlos Miguens
 */
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include "bluez.h"

// Based on hcitool
int cmd_scan(int dev_id)
{
	inquiry_info *info = NULL;
	uint8_t lap[3] = { 0x33, 0x8b, 0x9e };
	int num_rsp, length, flags;
	char addr[18], name[249];
	int extcls = 0, extinf = 0, extoui = 0;
	int i, n, dd;

	length  = 8;	/* ~10 seconds */
	num_rsp = 0;
	flags   = 0;

	printf("Scanning ...\n");
	num_rsp = hci_inquiry(dev_id, length, num_rsp, lap, &info, flags);
	if (num_rsp < 0) {
		perror("Inquiry failed");
		exit(1);
	}

	dd = hci_open_dev(dev_id);
	if (dd < 0) {
		perror("HCI device open failed");
		free(info);
		exit(1);
	}

	if (extcls || extinf || extoui)
		printf("\n");

	for (i = 0; i < num_rsp; i++) {

		if (!extcls && !extinf && !extoui) {
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

			printf("\t%s\t%s\n", addr, name);
			continue;
		}

		printf("\n");
	}

	bt_free(info);

	hci_close_dev(dd);
	return 0;
}


