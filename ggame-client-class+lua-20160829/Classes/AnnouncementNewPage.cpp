
#include "stdafx.h"

#include "AnnouncementNewPage.h"
#include "StringConverter.h"
#include "BlackBoard.h"
#include "libOS.h"
#include "GamePlatform.h"
#include "SeverConsts.h"
REGISTER_PAGE("AnnouncementNewPage", AnnouncementNewPage);
USING_NS_CC;
USING_NS_CC_EXT;

void AnnouncementNewContent::refreshContent( void )
{
	mItem=AnnouncementTableManager::Get()->getAnnouncementItemById(getContentID());
	adventureId = mItem->adventureId;

	//活动title
	mTitle = dynamic_cast<CCLabelBMFont*>(mContentContainer->getVariable("mAnnSubTitle"));
	if (mTitle)
	{
		mTitle->setString(mItem->title.c_str());
	}

	//活动时间说明
	CCLabelBMFont* timeMsg = dynamic_cast<CCLabelBMFont*>(mContentContainer->getVariable("mTimeMsg"));
	if (mItem->timeMsg != "none")
	{
		timeMsg->setVisible(true);
		timeMsg->setString(mItem->timeMsg.c_str());
	}else{
		timeMsg->setVisible(false);
	}


	mMsg = dynamic_cast<CCLabelBMFont*>(mContentContainer->getVariable("mAnnSubMsg"));
	float fInitHeight = mMsg->getContentSize().height;
	
	//以“井号键”为作为换行符，
	std::vector<std::string> vecLines = GameMaths::tokenize(mItem->msg,"#");
	std::string contentStr = "";

	for (int i=0;i<vecLines.size();i++)
	{
		std::string sSeg = vecLines[i];
		if (sSeg.length() ==0) 
		{
			contentStr.push_back('\n');
		}else{
			//目前每行最多显示27个字，配置在Varible.txt中的AnnouncementPageDescribe=27;中
			std::string des;
			int lines;
			int width = StringConverter::parseInt(VaribleManager::Get()->getSetting("AnnouncementPageDescribe"));
			GameMaths::stringAutoReturn(sSeg, des, width, lines);
			contentStr +=des;
			contentStr.push_back('\n');
		}
	}


	if(mMsg)
	{
		mMsg->setString(contentStr.c_str());
	}

	//是否显示按钮以及显示文字
	CCNode* buttonNode = dynamic_cast<CCNode*>(getObjectByVariableName("mGoToBtnNode"));
	if (mItem->isHaveButton)
	{
		buttonNode->setVisible(true);
		CCLabelBMFont * gotoLabel = dynamic_cast<CCLabelBMFont*>(getObjectByVariableName("mGoToLabel"));
		if (gotoLabel && mItem->buttonText.length()>0)
		{
			gotoLabel->setString(mItem->buttonText.c_str());
		}
	}else{
		buttonNode->setVisible(false);
	}

	//包含所有内容的节点
	mContent=dynamic_cast<CCNode* >(getObjectByVariableName("mContentNode"));
	//MSG最新的高度
	float fNewMsgHeight = mMsg->getContentSize().height * mMsg->getScaleY();
	float fTitleHeight = 50.0f;
	float fBottomButtonHeight = 90.0f;
	float fSubMsg = 40.0f;
	//所有内容最新的节点

	float totalNewHeight = fNewMsgHeight + fTitleHeight + fSubMsg;
	if (mItem->isHaveButton)
	{
		totalNewHeight = totalNewHeight + fBottomButtonHeight;
	}
	mMiddle=dynamic_cast<CCNode* >(getObjectByVariableName("mMiddleNode"));
	float currentHeight = mMiddle->getContentSize().height*mMiddle->getScaleY();

	//如果有图片，加入图片的偏移量
	float picHeight = 0.0;
	if (mItem->picPath!="none")
	{
		CCSprite * annPic = dynamic_cast<CCSprite*>(getObjectByVariableName("mAnnPic"));
		if (annPic)
		{
			annPic->setTexture(mItem->picPath.c_str());
			annPic->setVisible(true);
		}
		picHeight = annPic->getContentSize().height;
		totalNewHeight = totalNewHeight + picHeight;

	}
	else
	{
		CCSprite * annPic = dynamic_cast<CCSprite*>(getObjectByVariableName("mAnnPic"));
		if (annPic)
		{
			annPic->setVisible(false);
		}

	}

	//如果位置有变化，则需要重新设置对应大小
	if(totalNewHeight>currentHeight)
	{
		//将内容节点向上移动增加的空间
		mContent->setPositionY(mContent->getPositionY()+totalNewHeight-currentHeight);

		

		cocos2d::extension::CCScale9Sprite* bgSprite = dynamic_cast<cocos2d::extension::CCScale9Sprite*>(getObjectByVariableName("mBackGround"));
		if (bgSprite)
		{
			//bg size is equal to middle size 
			bgSprite->setContentSize(CCSize(bgSprite->getContentSize().width,totalNewHeight));
		}


		//充值大小
		mMiddle->setContentSize(CCSize(mMiddle->getContentSize().width,totalNewHeight));
		this->getContainerNode()->setContentSize(CCSize(this->getContainerNode()->getContentSize().width,totalNewHeight));
	}

	//如果有picture
	if (picHeight>0.0f)
	{
		int oldY = mMsg->getPositionY();
		mMsg->setPositionY(oldY-picHeight);
	}
}


