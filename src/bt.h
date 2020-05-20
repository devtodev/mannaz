/*
 * bluez.h
 *
 *  Created on: May 13, 2020
 *      Author: Carlos Miguens
 */

#ifndef BT_H_
#define BT_H_

#include <stdint.h>
#include <bluetooth/bluetooth.h>

enum STATUS {disconnected, connected};
enum PAIRED {yes, no};
enum PROTOCOL {bt, ble};

typedef struct BTDevices{
	char *name;
	enum STATUS status;
	enum PAIRED paired;
	enum PROTOCOL protocol;
	char *type;
	char *address;
} BTDevice;

BTDevice *cmd_scan(int dev_id, int *num_rsp);
int cmd_rfcomm_connect(char *dest);
int cmd_rfcomm_read(char *buffer, int size);
int cmd_rfcomm_write(char *message, int size);
void cmd_rfcomm_disconnect();

#endif /* BT_H_ */
