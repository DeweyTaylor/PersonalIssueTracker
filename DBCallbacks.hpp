/*
 * Copyright 2010 James Dewey Taylor
 * All rights reserved.
 */

////////////////////////////////////////////////////////////////
//                                                            //
// db_GetVersion : Retrieves Ver.version for verifying the db //
//                                                            //
////////////////////////////////////////////////////////////////
int db_GetVersion(void *cbd, int argc, char **argv, char **azColName)
{
	if (argc != 1)
	{
		printf("Invalid database\n");
		return SQLITE_ABORT;
	}

	((CallbackData*)cbd)->caller->db_version = atoi(argv[0]);
	return SQLITE_OK;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// db_GetTicket : Retrieves the tickets info to be displayed (and modified) //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
int db_GetTicket(void *cbd, int argc, char **argv, char **azColName)
{
	// TODO: return ticket info (selected by id)
	TicketData* td = new TicketData();
	for (int i = 0; i < argc; i++)
	{
		BString colnm = azColName[i];
		if (colnm == "id")
			td->id = atoi(argv[i]);
		else if (colnm == "title")
			td->title = argv[i];
		else if (colnm == "details")
			td->details = argv[i];
		else if (colnm == "component")
			td->component = atoi(argv[i]);
		else if (colnm == "milestone")
			td->milestone = atoi(argv[i]);
		else if (colnm == "priority")
			td->priority = atoi(argv[i]);
		else if (colnm == "resolution")
			td->resolution = atoi(argv[i]);
		else if (colnm == "status")
			td->status = atoi(argv[i]);
		else if (colnm == "type")
			td->type = atoi(argv[i]);
		else
		{
			printf("invalid column name: '%s'\n", azColName[i]);
			return SQLITE_ABORT;
		}
	}
	if(((CallbackData*)cbd)->action == DB_UPDATELIST)
	{
		((CallbackData*)cbd)->caller->AddTicketItem(td);
	}
	else if (((CallbackData*)cbd)->action == DB_GETTICKET)
	{
		((CallbackData*)cbd)->caller->DisplayTicket(td);
	}
	return SQLITE_OK;
}

///////////////////////////////////////////////////////
//                                                   //
// db_PopulateList : Populates the ClsDataNode lists //
//                                                   //
///////////////////////////////////////////////////////
int db_PopulateList(void *clsdn, int argc, char **argv, char **azColName)
{
	ClsDataNode **dummy = (ClsDataNode**)clsdn;
	// ok, we have the head... now create a new node...
	ClsDataNode *newnode = new ClsDataNode();
	// id, value, title
	for (int i = 0; i < argc; i++)
	{
		BString colnm = azColName[i];
		if (colnm == "id")
			newnode->id = atoi(argv[i]);
		else if (colnm == "value")
			newnode->prival = atoi(argv[i]);
		else if (colnm == "title")
			newnode->name = strdup(argv[i]);
	}
	newnode->filtershow = true;
	newnode->Next = NULL;
	// now, find the end of the list...
	if(*dummy == NULL)
	{
		// insert at head
		*dummy = newnode;
	}
	else
	{
		ClsDataNode *tempnode = *dummy;
		while (tempnode->Next != NULL)
		{
			tempnode = tempnode->Next;
		}
		tempnode->Next = newnode;
	}
	return SQLITE_OK;
}