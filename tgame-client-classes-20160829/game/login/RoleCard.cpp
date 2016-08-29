#include "RoleCard.h"

#include "Resources.h"
#include "defines.h"

#include "CfgMaster/HeroCfgMaster.h"

#include "game/UserObj.h"

RoleCard::RoleCard()
:m_pBgNormalImg(NULL)
,m_pBgSelectedImg(NULL)
,m_pAddRoleImg(NULL)
,m_pRoleHeadImg(NULL)
,m_pRoleNameTxt(NULL)
,m_pRoleLevelTxt(NULL)
,m_uState(0)
{
}

RoleCard::~RoleCard()
{
}

RoleCard* RoleCard::create() {
	RoleCard *pRet = new RoleCard();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool RoleCard::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/RoleCardUI.ExportJson");
		addChild(m_pRootLayout);

		m_pBgNormalImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("role_card_normal_img"));

		m_pBgSelectedImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("role_card_selected_img"));

		m_pAddRoleImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("txt_add_role_img"));

		m_pRoleHeadImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("role_head_img"));

		m_pRoleNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("role_name_txt"));

		m_pRoleLevelTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("role_level_txt"));

		setState(CELL_NORMAL);

		bRet = true;
	} while(0);

	return bRet;
}

std::string RoleCard::getDescription() const {
	return "RoleCard";
}

unsigned int RoleCard::getState() {
	return m_uState;
}

void RoleCard::setState(unsigned int state) {
	m_uState = state;
	m_pAddRoleImg->setVisible(state == CELL_NONE);
	m_pBgNormalImg->setVisible(state != CELL_SELECTED);
	m_pBgSelectedImg->setVisible(state == CELL_SELECTED);
	m_pRoleHeadImg->setVisible(state != CELL_NONE);
	m_pRoleNameTxt->setVisible(state != CELL_NONE);
	m_pRoleLevelTxt->setVisible(state != CELL_NONE);

	setTouchEnabled(state != CELL_SELECTED);

}

const vmsg::CSPlayerRoleInfo& RoleCard::getRoleInfo() {
	return m_stRoleInfo;
}

void RoleCard::setRoleInfo(const vmsg::CSPlayerRoleInfo &stRoleInfo) {
	m_stRoleInfo = stRoleInfo;

	unsigned int uAvatarID = m_stRoleInfo.showid();
	std::string url = HEAD_ICON_PATH + HERO_CFG_MASTER->GetHeroCfgByID(uAvatarID)->headicon() + ".png";
	m_pRoleHeadImg->loadTexture(url.c_str());

	m_pRoleNameTxt->setText(m_stRoleInfo.nick());
	m_pRoleLevelTxt->setText(std::string("Lv.") + intToString(m_stRoleInfo.level()));
}
