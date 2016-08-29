#include "TipControl.h"
#include "UserObj.h"
#include "NounHelper.h"
#include "vip/VIPControl.h"
#include "vip/BuyCoinPanel.h"

#include "CostCfgMaster.h"
#include "GlobalCfgMaster.h"
#include "ItemDropTipCfgMaster.h"

using namespace std;

TipControl::TipControl()
:m_pPromptPayPanel(NULL)
,m_pItemLackPanel(NULL)
,m_pLevelNotEnoughPanel(NULL)
,m_pGiftTipPanel(NULL)
{

}

TipControl::~TipControl(){
	CC_SAFE_RELEASE_NULL(m_pPromptPayPanel);
	CC_SAFE_RELEASE_NULL(m_pItemLackPanel);
	CC_SAFE_RELEASE_NULL(m_pLevelNotEnoughPanel);
	CC_SAFE_RELEASE_NULL(m_pGiftTipPanel);
}

bool TipControl::GoldCostTip(unsigned int cost, CCObject *target, SEL_BtnEvent costEvent, 
	const string& strDescTxt /*= ""*/, unsigned int param1 /* = 0 */, unsigned int param2 /* = 0 */){
	
	string descStr = strDescTxt;
	if(strDescTxt == ""){  //默认显示"消耗XX星钻购买"
		descStr = TextStr(TEXT_COMMON_COST_TIP);
	}
	char str[200];
	if(param1 > 0 && param2 > 0){
		snprintf(str, 200, descStr.c_str(), cost, param1, param2);
	}else if(param1 > 0){
		snprintf(str, 200, descStr.c_str(), cost, param1);
	}else{
		snprintf(str, 200, descStr.c_str(), cost);
	}

	if(cost > P_USER_OBJ->getUserInfo()->gold){   //星砖不足,弹出统一的提示充值面板
		getPromptPayPanel()->openWithDiffGold(cost - P_USER_OBJ->getUserInfo()->gold);
		return false;
	}

	COMMON_DIALOG->show(str, target, NounStr(OK), NounStr(CANCEL), costEvent);

	return true;
}

bool TipControl::CoinCostTip(unsigned int cost, CCObject *target){
	if(P_USER_OBJ->checkCoin(cost) != 0){  //联邦币不足
		COMMON_DIALOG->show(TextStr(TEXT_COIN_NOT_ENOUGH), target, NounStr(OK), 
			NounStr(CANCEL), dialogbtneventselector(TipControl::openBuyCoin));
		return false;
	}

	return true;
}

void TipControl::openBuyCoin(){
	P_VIP_CTRL->getBuyCoinPanel()->open();
}

PromptPayPanel* TipControl::getPromptPayPanel() {
	if(m_pPromptPayPanel == NULL) {
		m_pPromptPayPanel = PromptPayPanel::create();
		CC_SAFE_RETAIN(m_pPromptPayPanel);
	}

	return m_pPromptPayPanel;
}

CWeaponRefitTipPanel* TipControl::getItemInfoTipPanel(){
    if(!(m_stRefitTipPanel.isInited())) {
        m_stRefitTipPanel.init();
    }
    return &m_stRefitTipPanel;
}


ItemDropTipListPanel* TipControl::getItemDropTipListPanel() {
    if(!(m_stItemDropTipPanel.isInited())) {
        m_stItemDropTipPanel.init();
    }
    return &m_stItemDropTipPanel;
}

CSweepCardShotPanel* TipControl::getSweepCardShotPanel() {
	if(!(m_stSweepCardShotPanel.isInited())) {
		m_stSweepCardShotPanel.init();
	}
	return &m_stSweepCardShotPanel;
}

ItemLackPanel* TipControl::getItemLackPanel() {
	if(m_pItemLackPanel == NULL) {
		m_pItemLackPanel = ItemLackPanel::create();
		CC_SAFE_RETAIN(m_pItemLackPanel);
	}

	return m_pItemLackPanel;
}

LevelNotEnoughPanel* TipControl::getLevelNotEnoughPanel() {
	if(m_pLevelNotEnoughPanel == NULL) {
		m_pLevelNotEnoughPanel = LevelNotEnoughPanel::create();
		CC_SAFE_RETAIN(m_pLevelNotEnoughPanel);
	}

	return m_pLevelNotEnoughPanel;
}

GiftTipPanel* TipControl::getGiftTipPanel()
{
	if (m_pGiftTipPanel == NULL)
	{
		m_pGiftTipPanel = GiftTipPanel::create();
		CC_SAFE_RETAIN(m_pGiftTipPanel);
	}
	return m_pGiftTipPanel;
}

void TipControl::LowManualBuyTip(CCObject* target,  SEL_BtnEvent costEvent){
	string lowManualStr = TextStr(TEXT_MANUAL_LOW_TIP);
	int costGold = GLOBAL_CFG_MASTER->GetCfg().buymanualgold();
	int goldAdd = GLOBAL_CFG_MASTER->GetCfg().buymanualgoldadd();
	int maxCnt = P_USER_OBJ->getUserInfo()->maxManualCnt;
	int buyCnt = P_USER_OBJ->getUserInfo()->buyManualCnt;
	int manualNum = GLOBAL_CFG_MASTER->GetCfg().manual();
	if(buyCnt >= maxCnt){  //体力不足，购买次数已用完
		getPromptPayPanel()->openWithVIPModule(vmsg::VIP_MODULE_BUYMANUAL);
		return ;
	}

	//下次购买花费星钻
	costGold = COST_CFG_MASTER->GetCostCfgByCount(buyCnt + 1)->buymanualcost();

	P_TIP_CTRL->GoldCostTip(costGold, target, costEvent, 
		lowManualStr + TextStr(TEXT_MANUAL_COST_TIP), manualNum, maxCnt - buyCnt);
}

void TipControl::showItemTipPanel(const int iType, const int iCfgID, const int iNum, bool bIsShowDrop, bool bIsGift ){
	bool bIsDrop = bIsShowDrop;
	if(iType == vmsg::THING_TYPE_MONTHCARD || iType == vmsg::THING_TYPE_GEN){
		bIsDrop = false;
	}

	if(!bIsDrop){
		getItemInfoTipPanel()->setItemInfo(iType, iNum, iCfgID, false);
		getItemInfoTipPanel()->open();
	}else{
		if(NULL == ITEM_DROP_TIP_MASTER->GetOneCfg(iCfgID, iType)) {
			if (bIsGift)
			{
				getGiftTipPanel()->setItemInfo(iType,iCfgID);
				getGiftTipPanel()->open();
			}else
			{
				getItemInfoTipPanel()->setItemInfo(iType, iNum, iCfgID, false);
				getItemInfoTipPanel()->open();
			}
			
		}else{
			getItemDropTipListPanel()->showItemInfo(iType, iCfgID);
		}
	}
}

void TipControl::ShowVipLimitTip(string descStr){
	if(!(m_stVipLimitPanel.isInited())) {
		m_stVipLimitPanel.init();
	}

	m_stVipLimitPanel.open();
	m_stVipLimitPanel.updateUI(descStr);
}



