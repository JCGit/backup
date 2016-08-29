#pragma once

#include "Singleton.h"
#include "GameData.h"
#include "cocos2d.h"
#include "net/proto/SaveData.pb.h" 
#include "game/data/SyncData.h"
#include "Constant.h"
#include "UserProxy.h"
#include "CVTime.h"
#ifdef MGAME_PLATFORM_MPLUS
#include "mplus/MplusControl.h"
#endif
USING_NS_CC;

class UserObj
{
public:

	UserObj();
	~UserObj(); 

	enum PROPERTYTYPE
	{
		PT_PVEPOWER = 1,		//����
		PT_PVPPOWER,		//����
		PT_DIAMOND,			//��ʯ
		PT_ITEM,			//����
		POWER_SOLT,			//--������
		ENERGY_SOLT,		//--������
		MONTH_CARD,			//--�¿�
		TITLE_ADD,			//--��������
		ROOM_TICKET,		//--����ȯ
		PLAYER_EXP,			//--����ܾ���
		Honor,				//����
		STUFF,				//���Ĳ���
		POINT,				//����ܻ���
		CHAT,				//������Ѵ���
		ASSISTANT = 15,		//����
		GLYPH = 16,			//����

	};

	

	void			sendGetCheckPtData();
	void			sendGetNewBieList();
	void			sendEnterPoint(int pointid);
	void			sendUpdateCheckPoint();
	void            sendDailyMissionData(bool isPvp);                      

	void			doGetCheckPtData(const ::fgame::CSGetCheckPtDataResp&);
	void			doSaveCheckPtData(const fgame::CSSaveCheckPtDataResp&);
	void			doGetGiftReward(const fgame::CSRetriveCheckPtRewardResp&);		//��ȡ���
	void			doGetNewBieList(const ::fgame::CSGetNewBieListResp&);
	void			doGetNewBieReward(const ::fgame::CSPveCheckAwardResp&);			//����ѵ��Ӫ������
	void			doPropertyUpdate(const fgame::CSPropertyUpdateResp&);
	
	void			doEnterPoint();

	void			initUserObj(); 
	void			initPlayerData(); 
	void			loadPlayerData();
	void			saveToFile();
	
	void			update(float dt);
 
	// 	PVE�������
	void			changePvePower(int value);

	// 	PVP�������
	void			changePvpPower(int value);

	//	���ҵ�λ���
	void			setDiamond(int value, int src = 0);

	//  ���������������
	void			setGuideFinish(int groupID); 
	void			syncGuideFinish(std::map<int, bool>& idMap);
	bool			getGuideFinish(int groupID);
	int				getMaxGuideGroup();
	void			sendAddGuideDiamond(int count);
	void			doAddGuideDiamond(int count); 
	void			addShopGuideNum(int value);

	void			sendAddGuideGlyphStuff();
	void			doAddGuideGlyphStuff(int value);
	void			addGlyphStuffGuideNum(int value);

	//���ֱ����������
	void			setBibleTabItemId(int tabItemID);
	int 			getBibleTabItemId();

	//pve����ʧ�ܽ����������
	void			setFirstFail(int failTag);
	int 			getFirstFail();

	//	������� 
	void			setProp(int id, int count);
	int				getProp(int id);  
	void			initProp(const fgame::CSSycPlayerInfoResp&);
	std::map<int, int> getPropList();

	//����ѡ��
	void			setPropItem(int pos, int id);
	void			cleanPropItem(int id);
	int				getPropItem(int pos);
	bool			refreshPropItemData(int pos, int id);
	std::map<int, int> getPropItemList();

