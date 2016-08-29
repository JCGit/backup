#pragma once
#include "Singleton.h"
#include "TableReader.h"
#include "IteratorWrapper.h"
#include "inifile.h"
#include "ResManager.h"
#include "cocos2d.h"
#include "GameMaths.h"
#include "StringConverter.h"
#include "json/json.h"
#include <string>
#include <map>
#include <utility>

cocos2d::ccColor3B getFrameColor(unsigned int quality);
cocos2d::CCSprite* getFrameNormalSpirte(unsigned int quality, std::string ext="");
cocos2d::CCSprite* getFrameSelectedSpirte(unsigned int quality, std::string ext="");
cocos2d::CCSprite* getFrameDisabledSpirte(unsigned int quality, std::string ext="");
cocos2d::CCSprite* getContentUnselectedSpirte(std::string ext="");
cocos2d::CCSprite* getContentSelectedSpirte(std::string ext="");

std::string getQualityMaskImageFile(unsigned int quality);

std::string getQualityImageFile(unsigned int quality);

std::string getQualityFramePersonImageFile(void);

std::string getQualityFrameNormalImageFile(void);
std::string getQualityFrameSelectImageFile(void);

std::string getQualityFrameNewNormalImageFile(void);
std::string getQualityFrameNewSelectImageFile(void);

std::string getRecruitNumImageFile(unsigned int digit);//1-9
std::string getVipLevelImageFile(unsigned int level);
std::string getTeamDefaultPic(void);
std::string getTeamLockPic(void);

typedef std::vector<unsigned int > idPoolVec;

class VaribleManager
	: public ConfigFile
	,public Singleton<VaribleManager>
{
public:
	VaribleManager() {reload();};
	static VaribleManager* getInstance();
    void reload(){load("Varible.txt");};
};
// YUANFEN::type	触发条件		机缘前半句文案生成规则
// 	1				与他人同阵营	与XXX齐上阵，
// 	2				佩戴指定技能	装备XXX（技能），
// 	3				佩戴指定装备	装备YYYY(装备)，
// 
// 	YUANFEN::upgradeType	提升属性	机缘后半句文案生成规则
// 	1						血			生命提高N%
// 	2						攻			攻击提高N%
// 	3						防			防御提高N%
// 	4						内			内力提高N%
#define YUANFEN_NUM 2
class YuanfenItem
{
public:	
	unsigned int	itemID;
	std::string		name;
	unsigned int	type;
	std::string		TriggerCondition;
	std::map<unsigned int ,unsigned int> upgradeAttr;	
	std::string		describe;

public:
	unsigned int getUpgradeAttrTypeByIndex(unsigned int index);
	unsigned int getUpgradeAttrValByIndex(unsigned int index);
	unsigned int getUpgradeAttrSize();
private:
	void readline(std::stringstream& _stream);

	friend class YuanfenTableManager;
};

class YuanfenTableManager
	: public TableReader
	,public Singleton<YuanfenTableManager>
{
public:

	YuanfenTableManager()
	{
		init("Yuanfens.txt");
	}

	typedef std::map<unsigned int, YuanfenItem* > YuanfenList;
	typedef ConstMapIterator<YuanfenList> YuanfenListIterator;

	YuanfenListIterator getYuanfenIterator(void) 
	{
		return YuanfenListIterator(mYuanfenList.begin(), mYuanfenList.end());
	}

	void init(const std::string& filename);
	const YuanfenItem * getYuanfenItemByID(unsigned int id);
	const YuanfenItem * getYuanfenItemByName(const std::string& name);

	inline unsigned int getTotalNum() {return mYuanfenList.size();};

	static YuanfenTableManager* getInstance();

private:
	virtual void readline(std::stringstream& _stream);

	YuanfenList mYuanfenList;
};

#define EVOLUTION_MAX_STAGE 7

class DiscipleEvolutionItem
{
public:
	unsigned int	itemID;
	unsigned int	stage;
	unsigned int	potential;
	unsigned int	openTalent;
	unsigned int	needLevel;
	std::string		consumeItemStr;
	std::vector<unsigned int> consumeItems;
	unsigned int	consumeSilver;
	unsigned int	consumeCardCount;
	std::string		consumeTreasureStr;
	std::vector<unsigned int> consumeTreasures;
	unsigned int	addHP;
	unsigned int	addAttack;
	unsigned int	addPhysicsDefence;
	unsigned int	addMagicDefence;

	void readline(std::stringstream& _stream);
	friend class DiscipleEvolutionTableManager;
};

class DiscipleEvolutionTableManager
	: public TableReader
	,public Singleton<DiscipleEvolutionTableManager>
{
public:

	DiscipleEvolutionTableManager()
	{
		init("DiscipleEvolution.txt");
	}

	typedef std::map<unsigned int, DiscipleEvolutionItem* > DiscipleEvolutionList;
	typedef ConstMapIterator<DiscipleEvolutionList> DiscipleEvolutionListIterator;

	DiscipleEvolutionListIterator getDiscipleIterator(void) 
	{
		return DiscipleEvolutionListIterator(mDiscipleEvolutionList.begin(), mDiscipleEvolutionList.end());
	}
	DiscipleEvolutionList getDiscipleList(void){

		return mDiscipleEvolutionList;
	}

	void init(const std::string& filename);
	const DiscipleEvolutionItem* getByStageAndPotential(int stage,int potential);
	inline unsigned int getTotalNum() {return mDiscipleEvolutionList.size();}

private:
	virtual void readline(std::stringstream& _stream);

	DiscipleEvolutionList mDiscipleEvolutionList;
};

#define MAX_YUANFEN_NUM 6
#define MAX_TALENT_NUM 6
class DiscipleItem
{
	DiscipleItem()
	{
		initPhysicDefence = 0;
		initMagicDefence = 0;
		initPhysique = 0;
		initStrength = 0;
		initArmor = 0;
		orderId = 0;
		bornSkillItemID = 0;
	}
public:
	unsigned int	itemID;
	std::string		name;
	unsigned int	type;			//阵营
	unsigned int	quality;		//品质
	unsigned int	potentiality;	//资质
	unsigned int	initExp;
	unsigned int	sellPrice;		//出售银币 
	double			levelupRate;	//升级经验系数 
	unsigned int	skill1;
	unsigned int	skill2;
	unsigned int	initPhysique;	//体质
	unsigned int	initStrength;	//武力
	unsigned int	initArmor;		//护甲
	unsigned int	initHealth;
	unsigned int	initAttack;
	unsigned int	initPhysicDefence;
	unsigned int	initMagicDefence;
	unsigned int	growthHealth;
	unsigned int	growthAttack;
	unsigned int	growthPhysicDefence;
	unsigned int	growthMagicDefence;
	unsigned int	yuanfens[MAX_YUANFEN_NUM];
	unsigned int	talent[MAX_TALENT_NUM];
	std::string		describe;
	bool			showEnable;
	std::string		iconPic;
	std::string		bodyPic;
	std::string		namePic;
	std::string		backPic;
	std::string		battlePic;
	std::string		soundUrl;
	//dmq
	int orderId;
	unsigned int	favorability;

	unsigned int	bornSkillItemID;
	unsigned int	morals;					//1:善；2：恶；3：主角

private:
	void readline(std::stringstream& _stream);	
	friend class DiscipleTableManager;
};

class DiscipleTableManager
	: public TableReader
	,public Singleton<DiscipleTableManager>
{
public:

	DiscipleTableManager()
	{
		init("Disciples.txt");
	}

	typedef std::map<unsigned int, DiscipleItem* > DiscipleList;
	typedef ConstMapIterator<DiscipleList> DiscipleListIterator;

	DiscipleListIterator getDiscipleIterator(void) 
	{
		return DiscipleListIterator(mDiscipleList.begin(), mDiscipleList.end());
	}
	DiscipleList getDiscipleList(void){

		return mDiscipleList;
	}

	void init(const std::string& filename);
	const DiscipleItem * getDiscipleItemByID(unsigned int id);
	const DiscipleItem * getDiscipleItemByName(const std::string& name);

	const std::string getYuanfenDescribe(unsigned int id);
	int mTotalHandbookShownNum;
	unsigned int getHandBookShownTotalNum();
	inline unsigned int getTotalNum() {return mDiscipleList.size();};
	unsigned int getRandomDiscipleId();

	static DiscipleTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);

	DiscipleList mDiscipleList;

	idPoolVec mIdPool;
}; 
//龙魂的本地数据
class DragonSoulItem
{
public:
	int itemID;
	std::string name;
	int quality;
	int qualityLevel;
	std::map<int,int> initAttr;
	std::map<int,int> attrGrouth;
	std::string dynamicPic;
	std::string staticPic;
	int exp;
	int expFactor;
	int exchagePrice;
	int order;
	void readline(std::stringstream& _stream);
};
class DragonSoulTableManager:public TableReader,public Singleton<DragonSoulTableManager>
{
public:
	DragonSoulTableManager()
	{
		init("dragonsoul.txt");
	}
	void init(const std::string& filename);
	const DragonSoulItem* getDragonSoulItemById(int id);
	std::string getCanExchageItemIds();
	inline int getTotalNum()
	{
		return mDragonSoulMap.size();
	}
	static DragonSoulTableManager* getInstance();
private:
	void readline(std::stringstream & _stream);
	std::map<int,DragonSoulItem*> mDragonSoulMap;
	std::vector<int> mIdPool;
};

