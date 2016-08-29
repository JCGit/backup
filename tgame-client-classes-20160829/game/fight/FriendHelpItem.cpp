#include "FriendHelpItem.h"
#include "defines.h"
#include "CSFriendMsg.pb.h"
#include "friend/FriendControl.h"
#include "friend/FriendProxy.h"
#include "Resources.h"

#include "HeroCfgMaster.h"

FriendHelpItem::FriendHelpItem():m_pHeadImg(NULL)
{

}

FriendHelpItem::~FriendHelpItem()
{
}

FriendHelpItem* FriendHelpItem::create() {
    FriendHelpItem* pEquipTransListCell = new FriendHelpItem();

    if(pEquipTransListCell != NULL && pEquipTransListCell->init()) {
        pEquipTransListCell->autorelease();
    } else {
        CC_SAFE_DELETE(pEquipTransListCell);
    }

    return pEquipTransListCell;
}



const vmsg::CSOneFriend& FriendHelpItem::getInfo() {
    return m_stFriendInfo;
}

/*
bIsFriendList 是好友列表还是黑名单列表
*/
void FriendHelpItem::setInfo(const vmsg::CSOneFriend& stFriend) {
    m_stFriendInfo = stFriend;

    m_pNameTxt->setText(stFriend.name());
    m_pFpTxt->setText( intToString(stFriend.fightpower()) );

    if(stFriend.has_invitecostcoin()) {
        m_pHelpBtnImgCoinTxt->setText( intToString(stFriend.invitecostcoin()) );
    } else {
        m_pHelpBtnImgCoinTxt->setText("0" );
    }
    
	const HeroCfg* heroCfg = HERO_CFG_MASTER->GetHeroCfgByID(stFriend.showid());
	if(heroCfg) {
		std::string url = HEAD_ICON_PATH + heroCfg->headicon() + ".png";
		m_pHeadImg->loadTexture(url.c_str());
	}
}

bool FriendHelpItem::init() {
    bool bRet = false;

    do {
        CC_BREAK_IF(!BaseLayout::init());

        initLayoutFromJsonFile("ui_v2/friend_help_item_ui.ExportJson");
        addChild(m_pRootLayout);

        m_pHeadImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("head_img"));

        m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
        m_pSelIcon = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("sel_icon"));
        m_pFpTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("fp_txt"));

        m_pHelpBtnImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("help_btn_img"));
        m_pHelpBtnImgCoinLabel = dynamic_cast<UILabel*>(m_pHelpBtnImg->getChildByName("coin_label"));
        m_pHelpBtnImgCoinIcon = dynamic_cast<UIImageView*>(m_pHelpBtnImg->getChildByName("coin_icon"));
        m_pHelpBtnImgCoinTxt = dynamic_cast<UILabel*>(m_pHelpBtnImg->getChildByName("coin_txt"));

        m_pSelIcon->setVisible(false);


        m_pHeadImg->loadTexture("share/default_head.png", UI_TEX_TYPE_PLIST); //TODO: default

        bRet = true;
    } while(false);

    return bRet;
}

std::string FriendHelpItem::getDescription() const {
    return "FriendHelpItem";
}


void FriendHelpItem::onItemTouched(const CCPoint& pt) {
    setSelected(true); //不测试是否点击按钮了
}

bool FriendHelpItem::getIsSelected() const {
    return (m_pSelIcon->isVisible());
}

void FriendHelpItem::setSelected(bool bSelected) {
    m_pSelIcon->setVisible(bSelected);

    m_pHelpBtnImg->setVisible(!bSelected);
}
