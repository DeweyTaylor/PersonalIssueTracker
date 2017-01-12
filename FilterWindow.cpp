/*
 * Copyright 2010 James Dewey Taylor
 * All rights reserved.
 */
#include "FilterWindow.h"

#include "MainWindow.h"
#include "FilterItem.h"

FilterWindow::FilterWindow(BWindow *mainwin) :
	BWindow(BRect(100,100,570,680), "Filter", B_TITLED_WINDOW,B_WILL_DRAW)
{
	mainwindow = mainwin;

	// TODO: synchronize controls with fd contents
	view = new BView(Bounds(),"filterview",B_FOLLOW_ALL, B_WILL_DRAW);

	view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	//add elements
	AddChild(view);

	//construct elements
	lstFilter = new BListView(BRect(235,35,460-B_V_SCROLL_BAR_WIDTH,540), "lstFilter", B_SINGLE_SELECTION_LIST, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS);
	BScrollView* scrScrollView1 = new BScrollView("scrScrollView", lstFilter, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS, false, true, B_PLAIN_BORDER);
	lstPriority = new BListView(BRect(10,35,225-B_V_SCROLL_BAR_WIDTH,95), "lstPriority", B_SINGLE_SELECTION_LIST, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS);
	BScrollView* scrScrollView2 = new BScrollView("scrScrollView", lstPriority, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS, false, true, B_PLAIN_BORDER);
	lstMilestone = new BListView(BRect(10,510,225-B_V_SCROLL_BAR_WIDTH,570), "lstMilestone", B_SINGLE_SELECTION_LIST, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS);
	BScrollView* scrScrollView3 = new BScrollView("scrScrollView", lstMilestone, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS, false, true, B_PLAIN_BORDER);
	lstComponent = new BListView(BRect(10,415,225-B_V_SCROLL_BAR_WIDTH,475), "lstComponent", B_SINGLE_SELECTION_LIST, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS);
	BScrollView* scrScrollView4 = new BScrollView("scrScrollView", lstComponent, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS, false, true, B_PLAIN_BORDER);
	lstResolution = new BListView(BRect(10,320,225-B_V_SCROLL_BAR_WIDTH,380), "lstResolution", B_SINGLE_SELECTION_LIST, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS);
	BScrollView* scrScrollView5 = new BScrollView("scrScrollView", lstResolution, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS, false, true, B_PLAIN_BORDER);
	lstType = new BListView(BRect(10,130,225-B_V_SCROLL_BAR_WIDTH,190), "lstType", B_SINGLE_SELECTION_LIST, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS);
	BScrollView* scrScrollView6 = new BScrollView("scrScrollView", lstType, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS, false, true, B_PLAIN_BORDER);
	lstStatus = new BListView(BRect(10,225,225-B_V_SCROLL_BAR_WIDTH,285), "lstStatus", B_SINGLE_SELECTION_LIST, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS);
	BScrollView* scrScrollView7 = new BScrollView("scrScrollView", lstStatus, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS, false, true, B_PLAIN_BORDER);
	btnOk = new BButton(BRect(380,550,460,570), "btnOk", "Ok", new BMessage(BTNOKCLICKED_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	strMilestone = new BStringView(BRect(10,485,139,500), "strMilestone", "Milestone:", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	strPriority = new BStringView(BRect(10,10,128,25), "strPriority", "Priority:", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	strType = new BStringView(BRect(10,105,124,120), "strType", "Type:", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	strStatus = new BStringView(BRect(10,200,134,215), "strStatus", "Status:", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	strResolution = new BStringView(BRect(10,295,134,310), "strResolution", "Resolution:", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	strComponent = new BStringView(BRect(10,390,139,405), "strComponent", "Component:", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	strFilter = new BStringView(BRect(235,10,300,25), "strFilter", "Filter:", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);

	//add elements
	view->AddChild(scrScrollView1);
	view->AddChild(scrScrollView2);
	view->AddChild(scrScrollView3);
	view->AddChild(scrScrollView4);
	view->AddChild(scrScrollView5);
	view->AddChild(scrScrollView6);
	view->AddChild(scrScrollView7);
	view->AddChild(btnOk);
	view->AddChild(strMilestone);
	view->AddChild(strPriority);
	view->AddChild(strType);
	view->AddChild(strStatus);
	view->AddChild(strResolution);
	view->AddChild(strComponent);
	view->AddChild(strFilter);

	lstFilter->SetSelectionMessage(new BMessage(FILTERCHANGED_MSG));
	lstStatus->SetSelectionMessage(new BMessage(STATUSCHANGED_MSG));
	lstType->SetSelectionMessage(new BMessage(TYPECHANGED_MSG));
	lstPriority->SetSelectionMessage(new BMessage(PRIORITYCHANGED_MSG));
	lstResolution->SetSelectionMessage(new BMessage(RESOLUTIONCHANGED_MSG));
	lstComponent->SetSelectionMessage(new BMessage(COMPONENTCHANGED_MSG));
	lstMilestone->SetSelectionMessage(new BMessage(MILESTONECHANGED_MSG));

	UpdateFilterView();
}

void
FilterWindow::UpdateFilterView()
{
	// clear the listviews
	lstFilter->MakeEmpty();
	lstStatus->MakeEmpty();
	lstPriority->MakeEmpty();
	lstType->MakeEmpty();
	lstResolution->MakeEmpty();
	lstComponent->MakeEmpty();
	lstMilestone->MakeEmpty();
	// cycle through all the lists and place the items where appropriate
	BString temp = "";
// FIX: message or pass the pointer in during construction
	ClsDataNode *tempnode = ((MainWindow*)mainwindow)->PriorityList;
	lstFilter->AddItem(new FilterItem("Priority", (ClsDataNode*)(-1)));
	// cycle through mainwindow::PriorityList
	while (tempnode)
	{
		temp = "";
		if(tempnode->filtershow == true)
		{
			temp<<"  "<<tempnode->name;
			lstFilter->AddItem(new FilterItem(temp, tempnode));
		}
		else
			lstPriority->AddItem(new FilterItem(tempnode->name, tempnode));
		tempnode = tempnode->Next;
	}
	lstFilter->AddItem(new FilterItem("Type", (ClsDataNode*)(-2)));
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwindow)->TypeList;
	while(tempnode)
	{
		temp = "";
		if(tempnode->filtershow == true)
		{
			temp<<"  "<<tempnode->name;
			lstFilter->AddItem(new FilterItem(temp, tempnode));
		}
		else
			lstType->AddItem(new FilterItem(tempnode->name, tempnode));
		tempnode = tempnode->Next;
	}
	lstFilter->AddItem(new FilterItem("Status", (ClsDataNode*)(-3)));
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwindow)->StatusList;
	while(tempnode)
	{
		temp = "";
		if(tempnode->filtershow == true)
		{
			temp<<"  "<<tempnode->name;
			lstFilter->AddItem(new FilterItem(temp, tempnode));
		}
		else
			lstStatus->AddItem(new FilterItem(tempnode->name, tempnode));
		tempnode = tempnode->Next;
	}
	lstFilter->AddItem(new FilterItem("Resolution", (ClsDataNode*)(-4)));
// FIX: message or pass the pointer in during construction
	if (((MainWindow*)mainwindow)->QueryResolution0)
	{
		lstFilter->AddItem(new FilterItem("  <none>", (ClsDataNode*)(-40)));
	}
	else
	{
		lstResolution->AddItem(new FilterItem("<none>", (ClsDataNode*)(-40)));
	}
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwindow)->ResolutionList;
	while(tempnode)
	{
		temp = "";
		if(tempnode->filtershow == true)
		{
			temp<<"  "<<tempnode->name;
			lstFilter->AddItem(new FilterItem(temp, tempnode));
		}
		else
			lstResolution->AddItem(new FilterItem(tempnode->name, tempnode));
		tempnode = tempnode->Next;
	}
	lstFilter->AddItem(new FilterItem("Component", (ClsDataNode*)(-5)));
// FIX: message or pass the pointer in during construction
	if (((MainWindow*)mainwindow)->QueryComponent0)
	{
		lstFilter->AddItem(new FilterItem("  <none>", (ClsDataNode*)(-50)));
	}
	else
	{
		lstComponent->AddItem(new FilterItem("<none>", (ClsDataNode*)(-50)));
	}
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwindow)->ComponentList;
	while(tempnode)
	{
		temp = "";
		if(tempnode->filtershow == true)
		{
			temp<<"  "<<tempnode->name;
			lstFilter->AddItem(new FilterItem(temp, tempnode));
		}
		else
			lstComponent->AddItem(new FilterItem(tempnode->name, tempnode));
		tempnode = tempnode->Next;
	}
	lstFilter->AddItem(new FilterItem("Milestone", (ClsDataNode*)(-6)));
// FIX: message or pass the pointer in during construction
	if (((MainWindow*)mainwindow)->QueryMilestone0)
	{
		lstFilter->AddItem(new FilterItem("  <none>", (ClsDataNode*)(-60)));
	}
	else
	{
		lstMilestone->AddItem(new FilterItem("<none>", (ClsDataNode*)(-60)));
	}
// FIX: message or pass the pointer in during construction
	tempnode = ((MainWindow*)mainwindow)->MilestoneList;
	while(tempnode)
	{
		temp = "";
		if(tempnode->filtershow == true)
		{
			temp<<"  "<<tempnode->name;
			lstFilter->AddItem(new FilterItem(temp, tempnode));
		}
		else
			lstMilestone->AddItem(new FilterItem(tempnode->name, tempnode));
		tempnode = tempnode->Next;
	}
}

void
FilterWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case BTNOKCLICKED_MSG:
		{
			// dispatch a message to the main window telling it that we changed the filter
			mainwindow->PostMessage(new BMessage(SETFILTER_MSG));
			Quit();
			break;
		}
		case FILTERCHANGED_MSG:
		{
			// something got clicked...
			// get the item
			// get the pointer from the item
			// change the flag in the node
			if((int64)((FilterItem*)lstFilter->ItemAt(lstFilter->CurrentSelection()))->itemnode>0)
			{
				((FilterItem*)lstFilter->ItemAt(lstFilter->CurrentSelection()))->itemnode->filtershow = false;
			}
			else
			{
				if ((int64)((FilterItem*)lstFilter->ItemAt(lstFilter->CurrentSelection()))->itemnode == -1)
				{
					// remove all priority items
// FIX: message or pass the pointer in during construction
					ClsDataNode *tempnode = ((MainWindow*)mainwindow)->PriorityList;
					while (tempnode)
					{
						tempnode->filtershow = false;
						tempnode = tempnode->Next;
					}
				}
				else if ((int64)((FilterItem*)lstFilter->ItemAt(lstFilter->CurrentSelection()))->itemnode == -2)
				{
					// type
// FIX: message or pass the pointer in during construction
					ClsDataNode *tempnode = ((MainWindow*)mainwindow)->TypeList;
					while (tempnode)
					{
						tempnode->filtershow = false;
						tempnode = tempnode->Next;
					}

				}
				else if ((int64)((FilterItem*)lstFilter->ItemAt(lstFilter->CurrentSelection()))->itemnode == -3)
				{
					// status
// FIX: message or pass the pointer in during construction
					ClsDataNode *tempnode = ((MainWindow*)mainwindow)->StatusList;
					while (tempnode)
					{
						tempnode->filtershow = false;
						tempnode = tempnode->Next;
					}
				}
				else if ((int64)((FilterItem*)lstFilter->ItemAt(lstFilter->CurrentSelection()))->itemnode == -4)
				{
					// resolution
					ClsDataNode *tempnode = ((MainWindow*)mainwindow)->ResolutionList;
// FIX: message or pass the pointer in during construction
					while (tempnode)
					{
						tempnode->filtershow = false;
						tempnode = tempnode->Next;
					}
				}
				else if ((int64)((FilterItem*)lstFilter->ItemAt(lstFilter->CurrentSelection()))->itemnode == -5)
				{
					// component
					ClsDataNode *tempnode = ((MainWindow*)mainwindow)->ComponentList;
// FIX: message or pass the pointer in during construction
					while (tempnode)
					{
						tempnode->filtershow = false;
						tempnode = tempnode->Next;
					}
				}
				else if ((int64)((FilterItem*)lstFilter->ItemAt(lstFilter->CurrentSelection()))->itemnode == -6)
				{
					// milestone
					ClsDataNode *tempnode = ((MainWindow*)mainwindow)->MilestoneList;
// FIX: message or pass the pointer in during construction
					while (tempnode)
					{
						tempnode->filtershow = false;
						tempnode = tempnode->Next;
					}
				}
				else if ((int64)((FilterItem*)lstFilter->ItemAt(lstFilter->CurrentSelection()))->itemnode == -40)
				{
					// QueryResolution0
// FIX: message or pass the pointer in during construction
					((MainWindow*)mainwindow)->QueryResolution0 = false;
				}
				else if ((int64)((FilterItem*)lstFilter->ItemAt(lstFilter->CurrentSelection()))->itemnode == -50)
				{
					// QueryComponent0
// FIX: message or pass the pointer in during construction
					((MainWindow*)mainwindow)->QueryComponent0 = false;
				}
				else if ((int64)((FilterItem*)lstFilter->ItemAt(lstFilter->CurrentSelection()))->itemnode == -60)
				{
					// QueryMilestone0
// FIX: message or pass the pointer in during construction
					((MainWindow*)mainwindow)->QueryMilestone0 = false;
				}
			}
			// update stuff
			UpdateFilterView();
			break;
		}
		case STATUSCHANGED_MSG:
		{
			((FilterItem*)lstStatus->ItemAt(lstStatus->CurrentSelection()))->itemnode->filtershow = true;
			UpdateFilterView();
			break;
		}
		case TYPECHANGED_MSG:
		{
			((FilterItem*)lstType->ItemAt(lstType->CurrentSelection()))->itemnode->filtershow = true;
			UpdateFilterView();
			break;
		}
		case PRIORITYCHANGED_MSG:
		{
			((FilterItem*)lstPriority->ItemAt(lstPriority->CurrentSelection()))->itemnode->filtershow = true;
			UpdateFilterView();
			break;
		}
		case RESOLUTIONCHANGED_MSG:
		{
			if((int64)((FilterItem*)lstResolution->ItemAt(lstResolution->CurrentSelection()))->itemnode>0)
			{
				((FilterItem*)lstResolution->ItemAt(lstResolution->CurrentSelection()))->itemnode->filtershow = true;
			}
			else
			{
// FIX: message or pass the pointer in during construction
				((MainWindow*)mainwindow)->QueryResolution0 = true;
			}
			UpdateFilterView();
			break;
		}
		case COMPONENTCHANGED_MSG:
		{
			if((int64)((FilterItem*)lstComponent->ItemAt(lstComponent->CurrentSelection()))->itemnode>0)
			{
				((FilterItem*)lstComponent->ItemAt(lstComponent->CurrentSelection()))->itemnode->filtershow = true;
			}
			else
			{
// FIX: message or pass the pointer in during construction
				((MainWindow*)mainwindow)->QueryComponent0 = true;
			}
			UpdateFilterView();
			break;
		}
		case MILESTONECHANGED_MSG:
		{
			if((int64)((FilterItem*)lstMilestone->ItemAt(lstMilestone->CurrentSelection()))->itemnode>0)
			{
				((FilterItem*)lstMilestone->ItemAt(lstMilestone->CurrentSelection()))->itemnode->filtershow = true;
			}
			else
			{
// FIX: message or pass the pointer in during construction
				((MainWindow*)mainwindow)->QueryMilestone0 = true;
			}
			UpdateFilterView();
			break;
		}
	}
}
