#include "GuildMemberPanel.h"

#include "game/bag/BagControl.h"
#include "game/bag/EquipCell.h"
#include "game/login/LoginObj.h"
#include "game/friend/FriendControl.h"

#include "basic/NounHelper.h"

#include "CfgMaster/EquipmentCfgMaster.h"

#include "defines.h"
#include "Macros.pb.h"
#include "UserObj.h"

#include "GuildControl.h"

GuildMemberPanel::GuildMemberPanel()
:m_pCloseBtn(NULL)
,m_pArmorCell(NULL)
,m_pRingCell(NULL)
,m_pShoesCell(NULL)
,m_pCloakCell(NULL)
,m_pOptionsListView(NULL)
,m_pHpValTxt(NULL)
,m_pAttackValTxt(NULL)
,m_pDefenseValTxt(NULL)
,m_pAtkSpeedValTxt(NULL)
,m_pMoveSpeedValTxt(NULL)
,m_pAddValTxt(NULL)
,m_pAtkTypeValTxt(NULL)
,m_pDefTypeValTxt(NULL)
,m_pDetailBtn(NULL)
,_panelYype(TYPE_MEMBER_GUILD)
{
}

GuildMemberPanel::~GuildMemberPanel()
{
}

GuildMemberPanel* GuildMemberPanel::create() {
	GuildMemberPanel *pRet = new GuildMemberPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildMemberPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui/GuildMemberPanelUI.json");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(GuildMemberPanel::touchEventHandler));

		m_pArmorCell = EquipCell::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("armor_cell")));

		m_pRingCell = EquipCell::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("amulet_cell")));

		m_pShoesCell = EquipCell::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("shoes_cell")));

		m_pCloakCell = EquipCell::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("cloak_cell")));

		m_pOptionsListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("options_list"));
		m_pOptionsListView->addEventListenerListView(this, listvieweventselector(GuildMemberPanel::listViewEventHandler));

		m_pHpValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("hp_val_txt"));

		m_pAttackValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("attack_val_txt"));

		m_pDefenseValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("defense_val_txt"));

		m_pAtkSpeedValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("atk_speed_val_txt"));

		m_pMoveSpeedValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("move_speed_val_txt"));

		m_pAddValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("hp_add_val_txt"));

		m_pAtkTypeValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("atk_type_val_txt"));

		m_pDefTypeValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("def_type_val_txt"));

		m_pDetailBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("detail_btn"));
		m_pDetailBtn->addTouchEventListener(this, toucheventselector(GuildMemberPanel::touchEventHandler));

		bRet = true;
	} while(0);

	return bRet;
}

