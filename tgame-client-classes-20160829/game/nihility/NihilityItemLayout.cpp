#include "NihilityItemLayout.h"
#include "ItemUtils.h"
#include "IconCell.h"
#include "ClientSvrMsg.pb.h"
#include "bag/BagControl.h"

using namespace vmsg;

CNihilityItemLayout::CNihilityItemLayout()
:m_bIsSelect(false)
,m_bIsEnable(false)
,m_uCfgID(0)
{

}

CNihilityItemLayout::~CNihilityItemLayout(){

}

bool CNihilityItemLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/nihility_tool_item.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

CNihilityItemLayout* CNihilityItemLayout::createWithLayout(UILayout *pRefLayout){
	CNihilityItemLayout *pRet = new CNihilityItemLayout();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CNihilityItemLayout::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CNihilityItemLayout::initUI(){
	m_pDescTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("desc_txt"));
	m_pSelectBtn = dynamic_cast<CheckBox*>(m_pRootLayout->getChildByName("select_btn"));

	ImageView* pIcon = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("item_icon"));
	ImageView* pBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("item_bg"));
	pIcon->setVisible(false);

	m_pIconCell = IconCell::create();
	m_pIconCell->setZOrder(2);
	m_pIconCell->showNumTxt(true);
	m_pIconCell->setAnchorPoint(ccp(0.5f, 0.5f));
	pBg->addChild(m_pIconCell);
}

void CNihilityItemLayout::setCfgID(unsigned int uCfgID){
	m_uCfgID = uCfgID;
	m_pDescTxt->setText(ItemUtils::getItemDesc(THING_TYPE_ITEM, m_uCfgID));

	unsigned int uCnt = P_BAG_CTRL->getItemCnt(uCfgID);
	m_pIconCell->setInfo(THING_TYPE_ITEM, uCnt, m_uCfgID);

	setIsEnable(true);
	setSelected(false);
}

void CNihilityItemLayout::setSelected(bool bIsSelected){
	m_bIsSelect = bIsSelected;
	m_pSelectBtn->setSelectedState(bIsSelected);
}

void CNihilityItemLayout::setIsEnable(bool var){
	m_bIsEnable = var;
	m_pSelectBtn->setVisible(m_bIsEnable);
}

