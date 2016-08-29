//
//  BulletinManager.h
//  Game
//
//  Created by hgs on 9/9/14.
//
//

#pragma once

#include "cocos2d.h"

#include "BulletinBoardPage.h"
#include "CurlDownload.h"
#include "libOS.h"

#include "stdafx.h"


class BulletinManager
    : public BulletinBoardPageListener
    , public CurlDownload::DownloadListener
    , public libOSListener
{
public:
    BulletinManager()
        : m_bIsDownloading(false)
        , m_uTryTimes(0)
    {};
    
    void showBulletin();
    void closeBulletin();
    void update(float dt);
    void doDownload(std::string file);
    void doShow(std::string file);
    
private:
    bool m_bIsDownloading;
    unsigned int m_uTryTimes;
    
    bool needFetchFile();
    void showErrorBox();
    
    void onBtnAction();
    
    void downloaded(const std::string& url, const std::string& filename);
    void downloadFailed(const std::string& url, const std::string& filename);
    
    void onMessageboxEnter(int tag);
};