/*
*宝物相关表格
*/
#define EXT_ATTR_NUM 2
#define REFINE_MAX_STAGE 10
class TreasureItem
{
public:
	unsigned int	itemID;
	std::string		name;
	std::string		describe;
	unsigned int	quality;				//品质
	unsigned int	qualityLevel;			//品级
	unsigned int	type;					//类型
	double			levelupRate;			//升级经验系数，使用的
	unsigned int	baseEatExp;				//基本吞噬经验 
	std::string		baseAttrStr;			//基础属性
	std::vector<unsigned int> baseAtrr;
	std::string		extAttrStr[EXT_ATTR_NUM];			//附加属性
	std::vector<unsigned int> extAttr[EXT_ATTR_NUM];
	std::string		attrFor20Str;						//20解锁属性
	std::vector<unsigned int> attrFor20;				
	std::string		attrFor40Str;						//40级解锁属性
	std::vector<unsigned int> attrFor40;
	std::string		fragmentsStr;						//碎片组成
	std::vector<unsigned int>	fragments;				
	std::string		iconPic;							
	std::string		bodyPic;							
	std::string		namePic;	
	unsigned int	salePrice;
	unsigned int	residentSnatch;
	unsigned int	canUseEquip;
	unsigned int	autoOrder;
	unsigned int	canTreasureRefine;
	unsigned int	canRefine;
	unsigned int	refineMaterialID;
	std::string		attrRefinesStr;						//精炼属性类型
	std::vector<unsigned int>	attrRefines;
	unsigned int	canIntensify;						//可否强化
	void readline(std::stringstream& _stream);
};

class TreasureTableManager
	: public TableReader
	,public Singleton<TreasureTableManager>
{
public:

	TreasureTableManager()
	{
		init("Treasures.txt");
	}

	typedef std::map<unsigned int, TreasureItem* > TreasureList;
	typedef ConstMapIterator<TreasureList> TreasureListIterator;

	TreasureListIterator getTreasureIterator(void) 
	{
		return TreasureListIterator(mTreasureList.begin(), mTreasureList.end());
	}

	void init(const std::string& filename);
	const TreasureItem * getTreasureItemByID(unsigned int id);	
	inline unsigned int getTotalNum() {return mTreasureList.size();};
	unsigned int getRandomTreasureId();
	static TreasureTableManager* getInstance();

private:
	virtual void readline(std::stringstream& _stream);

	TreasureList mTreasureList;
	idPoolVec mIdPool;
};

class TreasureLevelParam
{
public:
	unsigned int	index;
	unsigned int	param;

private:
	void readline(std::stringstream& _stream);	

	friend class TreasureLevelParamManager;
};

class TreasureLevelParamManager
	: public TableReader
	,public Singleton<TreasureLevelParamManager>
{
public:

	TreasureLevelParamManager()
	{
		init("TreasureLevelParam.txt");
	}

	typedef std::map<unsigned int, TreasureLevelParam* > TreasureLevelParamList;
	typedef ConstMapIterator<TreasureLevelParamList> TreasureLevelParamListIterator;

	TreasureLevelParamListIterator getToolIterator(void) 
	{
		return TreasureLevelParamListIterator(mTreasureLevelParamList.begin(), mTreasureLevelParamList.end());
	}

	void init(const std::string& filename);
	unsigned int getParamByLevel(unsigned int level);
	unsigned int getTotalParamByLevel(unsigned int level);
	inline unsigned int getTotalNum() {return mTreasureLevelParamList.size();}

private:
	virtual void readline(std::stringstream& _stream);

	TreasureLevelParamList mTreasureLevelParamList;
};

class TreasurePurify
{
public:
	unsigned int itemID;
	unsigned int quality;	//品质
	unsigned int refineLevel;	//精炼等级
	unsigned int times;			//属性倍数
	unsigned int number;		//材料个数
	unsigned int gold;			//金币

private:
	void readline(std::stringstream& _stream);

	friend class TreasurePurifyManager; 
};

class TreasurePurifyManager
	: public TableReader
	,public Singleton<TreasurePurifyManager>
{
public:
	TreasurePurifyManager()
	{
		init("TreasurePurify.txt");
	};

	typedef std::map<unsigned int, TreasurePurify* > TreasurePurifyList;
	typedef ConstMapIterator<TreasurePurifyList> TreasurePurifyListIterator;

	TreasurePurifyListIterator getToolIterator(void) 
	{
		return TreasurePurifyListIterator(mTreasurePurifyList.begin(), mTreasurePurifyList.end());
	}

	void init(const std::string& filename);
	const TreasurePurify* getTreasurePurifyByID(unsigned int qualityID,unsigned int refineLevel);	
	inline unsigned int getTotalNum() {return mTreasurePurifyList.size();}

	static TreasurePurifyManager* getInstance();

private:
	virtual void readline(std::stringstream& _stream);

	TreasurePurifyList mTreasurePurifyList;
};

/*
*套装相关表格 
*/
#define SUIT_NUM 4
class SuitItem
{
public:
	unsigned int itemID;
	std::string name;
	unsigned int suit[SUIT_NUM];
	std::string addAttrStrFor2;
	std::map<unsigned int ,unsigned int> addAttrFor2;
	std::string addAttrStrFor3;
	std::map<unsigned int,unsigned int> addAttrFor3;
	std::string addAttrStrFor4;
	std::map<unsigned int,unsigned int> addAttrFor4;

	void readline(std::stringstream& _stream);

	friend class SuitTableManager; 
};

class SuitTableManager
	: public TableReader
	,public Singleton<SuitTableManager>
{
public:

	SuitTableManager()
	{
		init("Suits.txt");
	}

	typedef std::map<unsigned int, SuitItem* > SuitList;
	typedef ConstMapIterator<SuitList> SuitListIterator;

	void init(const std::string& filename);
	const SuitItem * getSuitItemByID(unsigned int id);
	unsigned int getSuitNumByID(unsigned int id);
	inline unsigned int getTotalNum() {return mSuitList.size();};
private:
	virtual void readline(std::stringstream& _stream);

	SuitList mSuitList;
};


// EquipItem::Type	装备类别
//部位
//	1 武器
//	2 衣服
//	3 头盔
//	4 腰带
// const std::string Equip1_ImageFile("career/cash.png");
// const std::string Equip2_ImageFile("career/cash.png");
// const std::string Equip3_ImageFile("career/cash.png");
// const std::string& getEquipAtkTypeImageFile(unsigned int type);

const unsigned int EquipQualityNum = 5;
const unsigned int EquipTypeNum = 3;
const unsigned int EquipUpgradeMoneyConsumeFactor[EquipQualityNum][EquipTypeNum] = 
{
	{90,	48,	120},
	{72,	36,	96},
	{54,	24,	72},
	{36,	12,	48},
	{108,	60,	144},
};

const float EquipQualityFactor[EquipQualityNum] = {5.25f, 4.41f, 3.21f, 1.63f, 5.85f};

#define  BAPTIZE_NUM 4


class CompAttr
{  
public:
	bool operator () (const int & ls, const int &rs) const
	{  
		return ls < rs;
	}  
};  

typedef std::map<int ,int,CompAttr> AttrMap;

class EquipItem
{
public:
	unsigned int	itemID;
	std::string		name;
	unsigned int	type;
	unsigned int	quality;			//品质
	unsigned int	qualityLevel;		//品级
	double	strengthConsumeRate;	// 强化消耗系数 
	unsigned int	suitId;				//套装ID
	unsigned int	sellPrice;			//出售价格
	std::string		attrStr;
	AttrMap			initAttr;
	std::string		growthAttrStr;
	AttrMap			grwothAttr;
	std::string		baptizeAttrStr[BAPTIZE_NUM];
	AttrMap			baptizeAttr;
	std::string		describe;
	std::string		iconPic;
	std::string		bodyPic;
	std::string		namePic;
	//dmq
	int orderId;
	//升星
	std::string		starAttr;

	std::vector<unsigned int> starAttrs;

private:
	void readline(std::stringstream& _stream);	
	friend class EquipTableManager;
};

class EquipTableManager
	: public TableReader
	,public Singleton<EquipTableManager>
{
public:

	EquipTableManager()
	{
		init("Equips.txt");
	}

	typedef std::map<unsigned int, EquipItem* > EquipList;
	typedef ConstMapIterator<EquipList> EquipListIterator;

	EquipListIterator getEquipIterator(void) 
	{
		return EquipListIterator(mEquipList.begin(), mEquipList.end());
	}

	void init(const std::string& filename);
	const EquipItem * getEquipItemByID(unsigned int id);
	const EquipItem * getEquipItemByName(const std::string& name);

	inline unsigned int getTotalNum() {return mEquipList.size();};
	unsigned int getRandomEquipId();

	static DiscipleTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);

	EquipList mEquipList;
	idPoolVec mIdPool;
};

class EquipLevelParam
{
public:
	unsigned int	index;
	float			param;

private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>index
			>>param;
	}

	friend class EquipLevelParamManager;
};

