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
	//ȡ��Ʒ�б�������λ���
	//������� 
	USER_PROPERTY_TYPE = 1,
	//�������С���
	USER_PROPERTY_GOLD_COINS = 1001,
	USER_PROPERTY_SILVER_COINS = 1002,
	USER_PROPERTY_EXP = 1003,
	USER_LEAGUE_CONTRIBUTION = 1004,
	USER_PROPERTY_HEROSOUL = 1007,				// ����
	USER_LEVEL_PROPERTY = 12001,
	USER_PROPERTY_LEVEL_SILVER_COINS = 2002,
	USER_PRESTIGE=1009,//����������
	//�佫����
	DISCIPLE_BOOK=2,
	//�佫
	DISCIPLE_TYPE=3,
	A_DISCIPLE=31001,
	B_DISCIPLE=31002,
	C_DISCIPLE=31003,
	D_DISCIPLE=31004,
	S_DISCIPLE=31005,
	//����
	TREASURE_TYPE=4,
	//װ��
	EQUIP_TYPE=5,
	//װ����Ƭ
	EQUIP_BOOK_TYPE=8,
	//������Ƭ
	TREASURE_BOOK_TYPE=9,
	//������
	TOOLS_CHANGENAME_ITEMID=10046,
	//����&����
	TOOLS_TYPE=6,
	DRAGONSOUL_TYPE=7,
	USER_INTERFACE=71001,

	COPPER_TREASURE=10006,//new pair tool by zhenhui 2014/2/18
	SILVER_TREASURE=10008,
	GOLD_TREASURE=10010,
	COPPER_KEY=10007,
	SILVER_KEY=10009,
	GOLD_KEY=10011,
	RECRUIT_PASS_ID = 10012, // add by zhenhui for the ��ļ��
	TOOL_WULONG_ID = 10017, // �̵깺�����������ID
	DISCIPLE_WULONG_ID = 1000,//�佫��������ID
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

//Tab��Normal��TTF����Tag����119��
//Tab��Select��TTF����Tag����120��
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

	//������frame�ı߿������scale��Ҫ��ʾ�������С����̬��������ICON������ͼƬ������
	bool setDynamicIconTex(CCBContainer* containner, std::string frameName ,std::string iconName , std::string iconStr);
	//���ñ߿����ɫ������Ʒ��
	void setFrameColorByQuality(CCBContainer* containner, std::string frameName ,int quality);
	//���ô�Ʒ�ʿ�add by xinghui
	void setBigFrameTextureByQuality(CCBContainer* container, std::string frameName, int quality);
	//�����ı�����ɫ��
	void setLabelColorByQuality(CCBContainer* containner, std::string labelName ,int quality);


	void setTabLabelSelected(cocos2d::CCMenuItemImage* tabImage);

	void setTabLabelUnselected(cocos2d::CCMenuItemImage* tabImage);

	//����ҳ��tabһ���Ե����䣬��̬���÷�ѡ��tab����ʾ״̬��add by xinghui
	void setTabLabelAdapte(std::map<unsigned int, CCMenuItemImage*> menuItemImageMap, int index);
	//expose to lua, when user in lua, �������ң���1������˳��ֵ�����û����ô�࣬����nil���ơ�
	void setTabLabelAdapte(CCMenuItemImage* menuItemImage1, CCMenuItemImage* menuItemImage2, CCMenuItemImage* menuItemImage3, CCMenuItemImage* menuItemImage4, CCMenuItemImage* menuItemImage5, int index);

	////������WIN32�²�������ccbi�õ���������Դadd by zhenhui��Ctrl + U �������ø÷���
	//����ccbiĿ¼�����е�ccbi�ļ�����ӡ��Ӧccbi�õ���������Դ��־���������ȱͼ��������������Դ�������õ���Դ���⡣
#ifdef WIN32
	void checkCCBIResource();
#endif
	
	
	////����topBannerͳһ���
	//void refreshTopBanner(CCBContainer* container);

	////����UsrInfoBannerͳһ���
	//void refreshUsrInfoBanner(CCBContainer* container);

	//��Code_core�г�����߼���صĴ���by zhenhui 2014/7/21
	static std::string attrToString(unsigned int type,unsigned int val);
	static std::string attrToString(unsigned int type);
	static std::string attrValToString(unsigned type, unsigned int val);

private:
	ResMap mPairList;
};

//������frame�ı߿������scale��Ҫ��ʾ�������С����̬��������ICON������ͼƬ�����ţ�Ŀǰ��Ĭ���ڱ߿������ߴ�Ϊ81���ء�
#define  CCB_SET_ICON_TEX(ccb,outFrameVariable,innerIconVariable,iconTexture) ResManager::Get()->setDynamicIconTex(ccb,outFrameVariable,innerIconVariable,iconTexture);

//���ñ߿����ɫ������Ʒ��
#define  CCB_SET_FRAME_TEX(ccb,outFrameVariable,quality) ResManager::Get()->setFrameColorByQuality(ccb,outFrameVariable,quality);

//���ô�Ʒ�ʿ�:add by xinghui
#define CCB_SET_CARD_FRAME_TEX(ccb,outFrameVariable,quality) ResManager::Get()->setBigFrameTextureByQuality(ccb,outFrameVariable,quality)

//�����������ɫ������Ʒ��
#define  CCB_SET_LABEL_COLOR(ccb,outVariable,quality) ResManager::Get()->setLabelColorByQuality(ccb,outVariable,quality);

//����topBannerͳһ���
#define CCB_REFRESH_TOP_BANNER(ccb)	 ResManager::Get()->refreshTopBanner(ccb);
//����UsrInfoBannerͳһ���
#define CCB_REFRESH_USER_BANNER(ccb)	 ResManager::Get()->refreshUsrInfoBanner(ccb);

//add by zhenhui for the use of selecting or unselecting the tab's label
#define TAB_SELECTED_COLOR  StringConverter::parseColor3B(VaribleManager::Get()->getSetting("Tab_Selected_Color"))
#define TAB_UNSELECTED_COLOR  StringConverter::parseColor3B(VaribleManager::Get()->getSetting("Tab_UnSelected_Color"))

//���ݴ���ccMenuimage,��ȡ��tab node ����������labelttf,�����ö�Ӧ״̬
#define CCB_SET_TAB_LABEL_SELECT(tabname)	 ResManager::Get()->setTabLabelSelected(tabname);
#define CCB_SET_TAB_LABEL_UNSELECT(tabname)	 ResManager::Get()->setTabLabelUnselected(tabname);