	//�ؿ����	
	bool			getIsBranch(int levelID);													//�Ƿ�֧��(�����ӿ�)
	int				getCheckPointStar(int levelID);												//��ȡ������(�����ӿ�)
	int				getTotalStars();															//��ѯ���е�����
	int				getMaxOpenCheckPoint(bool realTime = false);								//��ȡ������ؿ�(�����ӿ�)
	void			updateCheckPoint(int id, int star, int  passTime =0,int openCheckPt = 0);	//���¹ؿ�״̬
	int				getCheckPointState(int id);													//��ȡ�ؿ�״̬(�����ӿ�)
	int				getNowPassCheckPoint();
	int				getNextCheckPt();

	///////////////////////////////begin//////////////////////////////
	int			    getMaxOpenGateID() { return mMaxOpenGateID;  };
	void			setMaxOpenGateID(int gateID) { mMaxOpenGateID = gateID; }
	int				getGateState(int id); 
	void			setGateState(int id, int state);
	void			clearGateState();
	///////////////////////////////end//////////////////////////////
	int             getHasSkipCount();                                                           //��ȡ�������ش���
	void            onSkip(int id);                                                              //����
	void            setIsSkipSuccess(bool isSkip){ mIsSkipSuccess = isSkip; };
	bool            getIsSkipSuccess(){ return mIsSkipSuccess; };

	//������
	bool			getReceiveAward(int pointId ,int type);		//��ѯ�Ƿ���ȡ  type: 1ͨ����� 2�������
	void			setReceiveAward(int pointId,int type);		//�����ȡͨ����� 
	void			getNewBieReward();							//����ѵ��Ӫ������
	//��ʧ�����
	int				getOneLostPointData(int id);								//��ȡ��ʧ������״̬�������ڸ���ʧ�㣺-1 ���ڵ���ͨ����0 ���ڲ���ͨ���� 1
	void			updateLostPointData(int id, bool pass, bool save = true);	//������ʧ��
	bool			refreshLostPointData(int id, int state);
	 
	unsigned int	getUin();
	void			setUin(unsigned int value);

	unsigned int	getNewUin();
	unsigned int 	convertToNewUin(unsigned int Uin);
	unsigned int	convertToUin(unsigned int NewUin);
	std::string		getPlayerName();

	std::string		getOpenID();
	void			setOpenID(std::string value);

	void			setInputID(std::string value);

	//�¿�
	bool			isCanGetMonthCard();				//�Ƿ����¿�

	//��½���
	bool			isInitLoginData();
	void			updateLastServerTime();
	int64_t			getLastServerTime();
	void			setGameServerId(std::string name);
	std::string		getGameServerName();
	void			setLoginServerDir(std::string);
	std::string		getLoginServerDir();

	int				getRankGroup() const;				//��ȡ�������

	void			setScore(int val);					//����pvp���֣�ԭ����setPvpPoint

	int64_t			getNowTime();

	void			addCostMoney(int money);
	bool			isCostMax(int money);
	void			addTestMoney(int money);

	std::string		getPlayerDataStr();
	void			setPlayerDataStr(std::string s1);

	bool			checkNoSavaData();									//����Ƿ��´浵���
	void			setSaveDataOnLine(bool value);						//�ӷ�������ȡ�浵
	void			setSaveDataOffLine(bool value);						//�������ô浵
	
	bool			syncSaveDataServer(fgame::UserData data);			//ͬ���浵
	int				getCurSaveDataVersion();
	void			setCurSaveDataVersion(int value);

	//�������
	void			initBadges(const fgame::CSSycPlayerInfoResp&);
	void			addBadge(int iBadge);
	void			addBadgeItem(std::string strJason);
	std::vector<int> getBadges();
	const std::map<unsigned int, BadgeItem>& getObtainedBadges() const;
	bool			isHaveBadge(int iBadge);
	BadgeItem*		getBadgeItem(int iBadge);

	void			cleanCheckPtDataMap(){ mCheckPtDataMap.clear(); }

	void			addActNoticeData(int id);
	bool			checkIsHaveAllNoticeData();
	bool			checkIsHaveNoticeData(int id);
	void			cleanmActNoticeList() { mActNoticeList.clear(); }
	void			updateActInformData();
	int64_t			getActInformTime();
	void			initVipTime(std::string str);
	CVTimeSpan		getVipTime(int id);
	
