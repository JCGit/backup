#include "ToolUtils.h"

#include "ClientSvrMsg.pb.h"

#include "DebrisCfgMaster.h"

#include "game/notification/Badge.h"

void CToolUtils::ArrayChangeOrder(CCArray* pstArr){
	unsigned int count = pstArr->count();
	if(count <= 1){
		return;
	}

	for(unsigned int i = 0; i < count - 1; i++){
		pstArr->exchangeObjectAtIndex(i, i + 1);
	}
}

bool CToolUtils::checkResContains(CCString* str, CCArray* arr){
	bool isContain = false;
	CCObject* pstObj;
	CCARRAY_FOREACH(arr, pstObj) {
		CCString* pathStr = (CCString*)pstObj;
		if(std::strcmp(pathStr->getCString(), str->getCString()) == 0){
			isContain = true;
			break;
		}
	}

	return isContain;
}



void  CToolUtils::removeArmatureFile(const char* pstrArmatureFile) {
    if(NULL == pstrArmatureFile) {
        return;
    }
    //1. clear texture
    /////////////////
    CCRelativeData *data = CCArmatureDataManager::sharedArmatureDataManager()->getRelativeData(pstrArmatureFile);
    if(NULL == data) {
        CCLOG("[warn]CToolUtils::removeArmatureFile,fail to get data for %s", pstrArmatureFile);
        return;
    }
    string pngFile;
    string pvrFile;
    for (std::vector<std::string>::iterator i = data->plistFiles.begin(); i != data->plistFiles.end(); i++)
    {
        pngFile = *i;
        int pos = pngFile.rfind(".plist");
        if(pos != string::npos) {
            pvrFile = pngFile.substr(0, pos) + ".pvr.ccz";
            pngFile = pngFile.substr(0, pos) + ".png";
        } else {
            continue;
        }
        CCLOG("[trace]CToolUtils::removeArmatureFile,del texture %s", pvrFile.c_str());
        CCTextureCache::sharedTextureCache()->removeTextureForKey(pngFile.c_str());
        CCTextureCache::sharedTextureCache()->removeTextureForKey(pvrFile.c_str());
    }
    /////////////
    /*
    rapidjson::Document json;
    unsigned long size;
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pstrArmatureFile);
    unsigned char *pBytes = CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str() , "r", &size);
    std::string load_str = std::string((const char*)pBytes, size);

    json.Parse<0>(load_str.c_str());

    if (json.HasParseError()) {
        CCLOG("GetParseError %s\n",json.GetParseError());
    } else {
        int length =  DICTOOL->getArrayCount_json(json, "config_file_path"); // json[CONFIG_FILE_PATH].IsNull() ? 0 : json[CONFIG_FILE_PATH].Size();
        for (int i = 0; i < length; i++)
        {
            const char *path = DICTOOL->getStringValueFromArray_json(json, "config_file_path", i); // json[CONFIG_FILE_PATH][i].IsNull() ? NULL : json[CONFIG_FILE_PATH][i].GetString();
            if (path == NULL)
            {
                CCLOG("load CONFIG_FILE_PATH error.");
                break;
            }

            std::string basefilePath(pstrArmatureFile);
            size_t pos = basefilePath.find_last_of("/");
            if (pos != std::string::npos)
            {
                basefilePath = basefilePath.substr(0, pos + 1);
            }
            else
            {
                basefilePath = "";
            }
            basefilePath += path;

            //CCLOG("%s", path);
            //CCLOG("%s", basefilePath.c_str());

            string pngFile = basefilePath;
            int ipos = pngFile.rfind(".plist");
            if(ipos != string::npos) {
                pngFile = pngFile.substr(0, ipos) + ".png";
            } else {
                break;
            }
            CCLOG("[trace]CToolUtils::removeArmatureFile,del texture %s", pngFile.c_str());
            CCTextureCache::sharedTextureCache()->removeTextureForKey(pngFile.c_str());
        }
    }
    CC_SAFE_DELETE_ARRAY(pBytes);
    */


    //2. clear armature
    CCLOG("[trace]CToolUtils::removeArmatureFile,del armature %s", pstrArmatureFile);
    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(pstrArmatureFile);

}


