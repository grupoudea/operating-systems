#ifndef _BRIDGE_H_
#define _BRIDGE_H_

#ifndef BRIDGE_MAJOR
#define BRIDGE_MAJOR 0   /* dynamic major by default */
#endif

#ifndef BRIDGE_NR_DEVS
#define BRIDGE_NR_DEVS 1
#endif

#include <linux/list.h>

struct bridge_dev {
	struct list_head *listHead;  /* Pointer to the head of the list*/
	unsigned long size;          /* amount of elements in the list */
	struct cdev cdev;	     /* Char device structure          */
};

/*
 * The different configurable parameters
 */
extern int bridge_major;
extern int bridge_nr_devs;

#endif /* _BRIDGE_H_ */