void AnnouncementNewContent::onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender, int tag )
{
	if (itemName == "onGo")
	{
		if(mItem->btnType==1)
		{
			int _openType=StringConverter::parseInt(VaribleManager::Get()->getSetting("OpenAnnouncementStatus"));
			if(_openType==1)
			{
				libOS::getInstance()->openURL(mItem->buttonGoPage);
			}
			else
			{
				libPlatformManager::getPlatform()->openBBS();
			}
		}
		else
		{

			if(mItem->changeOrPush==1)
			{
				//BlackBoard::Get()->ToAdventruePageType = 0;
				MsgMainFramePushPage msg;
				msg.pageName = mItem->buttonGoPage;
				MessageManager::Get()->sendMessage(&msg);
			}
			else if(mItem->changeOrPush==0)
			{
				//BlackBoard::Get()->ToAdventruePageType = adventureId;
				MsgMainFrameChangePage msg;
				msg.pageName = mItem->buttonGoPage;
				MessageManager::Get()->sendMessage(&msg);
			}
			MsgMainFramePopPage popMsg;
			popMsg.pageName = "AnnouncementPage";
			MessageManager::Get()->sendMessage(&popMsg);
		}
	}
}

cocos2d::CCObject * AnnouncementNewContent::getObjectByVariableName(const std::string& variableName)
{
	cocos2d::CCObject* obj = getContainerNode()->getVariable(variableName);
	if(!obj)
	{
		char errMsg[256];
		sprintf(errMsg,"%s%s","Failed to find variable:",variableName.c_str());
		CCMessageBox(errMsg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());
	}
	return obj;
}

AnnouncementNewContent::AnnouncementNewContent( unsigned int id) : ContentBase(id)
{
}



AnnouncementNewPage::AnnouncementNewPage(void)
{
	members = NULL;
	memberRootNode = NULL;
	mContentList.clear();
	mDownloadFile=false;
	isInStartDown=false;
	mFileErrCount=0;
}


AnnouncementNewPage::~AnnouncementNewPage(void)
{
	mDownloadFile=false;
	isInStartDown=false;
	mFileErrCount=0;
}

