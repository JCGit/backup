
#include "stdafx.h"

#include "ResManager.h"
#include "DataTableManager.h"
#include "Language.h"
#include "GameMaths.h"
#include "MessageBoxPage.h"
#include "BlackBoard.h"


ResInfo* ResManager::getResInfoByTypeAndId(unsigned int type,unsigned int itemId,unsigned int count)
{	
	static ResInfo s_info;
	s_info.count = count;
	s_info.itemId = itemId;
	s_info.type = type;
	setResInfo(&s_info);
	return &s_info;
}
void ResManager::setResInfo(ResInfo* info)
{
	const unsigned int resType = getResMainType(info->type);
	const ResPropertyItem* _item=ResPropertyTableManager::Get()->getResPropertyItemByID(info->type);
	if(_item)
	{
		info->typeName=_item->name;
	}
	else
	{
		info->typeName="";
	}
	char errmsg[256] = "";
	bool bUnKnownType = false;
	switch(resType)
	{
	case TOOLS_TYPE://道具&礼物
		{
			const ToolItem * toolItem = ToolTableManager::Get()->getToolItemByID(info->itemId);
			if (toolItem)
			{
				info->name = toolItem->name;
				info->describe = toolItem->describe;
				info->icon = toolItem->iconPic;
				info->quality = toolItem->quality;
			}
			else
			{
				info->name = "ToolsId not exists!";
				info->describe = "none";
				info->icon = "empty.png";
				info->quality = 4;
				
				sprintf(errmsg, "%s(ToolItem%d)", Language::Get()->getString("@ResNotExistError").c_str(), info->itemId);
			}
		}
		break;
	case TREASURE_TYPE://宝物
		{
			const TreasureItem* treasureItem = TreasureTableManager::Get()->getTreasureItemByID(info->itemId);
			if (treasureItem)
			{
				info->name = treasureItem->name;
				info->describe = treasureItem->describe;	
				info->icon = treasureItem->iconPic;
				info->quality = treasureItem->quality;
			}
			else
			{
				info->name = "TreasureId not exists!";
				info->describe = "none";
				info->icon = "empty.png";
				info->quality = 4;
				sprintf(errmsg, "%s(treasureItem%d)", Language::Get()->getString("@ResNotExistError").c_str(), info->itemId);
			}
		}
		break;
	case EQUIP_BOOK_TYPE://装备碎片
		{
			const FragmentBookTable::FragmentItem* _fragmentBookItem=FragmentBookTableManager::Get()->getFragmentItemByFragmentId(info->itemId);
			if(_fragmentBookItem)
			{
				const EquipItem* _item = EquipTableManager::Get()->getEquipItemByID(_fragmentBookItem->itemId);
				if (_item)
				{
					//info->name = _item->name+Language::Get()->getString("@FragmentPiece");
					info->name = _fragmentBookItem->name;
					info->describe = _item->describe;
					info->icon = _item->iconPic;
					info->quality = _item->quality;
				}
				else
				{
					info->name = "Equipbook:EquipId not exists!";
					info->describe = "none";
					info->icon = "empty.png";
					info->quality = 4;
					sprintf(errmsg, "%s(fragmentItem%d)", Language::Get()->getString("@ResNotExistError").c_str(), info->itemId);
				}
			}
			else
			{
				info->name = "Equipbook:fragmentId not exists ";
				info->describe = "none";
				info->icon = "empty.png";
				info->quality = 4;
				sprintf(errmsg, "%s(fragmentItem%d)", Language::Get()->getString("@ResNotExistError").c_str(), info->itemId);
			}
		}
		break;
	case TREASURE_BOOK_TYPE://宝物碎片
		{
			const FragmentBookTable::FragmentItem* _fragmentBookItem=FragmentBookTableManager::Get()->getFragmentItemByFragmentId(info->itemId);
			if(_fragmentBookItem)
			{
				const TreasureItem* _item = TreasureTableManager::Get()->getTreasureItemByID(_fragmentBookItem->itemId);
				if (_item)
				{
					//info->name = _item->name+Language::Get()->getString("@FragmentPiece");
					info->name = _fragmentBookItem->name;
					info->describe = _item->describe;
					info->icon = _item->iconPic;
					info->quality = _item->quality;
				}
				else
				{
					info->name = "TreasureBook:TreasureId not exists!";
					info->describe = "none";
					info->icon = "empty.png";
					info->quality = 4;
					sprintf(errmsg, "%s(treasureBook%d)", Language::Get()->getString("@ResNotExistError").c_str(), info->itemId);
				}
			}
			else
			{
				info->name = "TreasureBook:fragmentId not exists ";
				info->describe = "none";
				info->icon = "empty.png";
				info->quality = 4;
				sprintf(errmsg, "%s(treasureBook%d)", Language::Get()->getString("@ResNotExistError").c_str(), info->itemId);
			}
		}
		break;
	case EQUIP_TYPE://装备
		{
			const EquipItem* equipItem = EquipTableManager::Get()->getEquipItemByID(info->itemId);
			if(equipItem)
			{
				info->name = equipItem->name;
				info->describe = equipItem->describe;
				info->icon = equipItem->iconPic;
				info->quality = equipItem->quality;
			}
			else
			{
				info->name = "equip not exists ";
				info->describe = "none";
				info->icon = "empty.png";
				info->quality = 4;
				sprintf(errmsg,"equip Id:%d is not exist!",info->itemId);
			}
		}

		break;
	case DISCIPLE_BOOK://武魂
		{
			const DiscipleItem* soulItem = DiscipleTableManager::Get()->getDiscipleItemByID(info->itemId);
			if (soulItem)
			{
				info->name = soulItem->name+Language::Get()->getString("@MPTabSoul");
				info->describe = soulItem->describe;
				info->icon = soulItem->iconPic;
				info->quality = soulItem->quality;
			}
			else
			{
				info->name = "disciple not exists ";
				info->describe = "none";
				info->icon = "empty.png";
				info->quality = 4;
				sprintf(errmsg, "%s(DiscipleSoulItem%d)", Language::Get()->getString("@ResNotExistError").c_str(), info->itemId);
			}
		}
		break;
	case DISCIPLE_TYPE://武将
		{
			const DiscipleItem* discipleItem = DiscipleTableManager::Get()->getDiscipleItemByID(info->itemId);
			if (discipleItem)
			{
				info->name = discipleItem->name;
				info->describe = discipleItem->describe;
				info->icon = discipleItem->iconPic;
				info->quality = discipleItem->quality;
			}
			else
			{
				info->name = "disciple not exists ";
				info->describe = "none";
				info->icon = "empty.png";
				info->quality = 4;
				sprintf(errmsg, "%s(DiscipleItem%d)", Language::Get()->getString("@ResNotExistError").c_str(), info->itemId);
			}
		}
		break;
	case USER_PROPERTY_TYPE://用户属性
		{
			const UserPropertyItem* userPropertyItem = UserPropertyTableManager::Get()->getUserPropertyItemByID(info->itemId);
			if (userPropertyItem)
			{
				info->name = userPropertyItem->name;
				info->describe = userPropertyItem->describe;
				info->icon = userPropertyItem->iconPic;
				info->quality = userPropertyItem->quality;
				info->typeName=userPropertyItem->name;
			}
			else
			{
				info->name = "Use_Property not exists ";
				info->describe = "none";
				info->icon = "empty.png";
				info->quality = 4;
				sprintf(errmsg, "%s(UserPropertyItem%d)", Language::Get()->getString("@ResNotExistError").c_str(), info->itemId);

			}
		}
		break;
	case DRAGONSOUL_TYPE:
	default:
		info->name = "GOODS_TYPE Not Exists ";
		info->describe = "none";
		info->icon = "empty.png";
		info->quality = 4;
		bUnKnownType = true;
		sprintf(errmsg, "%s(unknown Type%d)", Language::Get()->getString("@ResNotExistError").c_str(), info->itemId);
		break;
	}

	int errorLength = strlen(errmsg);

	if (errorLength>0)
	{
		//与正芳确定过，如为A级武魂包，有infoType,itemId = 0,认为是正确的配置，不报错，否则提示配置有问题
		if(!bUnKnownType&&info->itemId ==0){
			CCLOG("@ResManager::setResInfo --It's a random pack for a specific tool type");
		}else{
			CCLOG("ResManager::setResInfo :%s",errmsg);
#if defined _DEBUG && defined _WIN32
			MSG_BOX(errmsg);
#endif
		}
		
	}
	


}
ResInfoMap ResManager::getResInfoListByResBagStr(const std::string& resBagStr)
{
	ResInfoMap resInfoList;
	std::vector<std::string> resItemsVec = GameMaths::tokenize(resBagStr,",");
	int len = resItemsVec.size();
	for(int i=0;i<len;i++)
	{
		std::vector<std::string> itemVec = GameMaths::tokenize(resItemsVec.at(i),"_");
		ResInfo* info = new ResInfo;
		if(itemVec.size()==3)
		{
			info->type = atoi(itemVec.at(0).c_str());
			info->itemId = atoi(itemVec.at(1).c_str());
			info->count = atoi(itemVec.at(2).c_str());
		}
		
		info->id = i+1;
		setResInfo(info);
		resInfoList.insert(ResInfoMap::value_type(i+1,info));
	}

	return resInfoList;
	
}

