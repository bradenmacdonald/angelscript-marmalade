#include "s3e.h"
#include "IwUI.h"
#include "IwResManager.h"

#include <string>
using std::string;
string string_format(const char* const fmt, ...);
#include <stdexcept>
using std::runtime_error;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////






#include <angelscript.h>
#include <angelscript/add_on/scriptstdstring.h>

// A function for scripts to call when they want to display a message:
void ShowMessage(const string& msg) { s3eDebugErrorShow(S3E_MESSAGE_CONTINUE, msg.c_str()); }

// AngelScript callback for errors, warnings, etc.
void asMessageCallback(const asSMessageInfo *msg, void *param) {
	if (msg->type == asMSGTYPE_INFORMATION) {
		s3eDebugTracePrintf("AngelScript: %s (at %s, line %d)", msg->message, msg->section, msg->row);
	} else if (msg->type == asMSGTYPE_WARNING) {
		string warning = string_format("AngelScript warning: %s (at %s, line %d)", msg->message, msg->section, msg->row);
		s3eDebugTraceLine(warning.c_str());
		s3eDebugAssertShow(S3E_MESSAGE_CONTINUE, warning.c_str());
	} else {
		string error = string_format("AngelScript error: %s (at %s, line %d)", msg->message, msg->section, msg->row);
		s3eDebugTraceLine(error.c_str());
		throw runtime_error(error);
	}
}

void RunAngelScriptMain() {
	asIScriptEngine* pASEngine;
	asIScriptModule* pASModule;
	
	pASEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	int r = pASEngine->SetMessageCallback(asFUNCTION(&asMessageCallback), 0, asCALL_CDECL); IwAssert(ANGELSCRIPT, r >= 0 );
	
	// Use STL strings as our string type:
	RegisterStdString(pASEngine);
	
	// Register the "ShowMessage" global:
	r = pASEngine->RegisterGlobalFunction("void ShowMessage(const string &in)", asFUNCTION(ShowMessage), asCALL_CDECL); IwAssert(ANGELSCRIPT, r >= 0 );
	
	// Load a script!
	// Create a new script module
	pASModule = pASEngine->GetModule("module", asGM_ALWAYS_CREATE);
	
	// Load and add the script sections to the module
	const char* SCRIPT_FILE = "demo.as";
	string demo_script;
	{
		s3eFile* file = s3eFileOpen(SCRIPT_FILE, "rb");
		if (file == NULL)
			throw runtime_error(string("Unable to open file ") + SCRIPT_FILE);
		const int file_size = s3eFileGetSize(file);
		char* file_buf = (char*)s3eMalloc(file_size + 1);
		if (!file_buf) {
			s3eFileClose(file);
			throw runtime_error("Unable to allocate enough memory to load file.");
		}
		s3eFileRead(file_buf, 1, file_size, file);
		s3eFileClose(file);
		file_buf[file_size] = '\0';
		demo_script = string(file_buf);
		s3eFree(file_buf);
	}
	pASModule->AddScriptSection("demo.as", demo_script.c_str());
	
	// Build the module
	r = pASModule->Build();
	if ( r < 0 ) {
		throw runtime_error("Build failed.");
		// The build failed. The message stream will have received
		// compiler errors that shows what needs to be fixed
	}
	
	asIScriptFunction* p_main_func = pASModule->GetFunctionByDecl("void main()");
	if(!pASModule) {
		// The function couldn't be found. Instruct the script writer
		// to include the expected function in the script.
		throw runtime_error("The script must have the function 'void main()'. Please add it and try again.\n");
	}
	
	// Create our context, prepare it, and then execute
	asIScriptContext* p_ctx = pASEngine->CreateContext();
	p_ctx->Prepare(p_main_func);
	r = p_ctx->Execute();
	if( r != asEXECUTION_FINISHED ) {
		// The execution didn't complete as expected. Determine what happened.
		if( r == asEXECUTION_EXCEPTION ) {
			// An exception occurred, let the script writer know what happened so it can be corrected.
			printf("An exception '%s' occurred. Please correct the code and try again.\n", p_ctx->GetExceptionString());
		}
	}
	p_ctx->Release();
	
	pASEngine->Release();
}





/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class EventHandler : public IIwUIEventHandler {
	virtual bool HandleEvent(CIwEvent* pEvent) {
		// If the user clicks the button:
		if (pEvent->GetID() == IWUI_EVENT_BUTTON)
			RunAngelScriptMain();
		return false;
	}
    virtual bool FilterEvent(CIwEvent* pEvent) { return false; }
};

void AddEventHandler(IIwUIEventHandler* pEventHandler);

// Boilerplate main loop
int main() {
    IwUIInit();

    //clear to white
    IwGxSetColClear(0xff, 0xff, 0xff, 0xff);

    CIwUIView* pView = new CIwUIView;
    CIwUIController* pController = new CIwUIController;
	EventHandler* pEventHandler = new EventHandler;
	pController->AddEventHandler(pEventHandler);

    IwGetResManager()->LoadGroup("ui.group");

    CIwUIElement* pUI = ((CIwUIElement*)IwGetResManager()->GetResNamed("main", "CIwUIElement"))->Clone();
    pView->AddElement(pUI);
    pView->AddElementToLayout(pUI);

    while(!s3eDeviceCheckQuitRequest()) {
        IwGxClear();
        pView->Update(20);
        pController->Update();
        pView->Render();
        IwGxFlush();
        IwGxSwapBuffers();
        s3eDeviceYield();
    }

	delete pEventHandler;
    delete pController;
    delete pView;

    IwUITerminate();

    return 0;
}

// To create an STL string using printf syntax
// http://stackoverflow.com/questions/2342162/stdstring-formating-like-sprintf
string string_format(const char* const fmt, ...) {
    int size=100; string str; va_list ap;
    while (1) {
        str.resize(size);
        va_start(ap, fmt); int n = vsnprintf((char *)str.c_str(), size, fmt, ap); va_end(ap);
        if (n > -1 && n < size) { str.resize(n); return str; }
        if (n > -1) { size=n+1; } else { size*=2; }
    }
}