class EquipLevelParamManager
	: public TableReader
	,public Singleton<EquipLevelParamManager>
{
public:

	EquipLevelParamManager()
	{
		init("EquipLevelParam.txt");
	}

	typedef std::map<unsigned int, EquipLevelParam* > EquipLevelParamList;
	typedef ConstMapIterator<EquipLevelParamList> EquipLevelParamListIterator;	

	void init(const std::string& filename);
	const float getParamByLevel(unsigned int level);

	inline unsigned int getTotalNum() {return mEquipLevelParamList.size();};

	static EquipLevelParamManager* getInstance();

private:
	virtual void readline(std::stringstream& _stream);

	EquipLevelParamList mEquipLevelParamList;
};

class EquipEnhanceSilver
{
public:
	unsigned int	index;
	unsigned int	param;

private:
	void readline(std::stringstream& _stream);	

	friend class EquipEnhanceSilverTableManager;
};

class EquipEnhanceSilverTableManager
	: public TableReader
	,public Singleton<EquipEnhanceSilverTableManager>
{
public:
	EquipEnhanceSilverTableManager()
	{
		init("EquipStrengthSilver.txt");
	}

	typedef std::map<unsigned int, EquipEnhanceSilver* > EquipEnhanceSilverList;
	typedef ConstMapIterator<EquipEnhanceSilverList> EquipEnhanceSilverListIterator;	

	void init(const std::string& filename);
	float getParamByLevel(unsigned int level);

	inline unsigned int getTotalNum() {return mEquipEnhanceSilverList.size();};

private:
	virtual void readline(std::stringstream& _stream);

	EquipEnhanceSilverList mEquipEnhanceSilverList;
};

class EquipFragmentItem
{
public:
	unsigned int itemID;
	std::string pieceTarget;
	std::string pieceConsume;
	unsigned int pieceRate;
private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>itemID
			>>pieceTarget
			>>pieceConsume
			>>pieceRate;
	}

	friend class EquipFragmentManager;
};



class EquipFragmentManager
	: public TableReader
	, public Singleton<EquipFragmentManager>
{
public:

	EquipFragmentManager()
	{
		init("equipmentFragment.txt");
	}

	typedef std::map<unsigned int, EquipFragmentItem* > EquipFragmentItemList;
	typedef ConstMapIterator<EquipFragmentItemList> EquipFragmentItemListIterator;

	EquipFragmentItemListIterator getToolIterator(void) 
	{
		return EquipFragmentItemListIterator(mEquipFragmentItemList.begin(), mEquipFragmentItemList.end());
	}

	void init(const std::string& filename);
	bool isForgingMap(unsigned int itemID);
	const EquipFragmentItem* getItemByID(unsigned int itemID);
	const std::string getRequirementByItemID(unsigned int itemID, bool lineBreak = false);

	static EquipFragmentManager* getInstance();
private:
	void readline(std::stringstream& _stream);
	EquipFragmentItemList mEquipFragmentItemList;
};

//装备升星属性-----------------------------------------------------------------------------
class StarAttrItem
{
public:
	unsigned int	starAttrID;//星属性id
	std::string		name;//星属性名称
	std::string		describe;//星属性介绍

private:
	void readline(std::stringstream& _stream);	
	friend class StarAttrTableManager;
};
//升星表管理
class StarAttrTableManager
	: public TableReader
	,public Singleton<StarAttrTableManager>
{
public:

	StarAttrTableManager()
	{
		init("starattr.txt");
	}

	typedef std::map<unsigned int, StarAttrItem* > StarAttrList;
	typedef ConstMapIterator<StarAttrList> StarAttrListIterator;

	StarAttrListIterator getStarAttrIterator(void) 
	{
		return StarAttrListIterator(mStarAttrList.begin(), mStarAttrList.end());
	}

	void init(const std::string& filename);
	const StarAttrItem * getStarAttrItemByID(unsigned int id);
	const StarAttrItem * getStarAttrItemByName(const std::string& name);

	inline unsigned int getTotalNum() {return mStarAttrList.size();};
	unsigned int getRandomStarAttrId();

	static DiscipleTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);

	StarAttrList mStarAttrList;
	idPoolVec mIdPool;
};
//---------------------------------------------------------------------------------------

//升星等级表---------------------------------------------------------------------------------------
class StarLevelItem
{
public:
	unsigned int	starLevel;//升星等级
	unsigned int	levelNeed;//升星的强化等级限制
	unsigned int	creamCost;//升星消耗碎片
	unsigned int	goldCost;//升星金钱消耗
	unsigned int    sympathyAttr;//共鸣属性等级
	std::string		sympathyDescribe;//共鸣属性介绍
	std::string		iconEffect;//图标特效
	std::string		sympathyEffect;//共鸣特效

private:
	void readline(std::stringstream& _stream);	
	friend class StarLevelTableManager;
};
//升星等级表管理
class StarLevelTableManager
	: public TableReader
	,public Singleton<StarLevelTableManager>
{
public:

	StarLevelTableManager()
	{
		init("starlevel.txt");
	}

	typedef std::map<unsigned int, StarLevelItem* > StarLevelList;
	typedef ConstMapIterator<StarLevelList> StarLevelListIterator;

	StarLevelListIterator getStarLevelIterator(void) 
	{
		return StarLevelListIterator(mStarLevelList.begin(), mStarLevelList.end());
	}

	void init(const std::string& filename);
	const StarLevelItem * getStarLevelItemByLevel(unsigned int id);

	inline unsigned int getTotalNum() {return mStarLevelList.size();};
	unsigned int getRandomStarLevelId();

	static DiscipleTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);

	StarLevelList mStarLevelList;
	idPoolVec mIdPool;
};
//---------------------------------------------------------------------------------------

//装备上限扩充
class ExtEquipBagInfo
{
public:
	//type	bagLimit	initCount	extStep	costInit	costStep	costLimit
	unsigned int type;
	unsigned int bagLimit;
	unsigned int initCount;
	unsigned int extStep;
	unsigned int costInit;
	unsigned int costStep;
	unsigned int costLimit;
private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>type
			>>bagLimit
			>>initCount
			>>extStep
			>>costInit
			>>costStep
			>>costLimit;
	}

	friend class ExtEquipBagManager;
};

class ExtEquipBagManager
	: public TableReader
	, public Singleton<ExtEquipBagManager>
{
public:

	ExtEquipBagManager()
	{
		init("extEquipBag.txt");
	}

	typedef std::map<unsigned int, ExtEquipBagInfo* > ExtEquipBagItemList;
	typedef ConstMapIterator<ExtEquipBagItemList> ExtEquipBagItemListIterator;

	ExtEquipBagItemListIterator getToolIterator(void) 
	{
		return ExtEquipBagItemListIterator(mExtEquipBagItemList.begin(), mExtEquipBagItemList.end());
	}

	void init(const std::string& filename);
	const ExtEquipBagInfo* getItemByID(unsigned int itemID);

	static ExtEquipBagManager* getInstance();
private:
	void readline(std::stringstream& _stream);
	ExtEquipBagItemList mExtEquipBagItemList;
};

enum ItemOperationType
{
	ItemOperationType_Disciple = 3,			//武将
	ItemOperationType_Treasure = 4,			//宝物
	ItemOperationType_Equip = 5,			//装备
};

class ItemOperation
{
public:
	unsigned int quality;			//品质
	unsigned int type;				//类型
	bool canRefinery;				//可否炼化
	bool canReborth;				//可否重生
	bool canSell;					//可否出售
public:
	void readline(std::stringstream& _stream);
};

class ItemOperationTableManager
	: public TableReader
	,public Singleton<ItemOperationTableManager>
{
public:

	ItemOperationTableManager()
	{
		init("ItemOperation.txt");
	}

	typedef std::vector<ItemOperation* > ItemOperationList;
	typedef ConstMapIterator<ItemOperation> ItemOperationIterator;	

	void init(const std::string& filename);
	//根据品质和类型获取操作类型
	ItemOperation * getItemOperation(unsigned int quality,unsigned int type);

	inline unsigned int getTotalNum() {return mItemOperationList.size();};

private:
	virtual void readline(std::stringstream& _stream);

	ItemOperationList mItemOperationList;
};


class SkillItem
{
public:
	typedef std::map<std::string, std::pair<std::string, std::string> > REPLACE_SPRITE_MAP;
	REPLACE_SPRITE_MAP mReplaceMap;
	enum
	{
		TYPE_SINGLE = 1,
		TYPE_MULTIPLE = 2,
		TYPE_BUFF_MIN = 3,		//buff type start
		TYPE_BUFF_HEALTH = 3,
		TYPE_BUFF_ATTACK = 4,
		TYPE_BUFF_DEFENSE = 5,
		TYPE_BUFF_POWER = 6,
		TYPE_BUFF_MAX = 6		//buff type end
	};
	enum
	{
		QUALITY_BEST = 1,
		QUALITY_GOOD = 2,
		QUALITY_NORMAL = 3,
		QUALITY_BAD = 4
	};
	unsigned int		itemID;
	std::string		name;
	unsigned int	quality;
	unsigned int	type;
	std::string		describe;
	float	initQuantity;
	double	initGrowthRate;
	std::string		iconPic;
	std::string		bodyPic;
	std::string		namePic;
	//std::string		backPic;
	float			duration;
	std::string		ccbFile;
	std::string		timeLine;
	std::string		soundUrl;
	//dmq
	int orderId;
private:
	void readline(std::stringstream& _stream);

	friend class SkillTableManager;
};

