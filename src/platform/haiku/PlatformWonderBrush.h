#ifndef PLATFORM_HAIKU_WONDER_BRUSH_H
#define PLATFORM_HAIKU_WONDER_BRUSH_H


#include <stdio.h>
#include <string.h>

#include <Application.h>
#include <Directory.h>
#include <File.h>
#include <FilePanel.h>
#include <Path.h>
#include <PathFinder.h>
#include <StringList.h>

#include "FontCache.h"
#include "FontRegistry.h"
#include "SavePanel.h"

class Window;

template<typename BaseClass>
class PlatformWonderBrush : public BApplication, protected BaseClass {
public:
	using BaseClass::InitialWindow;
	using BaseClass::WindowQuit;
	using BaseClass::StoreSettings;
	using BaseClass::RestoreSettings;
	using BaseClass::NotifyFontsLoaded;

	PlatformWonderBrush(int& argc, char** argv, BRect bounds)
		:
		BApplication("application/x-vnd.Yellowbites.WonderBrush2"),
		BaseClass(bounds)
	{
		// create file panels
		BMessenger messenger(this, this);
		BMessage message(B_REFS_RECEIVED);
		fOpenPanel = new BFilePanel(B_OPEN_PANEL, &messenger, NULL, B_FILE_NODE,
			true, &message);
	
		message.what = MSG_SAVE_AS;
		fSavePanel = new SavePanel("save panel", &messenger, NULL, B_FILE_NODE
			| B_DIRECTORY_NODE | B_SYMLINK_NODE, false, &message);
	}

	virtual void ReadyToRun()
	{
		FontRegistry* registry = FontRegistry::Default();
		if (registry->Lock()) {
			// Use font files from system, common and user fonts folders
			BStringList paths;
			BPathFinder::FindPaths(B_FIND_PATH_FONTS_DIRECTORY, NULL,
				B_FIND_PATH_EXISTING_ONLY, paths);
			int32 count = paths.CountStrings();
			for (int32 i = 0; i < count; i++)
				registry->AddFontDirectory(paths.StringAt(i));

			registry->StartWatchingAll(this);
			registry->Scan();
			registry->Unlock();
		}
		RestoreSettings();
		InitialWindow();
	}

	virtual	bool QuitRequested()
	{
		StoreSettings();
		return BApplication::QuitRequested();
	}

protected:
	virtual void Open(BMessage* message)
	{
		BMessage openMessage(B_REFS_RECEIVED);
		Window* window;
		if (message->FindPointer("window", (void**)&window) == B_OK)
			openMessage.AddPointer("window", window);
		fOpenPanel->SetMessage(&openMessage);
		fOpenPanel->Show();
	}

	virtual void SaveAs(BMessage* message)
	{
		BMessenger messenger;
		if (message->FindMessenger("target", &messenger) != B_OK)
			return;

		fSavePanel->SetExportMode(message->what == MSG_EXPORT_AS);
//		fSavePanel->Refresh();
		const char* saveText;
		if (message->FindString("save text", &saveText) == B_OK)
			fSavePanel->SetSaveText(saveText);
		fSavePanel->SetTarget(messenger);
		fSavePanel->Show();
	}

	status_t OpenSettingsFile(BFile& file, bool forWriting)
	{
		BPath path;
		status_t ret = find_directory(B_USER_SETTINGS_DIRECTORY, &path);
		if (ret != B_OK) {
			fprintf(stderr, "Failed to find the user settings directory: %s\n",
				strerror(ret));
			return ret;
		}
		ret = path.Append("WonderBrush3");
		if (ret != B_OK) {
			fprintf(stderr, "Failed to initialize the settings path: %s\n",
				strerror(ret));
			return ret;
		}

		ret = create_directory(path.Path(), 0777);
		if (ret != B_OK) {
			fprintf(stderr, "Failed to create the settings path: %s\n",
				strerror(ret));
			return ret;
		}

		ret = path.Append("main_settings");
		if (ret != B_OK) {
			fprintf(stderr, "Failed to initialize the settings path: %s\n",
				strerror(ret));
			return ret;
		}

		if (forWriting) {
			ret = file.SetTo(path.Path(), B_CREATE_FILE | B_ERASE_FILE
				| B_WRITE_ONLY);
		} else {
			ret = file.SetTo(path.Path(), B_READ_ONLY);
		}

		if (ret != B_OK) {
			if (ret != B_ENTRY_NOT_FOUND) {
				fprintf(stderr, "Failed to initialize the settings file (%s): "
					"%s\n", path.Path(), strerror(ret));
			}
			return ret;
		}

		return B_OK;
	}

private:
			BFilePanel*			fOpenPanel;
			SavePanel*			fSavePanel;
};


#endif	// PLATFORM_HAIKU_WONDER_BRUSH_H