const unsigned int ResManager::getTreasurePairType(unsigned int treasureId)
{
	if(mPairList.find(treasureId)!=mPairList.end())
	{
		return mPairList.find(treasureId)->second;
	}
	return 0;
}

const unsigned int ResManager::getResMainType(unsigned int resId)
{
	const ResPropertyItem* _item=ResPropertyTableManager::Get()->getResPropertyItemByID(resId);
	if(_item)
	{
		return _item->targetTypeId;
	}
	else
	{
		if(resId>10000)
		{
			resId=resId/10000;
		}
	}
	return resId;
};
//
//void ResManager::useToolByIDAndCount(unsigned int toolId,unsigned int toolItemId,unsigned int toolCount)
//{
//	BlackBoard::Get()->setUseToolId(toolItemId);
//	BlackBoard::Get()->setUseToolCount(toolCount);
//	OPUseTool useTool;
//	useTool.set_id(toolItemId);
//	useTool.set_count(toolCount);
//	PacketManager::Get()->sendPakcet(OPCODE_USE_TOOL_C,&useTool);
//}

int ResManager::getResInfoListByResBagStrCount( const std::string& ResBagStr )
{
	ResInfoMap map = getResInfoListByResBagStr(ResBagStr);
	return map.size();
}

ResInfo ResManager::getResInfoListByResBagStrByIndex( const std::string& ResBagStr,int index )
{
	ResInfoMap map = getResInfoListByResBagStr(ResBagStr);
	ResInfoMap::iterator it =map.begin();
	for(int i=0;i<index;++i)
		it++;
	return (*it->second);
}
//
//bool ResManager::isSatisfiedWithResBagStr(const std::string& resBagStr)
//{
//	ResInfoMap resInfoMap = getResInfoListByResBagStr(resBagStr);
//	if ( resInfoMap.size() <= 0 ) 
//		return true;
//
//	for( ResInfoMap::iterator itr = resInfoMap.begin(); itr != resInfoMap.end(); ++ itr )
//	{
//		if ( !itr->second )
//			continue;
//		switch ( getResMainType(itr->second->type) )
//		{
//		case TOOLS_TYPE:
//			UserToolInfo* userToolInfo = ServerDateManager::Get()->getUserToolInfoByItemId(itr->second->itemId);
//			if ( !userToolInfo || userToolInfo->count < itr->second->count )
//				return false;
//			break;
//		}
//	}
//	return true;
//}

