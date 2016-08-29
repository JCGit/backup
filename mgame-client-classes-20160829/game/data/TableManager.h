/* TableManager.h
* jacey
* 20150504
*/
#pragma once

#include "Singleton.h"
#include "Constant.h"
#include "Tables.h"

#include "HelperFunc.h"

#include "net/concurrency.h"
using namespace GameCom;

class CsvLoadTask;
class TableManager : public Ref
{
public:

	TableManager();
	~TableManager();


	virtual void update(float delta);


	void	loadAllTables();
	void	unloadAllTables();

	void	initLoadTables();
	void	loadErrorCodeCsv();
	void	loadPlatformCsv();

	const BuildAttr*		getBuildAttr(int id);
	const BuildIns*			getBuildIns(int); 
	const BuildInsVec		getBuildInsByGateID(int gateID);
	const CheckPoint*		getCheckPointByID(int);
	const CheckPoint*		getMapCPoint(int type, int mapid);
	const CheckPointTable::CheckPointList getCheckPoint(){return mCheckPointTable->getCheckPoint();};
	const MusicInfo*		getMusicInfoByID(int);
	const StateAction*		getStateActionByID(int);
	const TextHelper*		getTextHelperByID(int id);
	const InsistuteAddAttr* getInsistuteAddAttr(int id);  
	const PropInfo *getPropInfoById(int id);
	int  getPropType(int id);
	int getPropIdByType(ePropType type);
	const PropList& getPropInfoList(bool display = false);  

	const AchievementAward* getAchievementAwardByID(AchieveCategory,int);
	const AchievementAward* getAchievementById(int id);
	int getAchievementAwardCount(AchieveCategory);
	bool isAchieveMentAwardComplent(AchieveModule, bool isLast = false);
	void sortAchievementAward(std::map<int, AchievementAward::AchieveInfo>&);
	//const PropInfo*			getPropInfoByType(ePropType type);
	const GuideDataVec		getGuideDataVec();
	const GuideData*		getGuideDataByID(int guideID);
	const JungleDataVec		getJungleDataVec();
	const JungleDataVec		getJungleDataVecByID(int gateID);
	const JungleData*		getJungleDataByID(int buildID);
	std::string				getAnimationNameByCamp(eCampType type);
	std::string				getFlagNameByCamp(eCampType type);
	const AchievePointAward* getAchievePointAwardById(int id);
	const ActLogin* getActLogin(int day);
	std::string				getAnimationNameByExpress(eExpressType type);

	const MoraleDefenceDataVec getMoraleDefenceDataVec();
	const MoraleDefenceData* getMoraleDefenceDataByRange(float moraleValue);
	
	const int getMoraleLevel(float moraleValue);

	const MoraleBuffDataVec getMoraleBuffDataVec();
	const MoraleBuffDataVec getMoraleBuffDataVecByRange(float moraleValue);

	const ShopList& getShopList(){ return mShopTable->getShopList(); };
	const ShopList& getGiftList(){return mShopTable->getGiftList();}
	void                   updateShopInfo(std::map<int,ShopInfo>);
	const ShopInfo* getShopInfoById(int id);

	const ItemBuyList& getItemBuyList(){ return mItemBuyTable->getItemBuyList(); };
	const ItemBuyInfo* getItemBuyById(int id);

	const BuffData*			getBuffDataByID(int buffID);
	int						getWolrdAllStar(int wolrd);

	const CreepsPoint* getCreepsPoint(int id);
	const Creeps* getCreeps(int id);

	const PayLimit* getPayLimit();

	Bible* getBible(int id);
	BibleList* getBibleList();

	const Intelligent* getIntelligent(int id);
	/*
	* @return ���ص�ǰ�ؿ�״̬���ܿ����ĺ����ؿ�id
	* @cpCur ��ǰ�ؿ�
	* @cpStar ��ǰ�ؿ���õ��Ǽ���
	*/
	std::vector<int>	getNextCheckPoints(int cpCur, int cpStar);

	//@return  ��ǰ�ؿ���������
	int getCheckPointStars(int cpCur);

	//����ID��ȡ���
	const GiftAward * getGiftAward(int id);

	//����ID��ȡ��ʤ����
	const WinnerData * getWinnerData(int id);
	int getWinnerDataNum();

	//��ֵ
	RechargeList* getRechargeList();
	RechargeData* getRechargeData(int id);

	//��������
	PowerBuy* getPowerBuy(int id);
	DayTaskData* getDayTaskDataById(int id);
	DayTaskDataList* getDayTaskList();
	NoticeDataVec* getNoticeDataVec();
	NoticeData* getNoticeData(int id);
	void loadNoticeTable();
	
	NewPlayGiftList* getNewPlayGiftList();	
	NewPlayGiftData* getNewPlayGiftDataByType(int type);

	//crazy week
	const CrazyWeekInfo* getCrazyWeekInfoById(int id);

