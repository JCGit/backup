#include "FriendListItemUI.h"
#include "defines.h"
#include "CSFriendMsg.pb.h"
#include "FriendControl.h"
#include "FriendProxy.h"
#include "NounHelper.h"
#include "game/chat/ChatControl.h"
#include "CommonDialog.h"
#include "TextHelper.h"

#include "bag/BagControl.h"
#include "game/guide/GuideControl.h"
#include "Resources.h"
#include "HeroCfgMaster.h"

FriendListItemUI::FriendListItemUI():m_pHeadImg(NULL),m_bIsSelected(false)
{

}

FriendListItemUI::~FriendListItemUI()
{
}

FriendListItemUI* FriendListItemUI::create() {
	FriendListItemUI* pEquipTransListCell = new FriendListItemUI();

	if(pEquipTransListCell != NULL && pEquipTransListCell->init()) {
		pEquipTransListCell->autorelease();
	} else {
		CC_SAFE_DELETE(pEquipTransListCell);
	}

	return pEquipTransListCell;
}

bool FriendListItemUI::getIsSelected() const {
	return m_bIsSelected;
}

void FriendListItemUI::selected(bool bSelected) {
	if(m_bIsSelected != bSelected) {
		m_bIsSelected = bSelected;

		if(m_bIsSelected) {
			//m_pBgImg->loadTexture("ui/bag/bg_cell_selected.png");
		} else {
			//m_pBgImg->loadTexture("ui/bag/bg_cell_normal.png");
		}
	}
}


const vmsg::CSOneFriend& FriendListItemUI::getInfo() {
	return m_stFriendInfo;
}

/*
bIsFriendList 是好友列表还是黑名单列表
*/
void FriendListItemUI::setInfo(const vmsg::CSOneFriend& stFriend) {
	m_stFriendInfo = stFriend;

    m_pNameTxt->setText(stFriend.name() );
    m_pLvTxt->setText("Lv." + intToString(stFriend.level()));
    m_pFpTxt->setText( intToString(stFriend.fightpower()) );
    /*
    if (stFriend.isonline()) {
        m_pIsOnlieTxt->setText(NounStr(NOUN_ONLINE));
    } else {
        m_pIsOnlieTxt->setText(NounStr(NOUN_OFFLINE));
    }*/
    const HeroCfg* heroCfg = HERO_CFG_MASTER->GetHeroCfgByID(stFriend.showid());
    if(heroCfg) {
        std::string url = HEAD_ICON_PATH + heroCfg->headicon() + ".png";
        m_pHeadImg->loadTexture(url.c_str());
    }
    
    updateManualBtnStat();
    
}
void FriendListItemUI::updateManualBtnStat() {
    
    if(ESTAT_FRIEND == m_iStat) {
        //过滤一下体力按钮的状态
        bool bCanRecv = (P_FRIEND_CTRL->getRecvManualUinSet().count(m_stFriendInfo.uin()) > 0);
        m_pBtnOne->setEnabled(bCanRecv); //可以收体力就显示按钮
        m_pBtnOne->setVisible(bCanRecv);
        m_pBtnOne->setTitleText(NounStr(NOUN_RECV_MANUAL));
        m_pBtnOne->loadTextureNormal("share/btn_green_down.png", UI_TEX_TYPE_PLIST);

        if(!bCanRecv) {
            bool bCanSend = (P_FRIEND_CTRL->getSendManualUinSet().count(m_stFriendInfo.uin()) == 0);
            m_pBtnOne->setTitleText(NounStr(NOUN_SEND_MANUAL));
            m_pBtnOne->setEnabled(bCanSend); //可以送体力就显示
            m_pBtnOne->setVisible(bCanSend);
            m_pBtnOne->loadTextureNormal("share/btn_green_up.png", UI_TEX_TYPE_PLIST);
        }
    } else {
        m_pBtnOne->loadTextureNormal("share/btn_blue_up.png", UI_TEX_TYPE_PLIST);
        
    }
}

bool FriendListItemUI::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/friend_panel_item.ExportJson");
		addChild(m_pRootLayout);

		m_pHeadImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("head_img"));

        m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
        m_pLvTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("lv_txt"));
        m_pFpTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("fp_txt"));
        //m_pIsOnlieTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("isonline_txt"));

        m_pBtnOne = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_one"));
        m_pBtnTwo = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_two"));

        m_pBtnSingle = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_single"));

        m_pHeadImg->loadTexture("share/default_head.png", UI_TEX_TYPE_PLIST); //TODO: default

		bRet = true;
	} while(false);

	return bRet;
}

std::string FriendListItemUI::getDescription() const {
	return "FriendListItemUI";
}

void FriendListItemUI::doBtnOneClick() {


    switch (m_iStat) {
    case ESTAT_FRIEND:
        //manual
        //NounStr(NOUN_RECV_MANUAL)
		GUIDE_TOUCH(m_pBtnOne);

        if(NounStr(NOUN_RECV_MANUAL) == m_pBtnOne->getTitleText() ) {
            P_FRIEND_CTRL->getProxy()->sendRecvManualRqst(m_stFriendInfo.uin() );
			P_FRIEND_CTRL->setPresentedManual(true);
        } else if(NounStr(NOUN_SEND_MANUAL) == m_pBtnOne->getTitleText() ) {
            P_FRIEND_CTRL->getProxy()->sendSendManualRqst(m_stFriendInfo.uin() );
			P_FRIEND_CTRL->setPresentedManual(true);
        } 
        
        break;
    case ESTAT_MORE:
        break;
    case ESTAT_ADMIN:
        //add friend
        P_FRIEND_CTRL->getProxy()->sendAdminAddRqst(m_stFriendInfo.uin());
        break;
    case ESTAT_BLACKLIST:
        //del
        P_FRIEND_CTRL->getProxy()->sendDelRqst(m_stFriendInfo.uin() );
        break;
    }
}

