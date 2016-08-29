#pragma once

#include "Singleton.h"
#include <string>
#include <map>
#include <list>
#include "BasePage.h"
#include "cocos2d.h"
#include "ScriptContentBase.h"

USING_NS_CC;


class CCBManager :public Singleton<CCBManager>, public CCObject
{
public:
	CCBManager();
	~CCBManager(void);
	/*
	*/
	bool registerPage(const std::string& name, BasePage* page);
	/*
	*/
	void unRegisterPage(const std::string& name, BasePage* page);
	/*
	*/
	BasePage* getPage(const std::string& name);

	CCBContainer* createAndLoad(const std::string & ccbfile);
	ScriptContentBase* createAndLoad2(const std::string & ccbfile);
	//只加载，不缓存，不retain
	CCBContainer* loadCCbi( const std::string & ccbfile );

	//load ccbfilenew data by zhenhui 2014/7/21
	static CCBFileNew* loadCCBFileNew( const std::string & ccbfile );

	void purgeCachedData(void);
	void update(float dt);

	static CCBManager* getInstance() { return Get(); }
private:
	typedef std::map<std::string,BasePage*> PAGE_MAP;
	PAGE_MAP mPages;
	typedef std::list<CCBContainer*> CCBLIST;
	typedef std::list<ScriptContentBase*> CCBLIST2;
	typedef std::map<std::string, CCBLIST > CCBMAP;
	typedef std::map<std::string, CCBLIST2 > CCBMAP2;
	CCBMAP mCCBPool;
	CCBMAP2 mCCBPool2;
	typedef std::map<std::string, time_t> CCBTIMEMAP;
	CCBTIMEMAP m_CCBTimeMap;
	float	m_checkTime;
};


#define REGISTER_PAGE(_NAME_,_CLASS_) \
	_CLASS_ *__page_##_CLASS_ = _CLASS_::create(); \
	bool __ret_##_CLASS_ = CCBManager::Get()->registerPage(_NAME_,__page_##_CLASS_);

void registerScriptPage(const std::string& ccbfile);