bool ResManager::setDynamicIconTex(CCBContainer* containner, std::string frameName ,std::string iconName , std::string iconStr)
{
	//获取外边框scale
	CCObject* frameObj=containner->getVariable(frameName);
	if(!frameObj)
	{
		char errMsg[256];
		sprintf(errMsg,"Failed to find frame variable:%s",frameName.c_str());
		CCMessageBox(errMsg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());
		return false;
	}
	cocos2d::CCMenuItemImage* frameMenu=dynamic_cast<cocos2d::CCMenuItemImage* >(frameObj);
	float frameScale =0.0f;
	bool isHavReference=true;
	if(frameMenu)
	{
		frameScale = frameMenu->getScaleX();
	}
	else
	{
		cocos2d::CCSprite* frameSprite=dynamic_cast<cocos2d::CCSprite* >(frameObj);
		if(frameSprite)
		{
			frameScale=frameSprite->getScaleX();
		}
	}
	if(frameScale==0.0f)
	{
		return false;
	}
	//获取纹理图片的宽，目前以宽度为统一标杆处理
	float fTextureWidth = 80.0f;

	CCSprite* texSprite = CCSprite::create(iconStr.c_str());
	if (texSprite)
	{
		CCRect rect = CCRectZero;
		rect.size = texSprite->getContentSize();
		fTextureWidth = rect.size.width;
	}else{
		CCLOG("@ResManager::setDynamicIconTex --- texSprite create failed.");
		return false;
	}

	//获取icon物体
	CCObject* iconObj=containner->getVariable(iconName);
	if(!iconObj)
	{
		char errMsg[256];
		sprintf(errMsg,"Failed to find icon variable:%s",iconName.c_str());
		CCMessageBox(errMsg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());
		return false;
	}

	float innerFrameWidth = 81.0f;//当前所有显示ICON的内边框都为81.0，且为正方形
	cocos2d::CCSprite* iconSprite=dynamic_cast<cocos2d::CCSprite* >(iconObj);
	if(iconSprite)
	{
		//外边框缩放的scale（frameScale）同步到内部显示物体上
		iconSprite->setTexture(iconStr.c_str());
		float iconScale = innerFrameWidth*frameScale / fTextureWidth;
		iconSprite->setScale(iconScale);
	}
	return true;
}

