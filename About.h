/*
 * Copyright 2010 James Dewey Taylor
 * All rights reserved.
 */
#ifndef ABOUT_H
#define ABOUT_H

#include <InterfaceKit.h>

#define BTNOKCLICK_MSG 'abtb'

class AboutWindow : public BWindow
{
public:
					AboutWindow();
	virtual void	MessageReceived(BMessage* message);
private:
	BStringView	*fNotice;
	BButton		*fOk;
};

#endif
