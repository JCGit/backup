#pragma once
#include <string>
#include <map>
#include "IteratorWrapper.h"
#include "Singleton.h"
#include "CCBContainer.h"
#include <cocos2d.h>
USING_NS_CC;
enum ResDataTypeEnum
{
	//取物品列别属性万位编号
	//玩家属性 
	USER_PROPERTY_TYPE = 1,
	//玩家属性小编号
	USER_PROPERTY_GOLD_COINS = 1001,
	USER_PROPERTY_SILVER_COINS = 1002,
	USER_PROPERTY_EXP = 1003,
	USER_LEAGUE_CONTRIBUTION = 1004,
	USER_PROPERTY_HEROSOUL = 1007,				// 将魂
	USER_LEVEL_PROPERTY = 12001,
	USER_PROPERTY_LEVEL_SILVER_COINS = 2002,
	USER_PRESTIGE=1009,//竞技场积分
	//武将魂魄
	DISCIPLE_BOOK=2,
	//武将
	DISCIPLE_TYPE=3,
	A_DISCIPLE=31001,
	B_DISCIPLE=31002,
	C_DISCIPLE=31003,
	D_DISCIPLE=31004,
	S_DISCIPLE=31005,
	//宝物
	TREASURE_TYPE=4,
	//装备
	EQUIP_TYPE=5,
	//装备碎片
	EQUIP_BOOK_TYPE=8,
	//宝物碎片
	TREASURE_BOOK_TYPE=9,
	//更名卡
	TOOLS_CHANGENAME_ITEMID=10046,
	//道具&礼物
	TOOLS_TYPE=6,
	DRAGONSOUL_TYPE=7,
	USER_INTERFACE=71001,

	COPPER_TREASURE=10006,//new pair tool by zhenhui 2014/2/18
	SILVER_TREASURE=10008,
	GOLD_TREASURE=10010,
	COPPER_KEY=10007,
	SILVER_KEY=10009,
	GOLD_KEY=10011,
	RECRUIT_PASS_ID = 10012, // add by zhenhui for the 招募令
	TOOL_WULONG_ID = 10017, // 商店购买道具乌龙的ID
	DISCIPLE_WULONG_ID = 1000,//武将表乌龙的ID
	PowerToolId=10004,
	VitalityToolId=10005,
	GIFTBOXPAGE=2,
	YINBEI=1101002,
	PUNCHHOLE=5002001,
	REMOVESTONE=5002002,
	WINE_TYPE = 70,
	WINE_LANGMU = 1501001,
	WINE_PUTAO = 1501002,
	ForgingMap_WEAPON = 5004,
	ForgingMap_ARMOR = 5005,
	ForgingMap_ACCESSORIES = 5006
};
#define VIP_MAX_LEVEL 13
#define PVPConsumeVitalityNum 2
struct ResInfo
{
	unsigned int id;
	std::string name;
	std::string describe;
	unsigned int quality;
	std::string icon;
	unsigned int count;
	unsigned int itemId;
	unsigned int type;
	std::string	typeName;
};
typedef std::map<unsigned int,ResInfo* > ResInfoMap;
typedef ConstMapIterator<ResInfoMap > ResInfoMapIterator;
typedef std::map<unsigned int,unsigned int > ResMap;

//Tab的Normal的TTF增加Tag：“119”
//Tab的Select的TTF增加Tag：“120”
#define CCB_TAB_TTF_SELECT_TAG 120
#define CCB_TAB_TTF_UNSELECT_TAG 119

class ResManager : public Singleton<ResManager>
{
public:
	 ResManager()
	{
		mPairList.insert(ResMap::value_type(COPPER_TREASURE,COPPER_KEY));
		mPairList.insert(ResMap::value_type(SILVER_TREASURE,SILVER_KEY));
		mPairList.insert(ResMap::value_type(GOLD_TREASURE,GOLD_KEY));
		mPairList.insert(ResMap::value_type(COPPER_KEY,COPPER_TREASURE));
		mPairList.insert(ResMap::value_type(SILVER_KEY,SILVER_TREASURE));
		mPairList.insert(ResMap::value_type(GOLD_KEY,GOLD_TREASURE));
	}
	static ResManager* getInstance(){return ResManager::Get();}
	ResInfo* getResInfoByTypeAndId(unsigned int type,unsigned int Id,unsigned int count);
	void setResInfo(ResInfo* info);
	ResInfoMap getResInfoListByResBagStr(const std::string& ResBagStr);
	//method for lua iterator
	int getResInfoListByResBagStrCount(const std::string& ResBagStr);
	ResInfo  getResInfoListByResBagStrByIndex(const std::string& ResBagStr,int index);
	//bool isSatisfiedWithResBagStr(const std::string& resBagStr);

