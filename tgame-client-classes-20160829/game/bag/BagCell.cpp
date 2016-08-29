#include "BagCell.h"

#include "defines.h"

#include "ClientSvrMsg.pb.h"

#include "CfgMaster/ItemCfgMaster.h"
#include "CfgMaster/EquipmentCfgMaster.h"

#include "basic/CCColorUtil.h"
#include "basic/ToolUtils.h"

#include "game/UserObj.h"

using namespace std;

BagCell::BagCell()
:m_pBgItemImg(NULL)
,m_pItemImg(NULL)
,m_pItemFrameImg(NULL)
,m_pNumTxt(NULL)
,m_bSelected(false)
{
}

BagCell::~BagCell()
{
}

BagCell* BagCell::create(UILayout *pRefLayout) {
	BagCell *pRet = new BagCell();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

std::string BagCell::getDescription() const {
	return "BagCell";
}

bool BagCell::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		m_pBgItemImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_item_img"));

		m_pItemImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("item_img"));

		m_pItemFrameImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("item_frame_img"));

		m_pNumTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("num_txt"));
		CCColorUtil::addStroke(m_pNumTxt, 2.0f, ccc3(0xff, 0xff, 0xff));

		selected(false);
		m_pItemImg->setVisible(false);

		bRet = true;
	} while(0);

	return bRet;
}

bool BagCell::isSelected() {
	return m_bSelected;
}

void BagCell::selected(bool bSelected) {
	m_bSelected = bSelected;
	//m_pBgCellImg->setVisible(!m_bSelected);
	//m_pBgSelectedImg->setVisible(m_bSelected);
}

const vmsg::CSBagCellInfo& BagCell::getInfo() {
	return m_stBagCellInfo;
}

void BagCell::setInfo(const vmsg::CSBagCellInfo& stBagCellInfo) {
	m_stBagCellInfo = stBagCellInfo;
	unsigned int uItemType = m_stBagCellInfo.type();
	std::string iconURL = "";

	if(uItemType == vmsg::THING_TYPE_ITEM) { // item
		const vmsg::CSItemInfo &stItemInfo = m_stBagCellInfo.iteminfo();
		const ItemCfg *pItemCfg = ITEM_CFG_MASTER->GetItemCfgByID(stItemInfo.itemcfgid());

		iconURL = std::string("item/") + pItemCfg->itemicon() + ".png";

		m_pNumTxt->setText(intToString(stItemInfo.itemcnt()));
		m_pNumTxt->setVisible(stItemInfo.itemcnt() > 1);

		switch(pItemCfg->itemcolor()) {
			case vmsg::EQUIP_GRADE_WHITE:
				m_pItemFrameImg->loadTexture("share/item_frame_white.png", UI_TEX_TYPE_PLIST);
				break;
			case vmsg::EQUIP_GRADE_GREEN:
				m_pItemFrameImg->loadTexture("share/item_frame_green.png", UI_TEX_TYPE_PLIST);
				break;
			case vmsg::EQUIP_GRADE_BLUE:
				m_pItemFrameImg->loadTexture("share/item_frame_blue.png", UI_TEX_TYPE_PLIST);
				break;
			case vmsg::EQUIP_GRADE_PURPLE:
				m_pItemFrameImg->loadTexture("share/item_frame_purple.png", UI_TEX_TYPE_PLIST);
				break;
			case vmsg::EQUIP_GRADE_ORANGE:
				m_pItemFrameImg->loadTexture("share/item_frame_orange.png", UI_TEX_TYPE_PLIST);
				break;
			default:
				m_pItemFrameImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
		}
	} else if(uItemType == vmsg::THING_TYPE_EQUIP) { // equip
		const vmsg::CSEquipInfo &stEquipInfo = m_stBagCellInfo.equipinfo();
		const EquipmentCfg *pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquipInfo.equipid());

		iconURL = std::string("item/") + pEquipCfg->icon() + ".png";
		
		m_pNumTxt->setVisible(false);

		switch(pEquipCfg->color()) {
			case vmsg::EQUIP_GRADE_WHITE:
				m_pItemFrameImg->loadTexture("share/item_frame_white.png", UI_TEX_TYPE_PLIST);
				break;
			case vmsg::EQUIP_GRADE_GREEN:
				m_pItemFrameImg->loadTexture("share/item_frame_green.png", UI_TEX_TYPE_PLIST);
				break;
			case vmsg::EQUIP_GRADE_BLUE:
				m_pItemFrameImg->loadTexture("share/item_frame_blue.png", UI_TEX_TYPE_PLIST);
				break;
			case vmsg::EQUIP_GRADE_PURPLE:
				m_pItemFrameImg->loadTexture("share/item_frame_purple.png", UI_TEX_TYPE_PLIST);
				break;
			case vmsg::EQUIP_GRADE_ORANGE:
				m_pItemFrameImg->loadTexture("share/item_frame_orange.png", UI_TEX_TYPE_PLIST);
				break;
			default:
				m_pItemFrameImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
		}
	}

	m_pItemImg->loadTexture(iconURL.c_str());
	m_pItemImg->setVisible(!iconURL.empty());
	//m_pNumTxt->setVisible(std::atoi(m_pNumTxt->getStringValue()) > 1);

	float offsetX = getSize().width * 0.86f;
	float offsetY = getSize().height * 0.86f;

	if(m_stBagCellInfo.has_equipinfo()) {
		const vmsg::CSEquipInfo &stEquipInfo = m_stBagCellInfo.equipinfo();
		const EquipmentCfg *pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquipInfo.equipid());
		const vector<vmsg::CSEquipInfo>& stRoleEquipLst = P_USER_OBJ->getUserInfo()->stEquipLst;
		bool isPartEmpty = false;
		bool isAdvancedEquip = false;

		for(unsigned int i = 0; i < stRoleEquipLst.size(); i++) {
			const vmsg::CSEquipInfo &stCurEquipInfo = stRoleEquipLst[i];
			const EquipmentCfg *pCurEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stCurEquipInfo.equipid());
			
			if(pEquipCfg->part() == pCurEquipCfg->part()) {
				isPartEmpty = true;

				if(pEquipCfg->attr().attrid() == pCurEquipCfg->attr().attrid()
				&& pEquipCfg->attr().attrval() > pCurEquipCfg->attr().attrval()) {
					isAdvancedEquip = true;
					break;
				}
			}
		}

		if(!isPartEmpty || isAdvancedEquip) {
			CToolUtils::badgeWidgetWithPoint(this, 1, offsetX, offsetY);
		} else {
			CToolUtils::badgeWidgetWithPoint(this, 0, offsetX, offsetY);
		}
	} else {
		CToolUtils::badgeWidgetWithPoint(this, 0, offsetX, offsetY);
	}
}

void BagCell::clearInfo() {
    m_stBagCellInfo.Clear();

	selected(false);
	m_pItemImg->setVisible(false);
	m_pNumTxt->setVisible(false);
}


