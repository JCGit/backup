#include "QEventMgr.h"

CQEventMgr::CQEventMgr()
{
	m_stHandlers = CCDictionary::create();
	m_stHandlers->retain();
	
}

CQEventMgr::~CQEventMgr()
{
	
	CCDictElement* pstElement;
	CCDICT_FOREACH(m_stHandlers, pstElement){
		CCArray* handlers = (CCArray*)pstElement->getObject();
		for (unsigned int iIdx = 0; iIdx < handlers->count(); iIdx++){
			delete handlers->objectAtIndex(iIdx);
		}
	}
	m_stHandlers->autorelease();
	
}

int CQEventMgr::RegEventListener(CCObject* pTarget, const string& stEvent, SEL_QEventCallFunc pfHandler)
{

	CCArray* handlers = dynamic_cast<CCArray*>(m_stHandlers->objectForKey(stEvent));
	
	if (!handlers){
		handlers = CCArray::create();
		handlers->retain();
		handlers->addObject(new CQEventHandlerObj(pTarget, pfHandler));
		m_stHandlers->setObject(handlers, stEvent);
		
	}else{
		//bool bHasReg = false;		
		for (unsigned int iIdx = 0; iIdx < handlers->count(); iIdx++){
			if (((CQEventHandlerObj*)handlers->objectAtIndex(iIdx))->m_pfHandler == pfHandler){
				return 0;
			}
		}
		handlers->addObject(new CQEventHandlerObj(pTarget, pfHandler));
	}
	 
	return 0;
}
int CQEventMgr::RemoveEventListenser(CCObject* pTarget, const string& stEvent, SEL_QEventCallFunc pfHandler)
{
	
	CCArray* handlers = dynamic_cast<CCArray*>(m_stHandlers->objectForKey(stEvent));
	if (!handlers){
		return 0;
	}else{
		//bool bHasReg = false;		
		for (unsigned int iIdx = 0; iIdx < handlers->count(); iIdx++){
			CQEventHandlerObj* obj = (CQEventHandlerObj*)handlers->objectAtIndex(iIdx);
			if (obj->m_pfHandler == pfHandler){
				handlers->removeObjectAtIndex(iIdx);
				delete obj;
				if (handlers->count() == 0){
					handlers->release();
					m_stHandlers->removeObjectForKey(stEvent);
				}
				return 0;
			}
		}
		//handlers.addObject(new CQEventHandlerObj(pfHandler));
	}

	return 0;
}
int CQEventMgr::Event(CCObject* pstEventObj, const string& stEvent)
{
	CCArray* handlers = dynamic_cast<CCArray*>(m_stHandlers->objectForKey(stEvent));
	if (!handlers){
		return 0;
	}else{
		//bool bHasReg = false;	
		for (unsigned int iIdx = 0; iIdx < handlers->count(); iIdx++){
			CQEventHandlerObj* obj = (CQEventHandlerObj*)handlers->objectAtIndex(iIdx);
			if (obj->m_pfHandler){
				CCObject* pstTarget = obj->m_pTarget;
				SEL_QEventCallFunc pHandler = obj->m_pfHandler;
				(pstTarget->*pHandler)(stEvent, pstEventObj);

			}

			if(m_stHandlers == NULL || m_stHandlers->objectForKey(stEvent) == NULL){  //检查是否还有该事件持有者
				break;
			}
		}
		//handlers.addObject(new CQEventHandlerObj(pfHandler));
	}

	return 0;
}

int CQEventMgr::RemoveEventListener(const string& stEvent){
	CCArray* handlers = dynamic_cast<CCArray*>(m_stHandlers->objectForKey(stEvent));
	if (!handlers){
		return 0;
	}else{
		//bool bHasReg = false;		
		for (unsigned int iIdx = 0; iIdx < handlers->count(); iIdx++){
			CQEventHandlerObj* obj = (CQEventHandlerObj*)handlers->objectAtIndex(iIdx);			
			handlers->removeObjectAtIndex(iIdx);
			delete obj;
			if (handlers->count() == 0){
				m_stHandlers->removeObjectForKey(stEvent);
				handlers->release();
				return 0;
			}				
		}
		//handlers.addObject(new CQEventHandlerObj(pfHandler));
	}

	return 0;
}
