#include "SkillButton.h"
#include "Resources.h"
#include "CCColorUtil.h"
#include "defines.h"
#include "fight/FightObj.h"
#include "PopTipLayer.h"
#include "NounHelper.h"

#include "SkillCfgMaster.h"

const string SkillButton::SKILL_BTN_CLICK_EVENT = "skill_btn_click_event";

SkillButton::SkillButton()
:m_pSkillBtn(NULL)
,m_pSkillIcon(NULL)
,m_pProgressTimer(NULL)
,_isUsable(true)
,_isInCD(false)
,_isEnable(true)
,m_bIsBtnEnable(true)
{

}

SkillButton::~SkillButton(){

}

SkillButton* SkillButton::create(UILayout *pRefLayout, int idx, int cdTime, int skillID){
	SkillButton* pBtn = new SkillButton();

	if(pBtn != NULL && pBtn->initWithLayout(pRefLayout, idx, cdTime, skillID)){
		pBtn->autorelease();
	} else {
		CC_SAFE_DELETE(pBtn);
	}

	return pBtn;
}

bool SkillButton::initWithLayout(UILayout *pRefLayout, int idx, int cdTime, int skillID){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUI(pRefLayout);
		setName(pRefLayout->getName());

		m_uIDX = idx;
		m_uCDTime = cdTime;
		m_uSkillID = skillID;
		m_uTimeCount = cdTime;

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/fight.plist");
		UIButton* btn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("skill_btn"));
		CCPoint stBtnPos = btn->getPosition();
		int btnZorder = btn->getZOrder();
		btn->removeFromParentAndCleanup(true);
		m_pSkillBtn = FireButton::create();
		m_pSkillBtn->setName("skill_button");
		m_pSkillBtn->loadTextures("fight/fight_skill_btn_bg.png", "fight/fight_skill_btn_bg.png", "fight/fight_skill_btn_bg.png", UI_TEX_TYPE_PLIST);
		m_pSkillBtn->setPosition(stBtnPos);
		m_pRootLayout->addChild(m_pSkillBtn, btnZorder);

		m_pSkillIcon = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("skill_icon"));
		//ImageView* pImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("skill_icon"));
		//pImg->setVisible(false);

		//float radius = pImg->getSize().width/2;
		//ccColor4F fillColor = ccc4f(1, 1, 0, 1);
		//ccColor4F borderColor = ccc4f(0, 1, 0, 0);
		//const int nCount = 200;
		//const float angel = 2.0f*(float)M_PI/nCount;
		//static CCPoint circle[nCount];
		//for(int i = 0; i < nCount; i++){
		//	float radian = i*angel;
		//	circle[i].x = radius*cosf(radian);
		//	circle[i].y = radius*sinf(radian);
		//}
		//CCDrawNode* pStencil = CCDrawNode::create();
		//pStencil->drawPolygon(circle, nCount, fillColor, 0, borderColor);

		//m_pSkillIcon = ImageView::create();
		//m_pSkillIcon->setSize(CCSize(radius*2, radius*2));

		string skillIconStr;
		if(idx == 2){  //闪避图标
			skillIconStr = SKILLICON_RES_PATH + "sf00018" + PNG_TYPE_NAME;
		}else{
			skillIconStr = SKILLICON_RES_PATH + SKILL_CFG_MASTER->GetSkillCfgByID(m_uSkillID)->skillicon() + PNG_TYPE_NAME;
		}
		m_pSkillIcon->loadTexture(skillIconStr.c_str());

		//CCClippingNode *pClipNode = CCClippingNode::create();
		//pClipNode->setAnchorPoint(ccp(0.5, 0.5));
		//pClipNode->setInverted(false);
		//pClipNode->setAlphaThreshold(1.0f);
		//pClipNode->setPosition(pImg->getPosition());
		//pClipNode->addChild(m_pSkillIcon);

		//pClipNode->setStencil(pStencil);
		//addNode(pClipNode);

		string CDImgRes = SKILLICON_RES_PATH + "skill_cd_img.png";
		CCSprite* CDImgSp = CCSprite::create(CDImgRes.c_str());
		float scale = 1.0*110/CDImgSp->getContentSize().width;

		m_pProgressTimer = CCProgressTimer::create(CDImgSp);
		m_pProgressTimer->setScale(scale);
		m_pProgressTimer->setVisible(false);
		m_pProgressTimer->setPosition(m_pSkillIcon->getPosition());
		addNode(m_pProgressTimer, 100);

		unsigned int fontSize = 40;

		m_pTimeTxt = UILabel::create();
		//m_pTimeTxt->setColor(m_stColor);
		m_pTimeTxt->setFontName("fonts/04b_03b.ttf");
		m_pTimeTxt->setFontSize(fontSize);
		m_pTimeTxt->setTextHorizontalAlignment(kCCTextAlignmentRight); //右对齐
		m_pTimeTxt->setText(intToString(cdTime));
		m_pTimeTxt->setAnchorPoint(ccp(0.5, 0.5));
		m_pTimeTxt->setPosition(ccp(m_pSkillBtn->getSize().width/2, m_pSkillBtn->getSize().height/2));
		CCColorUtil::addStroke(m_pTimeTxt);
		addChild(m_pTimeTxt, 105);
		m_pTimeTxt->setVisible(false);

		m_pSkillBtn->addTouchEventListener(this, toucheventselector(SkillButton::touchEventHandler));

		bRet = true;
	} while(0);

	return bRet;
}