void AnnouncementNewPage::Enter( MainFrame* )
{
	if(AnnouncementTableManager::Get()->getTotalNum()==0)
	{
		std::string desFile(cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath());
		desFile=desFile+"/"+INTERNAL_ANNOUNCEMENT_FILE_DOWNLOADED;
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
bool AnnouncementNewPage::isNeedReDown()
{
	bool isNeedReDownload=false;
	unsigned int lastDownloadTime=CCUserDefault::sharedUserDefault()->getIntegerForKey(INTERNAL_ANNOUNCEMENT_DWONLOAD_TIME_KEY.c_str(),0);
	unsigned int intervalTime=GamePrecedure::Get()->getServerTime()-lastDownloadTime;
	unsigned int localCacheVersion=CCUserDefault::sharedUserDefault()->getIntegerForKey(INTERNAL_ANNOUNCEMENT_VERSION_KEY.c_str(),0);
	if(SeverConsts::Get()->getInternalAnnouncementVersion()>localCacheVersion||intervalTime>0&&(intervalTime/60)>30)
	{//版本号变更后或者距离上次获取公告时间超过30分钟后重新获取
		isNeedReDownload=true;
	}
	return isNeedReDownload;
}

void AnnouncementNewPage::Execute( MainFrame* )
{
	if(mDownloadFile)
	{
		CurlDownload::Get()->update(0);
	}
}

void AnnouncementNewPage::Exit( MainFrame* )
{
	isInStartDown=false;
	mDownloadFile=false;
	mFileErrCount=0;
	libOS::getInstance()->removeListener(this);
	CurlDownload::Get()->removeListener(this);
	clearAllItem();
}


void AnnouncementNewPage::startDown()
{
	isInStartDown=true;
	libOS::getInstance()->registerListener(this);
	CurlDownload::Get()->addListener(this);
	downInternalAnnouncementFile();
}

void AnnouncementNewPage::downInternalAnnouncementFile()
{
	mDownloadFile=true;
	mInternalAnnouncementFilePath=SeverConsts::Get()->getInternalAnnouncementFilePath();
	if(mInternalAnnouncementFilePath=="")
	{
		MSG_BOX_LAN("@GetAnnouncement_"+AnnouncementFileNotFound);
		MsgMainFramePopPage msg;
		msg.pageName = "AnnouncementNewPage";
		MessageManager::Get()->sendMessage(&msg);
	}
	else
	{
		std::string desFile(cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath());
		desFile=desFile+"/"+INTERNAL_ANNOUNCEMENT_FILE_DOWNLOADED;
		remove(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(desFile.c_str()).c_str());
		CurlDownload::Get()->downloadFile(mInternalAnnouncementFilePath,desFile);
	}
}


void AnnouncementNewPage::downloaded( const std::string& url,const std::string& filename )
{
	if(url.find(mInternalAnnouncementFilePath)!=url.npos)
	{
		mDownloadFile=false;
		loadAnnouncementFile(filename,true);
	}
}

void AnnouncementNewPage::downloadFailed( const std::string& url, const std::string& filename )
{
	if(url.find(mInternalAnnouncementFilePath)!=url.npos)
	{
		if(mFileErrCount<=Max_Download_Count)
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

void AnnouncementNewPage::loadAnnouncementFile(const std::string& announcementFile,bool isDownload)
{
	Json::Value root;
	Json::Reader jreader;
	Json::Value data;
	unsigned long filesize;
	char* pBuffer = (char*)getFileData(
		cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(announcementFile.c_str()).c_str(),
		"rt",&filesize,0,false);

	if(!pBuffer)
	{
		_retryShowDownFaildMsgBox(FILE_BUFFER_ERROR);
		CC_SAFE_DELETE_ARRAY(pBuffer);
	}

	bool ret = jreader.parse(pBuffer,data,false);
	if(!ret ||data["announcementConfig"].empty())
	{
		_retryShowDownFaildMsgBox(JSON_FILE_FORMAT_ERROR);
		CC_SAFE_DELETE_ARRAY(pBuffer);
		return;
	}

	Json::Value announcementItems = data["announcementConfig"];
	AnnouncementTableManager::Get()->clearAnnouncementItem();
	for(int i=0;i<announcementItems.size();++i)
	{
		if(	!announcementItems[i]["AnnouncementId"].empty() &&
			!announcementItems[i]["idName"].empty() &&
			!announcementItems[i]["Title"].empty() &&
			!announcementItems[i]["StartTime"].empty() &&
			!announcementItems[i]["EndTime"].empty() &&
			!announcementItems[i]["timeMsg"].empty() &&
			!announcementItems[i]["Msg"].empty() &&
			!announcementItems[i]["IsHaveButton"].empty() &&
			!announcementItems[i]["ButtonText"].empty() &&
			!announcementItems[i]["ButtonGoPage"].empty() &&
			!announcementItems[i]["ButtonType"].empty() &&
			!announcementItems[i]["AdventureId"].empty() &&
			!announcementItems[i]["changeOrPush"].empty() &&
			!announcementItems[i]["picPath"].empty() &&
			!announcementItems[i]["severDelayTime"].empty() &&
			!announcementItems[i]["orderNo"].empty() 
			)
		{
			AnnouncementTableManager::Get()->readInAnnouncementItem(announcementItems[i]);
		}
	}
	mDownloadFile=false;
	if(isDownload)
	{
		cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(INTERNAL_ANNOUNCEMENT_VERSION_KEY.c_str(),SeverConsts::Get()->getInternalAnnouncementVersion());
		cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(INTERNAL_ANNOUNCEMENT_DWONLOAD_TIME_KEY.c_str(),GamePrecedure::Get()->getServerTime());
		cocos2d::CCUserDefault::sharedUserDefault()->flush();
	}
	rebuildAllItem();
}


void AnnouncementNewPage::_retryShowDownFaildMsgBox(AnnouncementError errorCode)
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
		std::string errMsg="@GetAnnouncement_";
		int _tempCode=errorCode;
		libOS::getInstance()->showMessagebox(Language::Get()->getString(errMsg+StringConverter::toString(errorCode))+"["+StringConverter::toString(errorCode)+"]",_tempCode);
	}
}

void AnnouncementNewPage::onMessageboxEnter(int tag)
{
	bool isClose=false;

	if(tag!=DOWNLOADED_MAX&&mFileErrCount<Max_Download_Count)
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
		MsgMainFramePopPage msg;
		msg.pageName = "AnnouncementNewPage";
		MessageManager::Get()->sendMessage(&msg);
	}
}

void AnnouncementNewPage::onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender, int tag )
{
}

