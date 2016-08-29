#pragma once
#include "cocos-ext.h"
#include "cocos2d.h"
#include "CCBManager.h"
#include "CustomPage.h"
#include "StateMachine.h"
#include "MainFrame.h"
#include "CCBContainer.h"
#include "ContentBase.h"
#include "DataTableManager.h"
#include "GameMessages.h"
#include "MessageManager.h"
#include "GameMaths.h"
#include <time.h>

class AnnouncementContent : public ContentBase
{

public:
	AnnouncementContent(unsigned int id);
	~AnnouncementContent(){};
	virtual void refreshContent( void );
	virtual void onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender, int tag );

protected:
	virtual const std::string& getCcbiFileName( void ) const
	{
		static std::string ccbiFileName = "AnnouncementContent.ccbi";
		return ccbiFileName;
	}
	cocos2d::CCObject* getObjectByVariableName(const std::string& variableName);
	cocos2d::CCLabelBMFont* mMsg;
	cocos2d::CCLabelBMFont* mTimeMsg;
	cocos2d::CCLabelTTF* mTitle;
	cocos2d::CCNode* mTop;
	cocos2d::CCNode* mMiddle;
	cocos2d::CCNode* mEnd;
	cocos2d::CCNode* mBtnNode;
	cocos2d::CCLayerColor* mLayer;
	const AnnouncementItem* mItem;

	void _resizePage();
private: 
	unsigned int adventureId;
};

class AnnouncementPicContent : public ContentBase
{

public:
	AnnouncementPicContent(unsigned int id);
	~AnnouncementPicContent(){};
	virtual void refreshContent( void );
	virtual void onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender, int tag );

protected:
	virtual const std::string& getCcbiFileName( void ) const
	{
		static std::string ccbiFileName = "AnnouncementContent2.ccbi";
		return ccbiFileName;
	}
	cocos2d::CCObject* getObjectByVariableName(const std::string& variableName);
	cocos2d::CCLabelBMFont* mMsg;
	cocos2d::CCLabelBMFont* mTimeMsg;
	cocos2d::CCLabelBMFont* mTitle;
	cocos2d::CCNode* mTop;
	cocos2d::CCNode* mMiddle;
	cocos2d::CCNode* mEnd;
	cocos2d::CCNode* mBtnNode;
	cocos2d::CCLayerColor* mLayer;
	const AnnouncementItem* mItem;
	cocos2d::CCSprite* mAnnPic;
	void _resizePage();
private: 
	unsigned int adventureId;
};


class AnnouncementPage 
	: public CustomPage
	, public State<MainFrame>
	, public CCBContainer::CCBContainerListener
{
public:
	AnnouncementPage(void);
	~AnnouncementPage(void);

	CREATE_FUNC(AnnouncementPage);

	virtual PAGE_TYPE getPageType(){return CustomPage::PT_CONTENT;}
	virtual void Enter( MainFrame* );
	virtual void Execute( MainFrame* );
	virtual void Exit( MainFrame* );
	virtual void onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender, int tag );
	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender);
	virtual void load( void );
	void rebuildAllItem(void);
	void clearAllItem(void);
	void buildItem(void);
	void addItem(ContentBase* item);
	void removeItem(ContentBase* item);

private:
	CCNode* memberRootNode;
	cocos2d::extension::CCScrollView* members;
	typedef std::vector<ContentBase*> ContentList;
	ContentList mContentList;
	float mHeight;
};

