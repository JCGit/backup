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
#include "CurlDownload.h"
#include "SeverConsts.h"
#include "json/json.h"
#include <time.h>
#include "MessageBoxPage.h"
const std::string INTERNAL_ANNOUNCEMENT_FILE_DOWNLOADED = "announcementCfg.json";
const std::string INTERNAL_ANNOUNCEMENT_VERSION_KEY="internalAnnouncementVersion";
const std::string INTERNAL_ANNOUNCEMENT_DWONLOAD_TIME_KEY="internalAnnouncementDownloadTime";
#define Max_Download_Count 30
enum AnnouncementError
{
	JSON_FILE_FORMAT_ERROR=10005,
	DOWNLOADED_MAX,
	FILE_BUFFER_ERROR,
	FILE_DOWNLOADED_FAILED_RETRY,
	AnnouncementFileNotFound
};
class AnnouncementNewContent : public ContentBase
{

public:
	AnnouncementNewContent(unsigned int id);
	~AnnouncementNewContent(){};
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
	
	cocos2d::CCLabelBMFont* mTitle;
	
	cocos2d::CCNode* mMiddle;
	cocos2d::CCNode* mContent;
	
	cocos2d::CCLayerColor* mLayer;
	const AnnouncementItem* mItem;
	
private: 
	unsigned int adventureId;
};



class AnnouncementNewPage 
	: public CustomPage
	, public State<MainFrame>
	, public CCBContainer::CCBContainerListener
	, public CurlDownload::DownloadListener
	, public libOSListener
{
public:
	AnnouncementNewPage(void);
	~AnnouncementNewPage(void);

	CREATE_FUNC(AnnouncementNewPage);

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

	void startDown();
	void downInternalAnnouncementFile();
	void downloaded(const std::string& url,const std::string& filename);
	void downloadFailed(const std::string& url, const std::string& filename);
	virtual void onMessageboxEnter(int tag);
	void _retryShowDownFaildMsgBox(AnnouncementError errorCode);

private:
	bool isInStartDown;
	int mFileErrCount;
	bool mDownloadFile;
	std::string mInternalAnnouncementFilePath;
	unsigned int mInternalAnnouncementVersion;
	bool isNeedReDown();
	void	loadAnnouncementFile(const std::string& announcementFile,bool isDownload=false);
	CCNode* memberRootNode;
	cocos2d::extension::CCScrollView* members;
	typedef std::vector<ContentBase*> ContentList;
	ContentList mContentList;
	float mHeight;
};