void AnnouncementNewPage::onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender )
{
	if (itemName == "onClose")
	{
        bool _showToolBar=StringConverter::parseBool(VaribleManager::getInstance()->getSetting("ShowToolBarStatus"),false);
       // libPlatformManager::getPlatform()->setToolBarVisible(true);
		MsgMainFramePopPage msg;
		msg.pageName = "AnnouncementNewPage";
		MessageManager::Get()->sendMessage(&msg);
	}
}

void AnnouncementNewPage::load( void )
{
	loadCcbiFile("AnnouncementPage.ccbi");
	members = dynamic_cast<cocos2d::extension::CCScrollView*>(getVariable("mAnnMsgContent"));
	memberRootNode = members->getContainer();
}

void AnnouncementNewPage::rebuildAllItem( void )
{
	clearAllItem();
	buildItem();
}

void AnnouncementNewPage::clearAllItem( void )
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

bool _sortAnnoucementOrder(AnnouncementItem* a,AnnouncementItem* b)
{
	return a->order<b->order;
}

void AnnouncementNewPage::buildItem( void )
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
	orderlist.sort(_sortAnnoucementOrder);
	std::list<AnnouncementItem* >::iterator itOrdered = orderlist.begin();
	for(;itOrdered!=orderlist.end();++itOrdered)
	{
		ContentBase* content=NULL;
		content = new AnnouncementNewContent((*itOrdered)->itemId);
		content->init();
		addItem(content);
	}

	if(!isHavAnnouncementNew)
	{
		MsgMainFramePopPage msg;
		msg.pageName = "AnnouncementNewPage";
		MessageManager::Get()->sendMessage(&msg);
	}
}

void AnnouncementNewPage::addItem( ContentBase* item )
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

void AnnouncementNewPage::removeItem( ContentBase* item )
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