void ResManager::setBigFrameTextureByQuality(CCBContainer* container, std::string frameName, int quality)
{
	if (quality > 5 || quality < 1)
	{
		quality = 4;
	}
	std::string picDir = VaribleManager::Get()->getSetting("Card_ImageFile" + StringConverter::toString(quality));
	cocos2d::CCSprite* normalSprite = cocos2d::CCSprite::create(picDir.c_str());
	CCMenuItemImage* mFrame = dynamic_cast<CCMenuItemImage*>(container->getVariable(frameName));
	if (mFrame)
	{
		mFrame->setNormalImage(normalSprite);
		mFrame->setSelectedImage(normalSprite);
	}else
	{
		CCSprite* mFrameSprite = dynamic_cast<CCSprite*>(container->getVariable(frameName));
		if (mFrameSprite)
		{
			mFrameSprite->setTexture(picDir.c_str());
		}else
		{
			char errMsg[256];
			sprintf(errMsg, "%s%s", "Failed to find the frame variable:", frameName.c_str());
			CCLOG("%s Failed to find variable:",frameName.c_str());
		}
	}
}

void ResManager::setFrameColorByQuality(CCBContainer* containner, std::string frameName ,int quality)
{
	//品质为1-5，若超出则设定为4，D品质
	if(quality>5||quality<1)
	{
		quality=4;
	}

	//根据品质获取某种品质的边框图片路径
	std::string concastString = VaribleManager::Get()->getSetting("FrameNormal_ImageFile"+ StringConverter::toString(quality));
	cocos2d::CCSprite* NormalSprite = cocos2d::CCSprite::create(concastString.c_str());
	CCMenuItemImage* mFrame = dynamic_cast<CCMenuItemImage* >(containner->getVariable(frameName));
	if(mFrame)
	{
		mFrame->setNormalImage(NormalSprite);
		//mFrame->setSelectedImage(NormalSprite);只要有normal，selected就没用
	}
	else
	{
		CCSprite* mFrameSprite=dynamic_cast<CCSprite* >(containner->getVariable(frameName));
		if(mFrameSprite)
		{
			mFrameSprite->setTexture(concastString.c_str());
			//mFrameSprite->setColor(StringConverter::parseColor3B(color));
		}else{
			char errMsg[256];
			sprintf(errMsg,"%s%s","Failed to find the frame variable:",frameName.c_str());
			CCLOG("%s Failed to find variable:",frameName.c_str());
#ifdef _DEBUG
			//CCMessageBox(errMsg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());
#endif
		}
	}
	
}

void ResManager::setTabLabelAdapte(CCMenuItemImage* menuItemImage1, CCMenuItemImage* menuItemImage2, CCMenuItemImage* menuItemImage3, CCMenuItemImage* menuItemImage4, CCMenuItemImage* menuItemImage5, int index)
{
	std::map<unsigned int, CCMenuItemImage*> menuItemMap;
	menuItemMap[1] = menuItemImage1;
	menuItemMap[2] = menuItemImage2;
	menuItemMap[3] = menuItemImage3;
	menuItemMap[4] = menuItemImage4;
	menuItemMap[5] = menuItemImage5;
	setTabLabelAdapte(menuItemMap, index);
	menuItemMap.clear();
}

//设置页面tab一致性的适配，动态设置非选中tab的显示状态。add by xinghui
void ResManager::setTabLabelAdapte(std::map<unsigned int,CCMenuItemImage*> menuItemImageMap, int index)
{
	if (!menuItemImageMap.empty())
	{
		std::map<unsigned int, CCMenuItemImage*>::iterator itr = menuItemImageMap.begin();
		for (;itr!=menuItemImageMap.end();++itr)
		{
			CCMenuItemImage* pMenuItem = itr->second;
			if (pMenuItem != NULL)
			{
				if (itr->first < index)
				{
					pMenuItem->unselected();
					cocos2d::CCNode* pDisableImage = pMenuItem->getDisabledImage();
					cocos2d::CCNode* pNormalImage = pMenuItem->getNormalImage();
					if (pDisableImage != NULL && pNormalImage!=NULL)
					{
						pDisableImage->setVisible(true);
						pNormalImage->setVisible(false);
					}
					setTabLabelUnselected(pMenuItem);
				}else if (itr->first == index)
				{
					pMenuItem->selected();
					setTabLabelSelected(pMenuItem);
				}else
				{
					pMenuItem->unselected();
					setTabLabelUnselected(pMenuItem);
				}
			}
		}
	}
}

