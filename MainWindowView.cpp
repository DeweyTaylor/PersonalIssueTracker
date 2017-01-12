/*
	MainWindowView.cpp
	(c)2010 James Dewey Taylor
*/
#include "MainWindowView.h"

#include <stdio.h>

MainWindowView::MainWindowView(BRect rect, char *name, BWindow* mw)
	: BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW)
{
	btnFilter = new BButton(BRect(10,10,72,31), "btnFilter", "Filter...", new BMessage(FILTER_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btnNewTicket = new BButton(BRect(82,10,206,31), "btnNewTicket", "New Ticket...", new BMessage(NEWTICKET_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);

	//add elements
	AddChild(btnFilter);
	AddChild(btnNewTicket);

	btnFilter->SetEnabled(false);
	btnNewTicket->SetEnabled(false);
	TLMain = new TicketList(BRect(10,40,630,450), "Tickets", mw);
	AddChild(TLMain);
}