void GuildMemberPanel::updateUI() {
	const vmsg::CSPlayerInfo &stPlayerInfo = P_LOGIN_OBJ->getOtherPlayerInfo();
	const vmsg::CSPlayerDetailInfo &stPlayerDetailInfo = P_LOGIN_OBJ->getOtherPlayerDetailInfo();
	m_stPlayerInfo = stPlayerInfo;

	m_pArmorCell->clearInfo();
	m_pRingCell->clearInfo();
	m_pShoesCell->clearInfo();
	m_pCloakCell->clearInfo();

	unsigned int starLevel = 0;
	unsigned int hpAddVal = 0;

	for(int i = 0; i < stPlayerDetailInfo.equiplst_size(); i++) {
		const vmsg::CSEquipInfo &stEquipInfo = stPlayerDetailInfo.equiplst(i);//stPlayerDetailInfo.equiplst(i);
		const EquipmentCfg *pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquipInfo.equipid());
		unsigned int part = pEquipCfg->part();

		//starLevel += stEquipInfo.refinelv();

		if(part == vmsg::EQUIP_POS_ARMOR) {
			m_pArmorCell->setInfo(stEquipInfo);
		} else if(part == vmsg::EQUIP_POS_RING) {
			m_pRingCell->setInfo(stEquipInfo);
		} else if(part == vmsg::EQUIP_POS_SHOE) {
			m_pShoesCell->setInfo(stEquipInfo);
		} else if(part == vmsg::EQUIP_POS_CLOTH) {
			m_pCloakCell->setInfo(stEquipInfo);
		}

		if(pEquipCfg->attr().attrid() == vmsg::ATTR_HP) {
			hpAddVal += pEquipCfg->attr().attrval();
		}
	}

	/*for(unsigned int i = 0; i < starLevel / 10; i++) {
		StarCell *pStarCell = m_stStarCells[i];
		pStarCell->setState(StarCell::STATE_NORMAL);
		pStarCell->setLevel((i + 1) * 10);
	}*/

	m_pHpValTxt->setText(intToString(stPlayerDetailInfo.hp() - hpAddVal));
	m_pAttackValTxt->setText(intToString(stPlayerDetailInfo.atk()));
	m_pDefenseValTxt->setText(intToString(stPlayerDetailInfo.def()));
	m_pAtkSpeedValTxt->setText(intToString(stPlayerDetailInfo.atkspeed()));
	m_pMoveSpeedValTxt->setText(intToString(stPlayerDetailInfo.movespeed()));

	m_pAddValTxt->setText(std::string("+") + intToString(hpAddVal));
	m_pAddValTxt->setPositionX(m_pHpValTxt->getPositionX() + m_pHpValTxt->getSize().width + 16.0f);
	m_pAddValTxt->setVisible(hpAddVal > 0);

	std::string atkTypeName = "";

	if(stPlayerDetailInfo.atktype() == CLOSE_PANITRATE) {
		atkTypeName = NounStr(NOUN_CLOSE_PANITRATE);
	} else if(stPlayerDetailInfo.atktype() == CLOSE_BLUNT) {
		atkTypeName = NounStr(NOUN_CLOSE_BLUNT);
	} else if(stPlayerDetailInfo.atktype() == CLOSE_MAGIC) {
		atkTypeName = NounStr(NOUN_CLOSE_MAGIC);
	} else if(stPlayerDetailInfo.atktype() == CLOSE_ORDINARY) {
		atkTypeName = NounStr(NOUN_CLOSE_ORDINARY);
	} else if(stPlayerDetailInfo.atktype() == REMOGE_MAGIC) {
		atkTypeName = NounStr(NOUN_REMOTE_MAGIC);
	} else if(stPlayerDetailInfo.atktype() == REMOTE_PANITRATE) {
		atkTypeName = NounStr(NOUN_REMOTE_PANITRATE);
	} else if(stPlayerDetailInfo.atktype() == REMOTE_ORDINARY) {
		atkTypeName = NounStr(NOUN_REMOTE_ORDINARY);
	}  

	m_pAtkTypeValTxt->setText(atkTypeName);
	m_pAtkTypeValTxt->setPositionX(m_pAttackValTxt->getPositionX() + m_pAttackValTxt->getSize().width + 16.0f);
	m_pAtkTypeValTxt->setVisible(atkTypeName != "");

	std::string defTypeName = "";

	if(stPlayerDetailInfo.deftype() == HEARY_ARMOR_DEF) {
		defTypeName = NounStr(HEARY_ARMOR_DEF_STR);
	} else if(stPlayerDetailInfo.deftype() == LIGHT_ARMOR_DEF) {
		defTypeName = NounStr(LIGHT_ARMOR_DEF_STR);
	} else if(stPlayerDetailInfo.deftype() == NO_ARMOR_DEF) {
		defTypeName = NounStr(NONE_ARMOR_DEF_STR);
	}

	m_pDefTypeValTxt->setText(defTypeName);
	m_pDefTypeValTxt->setPositionX(m_pDefenseValTxt->getPositionX() + m_pDefenseValTxt->getSize().width + 16.0f);
	m_pDefTypeValTxt->setVisible(defTypeName != "");

	updateOptionBtns();
}