class SkillTableManager
	: public TableReader
	,public Singleton<SkillTableManager>
{
public:

	SkillTableManager()
	{
		init("Skills.txt");
	}

	typedef std::map<unsigned int, SkillItem* > SkillList;
	typedef ConstMapIterator<SkillList> SkillListIterator;

	SkillListIterator getSkillIterator(void) 
	{
		return SkillListIterator(mSkillList.begin(), mSkillList.end());
	}

	void init(const std::string& filename);
	const SkillItem * getSkillItemByID(unsigned int id);
	const SkillItem * getSkillItemByName(const std::string& name);

	//const std::string getSkillQualityFramePic();
	//const unsigned int getSkillValue();

	inline unsigned int getTotalNum() {return mSkillList.size();};
	unsigned int getRandomSkillId();
	
	static SkillTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);

	SkillList mSkillList;
	idPoolVec mIdPool;
};

class FragmentBookTable
{
public:
	class FragmentItem
	{
	public:
		unsigned int	fragmentId;
		unsigned int	fragmentType;//碎片类型，9为宝物碎片，8为装备碎片
		unsigned int	itemType;//物品类型，4为宝物，5为装备
		unsigned int	quanlity;//品质
		unsigned int	itemId;//所合成物品ID
		std::string		name;
		std::string		iconPic;
		unsigned int	mergeConsumeTimes;//合成消耗
		std::string		vsNpcHint;
		std::string		vsPersonHint;
		unsigned int	salePrice;
	private:
		void readline(std::stringstream& _stream)
		{
			_stream
				>>fragmentId
				>>name
				>>quanlity
				>>fragmentType
				>>itemType
				>>itemId
				>>mergeConsumeTimes
				>>vsNpcHint
				>>vsPersonHint
				>>iconPic
				>>salePrice;
		}

		friend class FragmentBookTableManager;
	};
	typedef std::map<unsigned int, FragmentItem* > FragmentItemList;
	typedef ConstMapIterator<FragmentItemList> FragmentListIterator;
	FragmentItemList mFragmentItemList;
	inline unsigned int getFragmentBookItemSize(){return mFragmentItemList.size();};
	friend class FragmentBookTableManager;
	friend class FragmentBookPage;
};

class FragmentBookTableManager
	: public TableReader
	,public Singleton<FragmentBookTableManager>
{
public:

	FragmentBookTableManager()
	{
		init("FragmentBooks.txt");
	}
	
	//存放大类型（装备、宝物）对应的物品拼合时所需要的碎片部件列表
	typedef std::map<unsigned int,FragmentBookTable* > FragmentBookTableItemList; //key:物品编号,value:物品 对应fragment成员列表
	//存放所有碎片实体，包含装备、宝物等全部碎片
	typedef std::map<unsigned int, FragmentBookTable::FragmentItem* > FragmentBookItemList;//key:碎片编号,value:碎片实体

	typedef std::map<unsigned int,FragmentBookTableItemList* > FragmentBookTableItemTypeList;//key:类别,value:该类别物品对应的碎片列表

	void init(const std::string& filename);

	inline unsigned int getTotalNum(){return mFragmentBookItemList.size();};

	FragmentBookTable* getFragmentBookTableByTypeAndItemId(unsigned int _itemType,unsigned int itemId);
	FragmentBookTable::FragmentItem* getFragmentItemByFragmentId(unsigned int id);
	FragmentBookTableItemList* getFragmentBookTableItemListByItemType(unsigned int _itemType);
	unsigned int getRandomFragmentId();
	static FragmentBookTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	FragmentBookTableItemTypeList mItemFragmentBookList;
	FragmentBookItemList mFragmentBookItemList;
	idPoolVec mIdPool;
};



class RandomRewardItem
{
public:
	unsigned int	id;
	unsigned int	poolIndex;
	unsigned int	itemType;
	unsigned int    itemId;
	unsigned int	quantity;
private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>id
			>>poolIndex
			>>itemType
			>>itemId
			>>quantity;
	}

	friend class RandomRewardTableManager;
};

class RandomRewardTableManager
	: public TableReader
	,public Singleton<RandomRewardTableManager>
{
public:

	RandomRewardTableManager()
	{
		init("RandomRewardPoolConfig.txt");
	}

	typedef std::map<unsigned int, RandomRewardItem* > RandomRewardList;
	typedef ConstMapIterator<RandomRewardList> RandomRewardListIterator;

	RandomRewardListIterator geRandomRewardListIterator(void) 
	{
		return RandomRewardListIterator(mRandomRewardList.begin(), mRandomRewardList.end());
	}

	void init(const std::string& filename);
	const RandomRewardItem * getRandomRewardItemByID(unsigned int id);
	inline unsigned int getTotalNum() {return mRandomRewardList.size();};

	const RandomRewardItem * randomReward();

	static RandomRewardTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	RandomRewardList mRandomRewardList;
	idPoolVec mIdPool;
};

class DisplePool{
public:
	unsigned int	displeID;
	std::string sDisplayPool;//招将预览
	std::vector<unsigned int> vecDisplePool;
	//根据ID,TYPE，判断该弟子是否在池中，如弟子1 对应vecDisplePool 为 1,2， 则该弟子只能在1,2号池中
	bool checkDispleInPoolByType(int type);
private:
	void readline(std::stringstream& _stream);
	
	friend class DisplePoolManager;
};

class DisplePoolManager
	: public TableReader
	, public Singleton<DisplePoolManager>
{
public:

	DisplePoolManager()
	{
		init("disciplePool.txt");
	}

	typedef std::map<unsigned int, DisplePool* > DisplePoolList;
	typedef ConstMapIterator<DisplePoolList> DisplePoolListIterator;

	DisplePoolListIterator getDisplePoolIterator(void) 
	{
		return DisplePoolListIterator(mDisplePoolList.begin(), mDisplePoolList.end());
	}

	void init(const std::string& filename);
	const DisplePool* getItemByID(unsigned int itemID);

	static DisplePoolManager* getInstance();
private:
	void readline(std::stringstream& _stream);
	DisplePoolList mDisplePoolList;
};


class ToolItem
{
public:
	unsigned int	itemID;
	std::string		name;
	std::string		describe;
	unsigned int	price;
	unsigned int	priceType;
	unsigned int	operationType;
	std::string		jumpPage;
	std::string		buttonString;
	std::string		iconPic;
	std::string		bodyPic;
	std::string		namePic;
	unsigned int	originPrice;
	int				pageID;
	int				buyCountLimit;
	std::string		includeStr;
	ResInfoMap		includeOther;
	unsigned int	isStone;//是否宝石，没用，暂时已经去掉
	unsigned int	showInBag;
	int quality;//品质
	int priceAddFactor;//价格增长
	unsigned int maxPrice;//最大价格
	std::string buyLimitTimePerDay;//每日限购次数
	unsigned int salePrice;//出售价格
	unsigned int existType;//存在类型，1为自动使用，0为普通
	std::vector<unsigned int> vecBuyLimitPerDay;
	unsigned int goodWill; //好感度
	
	int displayPackage; // 显示方式，用来点击查看后是显示海报还是礼包preview
	int shopOrderID; // 商店排序ID
	int getLimitPerDayByVipLevel(int vipLevel);

	//如果每日限购为-1或者0，则为不限购，否则，按照VIP等级算出限购次数
	bool isNotBuyLimit();
	int orderId;
private:
	void readline(std::stringstream& _stream);

	friend class ToolTableManager;
	friend class ToolsExtendTableManager;
};



class ToolTableManager
	: public TableReader
	,public Singleton<ToolTableManager>
{
public:

	ToolTableManager()
	{
		//init("Tools.txt");//显示init
	}

	typedef std::map<unsigned int, ToolItem* > ToolList;
	typedef ConstMapIterator<ToolList> ToolListIterator;

	ToolListIterator getToolIterator(void) 
	{
		return ToolListIterator(mToolList.begin(), mToolList.end());
	}

	void init(const std::string& filename);
	void initIncludeOther();
	ToolItem * getToolItemByID(unsigned int id);
	const ToolItem * getToolItemByName(const std::string& name);
	void  updateToolItemInfo(ToolItem* toolItem);
	void  removeToolItemInfoById(unsigned int itemId);
	inline unsigned int getTotalNum() {return mToolList.size();};

	unsigned int getRandomToolId();
	
	static ToolTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);

	ToolList mToolList;
	idPoolVec mIdPool;
};
//add by daimeiqi 20131218 武道星
class ToolsExtendTableManager
	: public TableReader
	,public Singleton<ToolsExtendTableManager>
{
public:
	ToolsExtendTableManager()
	{
		//init("Tools.txt");
	}

	typedef std::map<unsigned int, ToolItem* > ToolList;
	typedef ConstMapIterator<ToolList> ToolListIterator;

	ToolListIterator getToolIterator(void) 
	{
		return ToolListIterator(mToolList.begin(), mToolList.end());
	}
	void init(const std::string& filename);
	void clearList();
	static ToolsExtendTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);

	ToolList mToolList;
};

class DisciplesLevelParam
{
public:
	unsigned int	index;
	unsigned int	param;

private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>index
			>>param;
	}

	friend class DisciplesLevelParamManager;
};

