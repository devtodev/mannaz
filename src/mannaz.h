/*
 * mannaz.h
 *
 *  Created on: May 12, 2020
 *      Author: Carlos Miguens
 */

#ifndef MANNAZ_H_
#define MANNAZ_H_

enum STATUS {disconnected, connected};
enum PAIRED {yes, no};
enum PROTOCOL {bt, ble};

struct {
	char *name;
	enum STATUS status;
	enum PAIRED paired;
	enum PROTOCOL protocol;
	char *type;
	char *address;
	struct btDevices *next;
} btDevices;

int btScan();
int bleScan();

#endif /* MANNAZ_H_ */
