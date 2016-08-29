#ifndef __QEVENTMGR_H__
#define __QEVENTMGR_H__
#include "base/CVSingleton.h"
//#include "google/protobuf/stubs/hash.h"
#include <vector>
#include "QEventDispatcher.h"
#include "cocoa/CCArray.h"
#include "cocos2d.h"
#include "cocoa/CCObject.h"
#include "cocoa/CCDictionary.h"
using namespace CVLib;
//using namespace __gnu_cxx;
using namespace std;



class CQEventHandlerObj:public CCObject
{
public:
	SEL_QEventCallFunc m_pfHandler;
	CCObject* m_pTarget;
	CQEventHandlerObj(CCObject* pTarget, SEL_QEventCallFunc pfHandler){
		m_pfHandler = pfHandler;
		m_pTarget = pTarget;
	}
};

class CQEventMgr
{
public:
	CQEventMgr();
	~CQEventMgr();
	int RegEventListener(CCObject* pTarget, const string& stEvent, SEL_QEventCallFunc pfHandler);
	int RemoveEventListenser(CCObject* pTarget, const string& stEvent, SEL_QEventCallFunc pfHandler);
	int Event(CCObject* pstEventObj, const string& stEvent);
	int RemoveEventListener(const string& stEvent);
private:
	CCDictionary* m_stHandlers;
};

#define P_QEVENT_MGR CVSingleton<CQEventMgr>::Instance()

#endif