/*
 * Copyright 2010 James Dewey Taylor
 * All rights reserved.
 */
#ifndef FILTERITEM_H
#define FILTERITEM_H

#include <StringItem.h>

class FilterItem : public BStringItem
{
public:
	FilterItem(BString x, ClsDataNode *node) : BStringItem(x){itemnode = node;}
	ClsDataNode		*itemnode;
private:

};

#endif
