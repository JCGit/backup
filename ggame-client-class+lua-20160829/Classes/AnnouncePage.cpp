
#include "stdafx.h"

#include "AnnouncePage.h"
#include "StringConverter.h"
#include "BlackBoard.h"
#include "libOS.h"
#include "GamePlatform.h"
#include "SeverConsts.h"
REGISTER_PAGE("AnnouncePage", AnnouncePage);
USING_NS_CC;
USING_NS_CC_EXT;

AnnouncePage::AnnouncePage(void)
{
	members = NULL;
	memberRootNode = NULL;
	mContentList.clear();
	mDownloadFile=false;
	isInStartDown=false;
	mFileErrCount=0;
}


AnnouncePage::~AnnouncePage(void)
{
	mDownloadFile=false;
	isInStartDown=false;
	mFileErrCount=0;
}

void AnnouncePage::Enter( MainFrame* )
{
	if(AnnouncementTableManager::Get()->getTotalNum()==0)
	{
		std::string desFile(cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath());
		desFile=desFile+"/"+ANNOUNCEMENT_FILE_DOWNLOADED;
		if(CCFileUtils::sharedFileUtils()->isFileExist(desFile.c_str()))
		{
			if(isNeedReDown())
			{
				startDown();
			}
			else
			{
				loadAnnouncementFile(desFile);
			}
			
		}
		else
		{
			startDown();
		}
		
	}
	else
	{
		if(isNeedReDown())
		{
			startDown();
		}
		else
		{
			rebuildAllItem();
		}
	}
}

/************************************************************************/
/* //版本号变更后或者距离上次获取公告时间超过30分钟后重新获取           */
/************************************************************************/
bool AnnouncePage::isNeedReDown()
{
	const std::string kVersion(ANNOUNCEMENT_VERSION_KEY);
	int localVersion = CCUserDefault::sharedUserDefault()->getIntegerForKey(kVersion.c_str(), 0);
	if ( localVersion < SeverConsts::Get()->getInternalAnnouncementVersion() )
	{
		return true;
	}

	const std::string kDownloadTime(ANNOUNCEMENT_DWONLOAD_TIME_KEY);
	int downloadTime = CCUserDefault::sharedUserDefault()->getIntegerForKey(kDownloadTime.c_str(), 0);
	int offset = GamePrecedure::Get()->getServerTime() - downloadTime;
	const int maxOffset = StringConverter::parseInt(VaribleManager::Get()->getSetting("FetchBulletinOffset"), 1800);//unit: second
	if ( offset > maxOffset )
	{
		return true;
	}

	return false;
}

void AnnouncePage::Execute( MainFrame* )
{
	if(mDownloadFile)
	{
		CurlDownload::Get()->update(0);
	}
}

void AnnouncePage::Exit( MainFrame* )
{
	isInStartDown=false;
	mDownloadFile=false;
	mFileErrCount=0;
	libOS::getInstance()->removeListener(this);
	if ( !GamePrecedure::Get()->isInLoadingScene() )
	{
		CurlDownload::Get()->removeListener(this);
	}
	//clearAllItem();
}


void AnnouncePage::startDown()
{
	isInStartDown=true;
	libOS::getInstance()->registerListener(this);
	CurlDownload::Get()->addListener(this);
	downInternalAnnouncementFile();
}

void AnnouncePage::downInternalAnnouncementFile()
{
	mDownloadFile=true;
	mInternalAnnouncementFilePath=SeverConsts::Get()->getInternalAnnouncementFilePath();
	if(mInternalAnnouncementFilePath=="")
	{
		//MSG_BOX_LAN("@GetAnnouncement_"+AnnouncementFileNotFound);
		closePage();
	}
	else
	{
		std::string desFile(cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath());
		desFile=desFile+"/"+ANNOUNCEMENT_FILE_DOWNLOADED;
		remove(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(desFile.c_str()).c_str());
		CurlDownload::Get()->downloadFile(mInternalAnnouncementFilePath,desFile);
	}
}


void AnnouncePage::downloaded( const std::string& url,const std::string& filename )
{
	if( !mInternalAnnouncementFilePath.empty() && url.find(mInternalAnnouncementFilePath)!=url.npos)
	{
		mDownloadFile=false;
		loadAnnouncementFile(filename,true);
	}
}