void SkillButton::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED: // click
		if(std::strcmp(pszWidgetName, "skill_button") == 0) {
			if(m_uIDX != 2 && P_FIGHT_OBJ->getEp() < SKILL_CFG_MASTER->GetSkillCfgByID(m_uSkillID)->epcost()){
				POP_TIP_LAYER_MASTER->ShowTipStr(NounStr(ENERGY_NOT_ENOUGH_TXT).c_str());
				return;
			}

			if(_isInCD || !_isEnable){
				return;
			}

			// 冷却计时，即时状态技能按钮不可点击
			if(m_uCDTime > 0){
				startCD();
			}

			CCString* idxStr = CCString::createWithFormat("%d", m_uIDX);
			this->DispatchEvent(SKILL_BTN_CLICK_EVENT, idxStr);
		} 

		break;
	default:
		break;
	}
}

void SkillButton::startCD(){
	m_pSkillBtn->setTouchEnabled(false);
	_isInCD = true;

	m_pProgressTimer->setVisible(true);
	m_pProgressTimer->setType(kCCProgressTimerTypeRadial);
	m_pProgressTimer->setReverseProgress(true); // 设置进度条为逆时针

	m_uTimeCount = m_uCDTime;
	m_pTimeTxt->setVisible(true);
	m_pTimeTxt->setText(intToString(m_uTimeCount));
	this->schedule(schedule_selector(SkillButton::updateTime), 1);

	//准备一个旋转360度的动画(逐渐覆盖半透模板形成冷却效果;这里进行计时冷却动画的实现和时间控制)
	//注意这里冷却效果是从100%到0%顺时针变化的
	CCActionInterval* action_progress_from_to = CCProgressFromTo::create(m_uCDTime, 100, 0); 
	CCFiniteTimeAction* action_callback = CCCallFunc::create(this, callfunc_selector(SkillButton::skillCoolDownCallBack));
	m_pProgressTimer->runAction(CCSequence::create(action_progress_from_to, action_callback, NULL));
}

void SkillButton::pauseCD(){
	this->pauseSchedulerAndActions();
	m_pProgressTimer->pauseSchedulerAndActions();
}

void SkillButton::resumeCD(){
	this->resumeSchedulerAndActions();
	m_pProgressTimer->resumeSchedulerAndActions();
}

void SkillButton::skillCoolDownCallBack(){
	// 进度条技能不可见
	m_pProgressTimer->setVisible(false);

	// 按钮置为可用
	if(m_bIsBtnEnable){
		m_pSkillBtn->setTouchEnabled(true);
	}
	_isInCD = false;
	//m_pSkillBtn->setBright(true);

	//if(m_uIDX != 2){  //闪避技能，没有技能图标
	//	CCColorUtil::removeGray(m_pSkillIcon->getVirtualRenderer());
	//}

	m_pTimeTxt->setText("");
	m_pTimeTxt->setVisible(false);
	this->unschedule(schedule_selector(SkillButton::updateTime));
}

void SkillButton::updateTime(float dt){
	if(m_uTimeCount > 1){
		m_uTimeCount--;
		m_pTimeTxt->setText(intToString(m_uTimeCount));
	}else{
		m_pTimeTxt->setText("");
		m_pTimeTxt->setVisible(false);
		this->unschedule(schedule_selector(SkillButton::updateTime));
	}
}

void SkillButton::setIsUsable(bool var){
	_isUsable = var;

	if(_isUsable){

		if(_isInCD == false && m_bIsBtnEnable){  //cd中不用重置可点击
			m_pSkillBtn->setTouchEnabled(true);
		}
		m_pSkillBtn->setBright(true);

		CCColorUtil::removeGray(m_pSkillIcon->getVirtualRenderer());
	}else{
		m_pSkillBtn->setTouchEnabled(false);
		m_pSkillBtn->setBright(false);

		CCColorUtil::addGray(m_pSkillIcon->getVirtualRenderer());
	}
}

bool SkillButton::getIsUsable(){
	return _isUsable;
}

void SkillButton::updateSkillIcon(unsigned int skillID){
	m_uSkillID = skillID;
	m_uCDTime = (int)SKILL_CFG_MASTER->GetSkillCfgByID(skillID)->cd();

	string skillIconStr = SKILLICON_RES_PATH + SKILL_CFG_MASTER->GetSkillCfgByID(m_uSkillID)->skillicon() + PNG_TYPE_NAME;
	m_pSkillIcon->loadTexture(skillIconStr.c_str());
}

void SkillButton::setBtnEnable(bool var){
	m_bIsBtnEnable = var;
	m_pSkillBtn->setTouchEnabled(var);
}

