/*
	MainWindowView.h
	(c)2010 James Dewey Taylor
*/

#ifndef MAINWINDOWVIEW_H
#define MAINWINDOWVIEW_H

#include <Application.h>
#include <InterfaceKit.h>

#include "TicketList.h"

#define		FILTER_MSG	'fltr'
#define		NEWTICKET_MSG	'ntck'

class MainWindowView : public BView 
{
	public:
						MainWindowView(BRect frame, char *name, BWindow* mw); 

		BButton*		btnFilter;
		BButton*		btnNewTicket;

		TicketList*	TLMain;
	private:

};

#endif