	const ExchangeItem* getExchangeItem(int id);
	int					getExchangeItemCount();
	const GradeData* getGradeData(int point);		//ͨ������Ѱ�Ҷ�λ��Ϣ
	const GradeData* getGradeByGrade(int gradeId);	//ͨ����λ���Ѱ�Ҷ�λ��Ϣ
	VipData* getVipData(int id );

	int getLevelExp(int level);
	//�����̵���Ϣ
	void loadShopTable();
	void loadGuideTable();

	// ��Դ·����
	//RecourcePath::RecourcePathListIterator getRecourcePathIterator();

	PvpChatMap* getPvpChatMap();
	PvpChat* getPvpChat(int id);
	std::vector<PvpChat*>* getChatType(int type);

	std::string getPlatformName(int id);

	const ErrorCode*		getErrorCodeByID(int id);
	void loadActInformTable();
	ActInformList getActInformList();
	ActInformData* getActInformData(int id);
	ActInformData* getActInformDataByIdx(int idx);
	ActInformList getActInformListByType(int iType);

	GlyphStuffData* getGlyphStuffData(int id);
	const GlyphStuffList&  getGlyphStuffMap();

	PlayerInfo *getPlayerInfo(){ return mPlayerInfoTable->getPlayerInfo(); };

	const BadgeData* getBadgeData(int id);
	const BadgeDataTable* getBadgeDataTable() { return mBadgeDataTable; }
	const GoodsInfo *getGoodsInfo(int type, int id = 0);

	SevenDayMission* getSevenDayMissionByDay(int day);
public:

	 

	void setLoadState(eCsvLoadState state){ mLoadState = state; }
	eCsvLoadState getLoadState() { return mLoadState; }

	NameArea* getNameArea(int id) { return mNameAreaTable->getNameArea(id); }
	ScoreInfo* getScoreInfo(int id) { return mScoreInfoTable->getScoreInfo(id); }
protected:
	 

	void	syncLoadCsv();
	void	asyncLoadCsv();

	void queueAllTables();
	void addCsvLoadTask(TableReader* pTargetTable);


	//SingleThreadExecuter mThread;

	eCsvLoadState mLoadState;
	
 	std::list<CsvLoadTask*> mTaskList;
 	CsvLoadTask* mCurrentTask;
	 

private:

	
	BuildAttrTable*		mBuildAttrTable;
	BuildInsTable*		mBuildInsTable; 
	CheckPointTable*	mCheckPointTable;
	MusicTable *		mMusicTable;
	StateActionTable*	mStateActionTable;
	TextHelperTable*	mTextHelperTable;
	InsistuteAddAttrTable* mInsistuteAddAttrTable;
	PropTable* mPropTable;
	GuideDataTable* mGuideDataTable;
	AchievementAwardTable* mAchievementAwardTable;
	GiftTable *mGiftTable;
	AchievePointAwardTable* mAchievePointAwardTable;
	JungleDataTable*			mJungleDataTable;
	MoraleDefenceDataTable*		mMoralDefenceDataTable;
	ActLoginTable*				mActLoginTabel;
	MoraleBuffDataTable*		mMoralBuffDataTable;
	BuffDataTable*				mBuffDataTable;
	WinnerDataTable*			mWinnerDataTable;
	ShopTable*					mShopTable;
	ItemBuyTable*				mItemBuyTable;
	RechargeActivityTable*		mRechargeActivityTable;
	PowerBuyTable*				mPowerBuyTable;
	DayTaskTable*               mDayTaskTable;
	NoticeDataTable*			mNoticeDataTable;
	CreepsPointTable*			mCreepsPointTable;
	CreepsTable*				mCreepsTable;
	NewPlayGiftTable*			mNewPlayGiftTable;
	PvpChatTable*				mPvpChatTable;
	PlatformTable*				mPlatformTable;
	ErrorCodeTable*				mErrorCodeTable;
	PayLimitTable*						mPayLimitTable;
	BibleTable*                 mBibleTable;
	CrazyWeekTable*				mCrazyWeekTable;
	ExchangeItemTable*			mExchangeItemTable;
	GradeDataTable*				mGradeDataTable;
	ActInformTable*				mActInformTable;
	VipDataTable*				mVipDataTable;
	LevelTable*					mLevelTable;
	GlyphStuffTable*			mGlyphStuffTable;
	PlayerInfoTable*            mPlayerInfoTable;
	IntelligentTable*           mIntelligentTable;
	BadgeDataTable*					mBadgeDataTable;
	GoodsInfoTable*				mGoodsInfoTable;
	ScoreInfoTable*				mScoreInfoTable;

	NameAreaTable*				mNameAreaTable;
	SevenDayMissionTable*       mSevenDayMissionTable;
};

#define sgTableManager() Singleton<TableManager>::Get()
#define sgTextHelper(id) sgTableManager()->getTextHelperByID(id)->text
#define sgErrorCode(id) sgTableManager()->getErrorCodeByID(id)->text