	const unsigned int getTreasurePairType(unsigned int treasureId);
	const unsigned int getResMainType(unsigned int resId);
	//void useToolByIDAndCount(unsigned int toolId,unsigned int toolItemId,unsigned int toolCount);

	//根据外frame的边框的缩放scale和要显示的纹理大小，动态设置里面ICON的纹理图片和缩放
	bool setDynamicIconTex(CCBContainer* containner, std::string frameName ,std::string iconName , std::string iconStr);
	//设置边框的颜色，根据品质
	void setFrameColorByQuality(CCBContainer* containner, std::string frameName ,int quality);
	//设置大品质框：add by xinghui
	void setBigFrameTextureByQuality(CCBContainer* container, std::string frameName, int quality);
	//设置文本的颜色，
	void setLabelColorByQuality(CCBContainer* containner, std::string labelName ,int quality);


	void setTabLabelSelected(cocos2d::CCMenuItemImage* tabImage);

	void setTabLabelUnselected(cocos2d::CCMenuItemImage* tabImage);

	//设置页面tab一致性的适配，动态设置非选中tab的显示状态。add by xinghui
	void setTabLabelAdapte(std::map<unsigned int, CCMenuItemImage*> menuItemImageMap, int index);
	//expose to lua, when user in lua, 从左想右，从1递增，顺序赋值，如果没有这么多，就用nil复制。
	void setTabLabelAdapte(CCMenuItemImage* menuItemImage1, CCMenuItemImage* menuItemImage2, CCMenuItemImage* menuItemImage3, CCMenuItemImage* menuItemImage4, CCMenuItemImage* menuItemImage5, int index);

	////仅用于WIN32下测试所有ccbi用到的纹理资源add by zhenhui，Ctrl + U 用来调用该方法
	//遍历ccbi目录下所有的ccbi文件，打印对应ccbi用到的纹理资源日志，用来检查缺图，重名，纹理资源交叉引用等资源问题。
#ifdef WIN32
	void checkCCBIResource();
#endif
	
	
	////更新topBanner统一入口
	//void refreshTopBanner(CCBContainer* container);

	////更新UsrInfoBanner统一入口
	//void refreshUsrInfoBanner(CCBContainer* container);

	//从Code_core中抽出与逻辑相关的代码by zhenhui 2014/7/21
	static std::string attrToString(unsigned int type,unsigned int val);
	static std::string attrToString(unsigned int type);
	static std::string attrValToString(unsigned type, unsigned int val);

private:
	ResMap mPairList;
};

//根据外frame的边框的缩放scale和要显示的纹理大小，动态设置里面ICON的纹理图片和缩放，目前，默认内边框的物理尺寸为81像素。
#define  CCB_SET_ICON_TEX(ccb,outFrameVariable,innerIconVariable,iconTexture) ResManager::Get()->setDynamicIconTex(ccb,outFrameVariable,innerIconVariable,iconTexture);

//设置边框的颜色，根据品质
#define  CCB_SET_FRAME_TEX(ccb,outFrameVariable,quality) ResManager::Get()->setFrameColorByQuality(ccb,outFrameVariable,quality);

//设置大品质框:add by xinghui
#define CCB_SET_CARD_FRAME_TEX(ccb,outFrameVariable,quality) ResManager::Get()->setBigFrameTextureByQuality(ccb,outFrameVariable,quality)

//设置字体的颜色，根据品质
#define  CCB_SET_LABEL_COLOR(ccb,outVariable,quality) ResManager::Get()->setLabelColorByQuality(ccb,outVariable,quality);

//更新topBanner统一入口
#define CCB_REFRESH_TOP_BANNER(ccb)	 ResManager::Get()->refreshTopBanner(ccb);
//更新UsrInfoBanner统一入口
#define CCB_REFRESH_USER_BANNER(ccb)	 ResManager::Get()->refreshUsrInfoBanner(ccb);

//add by zhenhui for the use of selecting or unselecting the tab's label
#define TAB_SELECTED_COLOR  StringConverter::parseColor3B(VaribleManager::Get()->getSetting("Tab_Selected_Color"))
#define TAB_UNSELECTED_COLOR  StringConverter::parseColor3B(VaribleManager::Get()->getSetting("Tab_UnSelected_Color"))

//根据传入ccMenuimage,获取到tab node 中其他两个labelttf,并设置对应状态
#define CCB_SET_TAB_LABEL_SELECT(tabname)	 ResManager::Get()->setTabLabelSelected(tabname);
#define CCB_SET_TAB_LABEL_UNSELECT(tabname)	 ResManager::Get()->setTabLabelUnselected(tabname);
