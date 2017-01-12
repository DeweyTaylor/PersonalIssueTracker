/*
 * Copyright 2010 James Dewey Taylor
 * All rights reserved.
 */
#ifndef TICKETITEM_H
#define TICKETITEM_H

#include <StringItem.h>

class TicketItem : public BStringItem
{
public:
		TicketItem(BString x) : BStringItem(x){}
	int TicketNumber;								
private:

};

#endif