void CToolUtils::removePlistTexFile(const char* pszPlist) {
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszPlist);
    CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());
    if(NULL == dict) {
        CCLOG("[warn]CToolUtils::removePlistTexFile, fail to get file data. %s", pszPlist);
        return;
    }
    string texturePath("");

    CCDictionary* metadataDict = (CCDictionary*)dict->objectForKey("metadata");
    if (metadataDict)
    {
        // try to read  texture file name from meta data
        texturePath = metadataDict->valueForKey("textureFileName")->getCString();
    }
    if (! texturePath.empty())
    {
        // build texture path relative to plist file
        texturePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(texturePath.c_str(), pszPlist);
    }
    else
    {
        // build texture path by replacing file extension
        texturePath = pszPlist;

        // remove .xxx
        size_t startPos = texturePath.find_last_of("."); 
        texturePath = texturePath.erase(startPos);

        // append .png
        texturePath = texturePath.append(".png");

        //CCLOG("cocos2d: CCSpriteFrameCache: Trying to use file %s as texture", texturePath.c_str());
    }
    CCLOG("[trace]CToolUtils::removePlistTexFile,del texture %s", texturePath.c_str());
    CCTextureCache::sharedTextureCache()->removeTextureForKey(texturePath.c_str());
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(pszPlist);
    dict->release();
}


/*
* 解析utf8文本sin的每个字符到ret中
* 返回true表示解析成功
*/
bool CToolUtils::parseUTFStr(const string& sin, vector<string>& ret) {
    int l = sin.length();
    ret.clear();
    for(int p = 0; p < l; ) {
        int size, n = l - p;
        unsigned char c = sin[p], cc = sin[p + 1];
        if(c < 0x80) {
            size = 1;
        } else if(c < 0xc2) {
            return false; //invalid seq
        } else if(c < 0xe0) {
            if(n < 2) {
                return false; //missing seq
            }
            if(!((sin[p + 1] ^ 0x80) < 0x40)) {
                return false; //invalid seq
            }
            size = 2;
        } else if(c < 0xf0) {
            if(n < 3) {
                return false; //missing seq
            }
            if(!((sin[p + 1] ^ 0x80) < 0x40 && 
                (sin[p + 2] ^ 0x80) < 0x40 &&
                (c >= 0xe1 || cc >= 0xa0))) {
                    return false; //invalid seq
            }
            size = 3;
        } else if(c < 0xf8) {
            if(n < 4) {
                return false; //missing seq
            }
            if(!((sin[p + 1] ^ 0x80) < 0x40 &&
                (sin[p + 2] ^ 0x80) < 0x40 &&
                (sin[p + 3] ^ 0x80) < 0x40 &&
                (c >= 0xf1 || cc >= 0x90))) {
                    return false; //invalid seq
            }
            size = 4;
        } else if (c < 0xfc) {
            if(n < 5) {
                return false; //missing seq
            }
            if(!((sin[p + 1] ^ 0x80) < 0x40 &&
                (sin[p + 2] ^ 0x80) < 0x40 &&
                (sin[p + 3] ^ 0x80) < 0x40 && 
                (sin[p + 4] ^ 0x80) < 0x40 &&
                (c >= 0xfd || cc >= 0x88))) {
                    return false; //invalid seq
            }
            size = 5;
        } else if (c < 0xfe) {
            if(n < 6) {
                return false; //missing seq
            }
            if(!((sin[p + 1] ^ 0x80) < 0x40 &&
                (sin[p + 2] ^ 0x80) < 0x40 &&
                (sin[p + 3] ^ 0x80) < 0x40 && 
                (sin[p + 4] ^ 0x80) < 0x40 &&
                (sin[p + 5] ^ 0x80) < 0x40 &&
                (c >= 0xfd || cc >= 0x84))) {
                    return false; //invalid seq
            }
            size = 6;
        } else {
            return false; //invalid seq
        }
        string temp = "";
        temp = sin.substr(p, size);
        ret.push_back(temp);
        p += size;
    }
    return true; //解析成功
}

void CToolUtils::parseUTF8(const string& str, vector<string>& ret){
	int l = str.length();
	ret.clear();
	for(int p = 0; p < l; ) {
		int size;
		unsigned char c = str[p];
		if(c < 0x80) {
			size = 1;
		} else if(c < 0xc2) {
		} else if(c < 0xe0) {
			size = 2;
		} else if(c < 0xf0) {
			size = 3;
		} else if(c < 0xf8) {
			size = 4;
		} else if (c < 0xfc) {
			size = 5;
		} else if (c < 0xfe) {
			size = 6;
		}
		std::string temp = "";
		temp = str.substr(p, size);
		ret.push_back(temp);
		p += size;
	}
}