void ResManager::setTabLabelSelected(CCMenuItemImage* tabImage){
	//CCNode* tabMenuImage =dynamic_cast<CCNode*>(container->getVariable(tabName));
	if (!tabImage)
	{
		return;
	}
	

	CCNode * pParentNode = tabImage->getParent();
	if (pParentNode)
	{
		CCNode * pAncestorNode = pParentNode->getParent();
		if (pAncestorNode)
		{
			CCLabelTTF *labelNormalNode = dynamic_cast<CCLabelTTF*>(pAncestorNode->getChildByTag(CCB_TAB_TTF_UNSELECT_TAG));
			if (labelNormalNode)
			{
				labelNormalNode->setVisible(false);
			}
			
			CCLabelTTF *labelSelNode = dynamic_cast<CCLabelTTF*>(pAncestorNode->getChildByTag(CCB_TAB_TTF_SELECT_TAG));
			if (labelSelNode)
			{
				labelSelNode->setVisible(true);
			}
		}
	}
}


void ResManager::setTabLabelUnselected(CCMenuItemImage* tabImage){
	//CCNode* tabMenuImage =dynamic_cast<CCNode*>(container->getVariable(tabName));
	if (!tabImage)
	{
		return;
	}
	
	CCNode * pParentNode = tabImage->getParent();
	if (pParentNode)
	{
		CCNode * pAncestorNode = pParentNode->getParent();
		if (pAncestorNode)
		{
			CCLabelTTF *labelNormalNode = dynamic_cast<CCLabelTTF*>(pAncestorNode->getChildByTag(CCB_TAB_TTF_UNSELECT_TAG));
			if (labelNormalNode)
			{
				labelNormalNode->setVisible(true);
			}

			CCLabelTTF *labelSelNode = dynamic_cast<CCLabelTTF*>(pAncestorNode->getChildByTag(CCB_TAB_TTF_SELECT_TAG));
			if (labelSelNode)
			{
				labelSelNode->setVisible(false);
			}
		}
	}
}


void ResManager::setLabelColorByQuality(CCBContainer* containner, std::string labelName ,int quality)
{
	//品质为1-5，若超出则设定为4，D品质
	if(quality>5||quality<1)
	{
		quality=4;
	}
	std::string color = VaribleManager::Get()->getSetting("LabelColor_Quality" + StringConverter::toString(quality));
	CCB_FUNC(containner,labelName,CCLabelTTF,setColor(StringConverter::parseColor3B(color)));
	CCB_FUNC(containner,labelName,CCLabelBMFont,setColor(StringConverter::parseColor3B(color)));
}


#ifdef WIN32

void ResManager::checkCCBIResource(){
	////仅用于WIN32下测试所有ccbi用到的纹理资源add by zhenhui, 2014_1_3
	//遍历ccbi目录下所有的ccbi文件，打印对应ccbi用到的纹理资源日志，用来检查缺图，重名，纹理资源交叉引用等资源问题。
	char* foldname = "ccbi\\*";
	HANDLE file;
	WIN32_FIND_DATA fileData;
	char line[1024];
	wchar_t fn[1000];
	int fileCount = 0;
	mbstowcs(fn,(const char*)foldname,999);
	file = FindFirstFile(fn, &fileData);
	FindNextFile(file, &fileData);
	while(FindNextFile(file, &fileData)){
		wcstombs(line,(const wchar_t*)fileData.cFileName,259);
		if (strstr(line,".ccbi"))
		{
			CCBContainer* ccb = CCBManager::Get()->createAndLoad(line);
			//CCTextureCache::sharedTextureCache()->removeAllTextures();
			fileCount++;
		}
	}
	CCLog("@GamePrecedure: preload all ccbi %d",fileCount );
}

#endif

