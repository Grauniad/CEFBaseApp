#include "CefBaseDefaults.h"

#include "CefBaseApp.h"

#include "CefBaseDisplayGTKDefaultHandler.h"
#include <CefBaseLoadDefaultHandler.h>
#include <CefBaseLifeSpanDefaultHandler.h>
#include <CefBaseIPCExec.h>


void CefBaseDefaults::InstallDefaultHandlers(CefBaseApp& app) {
	CefBaseClient& client = app.Client();

	client.DisplayHandler().InstallHandler(
		std::shared_ptr<CefDisplayHandler>(
			new CefBaseDisplayDefaultGTKHandler));

	client.LifeSpanHandler().InstallHandler(
		std::shared_ptr<CefLifeSpanHandler>(
			new CefBaseLifeSpanDefaultHandler));

	client.LoadHandler().InstallHandler(
		std::shared_ptr<CefLoadHandler>(
			new CefBaseLoadDefaultHandler()));
}
