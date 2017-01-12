/*
	MainWindow.h
	(c)2010 James Dewey Taylor
*/

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <Application.h>
#include <InterfaceKit.h>

//#include <Menu.h>
//#include <MenuItem.h>
#include <MenuBar.h>
#include <FilePanel.h>
#include <File.h>
#include <cstdio>
//#include <sstream>
#include <NodeInfo.h>

#include <sqlite3.h>

#include "MainWindowView.h"
#include "FilterWindow.h"
#include "TicketItem.h"
#include "TicketWindow.h"
#include "About.h"
#include "ClassModWindow.h"
#include <ColumnListView.h>

#define NEWPROJECT_MSG		'nprj'
#define OPENPROJECT_MSG		'oprj'
#define OPENPROJECT2_MSG		'opr2'
#define EXIT_MSG			'exit'
#define NEWTICKET_MSG		'ntck'
#define MODIFYTICKET_MSG		'mtck'
#define DELETETICKET_MSG		'dtck'
#define ABOUT_MSG			'habt'
#define UPDATETICKET_MSG	'uptk'
#define SETFILTER_MSG		'stfr'
#define INVOKETICKET_MSG	'intk'
#define SHOWCLASS_MSG		'shcl'
#define UPDATECLS_MSG		'udtc'

#define DB_GETVERSION	1
#define DB_UPDATELIST	2
#define DB_GETTICKET	3

class MainWindow;

struct CallbackData
{
	MainWindow	*caller;
	int			action;
};

struct TicketData
{
	int			id;
	const char*	title;
	const char*	details;
	int			component;
	int			milestone;
	int			priority;
	int			resolution;
	int			status;
	int			type;
};

struct ClsDataNode
{
	int			id;
	int			prival;
	const char*	name;
	bool			filtershow;
	bool			delflag;
	ClsDataNode	*Next;
};

class MainWindow : public BWindow
{
	public:
								MainWindow();
		virtual bool			QuitRequested();
		virtual void			MessageReceived(BMessage* message);
		int					db_version;
		void					AddTicketItem(TicketData*);
		void					DisplayTicket(TicketData*);

		ClsDataNode			*ComponentList;
		ClsDataNode			*MilestoneList;
		ClsDataNode			*PriorityList;
		ClsDataNode			*ResolutionList;
		ClsDataNode			*StatusList;
		ClsDataNode			*TypeList;

		bool					QueryComponent0;
		bool					QueryMilestone0;
		bool					QueryResolution0;

		void					UpdateClsDataLists();
	private:
		MainWindowView*		vwMain;
		BMenuBar			*fMenuBar;
		BMenu				*fFileMenu;
		BMenu				*fTicketMenu;
		BMenu				*fOptionsMenu;
		BMenu				*fHelpMenu;
		status_t			_CreateNewProject(BMessage*);
		FilterWindow		*fw;
		sqlite3			*db;
		status_t			_OpenProject(BMessage*);
		const char*				fFilterQuery;
		BString			fBS_FilterQuery;
		void				_BuildFilterQuery();
		void				_UpdateListView();
		BPath				filepath;
		void				SetFileMimeType(BPath);
};

#endif
