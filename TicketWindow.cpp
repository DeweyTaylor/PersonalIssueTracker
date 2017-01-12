/*
 * Copyright 2010 James Dewey Taylor
 * All rights reserved.
 */

#include "TicketWindow.h"
#include "MainWindow.h"

class ComponentMenuItem : public BMenuItem
{
public:
	ComponentMenuItem(BString x, BMessage* m, ClsDataNode *node) : BMenuItem(x, m){itemnode = node;}
	ClsDataNode		*itemnode;
private:
};


TicketWindow::TicketWindow(TicketData* td, BWindow* mainwin) : BWindow(BRect(100,100,570,680), "View Ticket", B_TITLED_WINDOW,B_WILL_DRAW)
{
	if (td == NULL)
	{
		newtd = true;
		td = new TicketData();
		//title details component milestone priority resolution status type (and id of course)
		td->title = "";
		td->details = "";
		td->component = 0;
		td->milestone = 0;
		td->priority = 4;
		td->resolution = 0;
		td->status = 3;
		td->type = 3;
		td->id = 0;
	}
	else
	{
		newtd = false;
	}
	PriorityMenu = new BPopUpMenu("prioritymenu");
// FIX: message or pass the pointer in during construction
	ClsDataNode *tempnode = ((MainWindow*)mainwin)->PriorityList;
	while(tempnode)
	{
		PriorityMenu->AddItem(new BMenuItem(tempnode->name,NULL));
		tempnode = tempnode->Next;
	}
	if(td->priority == 0)
	{
		// set the default priority to normal (4)
		td->priority = 4;
	}
	PriorityMenu->ItemAt(td->priority-1)->SetMarked(true);
	StatusMenu = new BPopUpMenu("statusmenu");
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwin)->StatusList;
	while (tempnode)
	{
		StatusMenu->AddItem(new BMenuItem(tempnode->name, new BMessage(STATCHANGED_MSG)));
		tempnode = tempnode->Next;
	}
	if(td->status == 0)
		td->status = 3;
	StatusMenu->ItemAt(td->status-1)->SetMarked(true);
	ResolutionMenu = new BPopUpMenu("resolutionmenu");
	ResolutionMenu->AddItem(new BMenuItem("<none>", new BMessage(RESCHANGED_MSG)));
	ResolutionMenu->AddSeparatorItem();
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwin)->ResolutionList;
	while (tempnode)
	{
		ResolutionMenu->AddItem(new BMenuItem(tempnode->name, new BMessage(RESCHANGED_MSG)));
		tempnode = tempnode->Next;
	}
	if(td->resolution == 0)
	{
		ResolutionMenu->ItemAt(0)->SetMarked(true);
	}
	else
	{
		ResolutionMenu->ItemAt(td->resolution+1)->SetMarked(true); // +1 because of the <none> and --- items
	}
	TypeMenu = new BPopUpMenu("typemenu");
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwin)->TypeList;
	while (tempnode)
	{
		TypeMenu->AddItem(new BMenuItem(tempnode->name, NULL));
		tempnode = tempnode->Next;
	}
	if(td->type == 0)
		td->type = 3;
	TypeMenu->ItemAt(td->type-1)->SetMarked(true);
	ComponentMenu = new BPopUpMenu("componentmenu");
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwin)->ComponentList;
	ComponentMenu->AddItem(new ComponentMenuItem("<none>", NULL, NULL));
	ComponentMenu->AddSeparatorItem();
	while (tempnode)
	{
		ComponentMenuItem* tci = new ComponentMenuItem(tempnode->name, NULL, tempnode);
		ComponentMenu->AddItem(tci);
		if (td->component == tempnode->id)
			tci->SetMarked(true);
		tempnode = tempnode->Next;
	}
	if(td->component == 0)
	{
		ComponentMenu->ItemAt(0)->SetMarked(true);
	}
//	else
//	{
//		ComponentMenu->ItemAt(td->component+1)->SetMarked(true);
//	}
	MilestoneMenu = new BPopUpMenu("milestonemenu");
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwin)->MilestoneList;
	MilestoneMenu->AddItem(new ComponentMenuItem("<none>", NULL, NULL));
	MilestoneMenu->AddSeparatorItem();
	while (tempnode)
	{
		ComponentMenuItem* tci = new ComponentMenuItem(tempnode->name, NULL, tempnode);
		MilestoneMenu->AddItem(tci);
		if (td->milestone == tempnode->id)
			tci->SetMarked(true);
		tempnode = tempnode->Next;
	}
	if(td->milestone == 0)
	{
		MilestoneMenu->ItemAt(0)->SetMarked(true);
	}
