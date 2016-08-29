#ifndef BossEmergePanel_h__
#define BossEmergePanel_h__

#include "BasePanel.h"
using namespace std;

enum {
	SHOW_STATE_INIT,			//չʾ��ʼ��
	SHOW_STATE_ACTING,			//��չʾ��
	SHOW_STATE_END				//չʾ���
};

class CBossEmergePanel : public BasePanel
{
public:
	static const CCPoint initRolePos;
public:
	CBossEmergePanel();
	virtual ~CBossEmergePanel();

	CREATE_FUNC(CBossEmergePanel);

	bool init();

	void openWithCallFun(CCObject* target, SEL_CallFunc callbackFun);
	void open();
	void close();

	void loadArmature();

private:
	void initUI();

	void startAction();

	//������ʾboss�Ľ���
	void showDescByStep();				
	void stepUpdate(float dt);		

	//��ʾboss�˺�λ��
	void setHurtPos(const CCPoint& point);
	void updateHurtArmaturePos();
	void showHurtLocation();
	void showHurtArrow();
	void showHurtDesc();
	void showEnd();

	void showAllImmediately();

	void closeCountDown(float dt);

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void reset();

	CCActionManager* createNewCScheduler(); 
private:
	UIPanel* m_pBossDescLayer;
	UIPanel* m_pTouchLayer;
	UILabel* m_pBossNameTxt;
	UILabel* m_pBossDescTxt;
	ImageView* m_pHurtImg;
	ImageView* m_pHurtDescImg;
	ImageView* m_pLine0;
	ImageView* m_pLine1;
	ImageView* m_pLine2;
	UIPanel* m_pHurtLayer;

	CCArmature* m_pArmature;
	CCArmature* m_pHurtArmature;			//��������ָʾ��Ч
private:
	CC_PROPERTY(unsigned int, _cfgID, CfgID);

	string m_stResPath;

	string m_stDesc;					//�����ļ�
	unsigned int m_uIndex;

	unsigned int m_uShowState;			//չʾ��״̬

	CCPoint m_stDescPos;				//boss���ܵ�λ��
	CCPoint m_stHurtPos;				//boss�˺�����λ��

	unsigned int m_uTime;

	CCObject*    m_pTarget;
	SEL_CallFunc m_pEventSelector;
};

#endif // BossEmergePanel_h__
