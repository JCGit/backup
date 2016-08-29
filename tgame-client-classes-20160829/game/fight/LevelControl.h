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

	void doQueryFriendLstRsp();  //ͨ�������б���º�����ս

	void doInviteFriendFightRsp(const vmsg::CSPlayerDetailInfo& stMsg);

    void releasePanel(); //����ʱ�ͷ������Դ

    const std::string getWinTypeStr(const int iLevel); //��ȡ��ʤ�����ַ���
private:
	CLevelEnterPanel* pEnterPanel;		//�ؿ��������
	CCardDrawPanel* pCardDrawPanel;		//�������
	CResultPanel* pResultPanel;			//�������
	CFailPanel* pFailPanel;				//ʧ�����
	
    FriendHelpPanel m_stFriendHelpPanel;	//������ս���

	CBossEmergePanel m_stBossEmergePanel;	//boss���ֵ����
	CPausePanel m_stPausePanel;			//��ͣ���
	CSweepPanel m_stSweepPanel;			//ɨ���������
	CSweepInfoPanel m_stSweepInfoPanel;	//ɨ����Ϣ���
	CRelivePanel m_stReilivePanel;		//�������
	CFillBulletPanel m_stFillBulletPanel;	//���䵯ҩ���
	CBuyMedicalPanel m_stBuyMedicalPanel;	//����ҽҩ�����

	EliteWarEntryPanel* m_pEliteWarEntryPanel; // ��Ӣ�����������
	WarSweepInfoPanel* m_pWarSweepInfoPanel; // ��Ӣɨ��

};

#define P_LEVEL_CTL CVLib::CVSingleton<CLevelControl>::Instance()

#endif // LevelControl_h__