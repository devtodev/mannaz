/*
 * bluez.h
 *
 *  Created on: May 13, 2020
 *      Author: Carlos Miguens
 */

#ifndef BT_H_
#define BT_H_

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

#endif /* BT_H_ */
