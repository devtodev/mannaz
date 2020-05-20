/*
 * bluez.c
 *
 *  Created on: May 13, 2020
 *      Author: Carlos Miguens
 */
#include "bt.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>

enum rfcomm_state {
	UNKNOWN,
	CONNECTED,
	CLEAN,
	BOUND,
	LISTENING,
	CONFIG,
	DISCONNECTED,
	CLOSED
};

int s, rfcomm_status;
struct sockaddr_rc addr = { 0 };

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


int cmd_rfcomm_connect(char *dest)
{
    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = 1;
    str2ba(dest, &addr.rc_bdaddr );

    // connect to server
    rfcomm_status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
    if( rfcomm_status < 0 )
    {
    	printf("Can't connect with %s\n", dest);
    	close(s);
    	rfcomm_status = CLOSED;
    } else {
    	rfcomm_status = CONNECTED;
    }

    return rfcomm_status;
}

int cmd_rfcomm_read(char *buffer, int size)
{
	int status = -1;
	if (rfcomm_status == CONNECTED) {
		status = recv(s, buffer, size, 0);
	}
	return status;
}

int cmd_rfcomm_write(char *message, int size)
{
	int status = -1;
	if (rfcomm_status == CONNECTED) {
		status = send(s, message, size, 0);
	}
	return status;
}

void cmd_rfcomm_disconnect()
{
	if (rfcomm_status == CONNECTED) {
		close(s);
		rfcomm_status = DISCONNECTED;
	}
}
