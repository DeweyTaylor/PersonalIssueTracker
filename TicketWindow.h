/*
 * Copyright 2010 James Dewey Taylor
 * All rights reserved.
 */

#ifndef TICKETWINDOW_H
#define TICKETWINDOW_H

#include <Application.h>
#include <InterfaceKit.h>

#define DATACHANGED_MSG 'datc'
#define OKCLICKED_MSG 'okcl'
#define RESCHANGED_MSG 'reti'
#define STATCHANGED_MSG 'stti'

struct TicketData;

class TicketWindow : public BWindow
{
public:
							TicketWindow(TicketData*, BWindow*);
	virtual void			MessageReceived(BMessage* message);
	virtual bool			QuitRequested();
private:
	BStringView	*TicketNumber;
	BTextControl	*TitleView;
	BStringView	*strDetails;
	BTextView	*Details;
	BButton		*Ok;
	BView		*TheView;
	TicketData	*ticketdata;
	BWindow		*mainwindow;

	BPopUpMenu		*PriorityMenu;
	BPopUpMenu		*StatusMenu;
	BPopUpMenu		*ResolutionMenu;
	BPopUpMenu		*TypeMenu;
	BPopUpMenu		*ComponentMenu;
	BPopUpMenu		*MilestoneMenu;
	BMenuField	*PriorityMF;
	BMenuField	*StatusMF;
	BMenuField	*ResolutionMF;
	BMenuField	*TypeMF;
	BMenuField	*ComponentMF;
	BMenuField	*MilestoneMF;
	bool		newtd;
};

#endif