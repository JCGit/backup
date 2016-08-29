#pragma once

#include "Constant.h"
#include "Singleton.h"
#include "net/NetPack.h"
#define  INVALID_CAMP_UIN -111 //��Ч uin
#define  FIGHT_END_NUM -1000
#define PVP_FRO_WAIT_ITEM 0.5f
#define  PVP_END_LOCK_KEY "pvp_end_lock"
#define  PVP_RECOVER_ITEM_KEY "pvp_recover_item_key"
#define  PVP_END_LOCK_TIME 1.0f
#define  PVP_RECOVER_ITEM_TIME 0.5f

enum ePvpStatus
{
	PVP_STATUS_NULL = 0,
	PVP_STATUS_MAP,
	PVP_STATUS_DEAD,
	PVP_STATUS_C_END,		//pvp�ͻ����ж�����
	PVP_STATUS_S_END			//pvp�������ذ��ж�����
};

enum ePvpMode
{
	PVP_MODE_NULL = 0,
	PVP_MODE_RAND,//���
	PVP_MODE_ROOM,//����
	PVP_MODE_NOITEM,//�޵���
};

class PvpProxy;
class PvpControl
{
public:

	PvpControl();
	~PvpControl();

	PvpProxy *	getPvPProxy();

	fgame::CSPvpStartResp*	getStartRsp() { return &mStartRsp; }
	fgame::CSPvpMatchResp*  getMatchRsp() { return &mMatchRsp; }
	fgame::CSPvpEndResp*  getEndRsp() { return &mEndResp; }
	fgame::CSPvpGetAwardResp*  getPvpAwardResp() { return &mPvpAwardResp; }
	int		getWinUidByCamp(int camp);

	void	update(float dt);
	void	clean();

	void	doMatchRsp(const fgame::CSPvpMatchResp& stMsg);
	void	doStartRsp(const fgame::CSPvpStartResp& stMsg);
	void	doPingRsp(const fgame::CSPvpPingResp& stMsg);
	void	doMapRsp(const fgame::CSPvpMapResp& stMsg);
	void	doUpgrade(const fgame::CSPvpUpgradeResp& stMsg);
	void	doFight(const fgame::CSPvpFightResp& stMsg);
	void	doEnd(const fgame::CSPvpEndResp& stMsg);
	void	doLeave(const fgame::CSPvpLeaveResp& stMsg);
	void	doUseItem(const fgame::CSPvpUseItemResp& stMsg);
	void	doStopMatch(const fgame::CSPvpStopMatchResp& stMsg);
	void	doAppearCreep(const fgame::CSPvpAppearCreepResp& stMsg);
	void	doCampDead(const fgame::CSPvpObserverResp& stMsg);
	void	doRecoverItem(const fgame::CSPvpRecoverItemResp& stMsg);
	void	doUseAsstSkill(const fgame::CSPvpUseSkillResp& stMsg);
	void	doNoItemPvpBoxAward(const fgame::CSPvpGetAwardResp& stMsg);
	void	doHeartbeat();
	void	doGlyphLoadPvpSuccess(cocos2d::EventCustom* customData);
	void	doAsstLoadPvpSuccess(cocos2d::EventCustom* customData);
	bool	doErro(int erroCode);

	void	startGame(int type);//������Ϸ
	void	loadGame();//��ʼ��Ϸ
	eCampType getMyCampType();
	eCampType getCampTypeByUid(int uid);
	int		  getUinByCamp(eCampType type);
	int    getPlayerPoint(eCampType);

	bool checkIsAI(eCampType type);

	double getSTime(double ctime);
	double getCTime(double stime);

	CC_SYNTHESIZE(int, mMode, PVPMode);		// 0 for none, 1 for match, 2 for room 3 for no item
	CC_SYNTHESIZE(int, mCMode, PVPCMode);		// ǰ��ѡ����ģʽ
	CC_SYNTHESIZE(bool, mIsWaiing, IsWaiing);
	CC_SYNTHESIZE(bool, mIsDead, IsDead);
	CC_SYNTHESIZE(bool, mIsEnd, IsEnd);
	CC_SYNTHESIZE(int, mWinUin, WinUin);
	CC_SYNTHESIZE(bool, mIsReConne, IsReConne);//�Ƿ�����
	CC_SYNTHESIZE(ePvpStatus, mPvpStatus, PvpStatus);//pvp ״̬
	CC_SYNTHESIZE(bool, mIsHoures, IsHourse);
	CC_SYNTHESIZE(bool, m_lastOwnerIdx, LastOwnerIdx); 	//����Լ���pvpResult�е�idx
	CC_SYNTHESIZE(int , mLastPvpCount, LastPvpCount);
	void setPvpTest(ui::Text* pText);
 	const std::string& getPvpTest();

	//CC_SYNTHESIZE(ui::Text*, m_PvpTest, PvpTest);
	CC_SYNTHESIZE(bool, m_Lock, LockText);

	void setPvpTestStr(std::string text);
	void setPvpTestColor(const Color4B color);

	int getKillPeople(int uin);
	int getProductPeople(int uin);

	std::string getPvpPlayerIconId(eCampType camp);
	std::string getPvpPlayerName(eCampType camp);

	int getPvpType();

	//�Ƿ����޵��߳�
	bool getIsNoItemOpen() {return mIsNoItemOpen ;}
	void setIsNoItemOpen(unsigned long long _time);

protected:

	PvpProxy*	mPvpProxy;
	fgame::CSPvpMatchResp mMatchRsp;
	fgame::CSPvpStartResp mStartRsp;
	fgame::CSPvpMapResp mMapResp;
	int m_ping;
	int m_endCout;
	int m_RecoverItemId;
	double difTime;//������ʱ�� �� ����ʱ���ʱ���

	bool mStartParkComplete;
	bool mGlyphLoadComplete;
	bool mAsstLoadComplete;

	fgame::CSPvpEndResp mEndResp;
	fgame::CSPvpGetAwardResp mPvpAwardResp;
	ui::Text* m_PvpTest;
	
	bool mIsNoItemOpen;
};

#define sgPvpControl() Singleton<PvpControl>::Get()