/*
 * Copyright 2010 James Dewey Taylor
 * All rights reserved.
 */
#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include <Path.h>
#include <InterfaceKit.h>
#include <ScrollView.h>

#define BTNOKCLICKED_MSG 'fcok'
#define FILTERCHANGED_MSG 'fich'
#define STATUSCHANGED_MSG 'stch'
#define TYPECHANGED_MSG 'tych'
#define PRIORITYCHANGED_MSG 'prch'
#define RESOLUTIONCHANGED_MSG 'rech'
#define COMPONENTCHANGED_MSG 'coch'
#define MILESTONECHANGED_MSG 'mich'

class FilterWindow : public BWindow
{
public:
							FilterWindow(BWindow*);
	virtual void			MessageReceived(BMessage* message);
private:
	BView*			view;

//header code
	BListView*		lstFilter;
	BListView*		lstPriority;
	BListView*		lstMilestone;
	BListView*		lstComponent;
	BListView*		lstResolution;
	BListView*		lstType;
	BListView*		lstStatus;
	BButton*		btnOk;
	BStringView*		strMilestone;
	BStringView*		strPriority;
	BStringView*		strType;
	BStringView*		strStatus;
	BStringView*		strResolution;
	BStringView*		strComponent;
	BStringView*		strFilter;

	BWindow		*mainwindow;

	void			UpdateFilterView();
};

#endif
