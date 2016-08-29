#include "FriendProposeItem.h"
#include "defines.h"
#include "CSFriendMsg.pb.h"
#include "FriendControl.h"
#include "FriendProxy.h"
#include "Resources.h"

#include "HeroCfgMaster.h"

FriendProposeItem::FriendProposeItem():m_pHeadImg(NULL)
{

}

FriendProposeItem::~FriendProposeItem()
{
}

FriendProposeItem* FriendProposeItem::create() {
	FriendProposeItem* pEquipTransListCell = new FriendProposeItem();

	if(pEquipTransListCell != NULL && pEquipTransListCell->init()) {
		pEquipTransListCell->autorelease();
	} else {
		CC_SAFE_DELETE(pEquipTransListCell);
	}

	return pEquipTransListCell;
}



const vmsg::CSOneFriend& FriendProposeItem::getInfo() {
	return m_stFriendInfo;
}

/*
bIsFriendList 是好友列表还是黑名单列表
*/
void FriendProposeItem::setInfo(const vmsg::CSOneFriend& stFriend) {
	m_stFriendInfo = stFriend;

    m_pNameTxt->setText(stFriend.name());
    //m_pLvTxt->setText("Lv." + intToString(stFriend.level()));
    m_pFpTxt->setText( intToString(stFriend.fightpower()) );

	const HeroCfg* heroCfg = HERO_CFG_MASTER->GetHeroCfgByID(stFriend.showid());
	if(heroCfg) {
		std::string url = HEAD_ICON_PATH + heroCfg->headicon() + ".png";
		m_pHeadImg->loadTexture(url.c_str());
	}
}

bool FriendProposeItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/friend_propose_item_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pHeadImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("head_img"));

        m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
        //m_pLvTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("lv_txt"));
        m_pFpTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("fp_txt"));

        m_pHeadImg->loadTexture("share/default_head.png", UI_TEX_TYPE_PLIST); //TODO: default

		bRet = true;
	} while(false);

	return bRet;
}

std::string FriendProposeItem::getDescription() const {
	return "FriendProposeItem";
}