void AnnouncePage::downloadFailed( const std::string& url, const std::string& filename )
{
	if( !mInternalAnnouncementFilePath.empty() && url.find(mInternalAnnouncementFilePath)!=url.npos )
	{
        const unsigned int maxTryTimes = StringConverter::parseUnsignedInt(VaribleManager::Get()->getSetting("MaxFetchBulletinTimes"), 1);
		if(mFileErrCount<=maxTryTimes)
		{
			_retryShowDownFaildMsgBox(FILE_DOWNLOADED_FAILED_RETRY);
		}
		else
		{
			mDownloadFile=false;
			_retryShowDownFaildMsgBox(DOWNLOADED_MAX);
		}
	}
}

void AnnouncePage::loadAnnouncementFile(const std::string& announcementFile,bool isDownload)
{
	unsigned long filesize = 0;
	char* pBuffer = (char*)getFileData(
		cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(announcementFile.c_str()).c_str(),
		"rt",&filesize,0,false);

	if(!pBuffer)
	{
		_retryShowDownFaildMsgBox(FILE_BUFFER_ERROR);
		CC_SAFE_DELETE_ARRAY(pBuffer);
		return;
	}
	if ( pBuffer[filesize - 1] != '\0' )
	{
		char* newBuffer = new char[filesize + 1];
		memcpy(newBuffer, pBuffer, filesize);
		newBuffer[filesize] = '\0';
		CC_SAFE_DELETE_ARRAY(pBuffer);
		pBuffer = newBuffer;
	}
	CCSize size = members->getViewSize();
	CCHTMLLabel* label = CCHTMLLabel::createWithString(pBuffer, size);
	label->setPosition(ccp(0, 0));
	members->addChild(label);
	unsigned int height = label->getContentSize().height;
	members->setContentSize(CCSizeMake(members->getContentSize().width, height));
	members->setContentOffset(ccp(0, size.height - height));

	CC_SAFE_DELETE_ARRAY(pBuffer);
	setVisible(true);

	mDownloadFile=false;
	if(isDownload)
	{
		cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(ANNOUNCEMENT_VERSION_KEY.c_str(),SeverConsts::Get()->getInternalAnnouncementVersion());
		cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(ANNOUNCEMENT_DWONLOAD_TIME_KEY.c_str(),GamePrecedure::Get()->getServerTime());
		cocos2d::CCUserDefault::sharedUserDefault()->flush();
	}
	//rebuildAllItem();
}


void AnnouncePage::_retryShowDownFaildMsgBox(AnnouncementError errorCode)
{
	mFileErrCount++;

	mDownloadFile=false;

	if(!isInStartDown)
	{
		isInStartDown=true;
		libOS::getInstance()->registerListener(this);
		CurlDownload::Get()->addListener(this);
	}
	if(libOS::getInstance()->getNetWork()==NotReachable)
	{
		libOS::getInstance()->showMessagebox(Language::Get()->getString("@NoNetWork"),errorCode);
	}
	else
	{
		if ( errorCode < JSON_FILE_FORMAT_ERROR || errorCode > AnnouncementFileNotFound )
		{
			return;
		}
		libOS::getInstance()->showMessagebox(Language::Get()->getString("@FailFetchBulletin"), errorCode);
	}
}

void AnnouncePage::onMessageboxEnter(int tag)
{
	if ( tag < JSON_FILE_FORMAT_ERROR || tag > AnnouncementFileNotFound )
	{
		return;
	}
	bool isClose=false;

	const unsigned int maxTryTimes = StringConverter::parseUnsignedInt(VaribleManager::Get()->getSetting("MaxFetchBulletinTimes"), 1);
	if(tag!=DOWNLOADED_MAX&&mFileErrCount<maxTryTimes)
	{
		if((tag==FILE_BUFFER_ERROR||tag==JSON_FILE_FORMAT_ERROR))
		{
			if(!isInStartDown)
			{
				startDown();
			}
			else
			{
				downInternalAnnouncementFile();
			}
		}
		else
		{
			downInternalAnnouncementFile();
		}
	}
	else
	{
		isClose=true;
	}
	if(isClose)
	{
		mDownloadFile=false;
		isInStartDown=false;
		closePage();
	}
}

void AnnouncePage::onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender, int tag )
{
}

void AnnouncePage::onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender )
{
	if (itemName == "onClose")
	{
		closePage();
	}
}

