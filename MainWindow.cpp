/*
	MainWindow.cpp
	(c) 2010 James Dewey Taylor
*/
#include <cstdlib>
#include "MainWindow.h"

#include "DBCallbacks.hpp"

#include "TicketList.h"

BRect rect(100,100,740,580);

MainWindow::MainWindow()
	:
	BWindow(rect, "PIT v00.00.07 ©2010", B_TITLED_WINDOW,B_WILL_DRAW)
{
	db = NULL;
	fw = NULL;

	SetSizeLimits(50,30000,50,30000);
	
	// define objects
	BRect r(Bounds());
	r.bottom = 20;
	fMenuBar = new BMenuBar(r, "menubar");
	AddChild(fMenuBar);

	fFileMenu = new BMenu("Project");
	fTicketMenu = new BMenu("Ticket");
	fOptionsMenu = new BMenu("Options");
	fOptionsMenu->SetEnabled(false);

	fMenuBar->AddItem(fFileMenu);
	fMenuBar->AddItem(fTicketMenu);
	fMenuBar->AddItem(fOptionsMenu);

	fFileMenu->AddItem(new BMenuItem("About...", new BMessage(ABOUT_MSG)));
	fFileMenu->AddSeparatorItem();

	fFileMenu->AddItem(new BMenuItem("New Project...", new BMessage(NEWPROJECT_MSG)));
	fFileMenu->AddItem(new BMenuItem("Open Project...", new BMessage(OPENPROJECT_MSG)));
	fFileMenu->AddSeparatorItem();
	fFileMenu->AddItem(new BMenuItem("Exit", new BMessage(EXIT_MSG)));


	fTicketMenu->AddItem(new BMenuItem("New Ticket...", new BMessage(NEWTICKET_MSG)));
	fTicketMenu->AddItem(new BMenuItem("Modify Ticket...", new BMessage(MODIFYTICKET_MSG)));
	fTicketMenu->AddItem(new BMenuItem("Delete Ticket", new BMessage(DELETETICKET_MSG)));
	fTicketMenu->SetEnabled(false);

	fOptionsMenu->AddItem(new BMenuItem("Edit Classifications...", new BMessage(SHOWCLASS_MSG)));
	vwMain = new MainWindowView(BRect(0,20,rect.Width(),rect.Height()), "vwMain", this);

	vwMain->TLMain->SetInvocationMessage(new BMessage(INVOKETICKET_MSG));
	vwMain->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	// add objects
	AddChild(vwMain);

	ComponentList = NULL;
	MilestoneList = NULL;
	PriorityList = NULL;
	ResolutionList = NULL;
	StatusList = NULL;
	TypeList = NULL;

	QueryResolution0 = true;
	QueryComponent0 = true;
	QueryMilestone0 = true;
}

void
MainWindow::SetFileMimeType(BPath path)
{
printf("path: %s\n", path.Path());
	BNode node(path.Path());
	if (node.InitCheck() != B_OK)
		printf ("Error!\n");
	BNodeInfo info(&node);
	info.SetType("application/x-vnd.jdt-pit.file");
}

bool MainWindow::QuitRequested()
{
	if (db)
	{
		sqlite3_close(db);
		//tempfile.Sync();
	}
	be_app_messenger.SendMessage(B_QUIT_REQUESTED);
	return BWindow::QuitRequested();
}

void MainWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case NEWPROJECT_MSG:
		{
			BMessenger target(this);
			BFilePanel *SavePanel = new BFilePanel(B_SAVE_PANEL, &target, NULL, 0, false, NULL, NULL, true, true);
			SavePanel->Show();
			break;
		}
		case OPENPROJECT_MSG:
		{
			BMessenger target(this);
			BFilePanel *OpenPanel = new BFilePanel(B_OPEN_PANEL, &target, NULL, B_FILE_NODE, false, NULL, NULL, true, true);
			OpenPanel->Show();
			break;
		}
		case EXIT_MSG:
		{
			QuitRequested();
			break;
		}
		case NEWTICKET_MSG:
		{
			// create a new ticket window and display it
			TicketWindow *x = new TicketWindow(NULL, this);
			x->Show();
			break;
		}
		case MODIFYTICKET_MSG:
		{
			PostMessage(new BMessage(INVOKETICKET_MSG));
			break;
		}
		case DELETETICKET_MSG:
		{
			// get the currently selected ticket and run a query to delete it
			TicketRow *selected = (TicketRow*)vwMain->TLMain->CurrentSelection();
			if (selected == NULL)
			{
				break;
			}
			char* query = sqlite3_mprintf("DELETE FROM Tickets WHERE id = %d;", selected->ticketnum);//ti->TicketNumber);
			char* ErrorMsg = NULL;
			if(sqlite3_exec(db, query , NULL, NULL, &ErrorMsg) != 0)
			{
				printf("Error deleting ticket!\n");
			}
			if (ErrorMsg)
			{
				printf("%s\n", ErrorMsg);
			}
			_UpdateListView();
			break;
		}
		case ABOUT_MSG:
		{
			AboutWindow *a = new AboutWindow();
			a->Show();
			break;
		}
		case B_SAVE_REQUESTED:
		{
			_CreateNewProject(message);
			break;
		}
		case B_REFS_RECEIVED:
		{
			// open project
			_OpenProject(message);
			break;
		}
		case FILTER_MSG:
		{
			fw = new FilterWindow(this);
			fw->Show();
			break;
		}
		case SETFILTER_MSG:
		{
			_BuildFilterQuery();
			_UpdateListView();
			break;
		}
		case UPDATETICKET_MSG:
		{
			// there was a pointer around here somewhere...
			// if ticket->id == 0 then insert a new ticket, otherwise update an existing one
			TicketData *ticket;
			message->FindPointer("ticketdata", (void**)(&ticket));
			char* ErrorMsg = NULL;
			if (ticket->id == 0)
			{
				// insert a new row
				char* query = sqlite3_mprintf("INSERT INTO Tickets (title, details, component, milestone, priority, resolution, status, type) "
					" VALUES (%Q, %Q, %d, %d, %d, %d, %d, %d);", ticket->title, ticket->details, ticket->component, ticket->milestone, ticket->priority, ticket->resolution, ticket->status, ticket->type);
				sqlite3_exec(db, query, NULL, NULL, &ErrorMsg);
			}
			else
			{
				char* query = sqlite3_mprintf("UPDATE Tickets SET title = %Q, details = %Q, component = %d, milestone = %d, priority = %d, resolution = %d, status = %d, type = %d "
					"WHERE id = %d;", ticket->title, ticket->details, ticket->component, ticket->milestone, ticket->priority, ticket->resolution, ticket->status, ticket->type, ticket->id);
				sqlite3_exec(db, query, NULL, NULL, &ErrorMsg);
			}
			if (ErrorMsg)
			{
				printf("%s\n", ErrorMsg);
			}
			// update the listview's contents
			_UpdateListView();
			break;
		}
		case INVOKETICKET_MSG:
		{
			TicketRow *selected = (TicketRow*)vwMain->TLMain->CurrentSelection();
			if (selected == NULL)
			{
				break;
			}
			// need to get a TicketData from that ticket number...
			char* query = sqlite3_mprintf("SELECT * FROM Tickets WHERE id = %d;", selected->ticketnum);
			CallbackData* cbd = new CallbackData();
			cbd->caller = this;
			cbd->action = DB_GETTICKET;
			char* ErrorMsg = NULL;
			if(sqlite3_exec(db, query, db_GetTicket, cbd, &ErrorMsg) != 0)
			{
				printf("Error displaying ticket!\n");
				break;
			}
			if (ErrorMsg)
			{
				printf("%s\n", ErrorMsg);
			}

			break;
		}
		case SHOWCLASS_MSG:
		{
			ClassModWindow *tempwind = new ClassModWindow(this);
			tempwind->Show();
			break;
		}
		case UPDATECLS_MSG:
		{
			ClsDataNode *tempnode = ComponentList;
			while(tempnode)
			{
				if (tempnode->id == 0) // new item
				{
					if (!tempnode->delflag) // ignore deleted new nodes
					{
						// insert new row
						char* ErrorMsg;
						if (sqlite3_exec(db, sqlite3_mprintf("INSERT INTO Component (title) VALUES (%Q)", tempnode->name), NULL, NULL, &ErrorMsg) != 0)
							printf("Error!\n");
						printf("%s\n", ErrorMsg);
					}
				}
				else if (tempnode->delflag) // delete items...
				{
					// remove rows
					char* ErrorMsg;
					if (sqlite3_exec(db, sqlite3_mprintf("DELETE FROM Component WHERE id = %d;", tempnode->id), NULL, NULL, &ErrorMsg) != 0)
						printf("Error!\n");
					printf("%s\n", ErrorMsg);
				}
				tempnode = tempnode->Next;
			}
			tempnode = MilestoneList;
			while(tempnode)
			{
				if (tempnode->id == 0) // new item
				{
					if (!tempnode->delflag) // ignore deleted new nodes
					{
						// insert new row
						char* ErrorMsg;
						if (sqlite3_exec(db, sqlite3_mprintf("INSERT INTO Milestone (title) VALUES (%Q)", tempnode->name), NULL, NULL, &ErrorMsg) != 0)
							printf("Error!\n");
						printf("%s\n", ErrorMsg);
					}
				}
				else if (tempnode->delflag) // delete items...
				{
					// remove rows
					char* ErrorMsg;
					if (sqlite3_exec(db, sqlite3_mprintf("DELETE FROM Milestone WHERE id = %d;", tempnode->id), NULL, NULL, &ErrorMsg) != 0)
						printf("Error!\n");
					printf("%s\n", ErrorMsg);
				}
				tempnode = tempnode->Next;
			}
			// update the stuff...
			UpdateClsDataLists();
			// verify tickets
//******************************************** DO THIS!
// run a query for tickets where the component or milestone no longer exist
// store the ticket#'s in a ll
// run another query to set those components or milestones to 0 (default to nothing)
// can this be done with one query? maybe using a subquery? (or two?)
// UPDATE Tickets SET component = 0 WHERE component NOT IN (SELECT id FROM Component);
			char* query = "UPDATE Tickets SET component = 0 WHERE component NOT IN (SELECT id FROM Component);";
			char* query2 = "UPDATE Tickets SET milestone = 0 WHERE milestone NOT IN (SELECT id FROM Milestone);";
			char* ErrorMsg;
			if(sqlite3_exec(db, query, NULL, NULL, &ErrorMsg)!=0)
			{
				printf("Error executing query!\n");
			}
			printf("%s\n", ErrorMsg);
			if(sqlite3_exec(db, query2, NULL, NULL, &ErrorMsg)!=0)
			{
				printf("Error executing query!\n");
			}
			printf("%s\n", ErrorMsg);
			// update the display
			_UpdateListView();
			break;
		}
		default:
		{
			BWindow::MessageReceived(message);
		}
	}
}