/**
在中文字符串中间加入换行符。
src 需要转换的中文字符串（utf8格式）
iOneLine 一行最多几个中文

Example:
string strDesc(pCfg->itemdesc());
strDesc = splitChStr(strDesc, 12);

**/
string CToolUtils::splitChStr(const string& src, const int iOneLine) 
{
	string dest("");

	const int iOneLineChar = iOneLine * 3; //一个utf8占3个char

	int iMaxLine = src.size() / iOneLineChar;
	int iPrePos = 0;
	int iNextPos = 0;
	for (int i = 0; i < iMaxLine; ++i)
	{
		iNextPos = iOneLineChar * (i + 1);
		dest = dest + src.substr(iPrePos, iNextPos - iPrePos) + "\n";
		iPrePos = iNextPos;
	}
	dest = dest + src.substr(iPrePos);

	return dest;
}


/**
通关资源名获取资源的类型,如图片资源，动画资源等(.png, .ExportJson)

**/
string CToolUtils::getResType(const std::string& str){
	unsigned int lastIdx = str.find_last_of(".");
	std::string typeRes = str.substr(lastIdx);

	return typeRes;
}

/**
在字符串中间加入换行符。
src 需要转换的字符串（utf8格式）
iOneLine 一行最多几个中文
**/
string CToolUtils::splitUTFStr(const string& sin, const int iOneLine){
	string dest("");

	if(sin.size() == 0){
		return dest;
	}

	vector<string> stVec;
	parseUTFStr(sin, stVec);
	//parseUTF8(sin, stVec);

	for (unsigned int i = 0; i < stVec.size(); ++i){
		if(i != 0 && i%iOneLine == 0){
			dest += "\n";
		}
		dest += stVec.at(i);
	}

	return dest;
}

ccColor3B CToolUtils::parseColorToCC3(const int color){
	GLubyte red = (color>>16)&0xFF;
	GLubyte green = (color>>8)&0xFF;
	GLubyte blue = (color)&0xFF;

	//CCLOG("red is %d, green is %d, blue is %d", red, green, blue);

	ccColor3B c = {red, green, blue};
	return c;
}

string CToolUtils::SecondToStr(int sec){
	string str;
	string minStr;
	string secStr;
	int second = int(sec%60);
	int min = int(sec/60);
	secStr = AddZero(second);
	minStr = AddZero(min);

	str = minStr + ":" + secStr;
	return str;
}

string CToolUtils::AddZero(int time){
	string str;
	char buf[100];
	if(time < 10){
		snprintf(buf, 100, "0%d", time);
	}else{
		snprintf(buf, 100, "%d", time);
	}
	str = buf;
	return str;
}

void CToolUtils::loadArmatureRGBA4444(const string& strJsonPath) {
    CCTexture2DPixelFormat currentFormat = CCTexture2D::defaultAlphaPixelFormat();
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);//强制使用该格式

    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(strJsonPath.c_str());
    CCLOG("[trace]CToolUtils,load RGBA4444 armature %s", strJsonPath.c_str());
    CCTexture2D::setDefaultAlphaPixelFormat(currentFormat); //用完后恢复
}

void CToolUtils::loadSpriteFrameRGBA4444(const string& strPlistPath) {
    CCTexture2DPixelFormat currentFormat = CCTexture2D::defaultAlphaPixelFormat();
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);//强制使用该格式

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(strPlistPath.c_str());
    CCLOG("[trace]CToolUtils,load RGBA4444 sprite %s", strPlistPath.c_str());
    CCTexture2D::setDefaultAlphaPixelFormat(currentFormat); //用完后恢复
}

void CToolUtils::setTimeScale(float scale){
	CCDirector::sharedDirector()->getScheduler()->setTimeScale(scale);
}