//
//void ResManager::refreshUsrInfoBanner(CCBContainer* container){
//	UserBasicInfo userInfo = ServerDateManager::Get()->getUserBasicInfo();
//	//Vip pic
//	CCSprite* vipLevel=dynamic_cast<CCSprite* >(container->getVariable("mVipPic"));
//	if(vipLevel)
//	{
//		const std::string vipImage = ::getVipLevelImageFile(userInfo.viplevel);
//		if(vipImage!="")
//		{
//			vipLevel->setVisible(true);
//			vipLevel->initWithFile(vipImage.c_str());
//		}else
//		{
//			vipLevel->setVisible(false);
//		}
//	}
//
//
//	if (vipLevel)
//	{				
//		unsigned int defaultx = 282;
//		int x = defaultx + vipLevel->getContentSize().width * .5f;
//		vipLevel->setPositionX(x);
//	}
//
//	CCB_FUNC(container, "mPlayerLv", CCLabelBMFont, setString(StringConverter::toString(userInfo.level).c_str()));
//	CCB_FUNC(container, "mPlayerName", CCLabelTTF, setString(userInfo.name.c_str()));
//	CCB_FUNC(container, "mCoin", CCLabelBMFont, setString(StringConverter::toString(userInfo.silvercoins).c_str()));
//	CCB_FUNC(container, "mGold", CCLabelBMFont, setString(StringConverter::toString(userInfo.goldcoins).c_str()));
//}
//
//
//void ResManager::refreshTopBanner(CCBContainer* container){
//	const UserBasicInfo& userBasicInfo = ServerDateManager::Get()->getUserBasicInfo();
//
//	//名字等级
//	{
//		UserBattleInfo * userBattleInfo = ServerDateManager::Get()->getUserBattleInfoByIndex(0);
//		if (userBattleInfo)
//		{
//			int iDiscipleID =  userBattleInfo->discipleId;
//			Disciple disciple(userBattleInfo->discipleId);
//			//头像边框
//			CCB_SET_FRAME_TEX(container,"mTopFace",disciple.quality());
//			//头像
//			CCSprite * headPic = dynamic_cast<CCSprite* >(container->getVariable("mPlayerInfo"));
//			if (headPic)
//			{
//
//				if (userBattleInfo)
//				{
//
//					if(disciple.iconPic()!="")
//					{
//						headPic->setVisible(true);
//						headPic->initWithFile(disciple.iconPic().c_str());
//					}else
//					{
//						headPic->setVisible(false);
//					}
//				}
//			}
//		}
//
//
//
//
//		CCLabelTTF* name=dynamic_cast<CCLabelTTF* >(container->getVariable("mName"));
//		if(name)
//		{
//			name->setString(userBasicInfo.name.c_str());
//		}
//		CCLabelTTF* exp = dynamic_cast<CCLabelTTF* >(container->getVariable("mExp"));
//		if(exp)
//		{
//			exp->setString(ServerDateManager::Get()->getUserExpPercentStr().c_str());
//		}
//		CCLabelBMFont* gold = dynamic_cast<CCLabelBMFont* >(container->getVariable("mGold"));
//		if(gold)
//		{
//			gold->setString(StringConverter::toString(userBasicInfo.goldcoins).c_str());
//		}
//		CCLabelBMFont* coin = dynamic_cast<CCLabelBMFont* >(container->getVariable("mCoin"));
//		if(coin)
//		{
//			coin->setString(StringConverter::toString(userBasicInfo.silvercoins).c_str());
//		}
//
//		CCLabelBMFont* lv = dynamic_cast<CCLabelBMFont*>(container->getVariable("mLV"));
//		if (lv)
//		{
//			lv->setString(StringConverter::toString(userBasicInfo.level).c_str());
//		}
//
//		CCSprite* vipLevel=dynamic_cast<CCSprite* >(container->getVariable("mVipPic"));
//		if(vipLevel)
//		{
//			const std::string vipImage = ::getVipLevelImageFile(userBasicInfo.viplevel);
//			if(vipImage!="")
//			{
//				vipLevel->setVisible(true);
//				vipLevel->initWithFile(vipImage.c_str());
//			}else
//			{
//				vipLevel->setVisible(false);
//			}
//		}
//
//		//战斗力
//		CCLabelBMFont* fightVal = dynamic_cast<CCLabelBMFont*>(container->getVariable("mFightValue"));
//		if (fightVal)
//		{
//			fightVal->setString(StringConverter::toString(ServerDateManager::Get()->getUserBasicInfo().fightValue).c_str());
//		}
//	}
//
//	//经验条
//	{
//		float fExpbarInitScale =1.0f;
//		cocos2d::extension::CCScale9Sprite* expbar=dynamic_cast<cocos2d::extension::CCScale9Sprite* >(container->getVariable("mExpBar"));
//		if(expbar)
//		{
//			float expscale = 0.0f;
//			if(ServerDateManager::Get()->getNextLevelExp()>0)
//				expscale = ServerDateManager::Get()->getUserExpPercent();
//			if(expscale>1.0f)expscale=1.0f;
//			expbar->setScaleX(expscale*fExpbarInitScale);
//		}
//
//		/*CCSprite* phyBar = dynamic_cast<CCSprite*>(container->getVariable("mPhysicalBar"));
//		if (phyBar)
//		{
//		float scale = ServerDateManager::Get()->getUserTiPercent();
//		phyBar->setScaleX(scale*fExpbarInitScale);
//		}
//
//		CCSprite* vatiBar = dynamic_cast<CCSprite*>(container->getVariable("mStaminaBar"));
//		if (vatiBar)
//		{
//		float scale = ServerDateManager::Get()->getUserVitalityPercent();
//		vatiBar->setScaleX(scale*fExpbarInitScale);
//		}*/
//
//		CCLabelBMFont* ti = dynamic_cast<CCLabelBMFont*>(container->getVariable("mTi"));
//		if (ti)
//		{
//			ti->setString(ServerDateManager::Get()->getUserTiPercentStr().c_str());
//		}
//
//		CCLabelBMFont* vati = dynamic_cast<CCLabelBMFont*>(container->getVariable("mQi"));
//		if (vati)
//		{
//			vati->setString(ServerDateManager::Get()->getUserVitatityStr().c_str());
//		}
//	}
//
//}