status_t
MainWindow::_CreateNewProject(BMessage *message)
{
	if (!message)
		return B_ERROR;

	entry_ref dirRef;
	const char* name;
	if (message->FindRef("directory", &dirRef) != B_OK
		|| message->FindString("name", &name) != B_OK)
		return B_BAD_VALUE;

	BDirectory dir(&dirRef);
	BEntry entry(&dir, name);
	if (db)
	{
		// do whatever other stuff is required to close a db...
		sqlite3_close(db);
		{
//			BFile tempfile(filepath.Path(), B_READ_ONLY);
//			char *buffer="application/x-vnd.jdt-pit.file";
//			tempfile.WriteAttr("BEOS:TYPE",B_STRING_TYPE,0,buffer,strlen(buffer)+1);
//printf("path: %s\n", filepath.Path());
//		BNode tempfile(filepath.Path());
//		if (tempfile.InitCheck() != B_OK)
//			printf ("Error!\n");
//		BNodeInfo info(&tempfile);
//		char *buffer="application/x-vnd.jdt-pit.file";
//	info.SetType(buffer);
		}
	}
	BPath path;
	entry.GetPath(&path);
	if(sqlite3_open(path.Path(), &db))
	{
		sqlite3_close(db);
		return B_ERROR;
	}
SetFileMimeType(path);
	filepath = path;

	// create the tables
	char *ErrorMsg;
	printf("creating 'tickets' table... %d\n", sqlite3_exec(db, "CREATE TABLE Tickets(id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"title TEXT, details TEXT, component INTEGER, milestone INTEGER, priority INTEGER, "
		"resolution INTEGER, status INTEGER, type INTEGER);", NULL, NULL, &ErrorMsg));
	printf(" - %s\n", ErrorMsg);
	printf("creating 'component' table... %d\n", sqlite3_exec(db, "CREATE TABLE Component(id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"title TEXT);", NULL, NULL, &ErrorMsg));
	printf(" - %s\n", ErrorMsg);
	printf("creating 'milestone' table... %d\n", sqlite3_exec(db, "CREATE TABLE Milestone(id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"title TEXT);", NULL, NULL, &ErrorMsg));
	printf(" - %s\n", ErrorMsg);
	printf("creating 'priority' table... %d\n", sqlite3_exec(db, "CREATE TABLE Priority(id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"value INTEGER, title TEXT);", NULL, NULL, &ErrorMsg));
	printf(" - %s\n", ErrorMsg);
	printf("inserting values into 'priority' table");
	if (sqlite3_exec(db, "INSERT INTO Priority (value, title) VALUES (5, \"Blocker\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".");
	if (sqlite3_exec(db, "INSERT INTO Priority (value, title) VALUES (4, \"Critical\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".");
	if (sqlite3_exec(db, "INSERT INTO Priority (value, title) VALUES (3, \"High\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".");
	if (sqlite3_exec(db, "INSERT INTO Priority (value, title) VALUES (2, \"Normal\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".");
	if (sqlite3_exec(db, "INSERT INTO Priority (value, title) VALUES (1, \"Low\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".\n");
	printf(" - %s\n", ErrorMsg);
	printf("creating 'resolution' table... %d\n", sqlite3_exec(db, "CREATE TABLE Resolution(id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"title TEXT);", NULL, NULL, &ErrorMsg));
	printf(" - %s\n", ErrorMsg);
	printf("inserting values into 'resolution' table");
	if (sqlite3_exec(db, "INSERT INTO Resolution (title) VALUES (\"Fixed\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".");
	if (sqlite3_exec(db, "INSERT INTO Resolution (title) VALUES (\"Invalid\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".");
	if (sqlite3_exec(db, "INSERT INTO Resolution (title) VALUES (\"Junk\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".");
	if (sqlite3_exec(db, "INSERT INTO Resolution (title) VALUES (\"Duplicate\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".\n");
	printf(" - %s\n", ErrorMsg);
	printf("creating 'status' table... %d\n", sqlite3_exec(db, "CREATE TABLE Status(id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"title TEXT);", NULL, NULL, &ErrorMsg));
	printf(" - %s\n", ErrorMsg);
	printf("inserting values into 'status' table");
	if (sqlite3_exec(db, "INSERT INTO Status (title) VALUES (\"Closed\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".");
	if (sqlite3_exec(db, "INSERT INTO Status (title) VALUES (\"In Progress\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".");
	if (sqlite3_exec(db, "INSERT INTO Status (title) VALUES (\"New\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".");
	if (sqlite3_exec(db, "INSERT INTO Status (title) VALUES (\"Reopened\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".\n");
	printf(" - %s\n", ErrorMsg);
	printf("creating 'type' table... %d\n", sqlite3_exec(db, "CREATE TABLE Type(id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"title TEXT);", NULL, NULL, &ErrorMsg));
	printf(" - %s\n", ErrorMsg);
	printf("inserting values into 'type' table");
	if(sqlite3_exec(db, "INSERT INTO Type (title) VALUES (\"Bug\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".");
	if(sqlite3_exec(db, "INSERT INTO Type (title) VALUES (\"Enhancement\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".");
	if(sqlite3_exec(db, "INSERT INTO Type (title) VALUES (\"Feature\")", NULL, NULL, &ErrorMsg) != 0)
		printf("Error!\n");
	else
		printf(".\n");
	printf(" - %s\n", ErrorMsg);
	printf("creating 'blocking' table... %d\n", sqlite3_exec(db, "CREATE TABLE Blocking(blockedby INTEGER, blocking INTEGER);",
		NULL, NULL, &ErrorMsg));
	printf(" - %s\n", ErrorMsg);
	printf("creating 'version' table... %d\n", sqlite3_exec(db, "CREATE TABLE Ver(version INTEGER);",
		NULL, NULL, &ErrorMsg));
	printf("inserting version number... %d\n", sqlite3_exec(db, "INSERT INTO Ver (version) VALUES (1)", NULL, NULL, &ErrorMsg));
	printf(" - %s\n", ErrorMsg);

	UpdateClsDataLists();
	_BuildFilterQuery();
	_UpdateListView();
	fOptionsMenu->SetEnabled(true);
	// enable controls
	vwMain->btnFilter->SetEnabled(true);
	vwMain->btnNewTicket->SetEnabled(true);
	fTicketMenu->SetEnabled(true);
	return B_OK;
}

status_t
MainWindow::_OpenProject(BMessage* message)
{
	char* ErrorMsg;

	if (!message)
		return B_ERROR;

	entry_ref dirRef;
	const char* name;

	if(message->FindRef("refs", &dirRef) != B_OK)
	{
		return B_BAD_VALUE;
	}

	BEntry entry(&dirRef, true);
	if (db)
	{
		// do whatever other stuff is required to close a db...
		sqlite3_close(db);

//		BNode tempfile(filepath.Path());
//		if (tempfile.InitCheck() != B_OK)
//			printf ("Error!\n");
//		BNodeInfo info(&tempfile);
//		char *buffer="application/x-vnd.jdt-pit.file";
//	info.SetType(buffer);
	}

	BPath path;
	entry.GetPath(&path);
	if(sqlite3_open(path.Path(), &db))
	{
		sqlite3_close(db);
		return B_ERROR;
	}
SetFileMimeType(path);
	// get the version from the database to ensure compatibility
	CallbackData cd;
	cd.caller = this;
	cd.action = DB_GETVERSION;
	if(sqlite3_exec(db, "SELECT * FROM Ver;", db_GetVersion, &cd, &ErrorMsg) != 0)
	{
		printf("Error!\n");
		printf("%s\n", ErrorMsg);
		sqlite3_close(db);
		return B_ERROR;
	}
	printf("Database version = %d\n", db_version);
	if (db_version != 1)
	{
		printf("Error: Invalid database version!");
		sqlite3_close(db);
		return B_ERROR;
	}

// TODO: reset the filter to something more defaultish

	UpdateClsDataLists();
	_BuildFilterQuery();
	// run an initial query to populate the ItemListView
	_UpdateListView();
	fOptionsMenu->SetEnabled(true);
	// enable the controls
	vwMain->btnFilter->SetEnabled(true);
	vwMain->btnNewTicket->SetEnabled(true);
	fTicketMenu->SetEnabled(true);
	return B_OK;
}

void
MainWindow::AddTicketItem(TicketData* data)
{
	// TODO: put all the other crap in lists or arrays or whatever so we can concatenate it all into one string
	// for now, lets just put the ticket# and title in the listitem
	BString a;
	a<<"#"<<data->id<<": "<<data->title;
	TicketItem *x = new TicketItem(a);
	x->TicketNumber = data->id;

// TODO: implement coloring

	if (data->priority == 0)
	{
		// blocker!
		//x->SetViewColor(255,0,0);
		//x->SetHighColor(255,255,255);
		//x->SetLowColor(255,128,128);
	}
	else if (data->priority == 1)
	{
		// critical
	}
	else if (data->priority == 2)
	{
		// high
	}
	else if (data->priority == 3)
	{
		// normal
	}
	else if (data->priority == 4)
	{
		// low
	}
	vwMain->TLMain->AddTicket(data);
	delete data;
}


void
MainWindow::DisplayTicket(TicketData* data)
{
	// display the ticket here!
	TicketWindow *tw = new TicketWindow(data, this);
	tw->Show();
}


void
MainWindow::_UpdateListView()
{
	// clear the list view
	vwMain->TLMain->RemoveAll();
	// run the query
	char* ErrorMsg = NULL;
	CallbackData* cbd;
	cbd->caller = this;
	cbd->action = DB_UPDATELIST;
	if (sqlite3_exec(db, fFilterQuery, db_GetTicket , cbd, &ErrorMsg) != 0)
	{
		printf("Error in update!");
	}
	printf("%s\n", ErrorMsg);
}


void
MainWindow::_BuildFilterQuery()
{
	ClsDataNode *tempnode = NULL;
	BString query = "";
	query<<"SELECT * FROM Tickets WHERE (status = 0 OR type = 0 OR priority = 0) OR (";

	tempnode = StatusList;
	while (tempnode)
	{
		if (tempnode->filtershow)
		{
			query<<"status = "<<tempnode->id<<" OR ";
		}
		tempnode = tempnode->Next;
	}
	query<<"0) AND (";

	tempnode = TypeList;
	while (tempnode)
	{
		if (tempnode->filtershow)
		{
			query<<"type = "<<tempnode->id<<" OR ";
		}
		tempnode = tempnode->Next;
	}
	query<<"0) AND (";
	tempnode = PriorityList;
	while (tempnode)
	{
		if (tempnode->filtershow)
		{
			query<<"priority = "<<tempnode->id<<" OR ";
		}
		tempnode = tempnode->Next;
	}
	query<<"0) AND (";
	// do some fancy crap here!
	int count = 0;
	if(QueryResolution0)
	{
		query<<"resolution = 0 OR ";
	}
	tempnode = ResolutionList;
	while (tempnode)
	{
		if (tempnode->filtershow)
		{
			query<<"resolution = "<<tempnode->id<<" OR ";
		}
		tempnode = tempnode->Next;
		count++;
	}
	if(count>0)
		query<<"0) AND (";
	else
		query<<"1) AND (";
	count = 0;
	if(QueryComponent0)
	{
		query<<"component = 0 OR ";
	}
	tempnode = ComponentList;
	while (tempnode)
	{
		if (tempnode->filtershow)
		{
			query<<"component = "<<tempnode->id<<" OR ";
		}
		tempnode = tempnode->Next;
		count++;
	}
	if(count>0)
		query<<"0) AND (";
	else
		query<<"1) AND (";
	count = 0;
	if(QueryMilestone0)
	{
		query<<"milestone = 0 OR ";
	}
	tempnode = MilestoneList;
	while (tempnode)
	{
		if (tempnode->filtershow)
		{
			query<<"milestone = "<<tempnode->id<<" OR ";
		}
		tempnode = tempnode->Next;
		count++;
	}
	if(count>0)
		query<<"0);";
	else
		query<<"1);";
	fBS_FilterQuery = query;
	fFilterQuery = fBS_FilterQuery.String();
}

void
MainWindow::UpdateClsDataLists()
{
	ClsDataNode *temp;
	temp = ComponentList;
	while(temp != NULL)
	{
		ClsDataNode *dump = temp;
		temp = temp->Next;
		delete dump->name;
		delete dump;
	}
	ComponentList = NULL;
//ClsDataNode			*MilestoneList;
	temp = MilestoneList;
	while(temp != NULL)
	{
		ClsDataNode *dump = temp;
		temp = temp->Next;
		delete dump->name;
		delete dump;
	}
	MilestoneList = NULL;
//ClsDataNode			*PriorityList;
	temp = PriorityList;
	while(temp != NULL)
	{
		ClsDataNode *dump = temp;
		temp = temp->Next;
		delete dump->name;
		delete dump;
	}
	PriorityList = NULL;
//ClsDataNode			*ResolutionList;
	temp = ResolutionList;
	while(temp != NULL)
	{
		ClsDataNode *dump = temp;
		temp = temp->Next;
		delete dump->name;
		delete dump;
	}
	ResolutionList = NULL;
//ClsDataNode			*StatusList;
	temp = StatusList;
	while(temp != NULL)
	{
		ClsDataNode *dump = temp;
		temp = temp->Next;
		delete dump->name;
		delete dump;
	}
	StatusList = NULL;
//ClsDataNode			*TypeList;
	temp = TypeList;
	while(temp != NULL)
	{
		ClsDataNode *dump = temp;
		temp = temp->Next;
		delete dump->name;
		delete dump;
	}
	TypeList = NULL;

	// ok, the lists are cleared, now repopulate them!
	char* ErrorMsg = NULL;
	if (sqlite3_exec(db, "SELECT * FROM component;", db_PopulateList , &ComponentList, &ErrorMsg) != 0)
	{
		printf("Error retrieving Component list!");
	}
	if (ErrorMsg)
		printf("%s\n", ErrorMsg);
	if (sqlite3_exec(db, "SELECT * FROM milestone;", db_PopulateList , &MilestoneList, &ErrorMsg) != 0)
	{
		printf("Error retrieving Milestone list!");
	}
	if (ErrorMsg)
		printf("%s\n", ErrorMsg);
	if (sqlite3_exec(db, "SELECT * FROM priority;", db_PopulateList , &PriorityList, &ErrorMsg) != 0)
	{
		printf("Error retrieving Priority list!");
	}
	if (ErrorMsg)
		printf("%s\n", ErrorMsg);
	if (sqlite3_exec(db, "SELECT * FROM resolution;", db_PopulateList , &ResolutionList, &ErrorMsg) != 0)
	{
		printf("Error retrieving Resolution list!");
	}
	if (ErrorMsg)
		printf("%s\n", ErrorMsg);
	if (sqlite3_exec(db, "SELECT * FROM status;", db_PopulateList , &StatusList, &ErrorMsg) != 0)
	{
		printf("Error retrieving Status list!");
	}
	if (ErrorMsg)
		printf("%s\n", ErrorMsg);
	if (sqlite3_exec(db, "SELECT * FROM type;", db_PopulateList , &TypeList, &ErrorMsg) != 0)
	{
		printf("Error retrieving Type list!");
	}
	if (ErrorMsg)
		printf("%s\n", ErrorMsg);
	// theoretically, we're done... what now?...
}
