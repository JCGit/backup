#include "FightBossHeadUI.h"
#include "Macros.pb.h"
#include "defines.h"
#include "Resources.h"

CFightBossHeadUI::CFightBossHeadUI()
:bossHp(NULL)
,bossHpBg(NULL)
,hpNum(NULL)
{
	m_uType = ROLE_TYPE_JUNIOR_BOSS;
	m_uHpNum = SMALL_BOSS_HPNUM;
}

CFightBossHeadUI::~CFightBossHeadUI(){

}

bool CFightBossHeadUI::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/fight_boss_head_ui.ExportJson");
		addChild(m_pRootLayout);

		bossHp = dynamic_cast<UILoadingBar*>(m_pRootLayout->getChildByName("boss_hp"));
		bossHpBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("boss_hp_bg"));
		hpNum = dynamic_cast<LabelAtlas*>(bossHp->getChildByName("hp_num"));

		ImageView* pImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("head_bg"));
		m_pBossHead = dynamic_cast<ImageView*>(pImg->getChildByName("head_img"));

		bRet = true;
	} while (0);

	return bRet;
}

void CFightBossHeadUI::setBossType(unsigned int type, string headRes){
	m_uType = type;

	if(m_uType == ROLE_TYPE_JUNIOR_BOSS){  //小boss
		m_uHpNum = SMALL_BOSS_HPNUM;
	}else if(m_uType == ROLE_TYPE_SENIOR_BOSS){  //大boss
		m_uHpNum = BIG_BOSS_HPNUM;
	}

	string numStr = ":" + intToString(m_uHpNum);
	hpNum->setStringValue(numStr);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/fight.plist");
	string bgName = "fight/fight_boss_hp_" + intToString(m_uHpNum - 1) + ".png";
	bossHp->loadTexture(bgName.c_str(), UI_TEX_TYPE_PLIST);
	bgName = "fight/fight_boss_hp_" + intToString(m_uHpNum - 2) + ".png";
	bossHpBg->loadTexture(bgName.c_str(), UI_TEX_TYPE_PLIST);

	//boss头像
	string strRes = "";
	if(headRes == "" || headRes == "null"){
		strRes = "defailt_boss.png";
	}else{
		strRes = headRes + ".png";
	}

	strRes = BOSS_HEAD_IMG_PATH + strRes;
	m_pBossHead->loadTexture(strRes.c_str());
}

unsigned int CFightBossHeadUI::getBossType(){
	return m_uType;
}

void CFightBossHeadUI::setHPPercent(UINT64_t nowHP, UINT64_t maxHP){
	if(nowHP <= 0){
		this->setVisible(false);
		return;
	}

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/fight.plist");
	UINT64_t perHp = maxHP/m_uHpNum;							//每条血条的血量
	UINT64_t lastHp = maxHP - (m_uHpNum - 1)*perHp;			//最后一个血条的总血量
	if(nowHP <= lastHp){  //最后一个血条	
		bossHpBg->setVisible(false);
		string bgName = "fight/fight_boss_hp_0.png";
		bossHp->loadTexture(bgName.c_str(), UI_TEX_TYPE_PLIST);
		int percent = 100*nowHP/lastHp;
		bossHp->setPercent(percent);

		string numStr = ":1";
		hpNum->setStringValue(numStr);
	}else{
		int remainHpNum = ceil((nowHP - lastHp)/(float)perHp) + 1;	//剩余血条数

		bossHpBg->setVisible(true);
		string bgName = "fight/fight_boss_hp_" + intToString(remainHpNum - 1) + ".png";
		bossHp->loadTexture(bgName.c_str(), UI_TEX_TYPE_PLIST);
		bgName = "fight/fight_boss_hp_" + intToString(remainHpNum - 2) + ".png";
		bossHpBg->loadTexture(bgName.c_str(), UI_TEX_TYPE_PLIST);

		int percent = 100*(nowHP - lastHp - (remainHpNum - 2)*perHp)/perHp;
		bossHp->setPercent(percent);

		string numStr = ":" + intToString(remainHpNum);
		hpNum->setStringValue(numStr);
	}
}

void CFightBossHeadUI::setParent(CCNode* parent){
	BaseLayout::setParent(parent);
}

void CFightBossHeadUI::setVisible(bool visible){
	BaseLayout::setVisible(visible);
}

void CFightBossHeadUI::setAllChildOpacity(GLubyte opacity){
	CCArray* pArr = m_pRootLayout->getChildren();
	CCObject* pstObj;
	CCARRAY_FOREACH(pArr, pstObj){
		UIWidget* pNode = dynamic_cast<UIWidget*>(pstObj);
		if(pNode){
			pNode->setOpacity(opacity);
		}
	}
}

