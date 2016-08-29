
#include "stdafx.h"

#include "CCBManager.h"
#include "cocos2d.h"
#include "CCBScriptContainer.h"
#include "DataTableManager.h"
USING_NS_CC;

#define  CHECK_TIME 15.0f

CCBManager::CCBManager()
{
	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this,0,false);
	m_checkTime = CHECK_TIME;
}

CCBManager::~CCBManager(void)
{
	for(PAGE_MAP::iterator it = mPages.begin();it!=mPages.end();++it)
	{
		/*
			BasePage基本都是以全局变量注册的，
			最后退出时刻了，尝试尽量清理，
			如果下面while出问题，说明BasePage间有依赖或Page与引擎有交错，可以尝试去找找，修正
		*/
		//while (!it->second->isSingleReference())
		//{
		//	it->second->release();
		//}
		it->second->release();
	}
	CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
}

bool CCBManager::registerPage( const std::string& name, BasePage* page )
{
	if(mPages.find(name)!=mPages.end())
		CCMessageBox("ERROR! Page name redefined!!",Language::Get()->getString("@ShowMsgBoxTitle").c_str());

	page->retain();
	mPages.insert(std::make_pair(name,page));

	return true;
}

void CCBManager::unRegisterPage( const std::string& name, BasePage* page )
{
	PAGE_MAP::iterator it = mPages.find(name);
	if (it == mPages.end())
		return;
	//
	page->release();
	mPages.erase(it);
	//
}

BasePage* CCBManager::getPage( const std::string& name )
{
	PAGE_MAP::iterator it = mPages.find(name);
	if(it!=mPages.end())
		return it->second;
	else
		return NULL;
}
CCBContainer* CCBManager::loadCCbi( const std::string & ccbfile )
{
	CCBContainer * ccb = CCBContainer::create();
	ccb->loadCcbiFile(ccbfile);

	//CCLOG("[CCBManager::createAndLoad1] ccbfile: %s", ccbfile.c_str());

	//CCBLIST clist;
	//clist.push_back(ccb);

	//mCCBPool.insert(std::make_pair(ccbfile,clist));

	//time_t now = time(NULL);
	//m_CCBTimeMap[ccbfile] = now;
	return ccb;
}

CCBContainer* CCBManager::createAndLoad( const std::string & ccbfile )
{
	CCBMAP::iterator mapit = mCCBPool.find(ccbfile);
	if(mapit==mCCBPool.end())
	{
		CCBContainer * ccb = CCBContainer::create();
		ccb->retain();
		ccb->loadCcbiFile(ccbfile);

		//CCLOG("[CCBManager::createAndLoad1] ccbfile: %s", ccbfile.c_str());

		CCBLIST clist;
		clist.push_back(ccb);

		mCCBPool.insert(std::make_pair(ccbfile,clist));

		time_t now = time(NULL);
		m_CCBTimeMap[ccbfile] = now;
		return ccb;
	}
	else
	{
		CCBLIST::iterator listit = mapit->second.begin();
		for(;listit!=mapit->second.end();++listit)
		{
			CCBContainer* ccb = *listit;
			if(ccb->isSingleReference())
			{ 
				ccb->setListener(NULL);
				ccb->playAutoPlaySequence();

				CCLOG("[CCBManager::createAndCache] ccbfile: %s", ccbfile.c_str());

				time_t now = time(NULL);
				m_CCBTimeMap[ccbfile] = now;
				return ccb;
			}
		}
		
		CCBContainer * ccb = CCBContainer::create();
		ccb->retain();
		ccb->loadCcbiFile(ccbfile);

		CCLOG("[CCBManager::createAndLoad2] ccbfile: %s", ccbfile.c_str());

		mapit->second.push_back(ccb);

		time_t now = time(NULL);
		m_CCBTimeMap[ccbfile] = now;
		return ccb;
	}
}

