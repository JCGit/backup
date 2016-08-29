//
//  BulletinManager.cpp
//  Game
//
//  Created by hgs on 9/9/14.
//
//

#include "BulletinManager.h"
#include "cocos2d.h"
#include "platform/CCFileUtils.h"
#include "SeverConsts.h"
#include "DataTableManager.h"
#include "libPlatform.h"
#include "GamePlatform.h"
#include "GamePrecedure.h"
#include "Language.h"

using namespace cocos2d;

#define BULLETIN_FILE_NAME "notice.html"
#define BULLETIN_VERSION_KEY "BulletinVersion"
#define BULLETIN_DOWNLOADTIME_KEY "BulletinDownLoadTime"

bool BulletinManager::needFetchFile()
{
    const std::string kVersion(BULLETIN_VERSION_KEY);
    int localVersion = CCUserDefault::sharedUserDefault()->getIntegerForKey(kVersion.c_str(), 0);
    if ( localVersion < SeverConsts::Get()->getInternalAnnouncementVersion() )
    {
        return true;
    }
    
    const std::string kDownloadTime(BULLETIN_DOWNLOADTIME_KEY);
    int downloadTime = CCUserDefault::sharedUserDefault()->getIntegerForKey(kDownloadTime.c_str(), 0);
    int offset = GamePrecedure::Get()->getServerTime() - downloadTime;
    const int maxOffset = StringConverter::parseInt(VaribleManager::Get()->getSetting("FetchBulletinOffset"), 1800);//unit: second
    if ( offset > maxOffset )
    {
        return true;
    }
    
    return false;
}

void BulletinManager::onBtnAction()
{
    GamePrecedure::Get()->closeBulletin();
}

void BulletinManager::doDownload(std::string file)
{
    std::string targetFile = SeverConsts::Get()->getAnnouncement();
    if ( targetFile.empty() )
    {
        GamePrecedure::Get()->closeBulletin();
        return;
    }
    CurlDownload::Get()->addListener((CurlDownload::DownloadListener*)this);
    CurlDownload::Get()->downloadFile(targetFile, file);
    m_bIsDownloading = true;
}

void BulletinManager::doShow(std::string file)
{
    BulletinBoardPage* page = BulletinBoardPage::getInstance();
    page->init(120, 70, 300, 500, (BulletinBoardPageListener*)this);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    unsigned long filesize;
    char* pBuffer = (char*)getFileData(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(file.c_str()).c_str(), "rt", &filesize, 0, false);
    if ( pBuffer )
    {
        page->showWithContent(pBuffer);
    }
    CC_SAFE_DELETE_ARRAY(pBuffer);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    page->show("content://" + file);
#else
    page->show(file);
#endif
}

void BulletinManager::showBulletin()
{
#ifdef WIN32
    const bool _webkitOpenStatus = false;
#else
    const PlatformRoleItem* item = PlatformRoleTableManager::Get()->getPlatformRoleByName(libPlatformManager::getPlatform()->getClientChannel());
    const bool _webkitOpenStatus = item ? item->webkitOpenStatus : false;
#endif
        
    if( _webkitOpenStatus )
    {
        const std::string file(CCFileUtils::sharedFileUtils()->getWritablePath() + BULLETIN_FILE_NAME);
        
        if ( !CCFileUtils::sharedFileUtils()->isFileExist(file) || needFetchFile() )
        {
            doDownload(file);
            return;
        }
        
        doShow(file);
    }
}

void BulletinManager::closeBulletin()
{
    BulletinBoardPage::getInstance()->close();
    CurlDownload::Get()->removeListener(this);
    libOS::getInstance()->removeListener(this);
    m_bIsDownloading = false;
    m_uTryTimes = 0;
}

void BulletinManager::update(float dt)
{
    if ( m_bIsDownloading )
    {
        CurlDownload::Get()->update(dt);
    }
}

void BulletinManager::downloaded(const std::string &url, const std::string &filename)
{
    if ( url.find(SeverConsts::Get()->getAnnouncement()) != std::string::npos )
    {
        m_bIsDownloading = false;
        doShow(filename);
        
        CCUserDefault* pUserDefault = CCUserDefault::sharedUserDefault();
        const std::string kVersion(BULLETIN_VERSION_KEY);
        pUserDefault->setIntegerForKey(kVersion.c_str(), SeverConsts::Get()->getInternalAnnouncementVersion());
        
        const std::string kDownloadTime(BULLETIN_DOWNLOADTIME_KEY);
        pUserDefault->setIntegerForKey(kDownloadTime.c_str(), GamePrecedure::Get()->getServerTime());
        
        pUserDefault->flush();
    }
}


void BulletinManager::downloadFailed(const std::string &url, const std::string &filename)
{
    if ( url.find(SeverConsts::Get()->getAnnouncement()) != std::string::npos )
    {
        m_bIsDownloading = false;
        const unsigned int maxTryTimes = StringConverter::parseUnsignedInt(VaribleManager::Get()->getSetting("MaxFetchBulletinTimes"), 1);
        if ( m_uTryTimes < maxTryTimes )
        {
            doDownload(filename);
            ++m_uTryTimes;
        }
        else
        {
            showErrorBox();
        }
    }
}

void BulletinManager::showErrorBox()
{
    libOS* pLibOs = libOS::getInstance();
    std::string errKey = pLibOs->getNetWork() == NotReachable ? "@NoNetWork" : "@FailFetchBulletin";
    pLibOs->showMessagebox(Language::Get()->getString(errKey));
}

void BulletinManager::onMessageboxEnter(int tag)
{
    GamePrecedure::Get()->closeBulletin();
}