//	else
//	{
//		MilestoneMenu->ItemAt(td->milestone+1)->SetMarked(true);
//	}
	// frame, name, label, menu
	PriorityMF = new BMenuField(BRect(10,60,230,80), "prioritymf", "Priority:", PriorityMenu);
	StatusMF = new BMenuField(BRect(240,60,460,80), "statusmf", "Status:", StatusMenu);
	ResolutionMF = new BMenuField(BRect(240,90,460,110), "resolutionmf", "Resolution:", ResolutionMenu);
	TypeMF = new BMenuField(BRect(10,90,230,110), "typemf", "Type:", TypeMenu);
	ComponentMF = new BMenuField(BRect(10,120,230,140), "componentmf", "Component:", ComponentMenu);
	MilestoneMF = new BMenuField(BRect(240,120,460,140), "milestonemf", "Milestone:", MilestoneMenu);
	PriorityMF->SetDivider(70);
	StatusMF->SetDivider(70);
	ResolutionMF->SetDivider(70);
	TypeMF->SetDivider(70);
	ComponentMF->SetDivider(70);
	MilestoneMF->SetDivider(70);
	// if status == closed then resolution = enabled else resolution = disabled
	// order: priority, status, resolution, type, component, milestone
	TheView = new BView(Bounds(), "theview", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	TheView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(TheView);
	TheView->AddChild(PriorityMF);
	TheView->AddChild(StatusMF);
	TheView->AddChild(ResolutionMF);
	TheView->AddChild(TypeMF);
	TheView->AddChild(ComponentMF);
	TheView->AddChild(MilestoneMF);
	strDetails = new BStringView(BRect(10,150,110,165),"detailstitle", "Details:");
	Ok = new BButton(BRect(400,550,460,570), "ok", "Ok", new BMessage(OKCLICKED_MSG));
	Details = new BTextView(BRect(10,175,445,540),"details", BRect(0,0,435,1000), B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW | B_PULSE_NEEDED);
	BScrollView* scrScrollView1 = new BScrollView("scrScrollView", Details, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS, false, true, B_PLAIN_BORDER);
	if (newtd)
	{
		TicketNumber = new BStringView(BRect(10,10,210,25), "ticketnumber", "New Ticket");
		TitleView = new BTextControl(BRect(10,35,460,50),"titleview", "Title:", "", new BMessage());
		Details->SetText("");
	}
	else
	{
		BString temp;
		temp<<"Ticket: #"<<td->id;
		TicketNumber = new BStringView(BRect(10,10,210,25), "ticketnumber", temp);
		TitleView = new BTextControl(BRect(10,35,460,50),"titleview", "Title:", td->title, new BMessage());
		Details->SetText(td->details);
	}
	TitleView->SetDivider(60.0);
	TheView->AddChild(TicketNumber);
	TheView->AddChild(TitleView);
	TheView->AddChild(strDetails);
	TheView->AddChild(scrScrollView1);
	TheView->AddChild(Ok);

	ticketdata = td;
	mainwindow = mainwin;
}

void
TicketWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case OKCLICKED_MSG:
		{
			PostMessage(new BMessage(B_QUIT_REQUESTED));
			break;
		}
		case STATCHANGED_MSG:
		{
			if(!StatusMenu->ItemAt(0)->IsMarked()) // if status!=closed
			{
				ticketdata->resolution = 0;
				ResolutionMenu->ItemAt(0)->SetMarked(true);
			}
			else
			{
				ticketdata->resolution = 1;
				// set to "fixed"
// FIX: don't hardcode these values
				ResolutionMenu->ItemAt(2)->SetMarked(true);
			}
			break;
		}
		case RESCHANGED_MSG:
		{
			// no matter what the resolution was changed to, status is closed
			ticketdata->status = 1;
			StatusMenu->ItemAt(0)->SetMarked(true);
			break;
		}
		default:
		{
			BWindow::MessageReceived(message);
		}
	}
}

bool
TicketWindow::QuitRequested()
{

// TODO: rewrite this whole section

	// check all the fields for changes, then if changed send a message to have that record updated
	bool changed = false;
	// check for changes

	// if any fields are not the same as they were, changes were made and need to be reflected
	{
		if(strcmp(TitleView->Text(), ticketdata->title)==0)
		{
			if(strcmp(Details->Text(), ticketdata->details)==0)
			{
				if (strcmp(ticketdata->title,"")==0 && strcmp(ticketdata->details,"")==0)
					return BWindow::QuitRequested();
			}
		}
	}
	ticketdata->title = TitleView->Text();
	ticketdata->details = Details->Text();
	newtd = true;
	// update the classification values
	int val = 0;
	for (int a=0; a<PriorityMenu->CountItems(); a++)
	{
		if(PriorityMenu->ItemAt(a)->IsMarked())
			ticketdata->priority = a+1;
	}
	for (int a=0; a<StatusMenu->CountItems(); a++)
	{
		if(StatusMenu->ItemAt(a)->IsMarked())
			ticketdata->status = a+1;
	}
	for (int a=0; a<ResolutionMenu->CountItems(); a++)
	{
		if (ResolutionMenu->ItemAt(a)->IsMarked())
			if (a == 0)
				ticketdata->resolution = 0;
			else
			{
				ticketdata->resolution = a-1;
			}
	}
	for (int a=0; a<TypeMenu->CountItems(); a++)
	{
		if(TypeMenu->ItemAt(a)->IsMarked())
			ticketdata->type = a+1;
	}
	for (int a=0; a<ComponentMenu->CountItems(); a++)
	{
		if (ComponentMenu->ItemAt(a)->IsMarked())
			if (a == 0)
				ticketdata->component = 0;
			else
				ticketdata->component = ((ComponentMenuItem*)ComponentMenu->ItemAt(a))->itemnode->id;
	}
	for (int a=0; a<MilestoneMenu->CountItems(); a++)
	{
		if (MilestoneMenu->ItemAt(a)->IsMarked())
			if (a == 0)
				ticketdata->milestone = 0;
			else
				ticketdata->milestone = ((ComponentMenuItem*)MilestoneMenu->ItemAt(a))->itemnode->id;
	}
	// if any changes, then update
	if (changed || newtd)
	{
		BMessage *bm = new BMessage(UPDATETICKET_MSG);
		bm->AddPointer("ticketdata", ticketdata);
		mainwindow->PostMessage(bm);
		delete bm;
	}
	return BWindow::QuitRequested();
}