void FriendListItemUI::doBtnTwoClick() {

    switch (m_iStat) {
    case ESTAT_FRIEND:
        //chat
        P_CHAT_CTRL->chatWith(m_stFriendInfo.uin(), m_stFriendInfo.name(), m_stFriendInfo.level(), m_stFriendInfo.showid());
        P_FRIEND_CTRL->getFriendPanelUI()->close();
        break;
    case ESTAT_MORE:
        //none
        break;
    case ESTAT_ADMIN:
        //refuse
        P_FRIEND_CTRL->getProxy()->sendAdminRefuseRqst(m_stFriendInfo.uin() );
        break;
    case ESTAT_BLACKLIST:
        //add friend
        P_FRIEND_CTRL->getProxy()->sendAdminAddRqst(m_stFriendInfo.uin());
        break;
    }
}

void FriendListItemUI::setStat(const int iStat)  {
    m_iStat = iStat;
    
    std::string strOneBtn("");
    std::string strTwoBtn("");
    std::string strSingleBtn("");
    switch (m_iStat) {
    case ESTAT_FRIEND:
        strOneBtn = NounStr(NOUN_SEND_MANUAL);
        strTwoBtn = NounStr(NOUN_CHAT1);
        break;
    case ESTAT_MORE:
        strSingleBtn = NounStr(NOUN_ADD);
        setDelBtnVisible(false);
        break;
    case ESTAT_ADMIN:
        strOneBtn = NounStr(NOUN_ADD);
        strTwoBtn = NounStr(NOUN_REFUSE);
        setDelBtnVisible(false);
        break;
    case ESTAT_BLACKLIST:
        strOneBtn = NounStr(NOUN_ADD);
        strTwoBtn = NounStr(NOUN_DEL);
        setDelBtnVisible(false);
        break;
    }
    if (strOneBtn.length() > 0) {
        m_pBtnOne->setTitleText(strOneBtn);
    } 
    m_pBtnOne->setEnabled(strOneBtn.length() > 0); 
    m_pBtnOne->setVisible(strOneBtn.length() > 0);

    if (strTwoBtn.length() > 0) {
        m_pBtnTwo->setTitleText(strTwoBtn);
    } 
    m_pBtnTwo->setEnabled(strTwoBtn.length() > 0); 
    m_pBtnTwo->setVisible(strTwoBtn.length() > 0);

    if(strSingleBtn.length() > 0) {
        m_pBtnSingle->setTitleText(strSingleBtn);
    }
    m_pBtnSingle->setEnabled(strSingleBtn.length() > 0); 
    m_pBtnSingle->setVisible(strSingleBtn.length() > 0);
    
}

void FriendListItemUI::setDelBtnVisible(bool isVisible) {
    /*
    if (NULL == m_pBtnDel) {
        m_pBtnDel = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("del_btn"));
        m_pBtnDel->addTouchEventListener(this, toucheventselector(FriendListItemUI::touchEventHandler));
    }

    m_pBtnDel->setVisible(isVisible);
    m_pBtnDel->setEnabled(isVisible);
    */
}


void FriendListItemUI::onItemTouched(const CCPoint& pt) {
    if(ESTAT_MORE == m_iStat) {
        if(m_pBtnSingle->hitTest(pt)) {
			GUIDE_TOUCH(m_pBtnSingle);
            //add
            P_FRIEND_CTRL->getProxy()->sendAddReqRqst(m_stFriendInfo.name());
            CCLOG("[trace]FriendListItemUI::onItemTouched,add friedn req:%s, uin %llu", 
                m_stFriendInfo.name().c_str(), m_stFriendInfo.uin());
        }
        return;
    } 
    if(m_pBtnOne->hitTest(pt)) {
        doBtnOneClick();
    } else if(m_pBtnTwo->hitTest(pt)) {
        doBtnTwoClick();
    } else {
        //打开查看好友详细信息界面
        P_BAG_CTRL->requestOtherRoleInfo(m_stFriendInfo.uin());
    }
}

UIButton* FriendListItemUI::getPresentManualBtn() {
	UIButton *pRet = NULL;

	if(ESTAT_FRIEND == m_iStat) {
		bool bCanRecv = (P_FRIEND_CTRL->getRecvManualUinSet().count(m_stFriendInfo.uin()) > 0);
		bool bCanSend = (P_FRIEND_CTRL->getSendManualUinSet().count(m_stFriendInfo.uin()) == 0);
		
		if(bCanRecv || bCanSend) {
			pRet = m_pBtnOne;
		}
	}

	return pRet;
}

UIButton* FriendListItemUI::getAddFriendBtn() {
	UIButton *pRet = NULL;

	if(m_pBtnSingle != NULL && m_pBtnSingle->isVisible()) {
		pRet = m_pBtnSingle;
	}

	return pRet;
}
