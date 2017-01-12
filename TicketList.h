/*
 * Copyright 2010 James Dewey Taylor
 * All rights reserved.
 */

#ifndef TICKETLIST_H
#define TICKETLIST_H


#include <ColumnListView.h>
#include <String.h>

struct TicketData;

enum {
	TicketNumberColumn,
	TicketPriorityColumn,
	TicketStatusColumn,
	TicketResolutionColumn,
	TicketTypeColumn,
	TicketTitleColumn,
	TicketComponentColumn,
	TicketMilestoneColumn
};


class TicketRow : public BRow {
public:
					TicketRow(int tn, const char* number, const char* priority, const char* status,
						const char* resolution, const char* type, const char* title,
						const char* component, const char* milestone);
	virtual		~TicketRow();
		int		ticketnum;

private:
	BString		fNumber;
	BString		fPriority;
	BString		fStatus;
	BString		fResolution;
	BString		fType;
	BString		fTitle;
	BString		fComponent;
	BString		fMilestone;
};


class TicketList : public BColumnListView {
public:
					TicketList(BRect, const char* name, BWindow*);
	virtual			~TicketList();
	void			RemoveAll();
	void			AddTicket(const TicketData*);
protected:
	virtual	void	SelectionChanged();
private:
	BWindow*	mainwin;
};

#endif
