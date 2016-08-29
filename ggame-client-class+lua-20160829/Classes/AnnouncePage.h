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
const std::string ANNOUNCEMENT_FILE_DOWNLOADED = "announcement.txt";
const std::string ANNOUNCEMENT_VERSION_KEY="announcementVersion";
const std::string ANNOUNCEMENT_DWONLOAD_TIME_KEY="announcementDownloadTime";
#define Max_Download_Count 30
enum AnnouncementError
{
	JSON_FILE_FORMAT_ERROR=10005,
	DOWNLOADED_MAX,
	FILE_BUFFER_ERROR,
	FILE_DOWNLOADED_FAILED_RETRY,
	AnnouncementFileNotFound
};

class AnnouncePage 
	: public CustomPage
	, public State<MainFrame>
	, public CCBContainer::CCBContainerListener
	, public CurlDownload::DownloadListener
	, public libOSListener
{
public:
	AnnouncePage(void);
	~AnnouncePage(void);

	CREATE_FUNC(AnnouncePage);

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

	void closePage();
};

