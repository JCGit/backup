#include "ArenaRulePanel.h"
#include "ArenaRuleItem.h"
#include "ToolUtils.h"

#include "ArenaLvCfgMaster.h"
#include "ArenaRuleCfgMaster.h"

CArenaRulePanel::CArenaRulePanel(){

}

CArenaRulePanel::~CArenaRulePanel(){

}

bool CArenaRulePanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/arena_rule_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pRuleList = dynamic_cast<ListView*>(m_pRootLayout->getChildByName("rule_list"));
		m_pRuleList->setItemsMargin(10);

		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CArenaRulePanel::touchEventHandler));

		updateUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CArenaRulePanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();			
		}
		break;
	default:

		break;
	}
}

void CArenaRulePanel::updateUI(){
	int size = ARENA_LV_CFG_MASTER->GetCfg().arenalvcfg_size();

	for(int i = 0; i < size; i++){
		CArenaRuleItem* pItem = CArenaRuleItem::create();
		pItem->setLv(ARENA_LV_CFG_MASTER->GetCfg().arenalvcfg(i).level());
		m_pRuleList->pushBackCustomItem(pItem);
	}

	UILayout* m_pRuleLayer = UILayout::create();

	UILabel* m_pRuleTxt = UILabel::create();
	m_pRuleTxt->setFontName("fonts/tgamefont.ttf");
	m_pRuleTxt->setFontSize(26);
	m_pRuleTxt->setColor(ccc3(0xff, 0xff, 0xff));
	m_pRuleTxt->setAnchorPoint(ccp(0, 1));
	m_pRuleTxt->setText(CToolUtils::splitUTFStr(ARENA_RULE_CFG->GetCfg().ruledesc(), 24));
	m_pRuleLayer->addChild(m_pRuleTxt);
	m_pRuleTxt->setPosition(ccp(25, m_pRuleTxt->getSize().height - 10));

	m_pRuleLayer->setSize(CCSize(750, m_pRuleTxt->getSize().height));
	m_pRuleList->pushBackCustomItem(m_pRuleLayer);
}

void CArenaRulePanel::open(){
	BasePanel::open();
}