#include "WeaponArrRulePanel.h"
#include "ToolUtils.h"
#include "WeaponArrayRuleCfgMaster.h"

CWeaponArrRulePanel::CWeaponArrRulePanel(){

}

CWeaponArrRulePanel::~CWeaponArrRulePanel(){

}

bool CWeaponArrRulePanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/weapon_arr_rule_ui.ExportJson");
		addChild(m_pRootLayout);

		setClickOutToClose(true);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CWeaponArrRulePanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CWeaponArrRulePanel::touchEventHandler));

	m_pRuleList = dynamic_cast<ListView*>(m_pRootLayout->getChildByName("rule_list"));
	m_pRuleList->setItemsMargin(20);

	updateUI();
}

void CWeaponArrRulePanel::updateUI(){
	int uSize = WEAPON_ARR_RULE_CFG_MASTER->GetCfg().weaponarrayrulecfg_size();

	for(int i = 0; i < uSize; i++){
		string str = "";
		str += WEAPON_ARR_RULE_CFG_MASTER->GetCfg().weaponarrayrulecfg(i).desc();
		UILabel* m_pRuleTxt = UILabel::create();
		m_pRuleTxt->setFontName("fonts/tgamefont.ttf");
		m_pRuleTxt->setFontSize(20);
		m_pRuleTxt->setColor(ccc3(0x00, 0xff, 0xf6));
		m_pRuleTxt->setAnchorPoint(ccp(0, 1));
		m_pRuleTxt->setText(CToolUtils::splitUTFStr(str, 27));
		m_pRuleList->pushBackCustomItem(m_pRuleTxt);

		//limitWidth(str, 26);
		//m_pRuleTxt->setText(str);
	}
}

void CWeaponArrRulePanel::limitWidth(std::string &text, unsigned int width) {
	int cnt = 0;
	vector<string> vecText;

	CToolUtils::parseUTFStr(text, vecText);
	text = "";

	for(unsigned int i = 0; i < vecText.size(); i++) {
		cnt++;

		if(cnt > 1) {
			text += vecText[i];
		} else if(vecText[i] != " ") {
			text += vecText[i];
		}


		if(vecText[i] == "\n") {
			cnt = 0;
		}

		if(vecText[i] == " ") {
			unsigned int n = 0;

			while(n < vecText.size() - i - 1) {
				n++;

				if(vecText[i + n] == " ") {
					break;
				}

			}

			if(cnt + n >= width) {
				text += "\n";
				cnt = 0;
			}
		}

		if(cnt != 0 && cnt % width == 0 && i < vecText.size() - 1) {
			text += "\n";
			cnt = 0;
		}
	}
}


void CWeaponArrRulePanel::touchEventHandler(CCObject *pSender, TouchEventType type){
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

void CWeaponArrRulePanel::open(){
	BasePanel::open();
}

void CWeaponArrRulePanel::close(){
	BasePanel::close();
}

