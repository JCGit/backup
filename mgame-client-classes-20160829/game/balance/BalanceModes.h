#pragma once

#include "Message.h"

class BaseMode : public MessageHandler
{
public:

	BaseMode(){}
	virtual ~BaseMode(){}

	virtual bool init();
	virtual void update();

	virtual void onReceiveMassage(GameMessage * message);
	virtual eBalanceTag getModeType() = 0;
	virtual void	start(int cp);
	virtual void	doFail();
	virtual void	doSuccess();

	//@return �Ƿ��Ѿ�����
	virtual bool	doPvpFail();
	//@return �Ƿ��Ѿ�����
	virtual bool	doPvpSuccess();

	int getCheckPointIndex() const { return mCheckPt; }

	bool hasTimeLimit();
	float getTimeLeft();

	void clearData();
	void starGameTime();

protected:

	void		getBuildListSize();
	void		getFormationListSize();


	int		mEneBuildSize;
	int		mEneFormationSize;

	int		mMineBuildSize;
	int		mMineFormationSize;

	int		mCheckPt;
};

// ����ģʽ
class ModeDestory : public BaseMode
{
public:
	virtual bool init();
	virtual void onReceiveMassage(GameMessage * message);
	virtual eBalanceTag getModeType(){ return BALANCE_TAG_DESTORY; }

	//@return �Ƿ��Ѿ�����
	virtual bool	doPvpFail();
	//@return �Ƿ��Ѿ�����
	virtual bool	doPvpSuccess();

protected:
	 
	void	getTargetBuildsSize();
	std::vector<int>	TagBuildList;
	int m_PlayerOccupyTags;
	int m_NonPlayerOccupyTags;
	std::map<int, eCampType>	TagBuildMap;
	bool mOneOccupy;
	eCampType mOwnCamp;
};



// ռ��ģʽ
class ModeOccupy : public BaseMode
{
public:
	virtual bool init();
	virtual void onReceiveMassage(GameMessage * message);
	virtual eBalanceTag getModeType(){ return BALANCE_TAG_OCCUPY; }

	//@return �Ƿ��Ѿ�����
	virtual bool	doPvpFail();
	//@return �Ƿ��Ѿ�����
	virtual bool	doPvpSuccess();

protected:

	void	getTargetBuildsSize();
	std::vector<int>	TagBuildList;
	bool mOneOccupy;
	eCampType mOwnCamp;
};


//  ����ģʽ
class ModeSurvival : public BaseMode
{
public:
	virtual bool init();
	virtual void update();
	virtual void onReceiveMassage(GameMessage * message);
	virtual eBalanceTag getModeType(){ return BALANCE_TAG_SURVIVAL; }
};

//��Դģʽ
class ModeResoure :public BaseMode
{
public:
	virtual bool init();
	virtual void update();
	virtual void onReceiveMassage(GameMessage * message);
	virtual eBalanceTag getModeType(){ return BALANCE_TAG_RESOURCE; }
	virtual void	doFail();


protected:
	void	getTargetBuildsSize();
	void	doEndMode(eCampType eSucc);
	// ��ͼ�еĻ��ֽ���
	std::map<int, std::string>	ScoreBuildList;
};

//�ػ�ģʽ
class  ModeGuard :public BaseMode
{
public:
	virtual bool init();
	virtual void update();
	virtual void onReceiveMassage(GameMessage * message);
	virtual eBalanceTag getModeType(){ return BALANCE_TAG_GUARD; }
protected:

	void	getTargetBuildsSize();
	std::vector<int>	TagBuildList;
};