/*
	app.h
	(c)2010-2017 James Dewey Taylor
*/

#ifndef _APP_H_
#define _APP_H_

#include "MainWindow.h"
#include <Application.h>
#include <MimeType.h>
#include <AppFileInfo.h>

#define APP_SIG "application/x-vnd.jdt-pit"

class app: public BApplication
{
	public:
						app();
		virtual void	ReadyToRun();

		// implement RefsReceived!!! (look it up if you forget)
		virtual void	RefsReceived(BMessage*);
	private:
		MainWindow*		mainwindow;
		void			InstallMimeType();
};

#endif


