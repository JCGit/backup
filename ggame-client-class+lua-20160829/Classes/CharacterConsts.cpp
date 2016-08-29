
#include "stdafx.h"

#include "CharacterConsts.h"
#include "cocos2d.h"
#include "json/json.h"
#include "Language.h"
#include "GamePlatform.h"

void CharacterConsts::init( const std::string& filename )
{
	Json::Reader jreader;
	Json::Value data;
	unsigned long filesize;
	char* pBuffer = (char*)getFileData(
		cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filename.c_str()).c_str(),
		"rt",&filesize);

	if(!pBuffer)
	{
		char msg[256];
		sprintf(msg,"Failed open file: %s !!",filename.c_str());
		cocos2d::CCMessageBox(msg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());
	}
	else
	{
		jreader.parse(pBuffer,data,false);
		CC_SAFE_DELETE_ARRAY(pBuffer);

		if(data["version"].asInt()==1)
		{
			Json::Value characterlist = data["characters"];
			if(!characterlist.empty() && characterlist.isArray())
			{
				for(int i = 0;i < characterlist.size();++i)
				{
					CHARACTER * charac  = new CHARACTER;
					Json::Value unit = characterlist[i];
					charac->id = unit["id"].asUInt();
					charac->name = Language::Get()->getString(unit["name"].asString());
					charac->describe = Language::Get()->getString(unit["describe"].asString());
					charac->stars = unit["stars"].asInt();
					charac->bornSkillID = unit["bornSkillID"].asUInt();
					charac->contry = unit["contry"].asInt();
					charac->facePic = unit["facePic"].asString();
					charac->bodyPic = unit["bodyPic"].asString();
					charac->backPic = unit["backPic"].asInt();
					charac->linkPlist = unit["link"].asString();
					Json::Value yuanfen = unit["yuanfen"];
					if(!yuanfen.empty() && yuanfen.isArray())
					{
						for(int i = 0;i < yuanfen.size() && i<CHARACTER::MAX_YUANFEN_COUNT; ++i)
						{
							Json::Value Yunit = yuanfen[i];
							charac->yuanfens[i] = Yunit["id"].asUInt();
						}
					}
					mCharacterList.insert(std::make_pair(charac->id,charac));
				}
			}
			Json::Value YuanfenList = data["yuanfens"];
			if(!YuanfenList.empty() && YuanfenList.isArray())
			{
				for(int i = 0;i < YuanfenList.size();++i)
				{
					YUANFEN * yuan  = new YUANFEN;
					Json::Value unit = YuanfenList[i];
					yuan->id = unit["id"].asUInt();
					yuan->describe = Language::Get()->getString(unit["describe"].asString());
					mYuanfenList.insert(std::make_pair(yuan->id,yuan));
				}
			}

		}
	}
	CC_SAFE_DELETE_ARRAY(pBuffer);
}

const CHARACTER* CharacterConsts::getCharacter( unsigned int id )
{
	CHARACTER_LIST::iterator it = mCharacterList.find(id);
	if(it == mCharacterList.end())
		return 0;

	return it->second;
}

const YUANFEN* CharacterConsts::getYuanfen( unsigned int id )
{
	YUANFEN_LIST::iterator it = mYuanfenList.find(id);
	if(it == mYuanfenList.end())
		return 0;

	return it->second;
}

void CharacterConsts::cleanUp()
{
	CHARACTER_LIST::iterator itCha = mCharacterList.begin();
	for(;itCha!=mCharacterList.end();++itCha)
	{
		delete itCha->second;
	}
	mCharacterList.clear();

	YUANFEN_LIST::iterator itYuan = mYuanfenList.begin();
	for(;itYuan!=mYuanfenList.end();++itYuan)
	{
		delete itYuan->second;
	}
	mYuanfenList.clear();
}