void GuildMemberPanel::updateOptionBtns(){
	const vmsg::CSPlayerInfo &stPlayerInfo = P_LOGIN_OBJ->getOtherPlayerInfo();

	m_pOptionsListView->removeAllItems();
	UIButton *pBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("option_btn"));

	if(_panelYype == TYPE_MEMBER_GUILD){
		if(vmsg::GUILD_PRIVI_CHAIRMAN == P_GUILD_CTRL->getMemberPrivilege(P_USER_OBJ->getUserInfo()->uin)) {
			UIButton *pChangePriviBtn = dynamic_cast<UIButton*>(pBtn->clone());
			pChangePriviBtn->setVisible(true);
			pChangePriviBtn->setName("change_privi_btn");

			if(vmsg::GUILD_PRIVI_DEPUTY == P_GUILD_CTRL->getMemberPrivilege(stPlayerInfo.uin())) {
				pChangePriviBtn->setTitleText(NounStr(NOUN_TO_MEMBER));
			} else {
				pChangePriviBtn->setTitleText(NounStr(NOUN_TO_DEPUTY));
			}

			m_pOptionsListView->pushBackCustomItem(pChangePriviBtn);
		}
	}

	UIButton *pBeFriendsBtn = dynamic_cast<UIButton*>(pBtn->clone());
	pBeFriendsBtn->setVisible(true);
	pBeFriendsBtn->setName("be_friend_btn");
	pBeFriendsBtn->setTitleText(NounStr(NOUN_BE_FRIEND));
	m_pOptionsListView->pushBackCustomItem(pBeFriendsBtn);

	UIButton *pChatBtn = dynamic_cast<UIButton*>(pBtn->clone());
	pChatBtn->setVisible(true);
	pChatBtn->setName("chat_btn");
	pChatBtn->setTitleText(NounStr(NOUN_CHAT));
	m_pOptionsListView->pushBackCustomItem(pChatBtn);

	UIButton *pIgnoreBtn = dynamic_cast<UIButton*>(pBtn->clone());
	pIgnoreBtn->setVisible(true);
	pIgnoreBtn->setName("ignore_btn");
	pIgnoreBtn->setTitleText(NounStr(NOUN_IGNORE_LIST));
	m_pOptionsListView->pushBackCustomItem(pIgnoreBtn);

	unsigned int myPrivi = P_GUILD_CTRL->getMemberPrivilege(P_USER_OBJ->getUserInfo()->uin);
	unsigned int memPrivi = P_GUILD_CTRL->getMemberPrivilege(stPlayerInfo.uin());

	if(_panelYype == TYPE_MEMBER_GUILD){
		if(vmsg::GUILD_PRIVI_CHAIRMAN == myPrivi
			|| (vmsg::GUILD_PRIVI_DEPUTY == myPrivi && vmsg::GUILD_PRIVI_MEMBER == memPrivi)) {
				UIButton *pKickOutBtn = dynamic_cast<UIButton*>(pBtn->clone());
				pKickOutBtn->setVisible(true);
				pKickOutBtn->setName("kick_out_btn");
				pKickOutBtn->setTitleText(NounStr(NOUN_KICKOUT_FROM_GUILD));
				m_pOptionsListView->pushBackCustomItem(pKickOutBtn);
		}
	}
}

void GuildMemberPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(std::strcmp(pszWidgetName, m_pDetailBtn->getName()) == 0) {
				const vmsg::CSPlayerInfo &stPlayerInfo = P_LOGIN_OBJ->getOtherPlayerInfo();
				const vmsg::CSPlayerDetailInfo &stPlayerDetailInfo = P_LOGIN_OBJ->getOtherPlayerDetailInfo();

			}
			break;
		default:
			break;
	}
}

void GuildMemberPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	int nSelectedIdx = m_pOptionsListView->getCurSelectedIndex();
	UIButton *pBtn = dynamic_cast<UIButton*>(m_pOptionsListView->getItem(nSelectedIdx));
	const char *pszBtnName = pBtn->getName();
	std::string btnTitle = pBtn->getTitleText();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(btnTitle == NounStr(NOUN_TO_DEPUTY)) {
				P_GUILD_CTRL->sendGuildChgPriviRqst(m_stPlayerInfo.uin(), vmsg::GUILD_PRIVI_DEPUTY);
			} else if(btnTitle == NounStr(NOUN_TO_MEMBER)) {
				P_GUILD_CTRL->sendGuildChgPriviRqst(m_stPlayerInfo.uin(), vmsg::GUILD_PRIVI_MEMBER);
			} else if(btnTitle == NounStr(NOUN_BE_FRIEND)) {
				P_FRIEND_CTRL->getProxy()->sendAddReqRqst(m_stPlayerInfo.nick());
			} else if(btnTitle == NounStr(NOUN_CHAT)) {
				
			} else if(btnTitle == NounStr(NOUN_IGNORE_LIST)) {
				P_FRIEND_CTRL->getProxy()->sendAddBlackRqst(m_stPlayerInfo.uin());
			} else if(btnTitle == NounStr(NOUN_KICKOUT_FROM_GUILD)) {
				P_GUILD_CTRL->sendGuildKickRqst(m_stPlayerInfo.uin());
			}

			close();
			break;
		default:
			break;
	}
}