//由于StringConvert在Core里面，需要定义属性的两套枚举，所以此处直接用数字
//
/*
*10	体质
*11	武力
*12	护甲
*13	生命值
*14	攻击
*15	物防
*16	法防
*17	生命值
*18	攻击
*19	物防
*20	法防
*21	命中率
*22	闪避率
*23	暴击率
*24	暴伤加成率
*25	抗暴率
*26	格挡率
*27	破击率
*28	额外伤害
29	伤害减免
30	额外伤害
31	伤害减免
32	治疗增幅
33	受治疗增幅
*/
std::string ResManager::attrToString(unsigned int type,unsigned int val)
{
	switch (type)
	{
	case 10:
		{
			return Language::Get()->getString("@Physique") + "+" + StringConverter::toString(val);	
		}
		break;
	case 11:
		{
			return Language::Get()->getString("@Force") + "+" + StringConverter::toString(val);	
		}
		break;
	case 12:
		{
			return Language::Get()->getString("@ArmorInfo") + "+" + StringConverter::toString(val);	
		}
		break;
	case 13:
		{
			return Language::Get()->getString("@MIHP") + "+" + StringConverter::toString(val);
		}
		break;
	case 14:
		{
			return Language::Get()->getString("@MIAttack") + "+" + StringConverter::toString(val);	
		}
		break;
	case 15:
		{
			return Language::Get()->getString("@MIPD") + "+" + StringConverter::toString(val);	
		}
		break;
	case 16:
		{
			return Language::Get()->getString("@MIMD") + "+" + StringConverter::toString(val);
		}
		break;
	case 17:
		{
			int integer = val / 100;
			int decimal = (val%100) / 10.0f;
			std::string str = StringConverter::toString(integer) + "." + StringConverter::toString(decimal) + "%";
			return  Language::Get()->getString("@MIHP") + "+" + str;
		}
		break;
	case 18:
		{
			int integer = val / 100;
			int decimal = (val%100) / 10.0f;
			std::string str = StringConverter::toString(integer) + "." + StringConverter::toString(decimal) + "%";
			return Language::Get()->getString("@MIAttack") + "+" + str;	
		}
		break;
	case 19:
		{
			int integer = val / 100;
			int decimal = (val%100) / 10.0f;
			std::string str = StringConverter::toString(integer) + "." + StringConverter::toString(decimal) + "%";
			return Language::Get()->getString("@MIPD") + "+" + str;
		}
		break;
	case 20:
		{
			int integer = val / 100;
			int decimal = (val%100) / 10.0f;
			std::string str = StringConverter::toString(integer) + "." + StringConverter::toString(decimal) + "%";
			return Language::Get()->getString("@MIMD") + "+" + str;
		}
		break;
	case 21:
		{
			int integer = val / 100;
			int decimal = (val%100) / 10.0f;
			std::string str = StringConverter::toString(integer) + "." + StringConverter::toString(decimal) + "%";
			return Language::Get()->getString("@HitRate") + "+" + str;
		}
		break;
	case 22:
		{
			int integer = val / 100;
			int decimal = (val%100) / 10.0f;
			std::string str = StringConverter::toString(integer) + "." + StringConverter::toString(decimal) + "%";
			return Language::Get()->getString("@DodgeRate") + "+" + str;
		}
		break;
	case 23:
		{
			int integer = val / 100;
			int decimal = (val%100) / 10.0f;
			std::string str = StringConverter::toString(integer) + "." + StringConverter::toString(decimal) + "%";
			return Language::Get()->getString("@ViolentRate") + "+" + str;
		}
		break;
	case 24:
		{
			int integer = val / 100;
			int decimal = (val%100) / 10.0f;
			std::string str = StringConverter::toString(integer) + "." + StringConverter::toString(decimal) + "%";
			return Language::Get()->getString("@ViolentAddRate") + "+" + str;
		}
		break;
	case 25:
		{
			int integer = val / 100;
			int decimal = (val%100) / 10.0f;
			std::string str = StringConverter::toString(integer) + "." + StringConverter::toString(decimal) + "%";
			return Language::Get()->getString("@ResistanceViolentRate") + "+" + str;
		}
		break;
	case 26:
		{
			int integer = val / 100;
			int decimal = (val%100) / 10.0f;
			std::string str = StringConverter::toString(integer) + "." + StringConverter::toString(decimal) + "%";
			return Language::Get()->getString("@ParryRate") + "+" + str;
		}
		break;
	case 27:
		{
			int integer = val / 100;
			int decimal = (val%100) / 10.0f;
			std::string str = StringConverter::toString(integer) + "." + StringConverter::toString(decimal) + "%";
			return Language::Get()->getString("@WrecKRate") + "+" + str;
		}
		break;
	case 28:
		{
			return Language::Get()->getString("@ExtraHurt") + "+" + StringConverter::toString(val);
		}
		break;
	case 29:
		{
			return Language::Get()->getString("@HurtExempt") + "+" + StringConverter::toString(val);
		}
		break;
	case 30:
		{
			int integer = val / 100;
			int decimal = (val%100) / 10.0f;
			std::string str = StringConverter::toString(integer) + "." + StringConverter::toString(decimal) + "%";
			return Language::Get()->getString("@ExtraHurt") + "+" + str;
		}
		break;
	default:
		break;
	}

	return "";
}

