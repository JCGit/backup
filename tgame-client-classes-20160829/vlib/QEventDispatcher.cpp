#include "QEventDispatcher.h"
#include "QEventMgr.h"

unsigned long CQEventDispatcher::m_uEventObjID = 0;

CQEventDispatcher::~CQEventDispatcher(){
	//把自己身上的所有事件侦听器全部删除	
	
	size_t offset = 0;
	size_t pos = 0;
	while((pos = m_stAllEvents.find(";", offset)) != string::npos){
		string singleEvent = m_stAllEvents.substr(offset, pos - offset);
		P_QEVENT_MGR->RemoveEventListener(singleEvent);
		offset = pos + 1;
	}

	string singleEvent = m_stAllEvents.substr(offset, m_stAllEvents.length() - offset);
	if (singleEvent.length()){
		P_QEVENT_MGR->RemoveEventListener(singleEvent);
	}
}

void CQEventDispatcher::AddEventListener(CCObject* pstTarget, const string& stEvent, SEL_QEventCallFunc pfHandler)
{
	string stRealEvent = ComposeEvent(stEvent);
	P_QEVENT_MGR->RegEventListener(pstTarget, stRealEvent, pfHandler);
	m_stAllEvents.append(";").append(stRealEvent);
}
void CQEventDispatcher::RemoveEventListener(CCObject* pstTarget, const string& stEvent, SEL_QEventCallFunc pfHandler)
{
	string stRealEvent = ComposeEvent(stEvent);
	P_QEVENT_MGR->RemoveEventListenser(pstTarget, stRealEvent, pfHandler);
}

string CQEventDispatcher::ComposeEvent(const string& stEvent)
{
	string stRealEvent = stEvent;
	char idStr[32];
	sprintf(idStr, "_%lu", m_uMyID);
	stRealEvent.append(idStr);
	return stRealEvent;
}

string CQEventDispatcher::GetEventString(const string& stOrigString){
	size_t pos = stOrigString.find_last_of("_");
	if (pos == string::npos){
		return stOrigString;
	}
	return stOrigString.substr(0, pos);
}

void CQEventDispatcher::DispatchEvent(const string& stEvent, CCObject* pstEventObj)
{
	string stRealEvent = ComposeEvent(stEvent);

	P_QEVENT_MGR->Event(pstEventObj, stRealEvent);
}

void CQEventDispatcher::DispatchEvents(const string& stEvents, CCObject* pstEventObj)
{
	string strEventStr;
	strEventStr.assign(stEvents);
	size_t offset = 0;
	size_t pos = 0;
	while((pos = strEventStr.find(";", offset)) != string::npos){
		string singleEvent = strEventStr.substr(offset, pos - offset);
		DispatchEvent(singleEvent, pstEventObj);
		offset = pos + 1;
	}

	string singleEvent = strEventStr.substr(offset, strEventStr.length() - offset);
	if (singleEvent.length()){
		DispatchEvent(singleEvent, pstEventObj);
	}
}
