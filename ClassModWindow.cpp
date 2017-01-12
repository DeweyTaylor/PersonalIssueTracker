/*
 * Copyright 2010 James Dewey Taylor
 * All rights reserved.
 */

#include "ClassModWindow.h"

#include "MainWindow.h"

ClassModWindow::ClassModWindow(BWindow* mainwin) : BWindow(BRect(100,100,490,460), "Modify Classifications", B_TITLED_WINDOW,B_WILL_DRAW)
{
	theview = new BView(Bounds(),"filterview",B_FOLLOW_ALL, B_WILL_DRAW);
	theview->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(theview);

	//construct elements
	strStringView1 = new BStringView(BRect(10,10,90,25), "strStringView1", "Component:", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	strStringView2 = new BStringView(BRect(200,10,280,25), "strStringView2", "Milestone:", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	lstComponent = new BListView(BRect(10,35,190-B_V_SCROLL_BAR_WIDTH,235), "lstComponent", B_SINGLE_SELECTION_LIST, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS);
	BScrollView* scrScrollView1 = new BScrollView("scrScrollView1", lstComponent, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS, false, true, B_PLAIN_BORDER);
	lstMilestone = new BListView(BRect(200,35,380-B_V_SCROLL_BAR_WIDTH,235), "lstMilestone", B_SINGLE_SELECTION_LIST, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS);
	BScrollView* scrScrollView2 = new BScrollView("scrScrollView2", lstMilestone, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS, false, true, B_PLAIN_BORDER);
	btnDelComponent = new BButton(BRect(10,245,190,265), "btnDelComponent", "Delete Component", new BMessage(BTNDELCOM_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btnDelMilestone = new BButton(BRect(200,245,380,265), "btnDelMilestone", "Delete Milestone", new BMessage(BTNDELMIL_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btnAddComponent = new BButton(BRect(10,300,190,320), "btnAddComponent", "Add Component", new BMessage(BTNADDCOM_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btnAddMilestone = new BButton(BRect(200,300,380,320), "btnAddMilestone", "Add Milestone", new BMessage(BTNADDMIL_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtNewComponent = new BTextControl(BRect(10,275,190,290), "txtNewComponent", "", "", new BMessage(BTNADDCOM_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtNewMilestone = new BTextControl(BRect(200,275,380,290), "txtNewMilestone", "", "", new BMessage(BTNADDMIL_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btnOk = new BButton(BRect(300,330,380,350), "btnOk", "Ok", new BMessage(CLSBTNOK_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtNewComponent->SetDivider(0);
	txtNewMilestone->SetDivider(0);
	btnDelComponent->SetEnabled(false);
	btnDelMilestone->SetEnabled(false);
	lstComponent->SetSelectionMessage(new BMessage(CSELCH_MSG));
	lstMilestone->SetSelectionMessage(new BMessage(MSELCH_MSG));

	//add elements
	theview->AddChild(strStringView1);
	theview->AddChild(strStringView2);
	theview->AddChild(scrScrollView1);
	theview->AddChild(scrScrollView2);
	theview->AddChild(btnDelComponent);
	theview->AddChild(btnDelMilestone);
	theview->AddChild(btnAddComponent);
	theview->AddChild(btnAddMilestone);
	theview->AddChild(txtNewComponent);
	theview->AddChild(txtNewMilestone);
	theview->AddChild(btnOk);

	winMainWindow = mainwin;

	UpdateViews();
}

void
ClassModWindow::UpdateViews()
{
	// empty the listviews
	lstComponent->MakeEmpty();
	lstMilestone->MakeEmpty();
	// fill the listviews
// FIX: message or pass the pointer in during construction
	ClsDataNode *tempnode = ((MainWindow*)winMainWindow)->ComponentList;
	while (tempnode)
	{
		if (!tempnode->delflag)
		{
			lstComponent->AddItem(new ClassificationItem(tempnode->name, tempnode));
		}
		tempnode = tempnode->Next;
	}
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)winMainWindow)->MilestoneList;
	while (tempnode)
	{
		if (!tempnode->delflag)
		{
			lstMilestone->AddItem(new ClassificationItem(tempnode->name, tempnode));
		}
		tempnode = tempnode->Next;
	}
}

void
ClassModWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case CSELCH_MSG:
		{
			btnDelComponent->SetEnabled(true);
			break;
		}
		case MSELCH_MSG:
		{
			btnDelMilestone->SetEnabled(true);
			break;
		}
		case BTNDELCOM_MSG:
		{
			// set the delete flag
// FIX: message or pass the pointer in during construction
			((ClassificationItem*)lstComponent->ItemAt(lstComponent->CurrentSelection()))->itemnode->delflag = true;
			btnDelComponent->SetEnabled(false);
			UpdateViews();
			break;
		}
		case BTNDELMIL_MSG:
		{
// FIX: message or pass the pointer in during construction
			((ClassificationItem*)lstMilestone->ItemAt(lstMilestone->CurrentSelection()))->itemnode->delflag = true;
			btnDelMilestone->SetEnabled(false);
			UpdateViews();
			break;
		}
		case BTNADDCOM_MSG:
		{
			if (strcmp(txtNewComponent->Text(), "")==0)
			{
				break;
			}
			ClsDataNode *newnode = new ClsDataNode();
			newnode->id = 0;
			newnode->name = strdup(txtNewComponent->Text());
			newnode->filtershow = true;
			newnode->delflag = false;
			newnode->Next = NULL;
			// get to the end of the list, then add the node
// FIX: message or pass the pointer in during construction
			ClsDataNode *tempnode = ((MainWindow*)winMainWindow)->ComponentList;
			if (tempnode == NULL)
			{
// FIX: message or pass the pointer in during construction
				((MainWindow*)winMainWindow)->ComponentList = newnode;
			}
			else
			{
				while (tempnode->Next != NULL)
				{
					tempnode = tempnode->Next;
				}
				tempnode->Next = newnode;
			}
			btnDelComponent->SetEnabled(false);
			btnDelMilestone->SetEnabled(false);
			txtNewComponent->SetText("");
			UpdateViews();
			break;
		}
		case BTNADDMIL_MSG:
		{
			if (strcmp(txtNewMilestone->Text(), "")==0)
			{
				break;
			}
			ClsDataNode *newnode = new ClsDataNode();
			newnode->id = 0;
			newnode->name = strdup(txtNewMilestone->Text());
			newnode->filtershow = true;
			newnode->delflag = false;
			newnode->Next = NULL;
// FIX: message or pass the pointer in during construction
			ClsDataNode *tempnode = ((MainWindow*)winMainWindow)->MilestoneList;
			if (tempnode == NULL)
			{
// FIX: message or pass the pointer in during construction
				((MainWindow*)winMainWindow)->MilestoneList = newnode;
			}
			else
			{
				while (tempnode->Next != NULL)
				{
					tempnode = tempnode->Next;
				}
				tempnode->Next = newnode;
			}
			btnDelComponent->SetEnabled(false);
			btnDelMilestone->SetEnabled(false);
			txtNewMilestone->SetText("");
			UpdateViews();
			break;
		}
		case CLSBTNOK_MSG:
		{
			PostMessage(new BMessage(B_QUIT_REQUESTED));
			break;
		}
	}
}

bool
ClassModWindow::QuitRequested()
{
	// sent a message to the main window telling it to redo the classification fields
	winMainWindow->PostMessage(new BMessage(UPDATECLS_MSG));
	return BWindow::QuitRequested();
}