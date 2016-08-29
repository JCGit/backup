#include "ActItem.h"

#include "Resources.h"

#include "CfgMaster/ActCfgMaster.h"
#include "ActControl.h"

#define  EFF_NODE_TAG 21
ActItem::ActItem()
:m_pBgImg(NULL)
,m_pIconImg(NULL)
,m_pActNameTxt(NULL)
,m_style(ACT_STYLE_GENERAL)
,m_bSelected(false)
,m_pstArmature(NULL)
{
}

ActItem::~ActItem()
{
}

ActItem* ActItem::create() {
	ActItem *pRet = new ActItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ActItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

unsigned int ActItem::getActStyle() {
	return m_style;
}

void ActItem::setActStyle(ActStyle style) {
	m_style = style;

	selected(m_bSelected);
}

bool ActItem::isSelected() {
	return m_bSelected;
}

void ActItem::selected(bool bSelected) {
	m_bSelected = bSelected;

	string url = "";

	if(m_bSelected){
		if(m_style == ACT_STYLE_GENERAL) {
			m_pBgImg->loadTexture("act_ui/act_tab_general_down.png", UI_TEX_TYPE_PLIST);
		} else if(m_style == ACT_STYLE_FESTIVE) {
			m_pBgImg->loadTexture("act_ui/act_tab_festive_down.png", UI_TEX_TYPE_PLIST);
		}
	}else{
		if(m_style == ACT_STYLE_GENERAL) {
			m_pBgImg->loadTexture("act_ui/act_tab_general_up.png", UI_TEX_TYPE_PLIST);
		} else if(m_style == ACT_STYLE_FESTIVE) {
			m_pBgImg->loadTexture("act_ui/act_tab_festive_up.png", UI_TEX_TYPE_PLIST);
		}
	}
}

const vmsg::CSCouponActStatusItem& ActItem::getInfo() {
	return m_stInfo;
}

void ActItem::setInfo(const vmsg::CSCouponActStatusItem& info) {
	m_stInfo = info;

	const ActCfg* pActCfg = ACT_CFG_MASTER->GetActCfgByType(m_stInfo.type());
	
	if(pActCfg != NULL) {
		setActStyle(transStyle(pActCfg->actstyle()));
		m_pActNameTxt->setText(pActCfg->name());

		//CCLog("ActItem::setInfo name:%s", pActCfg->name().c_str());

		string url = ICON_PATH + pActCfg->acticon() + ".png";
		m_pIconImg->loadTexture(url.c_str());
	}
	bool pIsCanGet = P_ACT_CTRL->IsActCanGet(info);
	if (pIsCanGet)
	{
		if(NULL == m_pstArmature) {
			m_pstArmature = CCArmature::create("ut00090");
			m_pstArmature->setAnchorPoint(ccp(0.5f, 0.5f));
			m_pstArmature->setPosition(ccp(m_pRootLayout->getSize().width/2 + 30, m_pRootLayout->getSize().height/2 - 5));
		}
		if(NULL == m_pstArmature->getParent()) {
			m_pRootLayout->addNode(m_pstArmature, 11, EFF_NODE_TAG);
		}
		m_pstArmature->getAnimation()->play("fly", -1, -1, 1); //loop
		m_pstArmature->setVisible(true);
	}else
	{
		if(m_pstArmature) {
			m_pstArmature->setVisible(false);
			m_pstArmature->getAnimation()->stop();
		}
	}
}

void ActItem::initUI() {
	initLayoutFromJsonFile("ui_v2/act_tab_ui.ExportJson");
	addChild(m_pRootLayout);

	m_pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_img"));

	m_pIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("icon_img"));

	m_pActNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));

	m_bUIInited = true;

	setActStyle(m_style);

}

ActItem::ActStyle ActItem::transStyle(unsigned int uStyle) {
	switch(uStyle) {
		case ACT_STYLE_GENERAL:
			return ACT_STYLE_GENERAL;
			break;
		case ACT_STYLE_FESTIVE:
			return ACT_STYLE_FESTIVE;
			break;
		default:
			return ACT_STYLE_GENERAL;
	}
}
