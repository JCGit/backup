#include "EquipSelMatItem.h"
#include "EquipmentCfgMaster.h"
#include "defines.h"
#include "NounHelper.h"
//#include "EquipRefineCostCfgMaster.h"
#include "EquipExpItemCfgMaster.h"
#include "ItemUtils.h"
#include "firearms/StarProgressBar.h"
#include "ClientSvrMsg.pb.h"
#include "EquipLibControl.h"

EquipSelMatItem::EquipSelMatItem()
{
    m_iListIdx = -1;
}

EquipSelMatItem::~EquipSelMatItem()
{
	CCLOG("destroy EquipSelMatItem");
}

EquipSelMatItem* EquipSelMatItem::create() {
	EquipSelMatItem *pEquipSelMatItem = new EquipSelMatItem();

	if(pEquipSelMatItem != NULL && pEquipSelMatItem->init()) {
		pEquipSelMatItem->autorelease();
	} else {
		CC_SAFE_DELETE(pEquipSelMatItem);
	}

	return pEquipSelMatItem;
}

EquipSelMatItem* EquipSelMatItem::create(UILayout *pRefLayout) {
	EquipSelMatItem *pRet = new EquipSelMatItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EquipSelMatItem::initWithLayout(UILayout *pRefLayout) {
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

bool EquipSelMatItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/equip_sel_mat_panel_item.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void EquipSelMatItem::initUI(){
	m_pEquipNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
	//m_pLvTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("lv_txt"));
	m_pExpTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("exp_txt"));

	m_pIcon = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_cell"));
	m_pEquipCell = ItemCell::create();
	m_pEquipCell->setPosition(m_pIcon->getPosition());
	m_pEquipCell->setZOrder(10);
	addChild(m_pEquipCell);

	m_pSelStatBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("sel_bg"));
	m_pSelStatIcon = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("sel_stat_pic"));

    m_pEquipStar = CStarProgressBar::create("share/little_star_dark.png", 
        "share/little_star_light.png", "ui_v2/share.png", MAX_EQUIP_STAR);
    m_pEquipStar->setAnchorPoint(ccp(0.5f, 1.0f));
    CCPoint pt = m_pEquipNameTxt->getPosition();
    //pt.x += 30;
    pt.y -= m_pEquipNameTxt->getSize().height;
    m_pEquipStar->setPosition(pt);
    m_pEquipStar->setStarMargin(1);
    addChild(m_pEquipStar);
}

std::string EquipSelMatItem::getDescription() const {
	return "EquipSelMatItem";
}


void EquipSelMatItem::setInfo(const EquipSelMatOneInfo& info, const int iListIdx)
{
    m_stMatInfo = info;
    m_iListIdx = iListIdx;
    
    if(info.itemCfgID > 0) {
        //µÀ¾ß
        m_pEquipNameTxt->setText(ItemUtils::getItemName(vmsg::THING_TYPE_ITEM, info.itemCfgID ));
        //m_pLvTxt->setText("");
        vmsg::CSItemInfo stItemInfo;
        stItemInfo.set_itemcfgid(info.itemCfgID);
        stItemInfo.set_itemcnt(1);
        m_pEquipCell->updateItemInfo(stItemInfo);
        const EquipExpItemCfg* pExpCfg = EQUIPEXPITEM_CFG_MASTER->GetEquipExpItemCfg( info.itemCfgID );
        if(pExpCfg) {
            m_pExpTxt->setText(intToString(pExpCfg->equipexp()));
        } else {
            m_pExpTxt->setText("0");
            CCLOG("[error]EquipSelMatItem::setInfo,fail to get EquipExpItemCfg,cfg id %d", info.itemCfgID);
        }
        m_pEquipStar->setVisible(false);
        return;
    }
    if(info.stEquip.equipInfo.equipguid() > 0) {
        const EquipmentCfg* pstCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(info.stEquip.equipInfo.equipid());
        m_pEquipNameTxt->setText(pstCfg->equipname());
        //m_pLvTxt->setText("Lv." + intToString(pstCfg->equiplv()));
        m_pEquipCell->updateEquipInfo(info.stEquip.equipInfo);
        m_pExpTxt->setText(intToString(P_EQUIP_LIB_CTRL->getObj()->getEquipEatExp(info.stEquip.equipInfo.totalexp(), info.stEquip.equipInfo.equipid()) ) );

        m_pEquipStar->setVisible(true);
        m_pEquipStar->setStarNum(pstCfg->initstar());
    }
     
}

void EquipSelMatItem::setSelect(bool isSel)
{
    m_pSelStatBg->setVisible(isSel);
    m_pSelStatIcon->setVisible(isSel);
}

bool EquipSelMatItem::isSelect() {
    return (m_pSelStatBg->isVisible());
}
