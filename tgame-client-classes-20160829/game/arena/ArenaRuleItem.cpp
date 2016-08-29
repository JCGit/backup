#include "ArenaRuleItem.h"
#include "ClientSvrMsg.pb.h"
#include "defines.h"

#include "ArenaDailyAwardCfgMaster.h"
#include "ArenaLvCfgMaster.h"

CArenaRuleItem::CArenaRuleItem(){

}

CArenaRuleItem::~CArenaRuleItem(){

}

bool CArenaRuleItem::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/arena_rule_item_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pLvTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("lv_txt"));
		m_pCoinTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("award_coin_txt"));
		m_pArenaCoinTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("award_arena_coin_txt"));

		bRet = true;
	} while (0);

	return bRet;
}

void CArenaRuleItem::setLv(unsigned int lv){
	m_uLevel = lv;

	const ArenaDailyAwardCfg* stCfg = ARENA_DAILY_AWARD_CFG_MASTER->GetOneCfg(lv);

	m_pLvTxt->setText(ARENA_LV_CFG_MASTER->GetOneCfg(lv)->name());
	
	const ArenaDailyAwardCfg* stAwardCfg = ARENA_DAILY_AWARD_CFG_MASTER->GetOneCfg(lv);
	for(int i = 0; i < stAwardCfg->elmtlst_size(); i++){
		const ArenaAwardElement& stElmt = stAwardCfg->elmtlst(i);
		if(stElmt.type() == vmsg::THING_TYPE_COIN){
			m_pCoinTxt->setText("x" + intToString(stElmt.cnt()));
		}else if(stElmt.type() == vmsg::THING_TYPE_ARENA_MONEY){
			m_pArenaCoinTxt->setText("x" + intToString(stElmt.cnt()));
		}
	}
}