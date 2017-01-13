/*
 * Copyright 2010 James Dewey Taylor
 * All rights reserved.
 */
#include "About.h"

AboutWindow::AboutWindow()
	:
	BWindow(BRect(300,200,600,300), "PIT (c)2010-2017", B_TITLED_WINDOW,B_WILL_DRAW)
{
	BView *mainView = new BView(Bounds(), "theview", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	mainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(mainView);
	BRect x(Bounds());
	x.bottom = x.bottom - 40;
	fNotice = new BStringView(x, "notification", "©2010-2017 James Dewey Taylor\n\tAll rights reserved.");
	mainView->AddChild(fNotice);
	x.left = Bounds().Width()-70;
	x.right = Bounds().Width()-10;
	x.top = Bounds().Height()-30;
	x.bottom = Bounds().Height()-10;
	fOk = new BButton(x, "okbutton", "Ok", new BMessage(BTNOKCLICK_MSG));
	mainView->AddChild(fOk);
}

void
AboutWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case BTNOKCLICK_MSG:
		{
			Quit();
			break;
		}
	}
}