ScriptContentBase* CCBManager::createAndLoad2( const std::string & ccbfile )
{
	CCBMAP2::iterator mapit = mCCBPool2.find(ccbfile);
	if(mapit==mCCBPool2.end())
	{
		ScriptContentBase * ccb = ScriptContentBase::create();
		ccb->retain();
		ccb->loadCcbiFile(ccbfile);

		//CCLOG("[CCBManager::createAndLoad1] ccbfile: %s", ccbfile.c_str());

		CCBLIST2 clist;
		clist.push_back(ccb);

		mCCBPool2.insert(std::make_pair(ccbfile,clist));

		time_t now = time(NULL);
		m_CCBTimeMap[ccbfile] = now;
		return ccb;
	}
	else
	{
		CCBLIST2::iterator listit = mapit->second.begin();
		for(;listit!=mapit->second.end();++listit)
		{
			ScriptContentBase * ccb = *listit;
			if(ccb->isSingleReference())
			{ 
				ccb->setListener(NULL);
				ccb->playAutoPlaySequence();

				CCLOG("[CCBManager::createAndCache] ccbfile: %s", ccbfile.c_str());

				time_t now = time(NULL);
				m_CCBTimeMap[ccbfile] = now;
				return ccb;
			}
		}

		ScriptContentBase * ccb = ScriptContentBase::create();
		ccb->retain();
		ccb->loadCcbiFile(ccbfile);

		CCLOG("[CCBManager::createAndLoad2] ccbfile: %s", ccbfile.c_str());

		mapit->second.push_back(ccb);

		time_t now = time(NULL);
		m_CCBTimeMap[ccbfile] = now;
		return ccb;
	}
}

void CCBManager::purgeCachedData( void )
{
	CCBMAP::iterator mapit = mCCBPool.begin();
	for(;mapit!=mCCBPool.end();++mapit)
	{
		CCBLIST listSwap;
		listSwap.swap(mapit->second);
		CCBLIST::iterator listit = listSwap.begin();
		for(;listit!=listSwap.end();++listit)
		{
			CCBContainer* ccb = *listit;
			if(ccb->isSingleReference())
			{
				ccb->release();
			}
			else
			{
				mapit->second.push_back(*listit);
			}
		}
	}
}


void registerScriptPage( const std::string& luaPagename )
{
	//如果没有注册lua页面，则注册，否则不再重复注册 by zhenhui 2014/9/8
	BasePage* page = CCBManager::Get()->getPage(luaPagename);
	if (page == NULL)
	{
		CCBScriptContainer *pRet = CCBScriptContainer::create(luaPagename);
		CCBManager::Get()->registerPage(luaPagename,pRet);
	}
}

void CCBManager::update(float dt)
{
	return;
	m_checkTime -= dt;
	if (m_checkTime < 0.0f)
	{
		CCBTIMEMAP::iterator it = m_CCBTimeMap.begin();
		CCBTIMEMAP::iterator end = m_CCBTimeMap.end();
		time_t now = time(NULL);
		for ( ; it != end; )
		{
			//判断该ccbi是否在可是否列表内
			ReleaseCCBFile* rcb = ReleaseCCBFileManager::Get()->getRelaseCCBFileByName(it->first);
			if (!rcb)
			{
				++it;
				continue;
			}
			time_t t = it->second;
			if (difftime(now,t) >= 300.0f)	//tmp
			{
				CCBMAP::iterator mapit = mCCBPool.find(it->first);
				if (mapit != mCCBPool.end())
				{
					CCBLIST& ccbList = mapit->second;
					CCBLIST::iterator ccbit = ccbList.begin();
					CCBLIST::iterator ccbend = ccbList.end();
					for ( ; ccbit != ccbend; ++ccbit)
					{
						CCBContainer* p = *ccbit;
						p->release();
					}
					ccbList.clear();
					CCLOG("remove file : %s",it->first.c_str());
				}
				mCCBPool.erase(mapit);

				m_CCBTimeMap.erase(it++);
			}
			else
			{
				++it;
			}

		}
		m_checkTime = CHECK_TIME;
	}
}


CCBFileNew* CCBManager::loadCCBFileNew( const std::string & ccbfile ){

	CCLOG("@CCBManager::loadCCBFileNew -- begin load %s",ccbfile.c_str());
	CCBFileNew* ccbFileNode = CCBFileNew::create();
	ccbFileNode->setCCBFileName(ccbfile);
	ccbFileNode->load(true);
	CCLOG("@CCBManager::loadCCBFileNew -- end load %s",ccbfile.c_str());
	return ccbFileNode;

}