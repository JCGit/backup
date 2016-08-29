#pragma once

#include "Constant.h"
#include "Singleton.h"
#include "DayTaskProxy.h"
#include "net/NetPack.h"
#include "game/data/TableManager.h"
#include "game/Message.h"

class DayMission: public cocos2d::Ref
{
public:
	DayMission();
	~DayMission();
	bool init();
	CREATE_FUNC(DayMission);

	int mId;
	int mValue;
	int mRequestValue;
	bool mIsFinished;
	bool mIsGet;
};

class DayTaskProxy;
class DayTaskDialog;
class DayTaskControl : public MessageHandler
{
public:

	DayTaskControl();
	~DayTaskControl();

	void				setDialog(DayTaskDialog* dialog) { mDialog = dialog; }
	DayTaskDialog*		getDialog() { return mDialog; }
	DayTaskProxy *		getProxy()const  { return mProxy; }
	DayTaskDataList*	getAllMissions();
	int					getAllMissionCount();
	DayMission*			getMissionById(int id);
	void				updateMission(const fgame::CSMission &resqMission);
	void				goMission(int pageId);		
	void				sortMission();					
	bool				checkMissionComplete();           // ����Ƿ����������
	void				checkShowTip();

	void				syncMissionFromServer(const ::fgame::CSMissionListResp &missionListResp);	// ͬ������������(ͬ�����������)
	void				missionUpdateFromServer(const fgame::CSMissionUpdateResp &missionUpdateResp);
	
	cocos2d::Vector<DayMission*>* getMissions() { return &mMissions; }
	// sort mission
	int					getMissionIdByIndex(int cellIndex);
	void				trySyncMission();

	//�¿�ʣ������
	int					getCardDaysLeft();
public:
	virtual void		onReceiveMassage(GameMessage * message);
protected:
	void				refreshDialog(); 
	void				hideDialog();
	
	DayTaskProxy*		mProxy;
	DayTaskDialog*		mDialog;
	cocos2d::Vector<DayMission*> mMissions;

	std::map<int, int> mMissionSortMap;
private:
	// check task usable.
	bool				checkVipDask(int vipLevel);
	int					VipTipsText[3];
	
	int					syncDay;  // ͬ�������б��ʱ��
};

#define sgDayTaskControl() Singleton<DayTaskControl>::Get()