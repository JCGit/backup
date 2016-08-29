#ifndef TipControl_h__
#define TipControl_h__

#include "vlib/base/CVSingleton.h"
#include "CommonDialog.h"
#include "GoldShotPanel.h"
#include "TextHelper.h"
#include "weaponlib/ItemDropTipListPanel.h"
#include "weaponlib/WeaponRefitTipPanel.h"
#include "PromptPayPanel.h"
#include "SweepCardShotPanel.h"
#include "VipLimitPanel.h"
#include "ItemLackPanel.h"
#include "LevelNotEnoughPanel.h"
#include "act/GiftTipPanel.h"

using namespace CVLib;
using namespace std;

class TipControl
{
public:
	TipControl();
	virtual ~TipControl();

	//����������ʾ��true����
	//cost����������
	//strDescTxt��Ĭ��Ϊ��ʾ����
	bool GoldCostTip(unsigned int cost, CCObject *target, SEL_BtnEvent costEvent,
		const string& strDescTxt = "", unsigned int param1 = 0, unsigned int param2 = 0);

	//�����������ʾ��true����
	bool CoinCostTip(unsigned int cost, CCObject *target);

	PromptPayPanel* getPromptPayPanel();

    CWeaponRefitTipPanel* getItemInfoTipPanel();

    ItemDropTipListPanel* getItemDropTipListPanel();

	CSweepCardShotPanel* getSweepCardShotPanel();

	ItemLackPanel* getItemLackPanel();

	LevelNotEnoughPanel* getLevelNotEnoughPanel();

	GiftTipPanel* getGiftTipPanel();

	void showItemTipPanel(const int iType, const int iCfgID, const int iNum, bool bIsShowDrop = true,bool bIsGift = false);

	//����������ʾ
	void LowManualBuyTip(CCObject* target, SEL_BtnEvent costEvent);	

	//vip������ʾ
	void ShowVipLimitTip(string descStr);
private:
	void openBuyCoin();

private:
	//���겻�����
	CGoldShotPanel m_stGoldShotPanel;

	PromptPayPanel *m_pPromptPayPanel;

    CWeaponRefitTipPanel m_stRefitTipPanel;

    ItemDropTipListPanel m_stItemDropTipPanel;

	CSweepCardShotPanel m_stSweepCardShotPanel;

	CVipLimitPanel m_stVipLimitPanel;

	ItemLackPanel *m_pItemLackPanel;

	LevelNotEnoughPanel *m_pLevelNotEnoughPanel;

	GiftTipPanel* m_pGiftTipPanel;

};

#define P_TIP_CTRL CVLib::CVSingleton<TipControl>::Instance()

#endif // TipControl_h__
