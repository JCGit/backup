#ifndef _QEVENT_H_
#define _QEVENT_H_
#include "cocos2d.h"
#include <string>
using namespace std;
USING_NS_CC;

typedef void (CCObject::*SEL_QEventCallFunc)(const string& stEvent, CCObject * stEventObj);

#define eventListner_selector(SELECTOR) (SEL_QEventCallFunc)(&SELECTOR)
#endif