class DisciplesLevelParamManager
	: public TableReader
	,public Singleton<DisciplesLevelParamManager>
{
public:

	DisciplesLevelParamManager()
	{
		init("DisciplesLevelParam.txt");
	}

	typedef std::map<unsigned int, DisciplesLevelParam* > DisciplesLevelParamList;
	typedef ConstMapIterator<DisciplesLevelParamList> DisciplesLevelParamListIterator;

	DisciplesLevelParamListIterator getToolIterator(void) 
	{
		return DisciplesLevelParamListIterator(mDisciplesLevelParamList.begin(), mDisciplesLevelParamList.end());
	}

	void init(const std::string& filename);
	const float getParamByLevel(unsigned int level);
	unsigned int getTotalParamByLevel(unsigned int level);

	inline unsigned int getTotalNum() {return mDisciplesLevelParamList.size();};

	static DisciplesLevelParamManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);

	DisciplesLevelParamList mDisciplesLevelParamList;
};

class RefineDescItem
{
public:
	unsigned int	itemID;
	unsigned int	type;	//1:炼化；2::重生
	std::string		describe;

public:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>itemID
			>>type
			>>describe;
	}
};

class RefineDescTableManager
	: public TableReader
	,public Singleton<RefineDescTableManager>
{
public:

	RefineDescTableManager()
	{
		init("RefineDesc.txt");
	}

	typedef std::map<unsigned int, RefineDescItem* > RefineDescList;
	typedef ConstMapIterator<RefineDescList> RefineDescListIterator;

	RefineDescListIterator getRefineDescIterator(void) 
	{
		return RefineDescListIterator(mRefineDescList.begin(), mRefineDescList.end());
	}

	void init(const std::string& filename);
	const RefineDescItem * getRefineDescItemByID(unsigned int id);

	inline unsigned int getTotalNum() {return mRefineDescList.size();};

private:
	virtual void readline(std::stringstream& _stream);
	RefineDescList mRefineDescList;
};

class HelpItem
{
public:
	unsigned int		itemID;
	std::string		iconPic;
	std::string		name;
	std::string		describe;

private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>itemID
			>>name
			>>describe
			>>iconPic;
	}

	friend class HelpTableManager;
};

class HelpTableManager
	: public TableReader
	,public Singleton<HelpTableManager>
{
public:

	HelpTableManager()
	{
		init("Help.txt");
	}

	typedef std::map<unsigned int, HelpItem* > HelpList;
	typedef ConstMapIterator<HelpList> HelpListIterator;

	HelpListIterator getHelpIterator(void) 
	{
		return HelpListIterator(mHelpList.begin(), mHelpList.end());
	}

	void init(const std::string& filename);
	const HelpItem * getHelpItemByID(unsigned int id);

	inline unsigned int getTotalNum() {return mHelpList.size();};

	static HelpTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	HelpList mHelpList;
};

#define MAX_VIP_DESCIBE_NUM 10
class VIPPrivilegeItem
{
public:
	unsigned int		itemID;
	std::string		iconPic;
	std::string		name;
	unsigned int	rechangeNum;
	unsigned int	describeNum;
	std::string		describe[MAX_VIP_DESCIBE_NUM];
private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>itemID
			>>name
			>>iconPic
			>>rechangeNum
			>>describeNum
			>>describe[0]
			>>describe[1]
			>>describe[2]
			>>describe[3]
			>>describe[4]
			>>describe[5]
			>>describe[6]
			>>describe[7]
			>>describe[8]
			>>describe[9];
	}
	friend class VIPPrivilegeTableManager;
};

class VIPPrivilegeTableManager
	: public TableReader
	,public Singleton<VIPPrivilegeTableManager>
{
public:

	VIPPrivilegeTableManager()
	{
		init("VIPPrivilege.txt");
	}

	typedef std::map<unsigned int,VIPPrivilegeItem* > VIPPrivilegeList;
	typedef ConstMapIterator<VIPPrivilegeList> VIPPrivilegeListIterator;

	VIPPrivilegeListIterator getVIPPrivilegeIterator(void){
		return VIPPrivilegeListIterator(mVIPPrivilegeList.begin(),mVIPPrivilegeList.end());
	}
	void init (const std::string& filename);
	const VIPPrivilegeItem * getVIPPrivilegeItemByID(unsigned int id);
	const VIPPrivilegeItem * getVIPPrivilegeDescribeItemByID(unsigned int id,int describeIndex);

	inline unsigned int getTotalNum(){return mVIPPrivilegeList.size();};

	static VIPPrivilegeTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	VIPPrivilegeList mVIPPrivilegeList;

};

class AboutItem
{
public:
	unsigned int		itemID;
	std::string		iconPic;
	std::string		name;
	std::string		describe;

private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>itemID
			>>name
			>>describe
			>>iconPic;
	}

	friend class AboutTableManager;
};

class AboutTableManager
	: public TableReader
	,public Singleton<AboutTableManager>
{
public:

	AboutTableManager()
	{
		init("About.txt");
	}

	typedef std::map<unsigned int, AboutItem* > AboutList;
	typedef ConstMapIterator<AboutList> AboutListIterator;

	AboutListIterator getAboutIterator(void) 
	{
		return AboutListIterator(mAboutList.begin(), mAboutList.end());
	}

	void init(const std::string& filename);
	const AboutItem * getAboutItemByID(unsigned int id);

	inline unsigned int getTotalNum() {return mAboutList.size();};

	static AboutTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	AboutList mAboutList;
};

class MainShowAdventureItem
{
public:
	unsigned int	adventureItemId;
	std::string		name;
	std::string		iconPic;
	unsigned int	showType;
	std::string		targetPage;
	unsigned int	openType;
	unsigned int	isShowName;
	unsigned int	orderNo;
	unsigned int	isHaveTimer;
private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>adventureItemId
			>>name
			>>iconPic
			>>showType
			>>targetPage
			>>openType
			>>isShowName
			>>orderNo
			>>isHaveTimer;
	}
	friend class MainShowAdventureTableManager;
};

class MainShowAdventureTableManager
	: public TableReader
	,public Singleton<MainShowAdventureTableManager>
{
public:

	MainShowAdventureTableManager()
	{
		init("MainShowAdventureIconConfig.txt");
	}

	typedef std::map<unsigned int, MainShowAdventureItem* > MainShowAdventureList;
	typedef ConstMapIterator<MainShowAdventureList> MainShowAdventureListIterator;

	MainShowAdventureListIterator getAdventureIterator(void) 
	{
		return MainShowAdventureListIterator(mMainShowAdventureList.begin(), mMainShowAdventureList.end());
	}

	void init(const std::string& filename);
	const MainShowAdventureItem * getMainShowAdventureItemByID(unsigned int id);

	inline unsigned int getTotalNum() {return mMainShowAdventureList.size();};

	static MainShowAdventureTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	MainShowAdventureList mMainShowAdventureList;
};

class AdventureItem
{
public:
	unsigned int	itemID;
	std::string		name;
	unsigned int	adventureType;
	std::string		iconPic;
	std::string		ccbi;
	unsigned int	checkVersion;
	unsigned int	orderNo;
	//modify by daimeiqi at 20131202 添加背景音乐字段
	std::string     music;
	unsigned int    mainType;			//冒险分类，具体参考Adventure_MainType的定义, add by liu longfei
	unsigned int	limitLevel;
private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>itemID
			>>name
			>>adventureType
			>>iconPic
			>>ccbi
			>>checkVersion
			>>orderNo
			>>music
			>>mainType
			>>limitLevel;
	}

	friend class AdventureTableManager;
};

class AdventureTableManager
	: public TableReader
	,public Singleton<AdventureTableManager>
{
public:

	AdventureTableManager()
	{
		init("AdventureConfig.txt");
	}

	typedef std::map<unsigned int, AdventureItem* > AdventureList;
	typedef ConstMapIterator<AdventureList> AdventureListIterator;

	AdventureListIterator getAdventureIterator(void) 
	{
		return AdventureListIterator(mAdventureList.begin(), mAdventureList.end());
	}

	void init(const std::string& filename);
	const AdventureItem * getAdventureItemByID(unsigned int id);
	const AdventureItem * getAdventureItemByType(unsigned int type);

	inline unsigned int getTotalNum() {return mAdventureList.size();};

	static AdventureTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	AdventureList mAdventureList;
};

class PlayerLevelParamItem
{
public:
	unsigned int level;
	unsigned int exp;
	unsigned int discipleMax;

public:
	void readLine(std::stringstream& _stream);
};

class PlayerLevelParamTableManager
	:public TableReader
	, public Singleton<PlayerLevelParamTableManager>
{
public:

	PlayerLevelParamTableManager()
	{
		init("PlayerLevelParam.txt");
	}

	typedef std::map<unsigned int, PlayerLevelParamItem*> PlayerLevelParamItemList;
	typedef ConstMapIterator<PlayerLevelParamItemList> PlayerLevelParamItemListIterator;

	PlayerLevelParamItemListIterator getGradeItemIterator(void)
	{
		return PlayerLevelParamItemListIterator(mPlayerLevelParamItemList.begin(), mPlayerLevelParamItemList.end());
	}

	void init(const std::string& filename);
	//根据总经验和当前等级，获取当前经验
	unsigned int getCurExp(unsigned int level, unsigned int totalExp);
	const PlayerLevelParamItem* getPlayerLevelParamByLevel(unsigned int level);
	unsigned int getNeedPlayerLevelForBattleEnableSize(unsigned int battle_enabel_size);

	inline unsigned int getTotalNum(){return mPlayerLevelParamItemList.size();};

private:
	virtual void readline(std::stringstream& _stream);
	PlayerLevelParamItemList mPlayerLevelParamItemList;
};

