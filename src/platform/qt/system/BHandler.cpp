#include "BHandler.h"

#include <string.h>

#include <new>

#include "PlatformMessageEvent.h"


// #pragma mark - BHandlerProxy


BHandlerProxy::BHandlerProxy(BHandler* handler)
	:
	fHandler(handler)
{
}


void
BHandlerProxy::customEvent(QEvent* event)
{
	if (fHandler != NULL && event->type()
			== PlatformMessageEvent::EventType()) {
		fHandler->MessageReceived(
			&dynamic_cast<PlatformMessageEvent*>(event)->Message());
	}
}


// #pragma mark - BHandler


BHandler::BHandler(const char* name)
	:
	fName(name != NULL ? strdup(name) : NULL),
	fProxy(new(std::nothrow) BHandlerProxy(this))
{
}


BHandler::~BHandler()
{
	fProxy->HandlerDeleted();
	free(fName);
}


void
BHandler::MessageReceived(BMessage* message)
{
}


void
BHandler::ObjectConstructed(QObject* object)
{
	fProxy->setParent(object);
}


void
BHandler::ObjectAboutToBeDestroyed(QObject* /*object*/)
{
	fProxy->setParent(NULL);
}