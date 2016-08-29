#ifndef __QEVENT_DISPATCHER_H__
#define __QEVENT_DISPATCHER_H__
#include "cocoa/CCObject.h"
#include "base/CVSingleton.h"
#include <string>
#include "QEvent.h"

using namespace std;
using namespace CVLib;
USING_NS_CC;


class CQEventDispatcher
{
public:
	CQEventDispatcher(){
		m_uEventObjID ++;
		m_uMyID = m_uEventObjID;
	}
	virtual ~CQEventDispatcher();
	void AddEventListener(CCObject* pstTarget, const string& stEvent, SEL_QEventCallFunc);
	void RemoveEventListener(CCObject* pstTarget, const string& stEvent, SEL_QEventCallFunc);
	void DispatchEvent(const string& stEvent, CCObject* pstEventObj);
	void DispatchEvents(const string& stEvents, CCObject* pstEventObj);
	static string GetEventString(const string& stOrigString);
private:
	string ComposeEvent(const string& stEvent);
	static unsigned long m_uEventObjID;
	unsigned long m_uMyID;
	string m_stAllEvents;
};


#endif