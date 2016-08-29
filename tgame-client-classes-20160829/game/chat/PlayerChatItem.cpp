#include "PlayerChatItem.h"
#include "Resources.h"

#include "HeroCfgMaster.h"

PlayerChatItem::PlayerChatItem()
:m_pBgSelectedImg(NULL)
,m_pHeadImg(NULL)
,m_pNameTxt(NULL)
,m_bSelected(false)
,m_uin(0)
{
}

PlayerChatItem::~PlayerChatItem()
{
}

PlayerChatItem* PlayerChatItem::create(UILayout *pRefLayout) {
	PlayerChatItem *pRet = new PlayerChatItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool PlayerChatItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		bRet = true;
	} while(0);

	return bRet;
}

bool PlayerChatItem::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		m_pBgSelectedImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_selected_img"));

		m_pHeadImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("head_img"));

		m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));

		selected(false);

		bRet = true;
	} while(0);

	return bRet;
}

bool PlayerChatItem::isSelected() {
	return m_bSelected;
}

void PlayerChatItem::selected(bool bSelected) {
	m_bSelected = bSelected;

	m_pBgSelectedImg->setVisible(m_bSelected);
}

void PlayerChatItem::updateUI(UIN_t uin, const std::string &nick, unsigned int showID) {
	m_uin = uin;
	m_pNameTxt->setText(nick);

	const HeroCfg* heroCfg = HERO_CFG_MASTER->GetHeroCfgByID(showID);
	if(heroCfg) {
		std::string url = HEAD_ICON_PATH + heroCfg->headicon() + ".png";
		m_pHeadImg->loadTexture(url.c_str());
	}
}

UIN_t PlayerChatItem::getPlayerUin() {
	return m_uin;
}
