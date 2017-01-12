/*
 * Copyright 2010 James Dewey Taylor
 * All rights reserved.
 */
#ifndef CLASSMODWINDOW_H
#define CLASSMODWINDOW_H

#include <InterfaceKit.h>

#define CSELCH_MSG 'cslc'
#define MSELCH_MSG 'mslc'
#define BTNDELCOM_MSG 'bdcc'
#define BTNDELMIL_MSG 'bdmc'
#define BTNADDCOM_MSG 'bacc'
#define BTNADDMIL_MSG 'bamc'
#define CLSBTNOK_MSG 'cbok'

struct ClsDataNode;

class ClassificationItem : public BStringItem
{
public:
	ClassificationItem(BString x, ClsDataNode *node) : BStringItem(x){itemnode = node;}
	ClsDataNode		*itemnode;
private:
};

class ClassModWindow : public BWindow
{
public:
					ClassModWindow(BWindow*);
	virtual void	MessageReceived(BMessage* message);
	virtual bool			QuitRequested();
private:
	BView*			theview;
//header code
	BStringView*		strStringView1;
	BStringView*		strStringView2;
	BListView*		lstComponent;
	BListView*		lstMilestone;
	BButton*		btnDelComponent;
	BButton*		btnDelMilestone;
	BButton*		btnAddComponent;
	BButton*		btnAddMilestone;
	BTextControl*		txtNewComponent;
	BTextControl*		txtNewMilestone;
	BButton*		btnOk;

	BWindow*		winMainWindow;

	void			UpdateViews();
};

#endif // _H