void AnnouncePage::closePage()
{
	if ( GamePrecedure::Get()->isInLoadingScene() )
	{
		setVisible(false);
		removeFromParent();
	}
	else
	{
		MsgMainFramePopPage msg;
		msg.pageName = "AnnouncePage";
		MessageManager::Get()->sendMessage(&msg);
	}
}

void AnnouncePage::load( void )
{
	setVisible(false);
	loadCcbiFile("AnnouncementPage.ccbi");
	//CCB_FUNC(this, "mTitle", CCLabelBMFont, setString(Language::Get()->getString("@Announcement").c_str()));
	members = dynamic_cast<cocos2d::extension::CCScrollView*>(getVariable("mAnnMsgContent"));
	memberRootNode = members->getContainer();
}

void AnnouncePage::rebuildAllItem( void )
{
	clearAllItem();
	buildItem();
}

void AnnouncePage::clearAllItem( void )
{
	ContentList::iterator it = mContentList.begin();
	
	while (it != mContentList.end())
	{
		delete (*it);
		++it;
	}

	ContentList list;
	mContentList.swap(list);
	memberRootNode->removeAllChildren();
}

void AnnouncePage::buildItem( void )
{
	// added by sunyj
	// 因为平台差异，如果当前item是本平台排除掉的信息，那么跳过这个item，（比如多酷跳过微信）
	std::string _itemidStr="";
	std::vector<std::string> excludedIds;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const PlatformRoleItem* roleItem=PlatformRoleTableManager::Get()->getPlatformRoleByID(libPlatformManager::getPlatform()->getPlatformId());
	if(roleItem)
	{
		excludedIds = GameMaths::tokenize(roleItem->excludedAnnoucementIds,",");
	}
#else
	#ifdef WIN32
	#else
	const PlatformRoleItem* roleItem=PlatformRoleTableManager::Get()->getPlatformRoleByName(libPlatformManager::getPlatform()->getClientChannel());
	if(roleItem)
	{
		excludedIds = GameMaths::tokenize(roleItem->excludedAnnoucementIds,",");
	}
	#endif
#endif
	
	// end sunyj
	mHeight=0.0f;
	AnnouncementTableManager::AnnouncementItemListIterator it = AnnouncementTableManager::Get()->getAnnouncementItemListIterator();
	bool isHavAnnouncementNew=false;
	int now=GamePrecedure::Get()->getServerTime();
	std::list<AnnouncementItem* > orderlist;
	while(it.hasMoreElements())
	{
		AnnouncementItem* item=it.getNext();

		// 过滤掉被排除的公告条目. sunyj
		_itemidStr=StringConverter::toString(item->itemId);
		if ( std::find(excludedIds.begin(), excludedIds.end(),_itemidStr) != excludedIds.end()) 
		{
			// 如果在excludedIds列表中找到此itemId，那么过滤掉这个Id
			continue;
		}
		// end


		long int startTimeStamp = item->startTime;
		long int endTimeStamp = item->endTime;
		
		bool _isValid=false;
		int lastTime=GamePrecedure::Get()->getServerOpenTime()+item->severDelayTime;
		if((now>= startTimeStamp && now<= endTimeStamp&&item->severDelayTime==0)||(item->severDelayTime!=0&&lastTime>now))
		{
			_isValid=true;
			isHavAnnouncementNew=true;
			orderlist.push_back(item);
		}
	}
	excludedIds.clear();


	if(!isHavAnnouncementNew)
	{
		MsgMainFramePopPage msg;
		msg.pageName = "AnnouncePage";
		MessageManager::Get()->sendMessage(&msg);
	}
}

void AnnouncePage::addItem( ContentBase* item )
{
	mContentList.push_back(item);
	memberRootNode->addChild(item->getContainerNode());
	CCPoint pos = ccp(0,mHeight);
	mHeight+=item->getContainerNode()->getContentSize().height;
	item->getContainerNode()->setPosition(pos);
	CCLOG("addItem %f",item->getContainerNode()->getContentSize().height);
	CCSize size = CCSize(item->getContainerNode()->getContentSize().width, mHeight);
	members->setContentSize(size);
	members->setContentOffset(ccp(0, members->getViewSize().height - members->getContentSize().height*members->getScaleY()));
}

void AnnouncePage::removeItem( ContentBase* item )
{
	ContentList::iterator it = mContentList.begin();

	while(it != mContentList.end())
	{
		if ((*it) == item)
		{
			memberRootNode->removeChild(item->getContainerNode());
			delete item;
			mContentList.erase(it);
		}
		++it;
	}
}