	void			updatePlayerExp(string str); 

	bool			loadGlyphStuff(); 
	int				getGlyphStuffNum(int stuffID);

	fgame::CSSycPlayerInfoResp* getPersonalInfo() { return &m_stPersonalInfo; }	//������Ϣ
	std::string		getPersonalString();										//������Ϣ���л���
	
	bool		isOpenGamePassTip();
	void		changeGamePassTip(bool bOpen);

	unsigned int	getChatBuyTip() { return mPlayerData.chatbuytip(); }
	void			setChatBuyTip(unsigned int tip) { mPlayerData.set_chatbuytip(tip); }
	unsigned int	getChatFreeCount() { return m_stPersonalInfo.chatfreecount(); }
	void			setChatFreeCount(unsigned int count) { m_stPersonalInfo.set_chatfreecount(count); }
	int64_t			getChatTimeLeft() { return mPlayerData.chattimeleft(); }
	void			setChatTimeLeft(int64_t left) { mPlayerData.set_chattimeleft(left); }

	// ������ʾ�����ID
	CC_SYNTHESIZE(int, mShowGiftId, ShowGiftId);
	void			showGiftInfoById(int gift_id);

	CC_SYNTHESIZE(int, mOccupyModeFlag, OccupyModeFlag);  
	CC_SYNTHESIZE(bool, mHaveGlyph, HaveGlyph);
	// ��¼���
	void			addLoginSrv2List(int channelId, const std::string& address);
	void			dltLoginSrvFromList(int channelId, const std::string& address);
	std::string		sltLoginSrvRand(int channelId);

	fgame::UserData* getSaveData() { return &mPlayerData; }


	//ÿ���������
	typedef std::map<int, Point> DailyMissionTips;
	void clearDailyMissionTips(){ mDailyMissionTipList.clear(); }
	void addDailyMissionTips(int type, Point pos);//type--0:1��ָ�� >0��2��ָ������
	Point getDailyMissionTipPointByType(int type);

	void setIsShowDailyMissionBtn(bool isShow){ mIsShowDailyMissionBtn = isShow; };
	bool getIsShowDailyMissionBtn(){ return mIsShowDailyMissionBtn; };

	void setIsAfterAwardOpenAgain(bool isOpen){ mIsAfterAwardOpenAgain = isOpen; };
	bool getIsAfterAwardOpenAgain(){ return mIsAfterAwardOpenAgain; };

	void setIsShowDailyMissionProgress(bool isShow){ mIsShowDailyMissionProgress = isShow; };
	bool getIsShowDailyMissionProgress(){return mIsShowDailyMissionProgress; };

	void setIsOpenDailyMissionFirst(bool isShowFirst){ mIsOpenDailyMissionFirst = isShowFirst; };
	bool getIsOpenDailyMissionFirst(){ return mIsOpenDailyMissionFirst; };

	void setIsOpenDailyMissionDialog(bool isOpen){ mIsOpenDailyMissionDialog = isOpen; };
	bool getIsOpenDailyMissionDialog(){ return mIsOpenDailyMissionDialog; };

	void resetDailyMissionData();

	bool getIsGoneNewerProtect();
protected:	 
	std::string mOpendID;
	CC_SYNTHESIZE(bool, mIsHost, IsHost);							//�Ƿ���pvp����
	CC_SYNTHESIZE(unsigned int, mWaitNum, WaitNum);
	CC_SYNTHESIZE(bool, mLvlUpgrade, LvlUpgrade);					//�����������Ҫ������������
	CC_SYNTHESIZE(int, mPlayerUpgradeState, PlayerUpgradeState);	//��Ҳ���������������״̬
	CC_SYNTHESIZE(bool, mIsShopBuy, IsShopBuy);
				