class PlayerGradeItem
{
public:
	unsigned int ID;
	unsigned int canDiscipleNum;
	unsigned int maxPower;
	unsigned int maxVitality;
	unsigned int goldReward;
	unsigned int silverReward;
	unsigned int exp;
	unsigned int levelSilverCoins;
private:
	void readLine(std::stringstream& _stream)
	{
		_stream>>ID>>canDiscipleNum>>maxPower>>maxVitality>>goldReward>>silverReward>>exp>>levelSilverCoins;
	}
	friend class PlayerGradeTableManager;
};

class PlayerGradeTableManager
	:public TableReader
	, public Singleton<PlayerGradeTableManager>
{
public:

	PlayerGradeTableManager()
	{
		init("PlayerGradeConfig.txt");
	}

	typedef std::map<unsigned int, PlayerGradeItem*> GradeItemList;
	typedef ConstMapIterator<GradeItemList> GradeItemListIterator;

	GradeItemListIterator getGradeItemIterator(void)
	{
		return GradeItemListIterator(mGradeItemList.begin(), mGradeItemList.end());
	}

	void init(const std::string& filename);
	const PlayerGradeItem* getGradeItemByID(unsigned int id);

	inline unsigned int getTotalNum(){return mGradeItemList.size();};

	static PlayerGradeTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	GradeItemList mGradeItemList;
};

class AnnouncementItem
{
public:
	unsigned int itemId;
	std::string idName;
	std::string title;
	long int startTime;
	long int endTime;
	std::string timeMsg;
	std::string msg;
	unsigned int isHaveButton;
	std::string buttonText;
	std::string buttonGoPage;
	int btnType;
	unsigned int adventureId;
	unsigned int changeOrPush;
	std::string picPath;
	int severDelayTime;
	int order;
private:
	/*void readLine(std::stringstream& _stream)
	{
		_stream>>itemId>>idName>>title>>startTime>>endTime>>timeMsg>>msg>>isHaveButton>>buttonText>>buttonGoPage>>btnType>>adventureId>>changeOrPush>>picPath>>severDelayTime>>order;
	}*/
	void readLine(Json::Value _stream)
	{
		itemId=_stream["AnnouncementId"].isNumeric()?_stream["AnnouncementId"].asUInt():0;
		idName=_stream["idName"].isString()?_stream["idName"].asString():"";
		title=_stream["Title"].isString()?_stream["Title"].asString():"";
		startTime=_stream["StartTime"].isNumeric()?_stream["StartTime"].asUInt():0;
		endTime=_stream["EndTime"].isNumeric()?_stream["EndTime"].asUInt():0;
		timeMsg=_stream["timeMsg"].isString()?_stream["timeMsg"].asString():"";
		msg=_stream["Msg"].isString()?_stream["Msg"].asString():"";
		isHaveButton=_stream["IsHaveButton"].isNumeric()?_stream["IsHaveButton"].asUInt():0;
		buttonText=_stream["ButtonText"].isString()?_stream["ButtonText"].asString():"";
		buttonGoPage=_stream["ButtonGoPage"].isString()?_stream["ButtonGoPage"].asString():"";
		btnType=_stream["ButtonType"].isNumeric()?_stream["ButtonType"].asInt():0;
		adventureId=_stream["AdventureId"].isNumeric()?_stream["AdventureId"].asUInt():0;
		changeOrPush=_stream["changeOrPush"].isNumeric()?_stream["changeOrPush"].asUInt():0;
		picPath=_stream["picPath"].isString()?_stream["picPath"].asString():"";
		severDelayTime=_stream["severDelayTime"].isNumeric()?_stream["severDelayTime"].asInt():0;
		order=_stream["orderNo"].isNumeric()?_stream["orderNo"].asInt():0;
	}

	friend class AnnouncementTableManager;
};

class AnnouncementTableManager
	: public TableReader
	, public Singleton<AnnouncementTableManager>
{
public:

	AnnouncementTableManager()
	{
		//init("AnnouncementConfig.txt");
	}

	typedef std::map<unsigned int, AnnouncementItem*> AnnouncementItemList;
	typedef ConstMapIterator<AnnouncementItemList> AnnouncementItemListIterator;

	AnnouncementItemListIterator getAnnouncementItemListIterator(void)
	{
		return AnnouncementItemListIterator(mAnnouncementItemList.begin(), mAnnouncementItemList.end());
	}

	void init();

	const AnnouncementItem* getAnnouncementItemById(unsigned int id);

	AnnouncementItemList getAnnouncementItemList(){return mAnnouncementItemList;};

	inline unsigned int getTotalNum(){return mAnnouncementItemList.size();};

	void clearAnnouncementItem();
	void readInAnnouncementItem(Json::Value _stream) { readline(_stream); };

	static AnnouncementTableManager* getInstance();
private:
	virtual void readline(Json::Value _stream);
	AnnouncementItemList mAnnouncementItemList;
};

class UserPropertyItem
{
public:
	unsigned int	itemID;
	std::string	name;
	unsigned int quality;
	std::string	describe;
	std::string	iconPic;

private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>itemID
			>>name
			>>quality
			>>describe
			>>iconPic;
	}

	friend class UserPropertyTableManager;
};
class UserPropertyTableManager
	: public TableReader
	,public Singleton<UserPropertyTableManager>
{
public:

	UserPropertyTableManager()
	{
		init("UserProperty.txt");
	}

	typedef std::map<unsigned int, UserPropertyItem* > UserPropertyList;
	typedef ConstMapIterator<UserPropertyList> UserPropertyListIterator;

	UserPropertyListIterator getUserPropertyListIterator(void) 
	{
		return UserPropertyListIterator(mUserPropertyList.begin(), mUserPropertyList.end());
	}

	void init(const std::string& filename);
	const UserPropertyItem * getUserPropertyItemByID(unsigned int id);

	inline unsigned int getTotalNum() {return mUserPropertyList.size();};

	static UserPropertyTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	UserPropertyList mUserPropertyList;
};

/************************************************************************/
/* 物品类别一览表                                                       */
/************************************************************************/
class ResPropertyItem
{
public:
	unsigned int typeId;
	unsigned int targetTypeId;
	std::string	name;
	std::string	describe;

private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>typeId
			>>name
			>>targetTypeId
			>>describe;
	}

	friend class ResPropertyTableManager;
};
class ResPropertyTableManager
	: public TableReader
	,public Singleton<ResPropertyTableManager>
{
public:

	ResPropertyTableManager()
	{
		init("ResPropertyConfig.txt");
	}

	typedef std::map<unsigned int, ResPropertyItem* > ResPropertyList;
	typedef ConstMapIterator<ResPropertyList> ResPropertyListIterator;

	void init(const std::string& filename);
	const ResPropertyItem * getResPropertyItemByID(unsigned int id);
	inline unsigned int getTotalNum() {return mResPropertyList.size();};
	static ResPropertyTableManager* getInstance();

private:
	virtual void readline(std::stringstream& _stream);
	ResPropertyList mResPropertyList;
};



class NewActivePromptItem
{
public:
	unsigned int	itemID;
	unsigned int	type;
	int	level;
	std::string	ccbiPath;
	std::string	pageName;
	std::string	pageParam;
	int	dropId;

private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>itemID
			>>type
			>>level
			>>ccbiPath
			>>pageName
			>>pageParam
			>>dropId;
	}

	friend class NewActivePromptTableManager;
};
class NewActivePromptTableManager
	: public TableReader
	,public Singleton<NewActivePromptTableManager>
{
public:

	NewActivePromptTableManager()
	{
		init("NewActivePrompt.txt");
	}

	typedef std::map<unsigned int, NewActivePromptItem* > NewActivePromptList;
	typedef ConstMapIterator<NewActivePromptList> NewActivePromptItemListIterator;

	NewActivePromptItemListIterator getNewActivePromptListIterator(void) 
	{
		return NewActivePromptItemListIterator(mNewActivePromptList.begin(), mNewActivePromptList.end());
	}

	NewActivePromptList * getNewAcrivePromptItemByLevel(unsigned int id,unsigned int type = 1);
	NewActivePromptList * getNewAcrivePromptItemByDropId(unsigned int id,unsigned int type = 2);
	void init(const std::string& filename);
	const NewActivePromptItem * getNewActivePromptItemByID(unsigned int id);

	inline unsigned int getTotalNum() {return mNewActivePromptList.size();};

	static NewActivePromptTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	NewActivePromptList mNewActivePromptList;
};