std::string ResManager::attrToString(unsigned int type)
{
	switch (type)
	{
	case 10:
		{
			return Language::Get()->getString("@Physique");	
		}
		break;
	case 11:
		{
			return Language::Get()->getString("@Force");	
		}
		break;
	case 12:
		{
			return Language::Get()->getString("@ArmorInfo");	
		}
		break;
	case 13:
		{
			return Language::Get()->getString("@MIHP");
		}
		break;
	case 14:
		{
			return Language::Get()->getString("@MIAttack");	
		}
		break;
	case 15:
		{
			return Language::Get()->getString("@MIPD");	
		}
		break;
	case 16:
		{
			return Language::Get()->getString("@MIMD");
		}
		break;
	case 17:
		{
			return  Language::Get()->getString("@MIHP");
		}
		break;
	case 18:
		{
			return Language::Get()->getString("@MIAttack");	
		}
		break;
	case 19:
		{
			return Language::Get()->getString("@MIPD");
		}
		break;
	case 20:
		{
			return Language::Get()->getString("@MIMD");
		}
		break;
	case 21:
		{
			return Language::Get()->getString("@HitRate");
		}
		break;
	case 22:
		{
			return Language::Get()->getString("@DodgeRate");
		}
		break;
	case 23:
		{
			return Language::Get()->getString("@ViolentRate");
		}
		break;
	case 24:
		{
			return Language::Get()->getString("@ViolentAddRate");
		}
		break;
	case 25:
		{
			return Language::Get()->getString("@ResistanceViolentRate");
		}
		break;
	case 26:
		{
			return Language::Get()->getString("@ParryRate");
		}
		break;
	case 27:
		{
			return Language::Get()->getString("@WrecKRate");
		}
		break;
	case 28:
		{
			return Language::Get()->getString("@ExtraHurt");
		}
		break;
	case 29:
		{
			return Language::Get()->getString("@HurtExempt");
		}
		break;
	case 30:
		{
			return Language::Get()->getString("@ExtraHurt");
		}
		break;
	default:
		break;
	}

	return "";
}

std::string ResManager::attrValToString(unsigned type, unsigned int val)
{
	switch (type)
	{
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 28:
	case 29:
		{
			return StringConverter::toString(val);	
		}
		break;	
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:

	case 30:
		{
			int integer = val / 100;
			int decimal = (val%100) / 10.0f;
			return StringConverter::toString(integer) + "." + StringConverter::toString(decimal) + "%";		
		}
		break;
	default:
		break;
	}

	return "";
}