	CC_SYNTHESIZE(int, mWinnerType, WinnerType);					//lua��ȡ����֮·��ҳǩ 0,1,2
	CC_SYNTHESIZE(bool, mIsOpenCrazyWeek, IsOpenCrazyWeek);			//lua��ȡ���ܿ���

	//CC_SYNTHESIZE(int, mPlayerWinCount, PlayerWinCount);			//���ʤ������
	//CC_SYNTHESIZE(int, mPlayerConWinCount, PlayerConWinCount);		//�����ʤ����

	bool mIsNewSavaData;											//�Ƿ����´浵���ݱ��

	typedef std::map<int, int> LostPointMap;
	//��ʧ��洢
	LostPointMap m_LostPointDataMap;

	//�����id�洢
	std::vector<int> mActNoticeList;

	typedef std::map<int, int> PropItemMap;
	PropItemMap mPropItemMap;
	
	typedef std::map<int, CVTimeSpan> VipTime;
	VipTime mVipTime;

	//�����require�ֶ�һ��Ҫ��ʼ��!!!
	fgame::UserData mPlayerData;

	//��õĻ���List
	std::vector<int> mBadgeGetVec;
	std::map<unsigned int, BadgeItem> mObtainedBadges;

	bool mSendGuideRqst;

	CC_SYNTHESIZE(std::string, mHeadIconUrl, HeadIconUrl);//sdkͷ��URL
private: 
	void	    onLuaGetGlyphStuff(lua_State* tolua_S, int argc);
	bool		loadDataFromFile(std::string path);
	void 		savePlayerData();
	bool		saveTempPlayerData(std::string path);


	void		loadPropItem();
	void		savePropItem();
	

	void		loadLostPointData();
	void		saveLostPointData();

	void		loadActNoticeData();
	void		saveActNoticeData();
 
	bool		isInitCostData();
	void		onGetSaveDataFinish(cocos2d::Ref* ref);

private:
	UserProxy mUserProxy;

	std::map<int, int > mGlyphStuffList;

	std::map<int ,fgame::CheckPtData> mCheckPtDataMap;		//��ͨ�عؿ���Ϣ
	
	int mOpenCheckPtId;										//���ŵ����ؿ�

	std::map<int, int> mGuideGateDataMap;					//�����عؿ���Ϣ
	fgame::CSGetNewBieListResp mGetNewBieListResp;			//����ɵ���������
	int mMaxOpenGateID;
	bool mIsSkipSuccess;     //��¼���سɹ�

	typedef std::map<int, int> PropMap;
	PropMap mPropMap;

	unsigned int     mUin;
	unsigned int	 mNewUin;
	int mNowPassPt;
	int mNowPassStar;
	int mNowPassTime;
	int mOpenPt;
	bool mIsWin;
		 
	CC_SYNTHESIZE(unsigned int, mCurInfoUin, CurInfoUin);	//��ҵ�ǰ����ĸ�����Ϣ��uin
	CC_SYNTHESIZE(int, mCurInfoType, CurInfoType);			//��ҵ�ǰ����ĸ�����Ϣ��type

	fgame::CSSycPlayerInfoResp	m_stPersonalInfo;			//������Ϣ
	int				getScore(){ return m_stPersonalInfo.score(); };			//������а����
	std::string					m_szPersonalString;			//������Ϣ���л���

	DailyMissionTips mDailyMissionTipList;
	bool mIsShowDailyMissionBtn;
	bool mIsAfterAwardOpenAgain;
	bool mIsShowDailyMissionProgress;
	bool mIsOpenDailyMissionFirst;
	bool mIsOpenDailyMissionDialog;//12��ˢ��
	CC_SYNTHESIZE(bool, mIsShowDailyMissionHot, IsShowDailyMissionHot);

	CC_SYNTHESIZE(std::string, mChannelPlayerName, ChannelPlayerName);	//��������û���
};

#define sgUserObj()		Singleton<UserObj>::Get()
