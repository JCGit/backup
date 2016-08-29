/*
* ͨ�ء����ǹ���
*/
#pragma once

#include "Singleton.h"
#include "Constant.h"
#include "message.h"

class BaseMode;
class BaseStar;
class StarFactory;

class BalanceControl : public MessageHandler
{
public:
	typedef std::map<int, BaseStar*>	StarList;
	typedef std::map<eBalanceScore, int> ScoreList;

public:

	BalanceControl();
	~BalanceControl();

	void	update();
	void	startCheckPoint(int cp);
	// @bReturn ��Ϸ�ڵ�����أ���������
	void	doCheckPtEnd(bool bSuccess, bool bReturn = false);
	BaseMode* getMode() const { return mCurMode; }

	float getTimeLeft() const;

	// ��ȡ��ǰ�ؿ��õ���������
	int		getStarNum();

	const StarList& getStarList();

	void clean();

	void	initCheckPtStarList(int cp);

	virtual void onReceiveMassage(GameMessage * message);

	BaseStar* getTargetStar(int tag);

	void starGameTime();

	void addScore(eBalanceScore type, int scroe);
	int getScore(eBalanceScore type);
	int getTimeScore();
	int getPropScore();

	int getGameScore();

	long long getBattleTime();

protected:

	// �ؿ�ʧ��ʱ��������������δ���
	void	doCheckPtFail();

	// ���ǽ��������Ǵ洢
	void	doSaveStars();

	BaseMode*	mCurMode;
	// ��ǰ�ؿ��Ƿ�ʤ��
	bool	mCurSuccess;

	// ͨ��ģʽ
	typedef std::map<eBalanceTag, BaseMode*> BalanceModeList;
	BalanceModeList mModeList;


	// ���ǹ���
	typedef std::map<int, StarFactory*> StarFactoryList;
	StarFactoryList mStarFactoryList;

	
	StarList	mCPStarList;	// ��ǰ�ؿ������б�
	ScoreList	mScoreList;

private:
	long long mStartTime;
	int mCurCheckPoint;
};

#define sgBalanceControl() Singleton<BalanceControl>::Get()