class ArenaRewardItem
{
public:
	unsigned int	itemID;
	std::string		name;
	unsigned int	type;
	std::string		title;
	unsigned int	needScore;
	unsigned int	rankLimit;
	unsigned int	dayLimitTimes;
	unsigned int	careerLimitTimes;
	std::string		rewardItem;
	std::vector<unsigned int>	rewards;			
private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>itemID
			>>name
			>>rewardItem
			>>needScore
			>>type
			>>rankLimit
			>>dayLimitTimes
			>>careerLimitTimes;
		StringConverter::parseVectorForUnderline(rewardItem,rewards);
	}

	friend class ArenaRewardTableManager;
};
class ArenaRewardTableManager
	: public TableReader
	,public Singleton<ArenaRewardTableManager>
{
public:

	ArenaRewardTableManager()
	{
		init("ArenaReward.txt");
	}

	typedef std::map<unsigned int, ArenaRewardItem* > ArenaRewardList;
	typedef ConstMapIterator<ArenaRewardList> ArenaRewardListIterator;

	ArenaRewardListIterator getArenaListIterator(void) 
	{
		return ArenaRewardListIterator(mArenaRewardList.begin(), mArenaRewardList.end());
	}

	void init(const std::string& filename);
	const ArenaRewardItem * getArenaRewardItemByID(unsigned int id);

	inline unsigned int getTotalNum() {return mArenaRewardList.size();};

	static ArenaRewardTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	ArenaRewardList mArenaRewardList;
};

class FightEndRewardItem
{
public:
	unsigned int	itemID;
	unsigned int	rewardCount;
	std::string		rewardInfo;

private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>itemID
			>>rewardCount
			>>rewardInfo;
	}

	friend class FightEndRewardTableManager;
};
class FightEndRewardTableManager
	: public TableReader
	,public Singleton<FightEndRewardTableManager>
{
public:

	FightEndRewardTableManager()
	{
		init("FightEndRewardConfig.txt");
	}

	typedef std::map<unsigned int, FightEndRewardItem* > FightEndRewardList;
	typedef ConstMapIterator<FightEndRewardList> FightEndRewardIterator;

	FightEndRewardIterator getFightEndRewardIterator(void) 
	{
		return FightEndRewardIterator(mFightEndRewardList.begin(), mFightEndRewardList.end());
	}

	void init(const std::string& filename);
	const FightEndRewardItem * getFightEndRewardItemByID(unsigned int id);

	inline unsigned int getTotalNum() {return mFightEndRewardList.size();};

	static FightEndRewardTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	FightEndRewardList mFightEndRewardList;
};

//天命
class DestinyItem
{
public:
	unsigned int	itemID;
	std::string		name;
	unsigned int	consumeStar;
	unsigned int	consumeSilver;
	std::map<unsigned int ,unsigned int> addAttr;
	std::string		iconPic;

	void readline(std::stringstream& _stream);
};

class DestinyTableManager
	: public TableReader
	,public Singleton<DestinyTableManager>
{
public:
	DestinyTableManager()
	{
		init("Destiny.txt");
	}

	typedef std::map<unsigned int, DestinyItem* > DestinyList;
	typedef ConstMapIterator<DestinyList> DestinyIterator;

	DestinyIterator getFortuneComeRewardIterator(void) 
	{
		return DestinyIterator(mDestinyList.begin(), mDestinyList.end());
	}

	void init(const std::string& filename);
	const DestinyItem * getDestinyItemByID(unsigned int id);

	inline unsigned int getTotalNum() {return mDestinyList.size();};

private:
	virtual void readline(std::stringstream& _stream);
	DestinyList mDestinyList;
};

class FortuneComeRewardItem
{
public:
	unsigned int	itemID;
	std::string		name;
	std::string		rewardGold;

private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>itemID
			>>name
			>>rewardGold;
	}

	friend class FortuneComeRewardTableManager;
};
class FortuneComeRewardTableManager
	: public TableReader
	,public Singleton<FortuneComeRewardTableManager>
{
public:

	FortuneComeRewardTableManager()
	{
		init("FortuneComeReward.txt");
	}

	typedef std::map<unsigned int, FortuneComeRewardItem* > FortuneComeRewardList;
	typedef ConstMapIterator<FortuneComeRewardList> FortuneComeRewardIterator;

	FortuneComeRewardIterator getFortuneComeRewardIterator(void) 
	{
		return FortuneComeRewardIterator(mFortuneComeRewardList.begin(), mFortuneComeRewardList.end());
	}

	void init(const std::string& filename);
	const FortuneComeRewardItem * getFortuneComeRewardItemByID(unsigned int id);

	inline unsigned int getTotalNum() {return mFortuneComeRewardList.size();};

	static FortuneComeRewardTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	FortuneComeRewardList mFortuneComeRewardList;
};

class VipDataItem
{
public:
	unsigned int	itemID;
	std::string		vipLevel;
	unsigned int	friendNumLimit;
	unsigned int	enemyNumLimit;
private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>itemID
			>>vipLevel
			>>friendNumLimit
			>>enemyNumLimit;
	}

	friend class VipDataTableManager;
};
class VipDataTableManager
	: public TableReader
	,public Singleton<VipDataTableManager>
{
public:

	VipDataTableManager()
	{
		init("VIPData.txt");
	}

	typedef std::map<unsigned int, VipDataItem* > VipDataList;

	void init(const std::string& filename);
	const VipDataItem * getVipDataByID(unsigned int id);

	inline unsigned int getTotalNum() {return mVipDataList.size();};
private:
	virtual void readline(std::stringstream& _stream);
	VipDataList mVipDataList;
};

class ActivityPopItem
{
public:
	unsigned int	activityID;
	std::string		name;
	unsigned int	referenceID;
	std::string	describe;
	unsigned int currVersion;
private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>activityID
			>>name
			>>referenceID
			>>describe
			>>currVersion;
	}
	friend class ActivityPopTableManager;
};
class ActivityPopTableManager
	: public TableReader
	,public Singleton<ActivityPopTableManager>
{
public:

	ActivityPopTableManager()
	{
		init("ActivityPopConfig.txt");
	}

	typedef std::map<unsigned int, ActivityPopItem* > ActivityPopList;

	void init(const std::string& filename);
	const ActivityPopItem * getActivityPopDataByID(unsigned int id);

	inline unsigned int getTotalNum() {return mActivityPopList.size();};
private:
	virtual void readline(std::stringstream& _stream);
	ActivityPopList mActivityPopList;
};

class PlatformRoleItem
{
public:
	unsigned int	id;//渠道编号
	std::string		name;//平台名称
	unsigned int	attentionStatus;//关注微信开关
	std::string		loadingFrameSeversConnection;//服务器开服提示各平台差异
	std::string		rechargeMaxLimit;//首冲翻倍额度
	unsigned int	bbsOpenStatus;//bbs开启状态
	std::string		showFeedBack;//反馈信息跳转页面规则
	unsigned int	webkitOpenStatus;//webkit公告页面开启状态
	std::string		aboutSetting;//关于页面设置
	std::string excludedAboutIds;//排除掉的关于页面条目id, 多个id用逗号分隔，形式如："1,2,3" 
	std::string excludedAnnoucementIds;//排除掉的公告页面条目id 多个id用逗号分隔，形式如："1,2,3" 
	unsigned int	isUsePrivateBigVersionUpdate;
	unsigned int	isUseSDKBBS;
	std::string		openBBSUrl;
	std::string		loadinScenceMsg;
	unsigned int	exit2platlogout;
	unsigned int    shareSwitch;//微信分享开关
private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>id
			>>name
			>>attentionStatus
			>>loadingFrameSeversConnection
			>>rechargeMaxLimit
			>>bbsOpenStatus
			>>showFeedBack
			>>webkitOpenStatus
			>>aboutSetting
			>>excludedAboutIds
			>>excludedAnnoucementIds
			>>isUsePrivateBigVersionUpdate
			>>isUseSDKBBS
			>>openBBSUrl
			>>loadinScenceMsg
			>>exit2platlogout
			>>shareSwitch;
	}
	friend class PlatformRoleTableManager;
};
class PlatformRoleTableManager
	: public TableReader
	,public Singleton<PlatformRoleTableManager>
{
public:

	PlatformRoleTableManager()
	{
		init("PlatformRoleConfig.txt");
	}

	typedef std::map<unsigned int, PlatformRoleItem* > PlatformRoleList;

	void init(const std::string& filename);
	const PlatformRoleItem * getPlatformRoleByID(unsigned int id);
	const PlatformRoleItem * getPlatformRoleByName(std::string name);

	inline unsigned int getTotalNum() {return mPlatformRoleList.size();};

	static PlatformRoleTableManager* getInstance();

private:
	virtual void readline(std::stringstream& _stream);
	PlatformRoleList mPlatformRoleList;
};

//军团相关
//建筑升级
class LeagueLevelParam
{
public:
	LeagueLevelParam()
	{
		level = 0;	
		type = 0;
		upgradeNeed = 0;
		memberCount = 0;
	}
public:
	unsigned int level;	
	unsigned int type;
	int	upgradeNeed;
	unsigned int memberCount;
public:
	void readline(std::stringstream& _stream);
};

class LeagueLevelParamManager
	: public TableReader
	, public Singleton<LeagueLevelParamManager>
{
public:

	LeagueLevelParamManager()
	{
		init("LeagueLevelParam.txt");
	}

	typedef std::vector<LeagueLevelParam*> LeagueLevelParamList;

	void init(const std::string filename);
	const LeagueLevelParam* getParamByLevelAndType(unsigned int level,unsigned int type);
	unsigned int getTotalNum() { return mLeagueLevelParamList.size(); }

	static LeagueLevelParamManager* getInstance();

private:
	void readline(std::stringstream& _stream);
	LeagueLevelParamList mLeagueLevelParamList;
};


//神龙殿奖励
class WorshipLevelParam
{
public:
	WorshipLevelParam()
	{
		level = 0;
		power = 0;
		reputation = 0;
		silver = 0;
	}
public:
	unsigned int level;
	unsigned int power;
	unsigned int reputation;
	unsigned int silver;
public:
	void readline(std::stringstream& _stream);
};