bool CToolUtils::adjustIconBgSize(const int iThingType, const int iThingCfgID, UIImageView* pBgImg) {
    int iFinalType = iThingType;
    if(vmsg::THING_TYPE_DEBRIS == iThingType) {
        const DebrisCfg* pDebrisCfg = DEBRIS_CFG_MASTER->GetDebrisCfgByID(iThingCfgID);
        if(pDebrisCfg) {
            iFinalType = pDebrisCfg->composetype();
        }
    }
    bool bIsBigIcon = (vmsg::THING_TYPE_WEAPON == iFinalType);
    if(NULL == pBgImg) {
        return bIsBigIcon;
    }
    if(bIsBigIcon) {
        pBgImg->loadTexture("share/weapon_frame.png", UI_TEX_TYPE_PLIST);
        pBgImg->setSize(CCSize(ccp(180, 100)));
    } else {
        pBgImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
        pBgImg->setSize(CCSize(ccp(100, 100)));
    }
    return bIsBigIcon;
}

void CToolUtils::adjustFragIcon(const int iThingType, UIImageView* pBgImg) {
    if(NULL == pBgImg) {
        return;
    }
    const int TAG_IMG_FRAG = 233;
    
    const bool bIsShowFragIcon = (vmsg::THING_TYPE_DEBRIS == iThingType);
    UIImageView* pFragImg = (UIImageView*)(pBgImg->getChildByTag(TAG_IMG_FRAG));
    if(bIsShowFragIcon){
        if(NULL == pFragImg) {
            pFragImg = UIImageView::create();
            pFragImg->loadTexture("other/icon_frag.png", UI_TEX_TYPE_PLIST);
            pFragImg->setAnchorPoint(ccp(0.0f, 0.0f));
        }
        if(NULL == pFragImg->getParent()) {
            pBgImg->addChild(pFragImg, 15, TAG_IMG_FRAG);
        }

		CCPoint ptZero = pBgImg->getAnchorPoint();
		ptZero.x = -( pBgImg->getSize().width  * ptZero.x) + 10;
		ptZero.y = -( pBgImg->getSize().height * ptZero.y) + 10;
		pFragImg->setPosition(ptZero);
    } else {
        if(pFragImg) {
            pBgImg->removeChild(pFragImg);
        }
    }

}

void CToolUtils::badgeWidget(Widget *pWidget, unsigned int uBadgeNum, float offsetX /* = 0.0f */, float offsetY /* = 0.0f */) {
	if(pWidget != NULL) {
		Badge *pBadge = dynamic_cast<Badge*>(pWidget->getChildByTag(Badge::BADGE_TAG));

		if(uBadgeNum > 0) {
			if(pBadge == NULL) {
				pBadge = Badge::create();
				pBadge->setPosition(ccp(offsetX, offsetY));
				pWidget->addChild(pBadge, pWidget->getChildrenCount(), Badge::BADGE_TAG);
			}

			pBadge->setNum(uBadgeNum);
		} else {
			if(pBadge != NULL) {
				pBadge->removeFromParent();
			}
		}
	}
}

void CToolUtils::badgeWidgetWithPoint(Widget *pWidget, unsigned int uBadgeNum, float offsetX /* = 0.0f */, float offsetY /* = 0.0f */) {
	if(pWidget != NULL) {
		Badge *pBadge = dynamic_cast<Badge*>(pWidget->getChildByTag(Badge::BADGE_TAG));

		if(uBadgeNum > 0) {
			if(pBadge == NULL) {
				pBadge = Badge::create();
				pBadge->setStat(Badge::BADGE_STAT_POINT);
				pBadge->setPosition(ccp(offsetX, offsetY));
				pWidget->addChild(pBadge, pWidget->getChildrenCount(), Badge::BADGE_TAG);
			}
		} else {
			if(pBadge != NULL) {
				pBadge->removeFromParent();
			}
		}
	}
}

void CToolUtils::effectEntryIcon(Widget *pWidget, bool bIsAdd) {
	if(pWidget != NULL) {
		CCArmature *pArmature = dynamic_cast<CCArmature*>(pWidget->getNodeByTag(121));

		if(bIsAdd) {
			if(pArmature == NULL) {
				pArmature = CCArmature::create("ut00085");
				pWidget->addNode(pArmature, 0, 121);
				pArmature->getAnimation()->play("fly", -1, -1, 1);
			}
		} else {
			if(pArmature != NULL) {
				pArmature->removeFromParent();
			}
		}
	}
}
