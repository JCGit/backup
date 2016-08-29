#ifndef LevelControl_h__
#define LevelControl_h__

#include "LevelEnterPanel.h"
#include "CardDrawPanel.h"
#include "ResultPanel.h"
#include "FailPanel.h"
#include "FriendHelpPanel.h"
#include "BossEmergePanel.h"
#include "PausePanel.h"
#include "SweepPanel.h"
#include "SweepInfoPanel.h"
#include "EliteWarEntryPanel.h"
#include "WarSweepInfoPanel.h"
#include "RelivePanel.h"
#include "FillBulletPanel.h"
#include "BuyMedicalPanel.h"

class CLevelControl
{
public:
	CLevelControl();
	~CLevelControl();

	CLevelEnterPanel* getEnterPanel();
	CCardDrawPanel* getCardPanel();
	CResultPanel* getResultPanel();
	CFailPanel* getFailPanel();
	FriendHelpPanel* getFriendHelpPanel();
	CPausePanel* getPausePanel();
	CSweepInfoPanel* getSweepInfoPanel();
	EliteWarEntryPanel* getEliteWarEntryPanel();
	WarSweepInfoPanel* getWarSweepInfoPanel();
	CRelivePanel* getRelivePanel();
	CFillBulletPanel* getFillBulletPanel();
	CBuyMedicalPanel* getBuyMedicalPanel();

	void openBossEmergePanelByCfgID(unsigned int cfgID, CCObject* target, SEL_CallFunc callbackFun);

	void openSweepPanelByLevelID(unsigned int levelID);

	void doQueryFriendLstRsp();  //通过好友列表更新好友助战

	void doInviteFriendFightRsp(const vmsg::CSPlayerDetailInfo& stMsg);

    void releasePanel(); //结束时释放面板资源

    const std::string getWinTypeStr(const int iLevel); //获取获胜条件字符串
private:
	CLevelEnterPanel* pEnterPanel;		//关卡进入面板
	CCardDrawPanel* pCardDrawPanel;		//抽牌面板
	CResultPanel* pResultPanel;			//结算面板
	CFailPanel* pFailPanel;				//失败面板
	
    FriendHelpPanel m_stFriendHelpPanel;	//好友助战面板

	CBossEmergePanel m_stBossEmergePanel;	//boss出现的面板
	CPausePanel m_stPausePanel;			//暂停面板
	CSweepPanel m_stSweepPanel;			//扫荡设置面板
	CSweepInfoPanel m_stSweepInfoPanel;	//扫荡信息面板
	CRelivePanel m_stReilivePanel;		//复活面板
	CFillBulletPanel m_stFillBulletPanel;	//补充弹药面板
	CBuyMedicalPanel m_stBuyMedicalPanel;	//购买医药箱面板

	EliteWarEntryPanel* m_pEliteWarEntryPanel; // 精英副本进入面板
	WarSweepInfoPanel* m_pWarSweepInfoPanel; // 精英扫荡

};

#define P_LEVEL_CTL CVLib::CVSingleton<CLevelControl>::Instance()

#endif // LevelControl_h__