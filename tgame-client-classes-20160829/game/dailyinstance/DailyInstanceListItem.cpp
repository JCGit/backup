#include "DailyInstanceListItem.h"
#include "DailyInstanceCfgMaster.h"
#include "TimeEx.h"



DailyInstanceListItem::DailyInstanceListItem()
{
    m_iCfgKey = 0;
    m_pstArmature = NULL;
}

DailyInstanceListItem::~DailyInstanceListItem()
{
}

DailyInstanceListItem* DailyInstanceListItem::create() {
	DailyInstanceListItem* pEquipTransListCell = new DailyInstanceListItem();

	if(pEquipTransListCell != NULL && pEquipTransListCell->init()) {
		pEquipTransListCell->autorelease();
	} else {
		CC_SAFE_DELETE(pEquipTransListCell);
	}

	return pEquipTransListCell;
}

bool DailyInstanceListItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/DailyInstanceItem.ExportJson");
		addChild(m_pRootLayout);

        m_pBgSel = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_sel"));

        m_pIconCell = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("icon_cell"));
        m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
        m_pDayTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("day_txt"));

        m_pBgSel->setVisible(false);

		bRet = true;
	} while(false);

	return bRet;
}

std::string DailyInstanceListItem::getDescription() const {
	return "DailyInstanceListItem";
}


void DailyInstanceListItem::setCfgKey(const int iCfgKey) {
    const DailyInstanceCfg* pCfg = DAILY_INST_CFG_MASTER->getOneCfg(iCfgKey);
    if(NULL == pCfg) {
        CCLOG("[error]DailyInstanceListItem::setCfgKey,fail to get cfg,key=%d", iCfgKey);
        return;
    }
    m_iCfgKey = iCfgKey;
    m_pNameTxt->setText(pCfg->instancename());
    m_pDayTxt->setText(pCfg->opendaydesc());

    char buff[50] = {0};
    snprintf(buff, 50, "ui_bg/dailyinstancebg/icon_%s", pCfg->bgname().c_str() );
    m_pIconCell->loadTexture(buff);
    //开启时显示特效
    const int iToday = TimeEx::getTime()->tm_wday; //0 标示星期天，1-6标示星期1到星期6
    bool isOpen = DAILY_INST_CFG_MASTER->isInstanceOpen(iToday, m_iCfgKey);

    if(isOpen) {
        //显示特效
        if(NULL == m_pstArmature) {
            m_pstArmature = CCArmature::create("ut00090");
            m_pstArmature->setAnchorPoint(ccp(0.5f, 0.5f));
            m_pstArmature->setPosition(ccp(m_pRootLayout->getSize().width/2 + 30, m_pRootLayout->getSize().height/2 - 5));
        }
        if(NULL == m_pstArmature->getParent()) {
            m_pRootLayout->addNode(m_pstArmature, 11, EFF_NODE_TAG);
        }
        m_pstArmature->getAnimation()->play("fly", -1, -1, 1); //loop
        m_pstArmature->setVisible(true);
    } else {
        if(m_pstArmature) {
            m_pstArmature->setVisible(false);
            m_pstArmature->getAnimation()->stop();
        }
        
    }
}

void DailyInstanceListItem::setSelect(bool bIsSel) {
    m_pBgSel->setVisible(bIsSel);
}

int DailyInstanceListItem::getCfgKey() {
    return m_iCfgKey;
}