class WorshipLevelParamManager
	: public TableReader
	, public Singleton<WorshipLevelParamManager>
{
public:

	WorshipLevelParamManager()
	{
		init("LeagueWorshipLevel.txt");
	}

	typedef std::map<unsigned int, WorshipLevelParam*> WorshipLevelParamList;

	void init(const std::string filename);
	const WorshipLevelParam* getParamByLevel(unsigned int level);
	unsigned int getTotalNum() { return mWorshipLevelParamList.size(); }

	static WorshipLevelParamManager* getInstance();
private:
	void readline(std::stringstream& _stream);
	WorshipLevelParamList mWorshipLevelParamList;
};

//------------------lineup System table-----------------------
class CabinItem
{
public:
	unsigned int id;
	std::string name;
	unsigned int openLevel;
	std::string unlockCondition;
	std::string ccbi;
	unsigned int seats;
	unsigned int maxLevel;

	CabinItem(): id(0), name(""), openLevel(0), unlockCondition("none")
			   , ccbi(""), seats(0), maxLevel(0)
	{};
private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>id
			>>name
			>>openLevel
			>>unlockCondition
			>>ccbi
			>>seats
			>>maxLevel;
	}

	friend class CabinTableManager;
};

class CabinTableManager
	: public TableReader
	, public Singleton<CabinTableManager>
{
public:

	CabinTableManager()
	{
		init("Cabin.txt");
	}

	typedef std::map<unsigned int, CabinItem*> CabinItemList;

	void init(const std::string filename);
	const CabinItem* getCabinById(unsigned int id);
	unsigned int getTotalNum() { return mCabinItemList.size(); }
private:
	void readline(std::stringstream& _stream);
	CabinItemList mCabinItemList;
};

class CabinSeatItem
{
public:
	unsigned int id;
	unsigned int cabinId;
	unsigned int position;
	std::string unlockCondition;
	std::string buff;
	unsigned int addition;
	unsigned int openLevel;
	std::vector<unsigned int> upAddition;
	std::vector<std::string> upConsume;

	CabinSeatItem(): id(0), cabinId(0), position(0), unlockCondition("none")
				   , buff(""), addition(0), openLevel(0)
	{};

	virtual ~CabinSeatItem()
	{
		upAddition.clear();
		upConsume.clear();
	}
private:
	void readline(std::stringstream& _stream)
	{
		std::string upAdditionStr, upConsumeStr;
		_stream
			>>id
			>>cabinId
			>>position
			>>unlockCondition
			>>buff
			>>addition
			>>openLevel
			>>upAdditionStr
			>>upConsumeStr;
		
		std::vector<std::string> tokenized = GameMaths::tokenize(upAdditionStr, ",");
		for ( std::vector<std::string>::iterator itr = tokenized.begin(); itr != tokenized.end(); ++itr )
		{
			upAddition.push_back(StringConverter::parseUnsignedInt(*itr, 0));
		}
		upConsume = GameMaths::tokenize(upConsumeStr, ";");
	}

	friend class CabinSeatTableManager;
};

class CabinSeatTableManager
	: public TableReader
	, public Singleton<CabinSeatTableManager>
{
public:

	CabinSeatTableManager()
	{
		init("CabinSeat.txt");
	}

	typedef std::map<unsigned int, CabinSeatItem* > CabinSeatList;
	
	void init(const std::string filename);
	const CabinSeatItem* getCabinSeatByCabinIdAndPosition(unsigned int cabinId, unsigned int position);
private:
	void readline(std::stringstream& _stream);

	CabinSeatList mCabinSeatList;
};

//------------------------------------------------------------
class ReleaseCCBFile
{
public:
	std::string		name;

private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>name;
	}

	friend class ReleaseCCBFileManager;
};

class ReleaseCCBFileManager
	: public TableReader
	,public Singleton<ReleaseCCBFileManager>
{
public:
	ReleaseCCBFileManager()
	{
		init("ReleaseCCBFile.txt");
	}

	typedef std::vector<ReleaseCCBFile*> CCBFileVec;

	void init(const std::string& filename);
	ReleaseCCBFile * getRelaseCCBFileByName(std::string name);

private:
	virtual void readline(std::stringstream& _stream);
	CCBFileVec m_fileVec;
};

class SoundEffectItem
{
public:
	unsigned int	itemId;
	std::string		module;
	std::string		file;
public:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>itemId
			>>module
			>>file;
	}
};

class SoundEffectTableManager
	: public TableReader
	,public Singleton<SoundEffectTableManager>
{
public:

	SoundEffectTableManager()
	{
		init("SoundEffect.txt");
	}

	typedef std::map<unsigned int, SoundEffectItem* > SoundEffectList;
	typedef ConstMapIterator<SoundEffectList> SoundEffectListIterator;	

	void init(const std::string& filename);
	SoundEffectItem* getSoundEffectById(unsigned int id);
	inline unsigned int getTotalNum() {return mSoundEffectList.size();};

private:
	virtual void readline(std::stringstream& _stream);

	SoundEffectList mSoundEffectList;
};


class PushPageItem
{
public:
	unsigned int	itemId;
	std::string		name;
	bool			isPushAni;
	bool			isBeforeEnter;
	bool			isPopAni;
	bool			isAfterExit;
	int				blurType;
public:
	void readline(std::stringstream& _stream);
};

class PushPageTableManager
	: public TableReader
	,public Singleton<PushPageTableManager>
{
public:

	PushPageTableManager()
	{
		init("PushPage.txt");
	}

	typedef std::map<unsigned int, PushPageItem* > PushPageList;
	typedef ConstMapIterator<PushPageList> PushPageListIterator;	

	void init(const std::string& filename);
	PushPageItem* getPushPageByName(const std::string name);
	inline unsigned int getTotalNum() {return mPushPageList.size();};

private:
	virtual void readline(std::stringstream& _stream);

	PushPageList mPushPageList;
};

enum DeviceCfg
{
	DeviceCfg_Low = 1,
	DeviceCfg_Middle = 2,
	DeviceCfg_High = 3,
};

class IOSDeviceItem
{
public:
	unsigned int	itemId;
	std::string		type;				//手机类型
	std::string		name;				//型号名字
	int	cfgType;						//配置类型（1：低；2：中；3：高）

public:
	void readline(std::stringstream& _stream);
};

class IOSDeviceTableManager
	: public TableReader
	,public Singleton<IOSDeviceTableManager>
{
public:

	IOSDeviceTableManager()
	{
		init("IOSDevices.txt");
	}

	typedef std::map<unsigned int, IOSDeviceItem* > IOSDeviceList;
	typedef ConstMapIterator<IOSDeviceList> IOSDeviceListIterator;	

	void init(const std::string& filename);
	IOSDeviceItem* getIOSDeviceItemByName(const std::string name);
	inline unsigned int getTotalNum() {return mIOSDevicesList.size();};

private:
	virtual void readline(std::stringstream& _stream);

	IOSDeviceList mIOSDevicesList;
};

class AndroidDeviceItem
{
public:
	unsigned int	itemId;
	unsigned int 	cpus;				//手机类型
	float			cpuHZ;				//型号名字
	float			memory;				//内存
	int				cfgType;			//配置类型（1：低；2：中；3：高）

public:
	void readline(std::stringstream& _stream);
};

class AndroidDeviceTableManager
	: public TableReader
	,public Singleton<AndroidDeviceTableManager>
{
public:

	AndroidDeviceTableManager()
	{
		init("AndroidDevices.txt");
	}

	typedef std::map<unsigned int, AndroidDeviceItem* > AndroidDeviceList;
	typedef ConstMapIterator<AndroidDeviceList> AndroidDeviceListIterator;	

	void init(const std::string& filename);	
	int getAndroidCfgType(unsigned int cpus,float cpuHZ,float memory);
	inline unsigned int getTotalNum() {return mAndroidDevicesList.size();};

private:
	virtual void readline(std::stringstream& _stream);

	AndroidDeviceList mAndroidDevicesList;
};


//------------------------------------------------------------

/************************************************************************/
/* 此为系统错误提示                                                     */
/************************************************************************/
class ErrMsgItem
{
public:
	unsigned int		msgId;
	std::string		msgContent;

private:
	void readline(std::stringstream& _stream)
	{
		_stream
			>>msgId
			>>msgContent;
	}

	friend class ErrMsgTableManager;
};

class ErrMsgTableManager
	: public TableReader
	,public Singleton<ErrMsgTableManager>
{
public:

	ErrMsgTableManager()
	{
		init("ErrorCode.txt");
	}

	typedef std::map<unsigned int, ErrMsgItem* > ErrMsgList;
	typedef ConstMapIterator<ErrMsgList> ErrMsgListIterator;

	ErrMsgListIterator getErrMsgIterator(void) 
	{
		return ErrMsgListIterator(mErrMsgList.begin(), mErrMsgList.end());
	}

	void init(const std::string& filename);
	const ErrMsgItem * getErrMsgItemByID(unsigned int id);

	inline unsigned int getTotalNum() {return mErrMsgList.size();};

	static ErrMsgTableManager* getInstance();
private:
	virtual void readline(std::stringstream& _stream);
	ErrMsgList mErrMsgList;
};