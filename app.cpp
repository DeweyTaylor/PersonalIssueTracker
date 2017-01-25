/*
	app.cpp
	(c)2010 James Dewey Taylor
*/

#include "app.h"

int main(int, char**)
{
	app* App = new app();
	App->Run();

	return(0);
}

app::app()
	: BApplication(APP_SIG)
{
	mainwindow = NULL;
	InstallMimeType();
}

void
app::ReadyToRun()
{
	if (mainwindow == NULL)
	{
		mainwindow = new MainWindow();
	}
	mainwindow->Show();
}

void
app::RefsReceived(BMessage* message)
{
	//BAlert* alert = new BAlert("Message", (const char*)message->what, "Ok");
	//alert->Go();
	if(!mainwindow)
	{
		mainwindow = new MainWindow();
		mainwindow->Show();
	}
	mainwindow->PostMessage(message);
}

void
app::InstallMimeType()
{
	printf("InstallMimeType()\n");
	BString filemimetype = APP_SIG;
	filemimetype << ".file";
	BMimeType mt(filemimetype.String());
	
	if (mt.IsInstalled())
	{
		mt.Delete();
	}

	image_info ii;
	int32 cookie = 0;
	while (get_next_image_info(0, &cookie, &ii) == B_OK)
	{
		if (ii.type == B_APP_IMAGE)
			break;
	}

	BPath mypath(ii.name);
	entry_ref theref;
printf("path: %s\n", mypath.Path());
	get_ref_for_path(mypath.Path(), &theref);

	mt.SetAppHint(&theref);
	if (B_OK != mt.SetPreferredApp(APP_SIG))
		printf("app set error\n");

	mt.Install();

	BFile f(mypath.Path(), B_READ_WRITE);
	BAppFileInfo afi(&f);
	afi.SetSignature(APP_SIG);
}
