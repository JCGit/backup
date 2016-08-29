#include "EquipSelItem.h"
#include "EquipmentCfgMaster.h"
#include "defines.h"
#include "NounHelper.h"
#include "firearms/StarProgressBar.h"
#include "ItemUtils.h"
#include "EquipLibControl.h"

EquipSelItem::EquipSelItem()
{
    m_iEquipBagIdx = -1;
}

EquipSelItem::~EquipSelItem()
{
	CCLOG("destroy EquipSelItem");
}

EquipSelItem* EquipSelItem::create() {
	EquipSelItem *pEquipSelItem = new EquipSelItem();

	if(pEquipSelItem != NULL && pEquipSelItem->init()) {
		pEquipSelItem->autorelease();
	} else {
		CC_SAFE_DELETE(pEquipSelItem);
	}

	return pEquipSelItem;
}

EquipSelItem* EquipSelItem::create(UILayout *pRefLayout) {
	EquipSelItem *pRet = new EquipSelItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EquipSelItem::initWithLayout(UILayout *pRefLayout) {
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

bool EquipSelItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/equip_sel_panel_item.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void EquipSelItem::initUI(){
	m_pEquipNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
	m_pPartTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("part_txt"));
	m_pAttrTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("attr_txt"));

	m_pIcon = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_cell"));
	m_pEquipCell = ItemCell::create();
	m_pEquipCell->setPosition(m_pIcon->getPosition());
	//m_pEquipCell->setSize(CCSizeMake(60, 60));
	m_pEquipCell->setZOrder(10);
	addChild(m_pEquipCell);

	m_pSelStatBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("sel_bg"));
	m_pSelStatIcon = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("sel_stat_pic"));
	m_pInUsePic = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("in_use_pic"));

    m_pEquipStar = CStarProgressBar::create("share/little_star_dark.png", 
        "share/little_star_light.png", "ui_v2/share.png", MAX_EQUIP_STAR);
    m_pEquipStar->setAnchorPoint(ccp(0.5f, 1.0f));
    CCPoint pt = m_pEquipNameTxt->getPosition();
    pt.y -= m_pEquipNameTxt->getSize().height;
    m_pEquipStar->setPosition(pt);
    m_pEquipStar->setStarMargin(1);
    addChild(m_pEquipStar);
}

std::string EquipSelItem::getDescription() const {
	return "EquipSelItem";
}


void EquipSelItem::updateEquipInfo(const vmsg::CSEquipInfo& stEquip, bool bIsWear, const int iBagIdx)
{
    m_stEquipInfo = stEquip;
    m_iEquipBagIdx = iBagIdx;

    const EquipmentCfg* pstCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquip.equipid());
    m_pEquipNameTxt->setText(pstCfg->equipname());
    m_pEquipCell->updateEquipInfo(stEquip);

    //当前装备属性
    unsigned int uAttr = ItemUtils::getEquipAttrVal(stEquip.equipid(), stEquip.level(), stEquip.quality());
    string strTemp = AttrTypeStr(pstCfg->attr().attrid()) + " +" + intToString(uAttr);
    m_pAttrTxt->setText(strTemp);
    
    m_pInUsePic->setVisible(bIsWear);

    string typeName;
    if(pstCfg->part() == vmsg::EQUIP_POS_ARMOR) {
        typeName = NounStr(ARMOR);
    } else if(pstCfg->part() == vmsg::EQUIP_POS_RING) {
        typeName = NounStr(RING);
    } else if(pstCfg->part() == vmsg::EQUIP_POS_SHOE) {
        typeName = NounStr(SHOE);
    } else if(pstCfg->part() == vmsg::EQUIP_POS_CLOTH) {
        typeName = NounStr(CLOTH);
    }

    m_pPartTxt->setText(typeName);
    m_pEquipStar->setStarNum(pstCfg->initstar(), MAX_EQUIP_STAR);
}

void EquipSelItem::setSelect(bool isSel)
{
    m_pSelStatBg->setVisible(isSel);
    m_pSelStatIcon->setVisible(isSel);
}

