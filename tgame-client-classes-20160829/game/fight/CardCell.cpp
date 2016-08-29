#include "CardCell.h"
#include "vlib/base/CVLog.h"
#include "ItemUtils.h"
#include "ClientSvrMsg.pb.h"
#include "defines.h"
#include "IconCell.h"
#include "ToolUtils.h"

const string CCardCell::OPEN_OTHER_CARDS_EVENT = "open_other_cards_event";


CCardCell::CCardCell()
:_itemID(0)
,m_pCell(NULL)
,_isOpen(false)
,_isGeted(true)
{
	_itemType = 0;
	_itemCnt = 0;
}

CCardCell::~CCardCell(){
	
}

bool CCardCell::init(){
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/card_cell_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pCardImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("card_img"));
		m_pIconBgImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("item_bg"));
		m_pGoldImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("icon_gold"));
		m_pCostTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("cost_txt"));

		m_pCell = IconCell::create();
		m_pCell->showNumTxt();
		m_pRootLayout->addChild(m_pCell);
		m_pCell->setPosition(m_pIconBgImg->getPosition());

		this->setAnchorPoint(ccp(0.5, 0.5));
		this->ignoreContentAdaptWithSize(false);
		this->setSize(m_pRootLayout->getContentSize());
		this->setTouchEnabled(true);

		bRet = true;
	} while (0);

	return bRet;
}

void CCardCell::setItemInfo(unsigned int itemID, unsigned int itemType, unsigned int itemCnt){
	_itemID = itemID;
	_itemType = itemType;
	_itemCnt = itemCnt;

	CCLOG("[trace]CCardCell::setItemInfo,item id == %u, item type == %u, item cnt == %u", itemID, itemType, itemCnt);

	updateUI();
}

void CCardCell::setCostType(unsigned int var){
	_costType = var;

	string cardStr = "card_panel_ui/normal_card_close_img.png";
	bool bIsCost = false;
	if(_costType == FREE_CARD){
        bIsCost = false;
		cardStr = "card_panel_ui/normal_card_close_img.png";
    }else if(_costType == COST_CARD){
		bIsCost = true;
		cardStr = "card_panel_ui/gold_card_close_img.png";
	}

	m_pGoldImg->setVisible(bIsCost);
	m_pCostTxt->setVisible(bIsCost);
	m_pCardImg->setVisible(true);
	m_pCardImg->loadTexture(cardStr.c_str(), UI_TEX_TYPE_PLIST);
}

unsigned int CCardCell::getCostType(){
	return _costType;
}

void CCardCell::updateUI(){
	m_pCell->setInfo(_itemType, _itemCnt, _itemID);

	//碎片类型要显示图标
	CToolUtils::adjustIconBgSize(_itemType, _itemID, m_pIconBgImg);
	CToolUtils::adjustFragIcon(_itemType, m_pIconBgImg);
}

void CCardCell::openCard(bool isGeted){
	if(_isOpen){
		return;
	}

	_isGeted = isGeted;
	m_pCardImg->setVisible(false);

	//播动画
	CCArmature* pArm = CCArmature::create("ut00071");
	string str = "fly_";
	if(_costType == FREE_CARD){
		if(_isGeted){
			str += "1";
		}else{
			str += "2";
		}
	}else{
		if(_isGeted){
			str += "3";
		}else{
			str += "4";
		}
	}
	pArm->setPosition(m_pCardImg->getPosition());
	pArm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CCardCell::handleEffComplete));
	pArm->getAnimation()->play(str.c_str(), -1, -1, 0);
	addNode(pArm);

	_isOpen = true;
}

void CCardCell::handleEffComplete(CCArmature * armature, MovementEventType type, const char * name){
	string actName = name;
	if (type == COMPLETE){
		if(actName == "fly_1"){
			armature->getAnimation()->play("fly_1_xh", -1, -1, -1);
		}else if(actName == "fly_2"){
			armature->getAnimation()->play("fly_2_xh", -1, -1, -1);
		}else if(actName == "fly_3"){
			armature->getAnimation()->play("fly_3_xh", -1, -1, -1);
		}else if(actName == "fly_4"){
			armature->getAnimation()->play("fly_4_xh", -1, -1, -1);
		}

		DispatchEvent(OPEN_OTHER_CARDS_EVENT, this);
	}
}

void CCardCell::lock(){
	this->setTouchEnabled(false);
	m_bLocked = true;
}

void CCardCell::unLock(){
	this->setTouchEnabled(true);
	m_bLocked = false;
}

