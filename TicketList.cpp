/*
 * Copyright 2010 James Dewey Taylor
 * All rights reserved.
 */
#include "TicketList.h"
#include "/boot/system/develop/headers/private/interface/ColumnTypes.h"
#include "MainWindow.h"

TicketRow::TicketRow(int tn, const char* number, const char* priority, const char* status,
						const char* resolution, const char* type, const char* title,
						const char* component, const char* milestone)
	: BRow(),
	ticketnum(tn),
	fNumber(number), fPriority(priority), fStatus(status), fResolution(resolution),
	fType(type), fTitle(title), fComponent(component), fMilestone(milestone)
{
	SetField(new BStringField(fNumber), TicketNumberColumn);
	SetField(new BStringField(fPriority), TicketPriorityColumn);
	SetField(new BStringField(fStatus), TicketStatusColumn);
	SetField(new BStringField(fResolution), TicketResolutionColumn);
	SetField(new BStringField(fType), TicketTypeColumn);
	SetField(new BStringField(fTitle), TicketTitleColumn);
	SetField(new BStringField(fComponent), TicketComponentColumn);
	SetField(new BStringField(fMilestone), TicketMilestoneColumn);
}


TicketRow::~TicketRow()
{
}


TicketList::TicketList(BRect rect, const char* name, BWindow* mainwindow)
	: BColumnListView(rect, name, B_FOLLOW_ALL, 0,
		B_NO_BORDER, true)
{
	BStringColumn* numberColumn;
	AddColumn(numberColumn = new BStringColumn("#", 50, 50, 500, B_TRUNCATE_END),TicketNumberColumn);
	AddColumn(new BStringColumn("Priority", 100, 50, 500, B_TRUNCATE_END),TicketPriorityColumn);
	AddColumn(new BStringColumn("Status", 100, 50, 500, B_TRUNCATE_END),TicketStatusColumn);
	AddColumn(new BStringColumn("Resolution", 100, 50, 500, B_TRUNCATE_END),TicketResolutionColumn);
	AddColumn(new BStringColumn("Type", 100, 50, 500, B_TRUNCATE_END),TicketTypeColumn);
	AddColumn(new BStringColumn("Title", 400, 50, 500, B_TRUNCATE_MIDDLE),TicketTitleColumn);
	AddColumn(new BStringColumn("Component", 100, 50, 500, B_TRUNCATE_END),TicketComponentColumn);
	AddColumn(new BStringColumn("Milestone", 100, 50, 500, B_TRUNCATE_END),TicketMilestoneColumn);
	SetSortColumn(numberColumn, false, true);
	mainwin = mainwindow;
}


TicketList::~TicketList()
{
	RemoveAll();
}


void
TicketList::AddTicket(const TicketData *td)
{
	BString tnum;//
	BString tpri;//
	BString tsta;//
	BString tres;//
	BString ttyp;//
	BString ttit;//
	BString tcom;
	BString tmil;
	
	BString temp;
	int tempnum;
	
	temp<<td->id;
	tempnum = temp.Length();
	tnum = "";
	for (int a = 0; a < 6-tempnum; a++)
		tnum<<' ';
	tnum<<temp;

	//tnum<<td->id;
// FIX: message or pass the pointer in during construction
	ClsDataNode *tempnode = ((MainWindow*)mainwin)->PriorityList;
	while(tempnode)
	{
		if (tempnode->id == td->priority)
			tpri<<tempnode->name;
		tempnode = tempnode->Next;
	}
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwin)->StatusList;
	while(tempnode)
	{
		if(tempnode->id == td->status)
			tsta<<tempnode->name;
		tempnode = tempnode->Next;
	}
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwin)->ResolutionList;
	while(tempnode)
	{
		if(tempnode->id == td->resolution)
			tres<<tempnode->name;
		tempnode = tempnode->Next;
	}
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwin)->TypeList;
	while(tempnode)
	{
		if(tempnode->id == td->type)
			ttyp<<tempnode->name;
		tempnode = tempnode->Next;
	}
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwin)->ComponentList;
	while(tempnode)
	{
		if(tempnode->id == td->component)
			tcom<<tempnode->name;
		tempnode = tempnode->Next;
	}
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwin)->MilestoneList;
	while(tempnode)
	{
		if(tempnode->id == td->milestone)
			tmil<<tempnode->name;
		tempnode = tempnode->Next;
	}
	ttit<<td->title;
	AddRow(new TicketRow(td->id, tnum, tpri, tsta, tres, ttyp, ttit, tcom, tmil));
}


void
TicketList::RemoveAll()
{
	BRow *row;
	while ((row = RowAt((int32)0, NULL))!=NULL) {
		RemoveRow(row);
		delete row;
	}
}


void
TicketList::SelectionChanged()
{
}
