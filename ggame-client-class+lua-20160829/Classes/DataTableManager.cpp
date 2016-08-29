
#include "stdafx.h"

#include "DataTableManager.h"
#include "StringConverter.h"
#include "math.h"
#include "Language.h"
#include "GameTypedef.h"
#include "MessageBoxPage.h"
//-----------------------------------------------------------------------------------------------
//Quality
//-----------------------------------------------------------------------------------------------
cocos2d::ccColor3B getFrameColor(unsigned int quality)
{
	std::string color = VaribleManager::Get()->getSetting("FrameColor_Quality" + StringConverter::toString(quality));
	return StringConverter::parseColor3B(color);
}

cocos2d::CCSprite* getFrameNormalSpirte(unsigned int quality, std::string ext/*=""*/)
{
	cocos2d::CCSprite* sprite = cocos2d::CCSprite::create(VaribleManager::Get()->getSetting("FrameNormal_ImageFile" + ext).c_str());
	sprite->setColor(getFrameColor(quality));
	return sprite;
}

cocos2d::CCSprite* getFrameSelectedSpirte(unsigned int quality, std::string ext/*=""*/)
{
	cocos2d::CCSprite* sprite = cocos2d::CCSprite::create(VaribleManager::Get()->getSetting("FrameSelected_ImageFile" + ext).c_str());
	sprite->setColor(getFrameColor(quality));
	return sprite;
}

cocos2d::CCSprite* getFrameDisabledSpirte(unsigned int quality, std::string ext/*=""*/)
{
	cocos2d::CCSprite* sprite = cocos2d::CCSprite::create(VaribleManager::Get()->getSetting("FrameDisabled_ImageFile" + ext).c_str());
	sprite->setColor(getFrameColor(quality));
	return sprite;
}
cocos2d::CCSprite* getContentUnselectedSpirte(std::string ext/*=""*/)
{
	cocos2d::CCSprite* sprite = cocos2d::CCSprite::create(VaribleManager::Get()->getSetting("ContentUnselected_ImageFile_"+ ext).c_str());
	return sprite;
}
cocos2d::CCSprite* getContentSelectedSpirte(std::string ext/*=""*/)
{
	cocos2d::CCSprite* sprite = cocos2d::CCSprite::create(VaribleManager::Get()->getSetting("ContentSelected_ImageFile_"+ ext).c_str());
	return sprite;
}

std::string getQualityMaskImageFile(unsigned int quality)
{
	return VaribleManager::Get()->getSetting("ImageFileQualityMask" + StringConverter::toString(quality));
}

std::string getQualityImageFile(unsigned int quality)
{
	return VaribleManager::Get()->getSetting("ImageFileQuality" + StringConverter::toString(quality));
}

std::string getQualityFramePersonImageFile()
{
	return VaribleManager::Get()->getSetting("FramePerson_ImageFile");
}

std::string getQualityFrameNormalImageFile()
{
	return VaribleManager::Get()->getSetting("FrameNormal_ImageFile");
}

std::string getQualityFrameSelectImageFile()
{
	return VaribleManager::Get()->getSetting("FrameSelected_ImageFile");
}

std::string getQualityFrameNewNormalImageFile()
{
	return VaribleManager::Get()->getSetting("FrameNormal_ImageFile_NewFoot");
}

std::string getQualityFrameNewSelectImageFile()
{
	return VaribleManager::Get()->getSetting("FrameSelected_ImageFile_NewFoot");
}

std::string getVipLevelImageFile(unsigned int level)
{
	return VaribleManager::Get()->getSetting("Vip_Level_" + StringConverter::toString(level));
}

std::string getRecruitNumImageFile(unsigned int digit)//1-9
{
	return VaribleManager::Get()->getSetting("Recruit_Num_" + StringConverter::toString(digit));
}
std::string getTeamDefaultPic(void)
{
	return VaribleManager::Get()->getSetting("DefaultIcon_ImageFile_100");
}
std::string getTeamLockPic(void)
{
	return VaribleManager::Get()->getSetting("DefaultIcon_ImageFile_TeamUnlock");
}

//-----------------------------------------------------------------------------------------------
//YuanfenTableManager
//-----------------------------------------------------------------------------------------------
void YuanfenItem::readline(std::stringstream& _stream)
{
	std::string str;
	_stream
		>>itemID
		>>name
		>>type
		>>TriggerCondition
		>>str
		>>describe;
	std::vector<unsigned int> vec;
	StringConverter::parseVector(str,vec);
	for (int i = 0;i < vec.size(); i += 2)
	{
		upgradeAttr.insert(std::make_pair(vec[i],vec[i+1]));
	}
}

unsigned int YuanfenItem::getUpgradeAttrTypeByIndex(unsigned int index)
{
	int idx = 0;
	std::map<unsigned int ,unsigned int>::iterator it = upgradeAttr.begin();
	std::map<unsigned int ,unsigned int>::iterator end = upgradeAttr.end();
	for ( ; it != end; ++it)
	{
		if (index == idx)
		{
			return it->first;
		}
	}

	return 0;
}

unsigned int YuanfenItem::getUpgradeAttrValByIndex(unsigned int index)
{
	int idx = 0;
	std::map<unsigned int ,unsigned int>::iterator it = upgradeAttr.begin();
	std::map<unsigned int ,unsigned int>::iterator end = upgradeAttr.end();
	for ( ; it != end; ++it)
	{
		if (index == idx)
		{
			return it->second;
		}
	}

	return 0;
}

unsigned int YuanfenItem::getUpgradeAttrSize()
{
	return upgradeAttr.size();
}

void YuanfenTableManager::init( const std::string& filename )
{
	YuanfenList::iterator itr = mYuanfenList.begin();
	while(itr != mYuanfenList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mYuanfenList.clear();

	parse(filename, 1);
}

//-----------------------------------------------------------------------------------------------
void YuanfenTableManager::readline( std::stringstream& _stream )
{
	YuanfenItem* data = new YuanfenItem;
	data->readline(_stream);
	mYuanfenList.insert(YuanfenList::value_type(data->itemID, data));
}

//-----------------------------------------------------------------------------------------------
const YuanfenItem * YuanfenTableManager::getYuanfenItemByID( unsigned int id )
{
	if(mYuanfenList.find(id) != mYuanfenList.end())
		return mYuanfenList.find(id)->second;

	return NULL;
}

//-----------------------------------------------------------------------------------------------
const YuanfenItem * YuanfenTableManager::getYuanfenItemByName( const std::string& name )
{
	YuanfenList::iterator itr = mYuanfenList.begin();
	while(itr != mYuanfenList.end())
	{
		if(itr->second->name == name)
			return itr->second;
	}
	 
	return NULL;
}

YuanfenTableManager* YuanfenTableManager::getInstance()
{
	return YuanfenTableManager::Get();
}

void DiscipleEvolutionItem::readline(std::stringstream& _stream)
{
	_stream
		>>itemID
		>>stage
		>>potential
		>>needLevel
		>>openTalent
		>>consumeItemStr
		>>consumeSilver
		>>consumeCardCount
		>>consumeTreasureStr
		>>addHP
		>>addAttack
		>>addPhysicsDefence
		>>addMagicDefence;

	StringConverter::parseVector(consumeItemStr,consumeItems);
	StringConverter::parseVector(consumeTreasureStr,consumeTreasures);
}

void DiscipleEvolutionTableManager::init( const std::string& filename )
{
	DiscipleEvolutionList::iterator itr = mDiscipleEvolutionList.begin();
	while(itr != mDiscipleEvolutionList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mDiscipleEvolutionList.clear();

	parse(filename, 1);
}

void DiscipleEvolutionTableManager::readline( std::stringstream& _stream )
{
	DiscipleEvolutionItem* data = new DiscipleEvolutionItem;
	data->readline(_stream);
	mDiscipleEvolutionList.insert(DiscipleEvolutionList::value_type(data->itemID, data));
}

const DiscipleEvolutionItem* DiscipleEvolutionTableManager::getByStageAndPotential(int stage,int potential)
{
	DiscipleEvolutionList::iterator it = mDiscipleEvolutionList.begin();
	DiscipleEvolutionList::iterator end = mDiscipleEvolutionList.end();
	for ( ; it != end; ++it)
	{
		if (it->second->stage == stage && it->second->potential == potential)
		{
			return it->second;
		}
	}

	return NULL;
}

void DiscipleItem::readline(std::stringstream& _stream)
{	
	unsigned int tmpLevelupRate = 0;
	_stream
		>>itemID
		>>name
		>>type
		>>quality
		>>potentiality
		>>initExp
		>>sellPrice
		>>tmpLevelupRate
		>>skill1
		>>skill2
		>>initPhysique
		>>initStrength
		>>initArmor
		>>initHealth
		>>initAttack
		>>initPhysicDefence
		>>initMagicDefence
		>>growthHealth
		>>growthAttack
		>>growthPhysicDefence
		>>growthMagicDefence
		>>yuanfens[0]
		>>yuanfens[1]
		>>yuanfens[2]
		>>yuanfens[3]
		>>yuanfens[4]
		>>yuanfens[5]
		>>talent[0]
		>>talent[1]
		>>talent[2]
		>>talent[3]
		>>talent[4]
		>>talent[5]
		>>describe
		>>showEnable
		>>iconPic
		>>bodyPic
		>>namePic
		>>backPic
		>>battlePic
		>>soundUrl
		>>orderId
		>>favorability
		>>morals;

		levelupRate = tmpLevelupRate / BASE_RATE_VAL;
}

//-----------------------------------------------------------------------------------------------
//DiscipleTableManager
//-----------------------------------------------------------------------------------------------
void DiscipleTableManager::init( const std::string& filename )
{
	DiscipleList::iterator itr = mDiscipleList.begin();
	while(itr != mDiscipleList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mDiscipleList.clear();
	mIdPool.clear();
	mTotalHandbookShownNum = -1;
	parse(filename, 1);
}

//-----------------------------------------------------------------------------------------------
void DiscipleTableManager::readline( std::stringstream& _stream )
{
	DiscipleItem* data = new DiscipleItem;
	data->readline(_stream);
	mDiscipleList.insert(DiscipleList::value_type(data->itemID, data));
	mIdPool.push_back(data->itemID);
}

//-----------------------------------------------------------------------------------------------
const DiscipleItem * DiscipleTableManager::getDiscipleItemByID( unsigned int id )
{
	if(mDiscipleList.find(id) != mDiscipleList.end())
		return mDiscipleList.find(id)->second;

	return NULL;
}

//-----------------------------------------------------------------------------------------------
const DiscipleItem * DiscipleTableManager::getDiscipleItemByName( const std::string& name )
{
	DiscipleList::iterator itr = mDiscipleList.begin();
	while(itr != mDiscipleList.end())
	{
		if(itr->second->name == name)
			return itr->second;
	}

	return NULL;
}

unsigned int DiscipleTableManager::getHandBookShownTotalNum(){
	if (mTotalHandbookShownNum>0)
	{
		return mTotalHandbookShownNum;
	}else{
		DiscipleList::iterator itr = mDiscipleList.begin();
		mTotalHandbookShownNum = 0;
		while(itr != mDiscipleList.end())
		{
			DiscipleItem* item = itr->second;
			if (item->showEnable)
			{
				mTotalHandbookShownNum++;
			}
			++itr;
		}
	}
	

	return 0;
}

const std::string DiscipleTableManager::getYuanfenDescribe( unsigned int id )
{
	std::string describe("");
	if(mDiscipleList.find(id) != mDiscipleList.end())
	{
		DiscipleList::iterator itr = mDiscipleList.find(id);
		DiscipleItem* item = itr->second; 
		if(item)
		{
			for(int i = 0; i<MAX_YUANFEN_NUM; ++i)
			{
				const YuanfenItem * yuanfen = YuanfenTableManager::Get()->getYuanfenItemByID(item->yuanfens[i]);
				if(yuanfen)
					describe += yuanfen->describe + "\n";
			}
		}
	}
	return describe;
}

unsigned int DiscipleTableManager::getRandomDiscipleId()
{
	idPoolVec::iterator it=mIdPool.begin()+rand()%getTotalNum();
	return *it;
}

DiscipleTableManager* DiscipleTableManager::getInstance()
{
	return DiscipleTableManager::Get();
}

//-----------------------------------------------------------------------------------------------
//EquipTableManager
//-----------------------------------------------------------------------------------------------
// const std::string& getEquipAtkTypeImageFile( unsigned int type )
// {
// 	{
// 		switch(type)
// 		{
// 		case 1:
// 			return Equip1_ImageFile;
// 		case 2:
// 			return Equip2_ImageFile;
// 		case 3:
// 			return Equip3_ImageFile;
// 		default:
// 			{
// 				return empty;
// 			}
// 		}
// 	}
// }
std::string DragonSoulTableManager::getCanExchageItemIds()
{
	std::string reVal;
	for(std::map<int,DragonSoulItem*>::iterator iter=mDragonSoulMap.begin();iter!=mDragonSoulMap.end();iter++)
	{
		if(iter->second->exchagePrice>0)
		{
			if(reVal=="")
			{
				reVal=StringConverter::toString(iter->second->itemID);
			}
			else
			{
				reVal=reVal+"_"+StringConverter::toString(iter->second->itemID);
			}
		}
	}
	return reVal;
}
void DragonSoulItem::readline(std::stringstream& _stream)
{
	std::string initAttrString,attrGrouthString;
	_stream>>itemID
		>>name
		>>quality
		>>qualityLevel
		>>initAttrString
		>>attrGrouthString
		>>dynamicPic
		>>staticPic
		>>exp
		>>expFactor
		>>exchagePrice
		>>order;
	std::vector<unsigned int> vec;
	StringConverter::parseVector(initAttrString,vec);
	for (int i = 0; i < vec.size()&&vec.size()%2==0; i+=2)
	{
		initAttr.insert(std::make_pair(vec[i],vec[i+1]));
	}
	vec.clear();

	StringConverter::parseVector(attrGrouthString,vec);
	for (int i = 0; i < vec.size()&&vec.size()%2==0; i+=2)
	{
		attrGrouth.insert(std::make_pair(vec[i],vec[i+1]));
	}
	vec.clear();

}
void TreasureItem::readline(std::stringstream& _stream)
{
	unsigned int levelup = 0;
	_stream
		>>itemID
		>>name
		>>describe
		>>quality
		>>qualityLevel
		>>type
		>>levelup
		>>baseEatExp
		>>baseAttrStr
		>>extAttrStr[0]
		>>extAttrStr[1]
		>>attrFor20Str
		>>attrFor40Str
		>>fragmentsStr
		>>iconPic
		>>bodyPic
		>>namePic
		>>salePrice
		>>residentSnatch
		>>canUseEquip
		>>autoOrder
		>>canTreasureRefine
		>>canRefine
		>>refineMaterialID
		>>attrRefinesStr
		>>canIntensify;
	levelupRate = levelup/BASE_RATE_VAL;
	StringConverter::parseVectorForUnderline(baseAttrStr,baseAtrr);
	StringConverter::parseVectorForUnderline(extAttrStr[0],extAttr[0]);
	StringConverter::parseVectorForUnderline(extAttrStr[1],extAttr[0]);
	StringConverter::parseVectorForUnderline(attrFor20Str,attrFor20);
	StringConverter::parseVectorForUnderline(attrFor40Str,attrFor40);
	StringConverter::parseVectorForUnderline(fragmentsStr,fragments);
	StringConverter::parseVectorForUnderline(attrRefinesStr,attrRefines);
}
void DragonSoulTableManager::init(const std::string& filename)
{
	std::map<int,DragonSoulItem*>::iterator iter=mDragonSoulMap.begin();
	while(iter!=mDragonSoulMap.end())
	{
		delete iter->second;
		iter->second=NULL;
		++iter;
	}
	mDragonSoulMap.clear();
	mIdPool.clear();
	parse(filename,1);
}
void TreasureTableManager::init( const std::string& filename )
{
	TreasureList::iterator itr = mTreasureList.begin();
	while(itr != mTreasureList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mTreasureList.clear();
	mIdPool.clear();
	parse(filename ,1);
}
void DragonSoulTableManager::readline(std::stringstream & _stream)
{
	DragonSoulItem* data=new DragonSoulItem;
	data->readline(_stream);
	mDragonSoulMap.insert(map<int,DragonSoulItem*>::value_type(data->itemID,data));
	mIdPool.push_back(data->itemID);
}
void TreasureTableManager::readline( std::stringstream& _stream )
{
	TreasureItem* data = new TreasureItem;
	data->readline(_stream);
	mTreasureList.insert(TreasureList::value_type(data->itemID, data));
	mIdPool.push_back(data->itemID);
}
const DragonSoulItem* DragonSoulTableManager::getDragonSoulItemById(int id)
{
	std::map<int,DragonSoulItem*>::iterator iter=mDragonSoulMap.find(id);
	if(iter!=mDragonSoulMap.end())
	{
		return iter->second;
	}
	CCAssert(false,"the DragonSoulItem you find is not exist,cehua make a mistake");
	return NULL;

}

const TreasureItem* TreasureTableManager::getTreasureItemByID(unsigned int id)
{
	if(mTreasureList.find(id) != mTreasureList.end())
		return mTreasureList.find(id)->second;

	return NULL;
}

unsigned int TreasureTableManager::getRandomTreasureId()
{
	idPoolVec::iterator it=mIdPool.begin()+rand()%getTotalNum();
	return *it;
}

void TreasureLevelParam::readline(std::stringstream& _stream)
{
	_stream
		>>index
		>>param;
}

void TreasureLevelParamManager::init(const std::string& filename)
{
	TreasureLevelParamList::iterator itr = mTreasureLevelParamList.begin();
	while(itr != mTreasureLevelParamList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mTreasureLevelParamList.clear();

	parse(filename, 1);
}

unsigned int TreasureLevelParamManager::getParamByLevel(unsigned int level)
{
	TreasureLevelParamList::iterator it = mTreasureLevelParamList.find(level);
	if (it != mTreasureLevelParamList.end())
	{
		return it->second->param;
	}

	return 0.0f;
}

unsigned int TreasureLevelParamManager::getTotalParamByLevel(unsigned int level)
{
	unsigned int total = 0;
	for(unsigned int i = 1; i < level; ++i)
	{
		total += mTreasureLevelParamList[i]->param;
	}
	return total;
}


void TreasureLevelParamManager::readline(std::stringstream& _stream)
{
	TreasureLevelParam* data = new TreasureLevelParam;
	data->readline(_stream);
	mTreasureLevelParamList.insert(TreasureLevelParamList::value_type(data->index, data));
}

TreasureTableManager* TreasureTableManager::getInstance()
{
	return TreasureTableManager::Get();
}

DragonSoulTableManager* DragonSoulTableManager::getInstance()
{
	return DragonSoulTableManager::Get();
}

void TreasurePurify::readline(std::stringstream& _stream)
{
	_stream
		>>itemID
		>>quality
		>>refineLevel
		>>times
		>>number
		>>gold;
}

void TreasurePurifyManager::init(const std::string& filename)
{
	TreasurePurifyList::iterator itr = mTreasurePurifyList.begin();
	while(itr != mTreasurePurifyList.end())
	{
		delete itr->second;
		itr->second = NULL;
		++itr;
	}

	mTreasurePurifyList.clear();
	parse(filename,1);
}

void TreasurePurifyManager::readline(std::stringstream& _stream)
{
	TreasurePurify* data = new TreasurePurify;
	data->readline(_stream);
	mTreasurePurifyList.insert(TreasurePurifyList::value_type(data->itemID,data));
}

const TreasurePurify* TreasurePurifyManager::getTreasurePurifyByID(unsigned int qualityID,unsigned int refineLevel)
{
	TreasurePurifyList::iterator it = mTreasurePurifyList.begin();
	TreasurePurifyList::iterator end = mTreasurePurifyList.end();
	for ( ; it != end; ++it)
	{
		if (it->second->quality == qualityID && it->second->refineLevel == refineLevel)
		{
			return it->second;
		}
	}
	return NULL;
}

TreasurePurifyManager* TreasurePurifyManager::getInstance()
{
	return TreasurePurifyManager::Get();
}


void SuitItem::readline(std::stringstream& _stream)
{
	_stream
		>>itemID
		>>name
		>>suit[0]
		>>suit[1]
		>>suit[2]
		>>suit[3]
		>>addAttrStrFor2
		>>addAttrStrFor3
		>>addAttrStrFor4;

		std::vector<unsigned int> vec;
		StringConverter::parseVector(addAttrStrFor2,vec);
		for (int i = 0; i < vec.size(); i+=2)
		{
			addAttrFor2.insert(std::make_pair(vec[i],vec[i+1]));
		}
		vec.clear();
		StringConverter::parseVector(addAttrStrFor3,vec);
		for (int i = 0; i < vec.size(); i+=2)
		{
			addAttrFor3.insert(std::make_pair(vec[i],vec[i+1]));
		}
		vec.clear();
		StringConverter::parseVector(addAttrStrFor4,vec);
		for (int i = 0; i < vec.size(); i+=2)
		{
			addAttrFor4.insert(std::make_pair(vec[i],vec[i+1]));
		}
}

void SuitTableManager::init( const std::string& filename )
{
	SuitList::iterator itr = mSuitList.begin();
	while(itr != mSuitList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mSuitList.clear();

	parse(filename ,1);
}

void SuitTableManager::readline( std::stringstream& _stream )
{
	SuitItem* data = new SuitItem;
	data->readline(_stream);
	mSuitList.insert(SuitList::value_type(data->itemID, data));
}

const SuitItem* SuitTableManager::getSuitItemByID(unsigned int id)
{
	if(mSuitList.find(id) != mSuitList.end())
		return mSuitList.find(id)->second;

	return NULL;
}

unsigned int SuitTableManager::getSuitNumByID(unsigned int id)
{
	const SuitItem* item = this->getSuitItemByID(id);
	if (!item)
	{
		return 0;
	}
	for (unsigned int i = 0;i < SUIT_NUM; ++i)
	{
		if (item->suit[i] == 0)
		{
			return i;
		}
	}

	return SUIT_NUM;
}

void EquipItem::readline(std::stringstream& _stream)
{
	unsigned int tmpRate = 0;
	_stream
		>>itemID
		>>name
		>>type
		>>quality
		>>qualityLevel
		>>tmpRate
		>>suitId
		>>sellPrice
		>>attrStr
		>>growthAttrStr
		>>baptizeAttrStr[0]
		>>baptizeAttrStr[1]
		>>baptizeAttrStr[2]
		>>baptizeAttrStr[3]
		>>describe			
		>>iconPic
		>>bodyPic
		>>namePic
		//dmq
		>>orderId
		//升星
		>>starAttr;
	strengthConsumeRate = tmpRate / BASE_RATE_VAL;
	std::vector<unsigned int> vec;
	StringConverter::parseVector(attrStr,vec);
	for (int i = 0; i < vec.size()&&vec.size()%2==0; i+=2)
	{
		initAttr.insert(std::make_pair(vec[i],vec[i+1]));
	}
	vec.clear();
	StringConverter::parseVector(growthAttrStr,vec);
	for (int i = 0; i < vec.size()&&vec.size()%2==0; i+=2)
	{
		grwothAttr.insert(std::make_pair(vec[i],vec[i+1]));
	}
	for (int i = 0; i < BAPTIZE_NUM; ++i)
	{
		vec.clear();
		StringConverter::parseVector(baptizeAttrStr[i],vec);
		for (int i = 0; i < vec.size()&&vec.size()%2==0; i+=2)
		{
			baptizeAttr.insert(std::make_pair(vec[i],vec[i+1]));
		}
	}

	//升星判断
	const char *split = ", _";
	char str[32];
	sprintf(str,starAttr.c_str());
	char *p;
	p = strtok(str,split);
	while (p)
	{
		starAttrs.push_back(atoi(p));
		p = strtok(NULL,split);
	}

#ifdef _DEBUG
	//CCAssert(Equip::isTypeValid(type), "equip teyp is invalid!!");
#endif
}

void EquipTableManager::init( const std::string& filename )
{
	EquipList::iterator itr = mEquipList.begin();
	while(itr != mEquipList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mEquipList.clear();
	mIdPool.clear();
	parse(filename ,1);
}

//-----------------------------------------------------------------------------------------------
void EquipTableManager::readline( std::stringstream& _stream )
{
	EquipItem* data = new EquipItem;
	data->readline(_stream);
	mEquipList.insert(EquipList::value_type(data->itemID, data));
	mIdPool.push_back(data->itemID);
}

//-----------------------------------------------------------------------------------------------
const EquipItem * EquipTableManager::getEquipItemByID( unsigned int id )
{
	if(mEquipList.find(id) != mEquipList.end())
		return mEquipList.find(id)->second;

	return NULL;
}

//-----------------------------------------------------------------------------------------------
const EquipItem * EquipTableManager::getEquipItemByName( const std::string& name )
{
	EquipList::iterator itr = mEquipList.begin();
	while(itr != mEquipList.end())
	{
		if(itr->second->name == name)
			return itr->second;
	}

	return NULL;
}

unsigned int EquipTableManager::getRandomEquipId()
{
	idPoolVec::iterator it=mIdPool.begin()+rand()%getTotalNum();
	return *it;
}

DiscipleTableManager* EquipTableManager::getInstance()
{
	return DiscipleTableManager::Get();
}

//-----------------------------------------------------------------------------------------------
//EquipLevelParamManager
//-----------------------------------------------------------------------------------------------
void EquipLevelParamManager::init( const std::string& filename )
{
	EquipLevelParamList::iterator itr = mEquipLevelParamList.begin();
	while(itr != mEquipLevelParamList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mEquipLevelParamList.clear();

	parse(filename, 1);
}

//-----------------------------------------------------------------------------------------------
void EquipLevelParamManager::readline( std::stringstream& _stream )
{
	EquipLevelParam* data = new EquipLevelParam;
	data->readline(_stream);
	mEquipLevelParamList.insert(EquipLevelParamList::value_type(data->index, data));
}

//-----------------------------------------------------------------------------------------------
const float EquipLevelParamManager::getParamByLevel( unsigned int level )
{
	unsigned int index = ceilf((float)level / 3);
	EquipLevelParamList::iterator itr = mEquipLevelParamList.find(index);
	if(itr == mEquipLevelParamList.end())
		return 0;

	return itr->second->param;
}

EquipLevelParamManager* EquipLevelParamManager::getInstance()
{
	return EquipLevelParamManager::Get();
}

void EquipEnhanceSilver::readline(std::stringstream& _stream)
{
	_stream
		>> index
		>> param;	
}

void EquipEnhanceSilverTableManager::init(const std::string& filename)
{
	EquipEnhanceSilverList::iterator itr = mEquipEnhanceSilverList.begin();
	while(itr != mEquipEnhanceSilverList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mEquipEnhanceSilverList.clear();

	parse(filename, 1);
}

void EquipEnhanceSilverTableManager::readline(std::stringstream& _stream)
{
	EquipEnhanceSilver* item = new EquipEnhanceSilver;
	item->readline(_stream);
	mEquipEnhanceSilverList.insert(EquipEnhanceSilverList::value_type(item->index, item));
}

float EquipEnhanceSilverTableManager::getParamByLevel(unsigned int level)
{
	EquipEnhanceSilverList::iterator it = mEquipEnhanceSilverList.find(level);
	if (it != mEquipEnhanceSilverList.end())
	{
		return it->second->param;
	}

	return 0.0f;
}

//-----------------------------------------------------------------------------------------------
//EquipFragmentManager
//-----------------------------------------------------------------------------------------------
void EquipFragmentManager::init( const std::string& filename )
{
	EquipFragmentItemList::iterator itr = mEquipFragmentItemList.begin();
	while(itr != mEquipFragmentItemList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mEquipFragmentItemList.clear();

	parse(filename, 1);
}

void EquipFragmentManager::readline( std::stringstream& _stream )
{
	EquipFragmentItem* item = new EquipFragmentItem;
	item->readline(_stream);
	mEquipFragmentItemList.insert(EquipFragmentItemList::value_type(item->itemID, item));
}

bool EquipFragmentManager::isForgingMap(unsigned int itemID)
{
	unsigned int typeId = itemID / 1000;
	return typeId == ForgingMap_WEAPON || typeId == ForgingMap_ARMOR || typeId == ForgingMap_ACCESSORIES;
}
const EquipFragmentItem * EquipFragmentManager::getItemByID( unsigned int id )
{
	if ( !isForgingMap(id) )
	{
		return NULL;
	}

	if( mEquipFragmentItemList.find(id) != mEquipFragmentItemList.end() )
	{
		return mEquipFragmentItemList.find(id)->second;
	}
	return NULL;
}

const std::string EquipFragmentManager::getRequirementByItemID(unsigned int itemID, bool lineBreak/* = false*/)
{
	const EquipFragmentItem* fragmentItem = getItemByID(itemID);
	if ( fragmentItem )
	{
		std::string requirement;
		ResInfoMap resInfoMap = ResManager::Get()->getResInfoListByResBagStr(fragmentItem->pieceConsume);
		for (ResInfoMap::iterator it = resInfoMap.begin(); it != resInfoMap.end(); ++it)
		{
			if ( it->second->itemId != itemID )
			{
				if ( lineBreak )
				{
					requirement += "\n";
				}
				requirement += it->second->name + "*";
				if ( lineBreak )
				{
					requirement += "\n";
				}
				requirement += StringConverter::toString(it->second->count) + ",";
			}
		}
		if ( requirement.at(requirement.length() - 1) == ',' ) {
			//requirement.pop_back(); //only in c++ 11
			requirement.erase(requirement.length() - 1);
		}
		return requirement;
	}
	return "";
}

EquipFragmentManager* EquipFragmentManager::getInstance()
{
	return EquipFragmentManager::Get();
}

//升星属性表---------------------------------------------------------------------------------------
void StarAttrItem::readline(std::stringstream& _stream)
{
	_stream
		>>starAttrID
		>>name
		>>describe;		
}
void StarAttrTableManager::init( const std::string& filename )
{
	StarAttrList::iterator itr = mStarAttrList.begin();
	while(itr != mStarAttrList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mStarAttrList.clear();
	mIdPool.clear();
	parse(filename ,1);
}

void StarAttrTableManager::readline( std::stringstream& _stream )
{
	StarAttrItem* data = new StarAttrItem;
	data->readline(_stream);
	mStarAttrList.insert(StarAttrList::value_type(data->starAttrID, data));
	mIdPool.push_back(data->starAttrID);
}

const StarAttrItem * StarAttrTableManager::getStarAttrItemByID( unsigned int id )
{
	if(mStarAttrList.find(id) != mStarAttrList.end())
		return mStarAttrList.find(id)->second;

	return NULL;
}

const StarAttrItem * StarAttrTableManager::getStarAttrItemByName( const std::string& name )
{
	StarAttrList::iterator itr = mStarAttrList.begin();
	while(itr != mStarAttrList.end())
	{
		if(itr->second->name == name)
			return itr->second;
	}

	return NULL;
}

unsigned int StarAttrTableManager::getRandomStarAttrId()
{
	idPoolVec::iterator it=mIdPool.begin()+rand()%getTotalNum();
	return *it;
}

DiscipleTableManager* StarAttrTableManager::getInstance()
{
	return DiscipleTableManager::Get();
}
//---------------------------------------------------------------------------------------

//升星等级表---------------------------------------------------------------------------------------
void StarLevelItem::readline(std::stringstream& _stream)
{
	_stream
		>>starLevel//升星等级
		>>levelNeed//升星的强化等级限制
		>>creamCost//升星消耗碎片
		>>goldCost//升星金钱消耗
	    >>sympathyAttr//共鸣属性等级
		>>sympathyDescribe//共鸣属性介绍
		>>iconEffect//图标特效
		>>sympathyEffect;//共鸣特效
}
void StarLevelTableManager::init( const std::string& filename )
{
	StarLevelList::iterator itr = mStarLevelList.begin();
	while(itr != mStarLevelList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mStarLevelList.clear();
	mIdPool.clear();
	parse(filename ,1);
}

void StarLevelTableManager::readline( std::stringstream& _stream )
{
	StarLevelItem* data = new StarLevelItem;
	data->readline(_stream);
	mStarLevelList.insert(StarLevelList::value_type(data->starLevel, data));
	mIdPool.push_back(data->starLevel);
}

const StarLevelItem * StarLevelTableManager::getStarLevelItemByLevel( unsigned int id )
{
	if(mStarLevelList.find(id) != mStarLevelList.end())
		return mStarLevelList.find(id)->second;

	return NULL;
}

unsigned int StarLevelTableManager::getRandomStarLevelId()
{
	idPoolVec::iterator it=mIdPool.begin()+rand()%getTotalNum();
	return *it;
}

DiscipleTableManager* StarLevelTableManager::getInstance()
{
	return DiscipleTableManager::Get();
}
//---------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
//ExtEquipBagManager
//-----------------------------------------------------------------------------------------------
void ExtEquipBagManager::init( const std::string& filename )
{
	ExtEquipBagItemList::iterator itr = mExtEquipBagItemList.begin();
	while(itr != mExtEquipBagItemList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mExtEquipBagItemList.clear();

	parse(filename, 1);
}

void ExtEquipBagManager::readline( std::stringstream& _stream )
{
	ExtEquipBagInfo* item = new ExtEquipBagInfo;
	item->readline(_stream);
	mExtEquipBagItemList.insert(ExtEquipBagItemList::value_type(item->type, item));
}

const ExtEquipBagInfo * ExtEquipBagManager::getItemByID( unsigned int id )
{
	if( mExtEquipBagItemList.find(id) != mExtEquipBagItemList.end() )
	{
		return mExtEquipBagItemList.find(id)->second;
	}
	return NULL;
}

ExtEquipBagManager* ExtEquipBagManager::getInstance()
{
	return ExtEquipBagManager::Get();
}

void ItemOperation::readline(std::stringstream& _stream)
{
	_stream 
		>>quality
		>>type
		>>canRefinery
		>>canReborth
		>>canSell;
}

void ItemOperationTableManager::init(const std::string& filename)
{
	ItemOperationList::iterator itr = mItemOperationList.begin();
	while(itr != mItemOperationList.end())
	{
		delete *itr;
		++itr;
	}
	mItemOperationList.clear();

	parse(filename,1);
}

void ItemOperationTableManager::readline(std::stringstream& _stream)
{
	ItemOperation* data = new ItemOperation;
	data->readline(_stream);
	mItemOperationList.push_back(data);
}

ItemOperation* ItemOperationTableManager::getItemOperation(unsigned int quality,unsigned int type)
{
	ItemOperationList::iterator it = mItemOperationList.begin();
	ItemOperationList::iterator end = mItemOperationList.end();
	for ( ; it != end; ++it)
	{
		ItemOperation* item = *it;
		if (item->quality == quality && item->type == type)
		{
			return item;
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------------------------
//SkillTableManager
//-----------------------------------------------------------------------------------------------

// const std::string& getSkillAtkTypeImageFile( unsigned int type )
// {
// 	{
// 		switch(type)
// 		{
// 		case 1:
// 			return Skill1_ImageFile;
// 		case 2:
// 			return Skill2_ImageFile;
// 		case 3:
// 			return Skill3_ImageFile;
// 		case 4:
// 			return Skill4_ImageFile;
// 		case 5:
// 			return Skill5_ImageFile;
// 		case 6:
// 			return Skill6_ImageFile;
// 		default:
// 			{
// 				return empty;
// 			}
// 		}
// 	}
// }

void SkillTableManager::init( const std::string& filename )
{
	SkillList::iterator itr = mSkillList.begin();
	while(itr != mSkillList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mSkillList.clear();
	mIdPool.clear();
	parse(filename,1);
}

//-----------------------------------------------------------------------------------------------
void SkillTableManager::readline( std::stringstream& _stream )
{
	SkillItem* data = new SkillItem;
	data->readline(_stream);
	mSkillList.insert(SkillList::value_type(data->itemID, data));
	mIdPool.push_back(data->itemID);
}

//-----------------------------------------------------------------------------------------------
const SkillItem * SkillTableManager::getSkillItemByID( unsigned int id )
{
	if(mSkillList.find(id) != mSkillList.end())
		return mSkillList.find(id)->second;

	return NULL;
}

//-----------------------------------------------------------------------------------------------
const SkillItem * SkillTableManager::getSkillItemByName( const std::string& name )
{
	SkillList::iterator itr = mSkillList.begin();
	while(itr != mSkillList.end())
	{
		if(itr->second->name == name)
			return itr->second;
	}

	return NULL;
}

unsigned int SkillTableManager::getRandomSkillId()
{
	idPoolVec::iterator it=mIdPool.begin()+rand()%getTotalNum();
	return *it;
}

SkillTableManager* SkillTableManager::getInstance()
{
	return SkillTableManager::Get();
}

//-----------------------------------------------------------------------------------------------
//FragmentBookTableManager
//-----------------------------------------------------------------------------------------------

void FragmentBookTableManager::init( const std::string& filename )
{
	FragmentBookTableItemTypeList::iterator fragItr=mItemFragmentBookList.begin();
	while(fragItr!=mItemFragmentBookList.end())
	{
		FragmentBookTableItemList* mFragmentBookList=NULL;
		if(fragItr->second)
		{
			mFragmentBookList=fragItr->second;
		}
		FragmentBookTableItemList::iterator itr = mFragmentBookList->begin();
		while(itr != mFragmentBookList->end())
		{
			FragmentBookTable::FragmentItemList::iterator it=itr->second->mFragmentItemList.begin();
			while(it!=itr->second->mFragmentItemList.end())
			{
				delete it->second;
				it->second=NULL;
				++it;
			}
			itr->second->mFragmentItemList.clear();
			delete itr->second;
			itr->second =NULL;
			++itr;
		}
		mFragmentBookList->clear();
		delete mFragmentBookList;
		delete fragItr->second;
		fragItr->second=NULL;
		++fragItr;
	}
	mItemFragmentBookList.clear();
	FragmentBookItemList::iterator itemItr=mFragmentBookItemList.begin();
	while(itemItr!=mFragmentBookItemList.end())
	{
	  if(itemItr->second)
	  {
		  delete itemItr->second;
		  itemItr->second=NULL;
	  }
	  ++itemItr;
	}
	mFragmentBookItemList.clear();
	mIdPool.clear();
	parse(filename,1);
}

//-----------------------------------------------------------------------------------------------

FragmentBookTableManager::FragmentBookTableItemList* FragmentBookTableManager::getFragmentBookTableItemListByItemType(unsigned int _itemType)
{
	if(mItemFragmentBookList.find(_itemType)!=mItemFragmentBookList.end())
	{
		return mItemFragmentBookList.find(_itemType)->second;
	}
	return NULL;
}

void FragmentBookTableManager::readline( std::stringstream& _stream )
{
	FragmentBookTable::FragmentItem* data = new FragmentBookTable::FragmentItem;
	data->readline(_stream);
	FragmentBookTable* skillTable=NULL;
	FragmentBookTableItemList *_fragmentBookTableMap=getFragmentBookTableItemListByItemType(data->itemType);
	if(!_fragmentBookTableMap)
	{
		_fragmentBookTableMap=new FragmentBookTableItemList;
		mItemFragmentBookList.insert(FragmentBookTableItemTypeList::value_type(data->itemType,_fragmentBookTableMap));
	}
	if(_fragmentBookTableMap->find(data->itemId)==_fragmentBookTableMap->end())
	{
		skillTable=new FragmentBookTable();
		_fragmentBookTableMap->insert(FragmentBookTableItemList::value_type(data->itemId,skillTable));
	}
	else
	{
		skillTable=_fragmentBookTableMap->find(data->itemId)->second;
	}
	skillTable->mFragmentItemList.insert(FragmentBookTable::FragmentItemList::value_type(data->fragmentId, data));
	mFragmentBookItemList.insert(FragmentBookItemList::value_type(data->fragmentId, data));
	mIdPool.push_back(data->fragmentId);
}


FragmentBookTable* FragmentBookTableManager::getFragmentBookTableByTypeAndItemId(unsigned int _itemType,unsigned int itemId)
{
	FragmentBookTableManager::FragmentBookTableItemList* _fragmentBookList=getFragmentBookTableItemListByItemType(_itemType);
	if(_fragmentBookList)
	{
		if(_fragmentBookList->find(itemId)!=_fragmentBookList->end())
		{
			return _fragmentBookList->find(itemId)->second;
		}
	}
	return NULL;
}

FragmentBookTable::FragmentItem* FragmentBookTableManager::getFragmentItemByFragmentId(unsigned int id)
{
	if(mFragmentBookItemList.find(id)!=mFragmentBookItemList.end())
	{
		return mFragmentBookItemList.find(id)->second;
	}
	return NULL;
}

FragmentBookTableManager* FragmentBookTableManager::getInstance()
{
	return FragmentBookTableManager::Get();
}

unsigned int FragmentBookTableManager::getRandomFragmentId()
{
	idPoolVec::iterator it=mIdPool.begin()+rand()%getTotalNum();
	return *it;
}

//-----------------------------------------------------------------------------------------------
//RandomRewardTableManager
//-----------------------------------------------------------------------------------------------
void RandomRewardTableManager::init( const std::string& filename )
{
	RandomRewardList::iterator itr = mRandomRewardList.begin();
	while(itr != mRandomRewardList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mRandomRewardList.clear();

	if(mIdPool.size()>0)
	{
		mIdPool.clear();
	}
	parse(filename,1);
}

//-----------------------------------------------------------------------------------------------
void RandomRewardTableManager::readline( std::stringstream& _stream )
{
	RandomRewardItem* data = new RandomRewardItem;
	data->readline(_stream);
	mRandomRewardList.insert(RandomRewardList::value_type(data->id, data));
	mIdPool.push_back(data->id);
}

//-----------------------------------------------------------------------------------------------
const RandomRewardItem * RandomRewardTableManager::getRandomRewardItemByID( unsigned int id )
{
	if(mRandomRewardList.find(id) != mRandomRewardList.end())
		return mRandomRewardList.find(id)->second;
	return NULL;
}

const RandomRewardItem * RandomRewardTableManager::randomReward()
{
	idPoolVec::iterator it=mIdPool.begin()+rand()%mRandomRewardList.size();
	return getRandomRewardItemByID(*it);
}


//-----------------------------------------------------------------------------------------------
//ToolTableManager
//-----------------------------------------------------------------------------------------------
void ToolTableManager::init( const std::string& filename )
{
	ToolList::iterator itr = mToolList.begin();
	while(itr != mToolList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mToolList.clear();
	mIdPool.clear();
	parse(filename, 1);

	//initIncludeOther();//这里又调了Get，stack overflow
}

//-----------------------------------------------------------------------------------------------
void ToolTableManager::readline( std::stringstream& _stream )
{
	ToolItem* data = new ToolItem;
	data->readline(_stream);
	mToolList.insert(ToolList::value_type(data->itemID, data));
	mIdPool.push_back(data->itemID);
}

//-----------------------------------------------------------------------------------------------
ToolItem * ToolTableManager::getToolItemByID( unsigned int id )
{
	if(mToolList.find(id) != mToolList.end())
		return mToolList.find(id)->second;

	return NULL;
}
void ToolTableManager::updateToolItemInfo(ToolItem* toolItem)
{
	if(!toolItem)
		return;
	ToolItem * item;
	unsigned int id=toolItem->itemID;
	if(mToolList.find(id) != mToolList.end())
	{
		item=mToolList.find(id)->second;
		if(item)
		{
			item->name = toolItem->name;
			item->describe = toolItem->describe;
		}
		else
		{
			mToolList.insert(ToolList::value_type(toolItem->itemID,toolItem));
		}
		
	}
	
}

void ToolTableManager::removeToolItemInfoById(unsigned int itemId)
{
	if(mToolList.find(itemId)==mToolList.end())
		return;
	mToolList.erase(itemId);
}
//-----------------------------------------------------------------------------------------------
const ToolItem * ToolTableManager::getToolItemByName( const std::string& name )
{
	ToolList::iterator itr = mToolList.begin();
	while(itr != mToolList.end())
	{
		if(itr->second->name == name)
			return itr->second;
	}

	return NULL;
}
void ToolTableManager::initIncludeOther()
{
	ToolList::iterator it = mToolList.begin();
	for(;it!=mToolList.end();it++)
	{
		try
		{
			bool flag = (it->second->includeStr=="none")||(it->second->includeStr=="0");
			if (!flag)
			{
				it->second->includeOther = ResManager::Get()->getResInfoListByResBagStr(it->second->includeStr);
			}
		}catch(...)
		{
			char errMsg[128]={0};
			sprintf(errMsg,"Tool includeOther string is not match the right fromat! id:%d",it->second->itemID);
			cocos2d::CCMessageBox(errMsg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());
		}
	}
	
}

unsigned int ToolTableManager::getRandomToolId()
{
	idPoolVec::iterator it=mIdPool.begin()+rand()%getTotalNum();
	return *it;
}

ToolTableManager* ToolTableManager::getInstance()
{
	return ToolTableManager::Get();
}
//-----------------------------------------------------------------------------------------------
//ToolsWdxTableManager
//-----------------------------------------------------------------------------------------------
void ToolsExtendTableManager::init( const std::string& filename )
{
	ToolList::iterator itr = mToolList.begin();
	while(itr != mToolList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mToolList.clear();

	parse(filename, 1);

}
//-----------------------------------------------------------------------------------------------
void ToolsExtendTableManager::clearList()
{
	ToolList::iterator itr = mToolList.begin();
	while(itr != mToolList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mToolList.clear();
}
//-----------------------------------------------------------------------------------------------
void ToolsExtendTableManager::readline( std::stringstream& _stream )
{
	ToolItem* data = new ToolItem;
	data->readline(_stream);
	mToolList.insert(ToolList::value_type(data->itemID, data));
}
//-----------------------------------------------------------------------------------------------
ToolsExtendTableManager* ToolsExtendTableManager::getInstance()
{
	return ToolsExtendTableManager::Get();
}
//-----------------------------------------------------------------------------------------------
//DisciplesLevelParamManager
//-----------------------------------------------------------------------------------------------
void DisciplesLevelParamManager::init( const std::string& filename )
{
	DisciplesLevelParamList::iterator itr = mDisciplesLevelParamList.begin();
	while(itr != mDisciplesLevelParamList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mDisciplesLevelParamList.clear();

	parse(filename, 1);
}

//-----------------------------------------------------------------------------------------------
void DisciplesLevelParamManager::readline( std::stringstream& _stream )
{
	DisciplesLevelParam* data = new DisciplesLevelParam;
	data->readline(_stream);
	mDisciplesLevelParamList.insert(DisciplesLevelParamList::value_type(data->index, data));
}

//-----------------------------------------------------------------------------------------------
const float DisciplesLevelParamManager::getParamByLevel( unsigned int level )
{
	return mDisciplesLevelParamList[ level ]->param;
}

//-----------------------------------------------------------------------------------------------
unsigned int DisciplesLevelParamManager::getTotalParamByLevel( unsigned int level )
{
	unsigned int total = 0;
	for(unsigned int i = 1; i < level; ++i)
	{
		total += mDisciplesLevelParamList[i]->param;
	}
	return total;
}

DisciplesLevelParamManager* DisciplesLevelParamManager::getInstance()
{
	return DisciplesLevelParamManager::Get();
}

void RefineDescTableManager::init(const std::string& filename)
{
	RefineDescList::iterator itr = mRefineDescList.begin();
	while(itr != mRefineDescList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mRefineDescList.clear();

	parse(filename, 1);
}

void RefineDescTableManager::readline( std::stringstream& _stream )
{
	RefineDescItem* data = new RefineDescItem;
	data->readline(_stream);
	mRefineDescList.insert(RefineDescList::value_type(data->itemID, data));
}

const RefineDescItem * RefineDescTableManager::getRefineDescItemByID( unsigned int id )
{
	if(mRefineDescList.find(id) != mRefineDescList.end())
		return mRefineDescList.find(id)->second;

	return NULL;
}

//-----------------------------------------------------------------------------------------------
//HelpTableManager
//-----------------------------------------------------------------------------------------------
void HelpTableManager::init( const std::string& filename )
{
	HelpList::iterator itr = mHelpList.begin();
	while(itr != mHelpList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mHelpList.clear();

	parse(filename, 1);
}

//-----------------------------------------------------------------------------------------------
void HelpTableManager::readline( std::stringstream& _stream )
{
	HelpItem* data = new HelpItem;
	data->readline(_stream);
	mHelpList.insert(HelpList::value_type(data->itemID, data));
}

//-----------------------------------------------------------------------------------------------
const HelpItem * HelpTableManager::getHelpItemByID( unsigned int id )
{
	if(mHelpList.find(id) != mHelpList.end())
		return mHelpList.find(id)->second;

	return NULL;
}

HelpTableManager* HelpTableManager::getInstance()
{
	return HelpTableManager::Get();
}

//-----------------------------------------------------------------------------------------------
//VIPPrivilegeTableManager
//-----------------------------------------------------------------------------------------------
void VIPPrivilegeTableManager::init(const std::string& filename){
	VIPPrivilegeList::iterator itr = mVIPPrivilegeList.begin();
	while(itr != mVIPPrivilegeList.end()){
		delete itr->second;
		itr->second = NULL;
		++itr;
	}
	mVIPPrivilegeList.clear();

	parse(filename, 1);
}
//-----------------------------------------------------------------------------------------------
void VIPPrivilegeTableManager::readline(std::stringstream& _stream){
	VIPPrivilegeItem* data = new VIPPrivilegeItem;
	data->readline(_stream);
	mVIPPrivilegeList.insert(VIPPrivilegeList::value_type(data->itemID,data));
}
//-----------------------------------------------------------------------------------------------
const VIPPrivilegeItem * VIPPrivilegeTableManager::getVIPPrivilegeItemByID(unsigned int id){
	if(mVIPPrivilegeList.find(id) != mVIPPrivilegeList.end()){
		return mVIPPrivilegeList.find(id)->second;
	}
	return NULL;
}
//-----------------------------------------------------------------------------------------------
const VIPPrivilegeItem * VIPPrivilegeTableManager::getVIPPrivilegeDescribeItemByID(unsigned int id,int describeIndex){
	if(mVIPPrivilegeList.find(id) != mVIPPrivilegeList.end()){
		return mVIPPrivilegeList.find(id)->second;
	}
	return NULL;
}

VIPPrivilegeTableManager* VIPPrivilegeTableManager::getInstance()
{
	return VIPPrivilegeTableManager::Get();
}

//-----------------------------------------------------------------------------------------------
//AboutTableManager
//-----------------------------------------------------------------------------------------------
void AboutTableManager::init( const std::string& filename )
{
	AboutList::iterator itr = mAboutList.begin();
	while(itr != mAboutList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mAboutList.clear();

	parse(filename, 1);
}

//-----------------------------------------------------------------------------------------------
void AboutTableManager::readline( std::stringstream& _stream )
{
	AboutItem* data = new AboutItem;
	data->readline(_stream);
	mAboutList.insert(AboutList::value_type(data->itemID, data));
}

//-----------------------------------------------------------------------------------------------
const AboutItem * AboutTableManager::getAboutItemByID( unsigned int id )
{
	if(mAboutList.find(id) != mAboutList.end())
		return mAboutList.find(id)->second;

	return NULL;
}

AboutTableManager* AboutTableManager::getInstance()
{
	return AboutTableManager::Get();
}

//-----------------------------------------------------------------------------------------------
//MainShowAdventureTableManager
//-----------------------------------------------------------------------------------------------
void MainShowAdventureTableManager::init( const std::string& filename )
{
	MainShowAdventureList::iterator itr = mMainShowAdventureList.begin();
	while(itr != mMainShowAdventureList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mMainShowAdventureList.clear();

	parse(filename, 1);
}

//-----------------------------------------------------------------------------------------------
void MainShowAdventureTableManager::readline( std::stringstream& _stream )
{
	MainShowAdventureItem* data = new MainShowAdventureItem;
	data->readline(_stream);
	mMainShowAdventureList.insert(MainShowAdventureList::value_type(data->adventureItemId, data));
}

//-----------------------------------------------------------------------------------------------
const MainShowAdventureItem * MainShowAdventureTableManager::getMainShowAdventureItemByID( unsigned int id )
{
	if(mMainShowAdventureList.find(id) != mMainShowAdventureList.end())
		return mMainShowAdventureList.find(id)->second;

	return NULL;
}


//-----------------------------------------------------------------------------------------------
//AdventureTableManager
//-----------------------------------------------------------------------------------------------
void AdventureTableManager::init( const std::string& filename )
{
	AdventureList::iterator itr = mAdventureList.begin();
	while(itr != mAdventureList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mAdventureList.clear();

	parse(filename, 1);
}

//-----------------------------------------------------------------------------------------------
void AdventureTableManager::readline( std::stringstream& _stream )
{
	AdventureItem* data = new AdventureItem;
	data->readline(_stream);
	mAdventureList.insert(AdventureList::value_type(data->itemID, data));
}

//-----------------------------------------------------------------------------------------------
const AdventureItem * AdventureTableManager::getAdventureItemByID( unsigned int id )
{
	if(mAdventureList.find(id) != mAdventureList.end())
		return mAdventureList.find(id)->second;

	return NULL;
}

const AdventureItem * AdventureTableManager::getAdventureItemByType(unsigned int type)
{
	AdventureList::iterator it = mAdventureList.begin();
	AdventureList::iterator end = mAdventureList.end();
	for ( ; it != end; ++it)
	{
		AdventureItem* d = it->second;
		if (d->adventureType == type)
		{
			return d;
		}
	}

	return NULL;
}

AdventureTableManager* AdventureTableManager::getInstance()
{
	return AdventureTableManager::Get();
}

void SkillItem::readline( std::stringstream& _stream )
{
	std::string		replaceResource;
	_stream
		>>itemID
		>>name
		>>quality
		>>type
		>>describe
		>>initQuantity
		>>initGrowthRate
		>>iconPic
		>>bodyPic
		>>namePic
		//>>backPic
		>>duration
		>>ccbFile
		>>timeLine
		>>replaceResource
		>>soundUrl
		//dmq
		>>orderId;
		
	if(!replaceResource.empty())
	{
		size_t  starter = replaceResource.find_first_of('{');
		size_t	ender = replaceResource.find_first_of('}');
		while(starter!=std::string::npos && ender!=std::string::npos && ender>starter+1)
		{
			do 
			{
				std::string sub = replaceResource.substr(starter+1,ender-starter-1);

				size_t colon = sub.find_first_of(":");
				if(colon==std::string::npos || sub.length()<=colon+1)
					break;

				std::string variable = sub.substr(0,colon);
				std::string texture = "";
				std::string plist = "";

				size_t flagAt = sub.find_first_of('@');
				if(flagAt == std::string::npos)
				{
					texture = sub.substr(colon+1,sub.length()-colon-1); 
				}
				else if(flagAt+1<sub.length() && flagAt>colon+1)
				{
					texture = sub.substr(colon+1,flagAt-colon-1);
					plist = sub.substr(flagAt+1,sub.length()-flagAt-1);
				}

				mReplaceMap.insert(std::make_pair(variable, std::make_pair(texture,plist)));

			} while (false);

			starter = replaceResource.find_first_of('{',ender);
			ender =  replaceResource.find_first_of('}',ender+1);
		}
	}
}

int ToolItem::getLimitPerDayByVipLevel(int vipLevel){
	if (vipLevel<0||vipLevel>13)
	{
		//MSG_BOX("error : vipLevel<0||vipLevel>13");
		return -1;
	}
	if (vecBuyLimitPerDay.size()!=14)
	{
		MSG_BOX("error : buy limit is not correspond to the vip level");
		return -1;
	}
	int limitTime = vecBuyLimitPerDay[vipLevel];
	if (limitTime<0)
	{
		MSG_BOX("error : limit <0");
		return -1;
	}
	return limitTime;
}


bool ToolItem::isNotBuyLimit(){
	if (buyLimitTimePerDay.compare("-1")==0||buyLimitTimePerDay.compare("0")==0)
	{
		return true;
	}else{
		return false;
	}
}

void ToolItem::readline( std::stringstream& _stream )
{
	_stream
		>>itemID
		>>name
		>>describe
		>>price
		>>priceType
		>>operationType
		>>jumpPage
		>>buttonString
		>>iconPic
		>>bodyPic
		>>originPrice
		>>pageID
		>>buyCountLimit
		>>includeStr
		>>showInBag
		>>quality
		>>priceAddFactor
		>>maxPrice
		>>buyLimitTimePerDay
		>>salePrice
		>>existType
		>>goodWill
		>>displayPackage
		>>shopOrderID
		>>orderId;
	
	if (buyLimitTimePerDay.length() == 1)
	{
		int iBuyLimit = StringConverter::parseInt(buyLimitTimePerDay);
		vecBuyLimitPerDay.push_back(iBuyLimit);
	}else{
		StringConverter::parseVector(buyLimitTimePerDay,vecBuyLimitPerDay);
	}
}

void DisplePool::readline( std::stringstream& _stream )
{
	_stream
		>>displeID
		>>sDisplayPool;

	StringConverter::parseVector(sDisplayPool,vecDisplePool);\
}

bool DisplePool::checkDispleInPoolByType(int type){
	int num = vecDisplePool.size();
	for (int i =0;i<num;i++)
	{
		int iPool = vecDisplePool[i];
		if (iPool == type)
		{
			return true;
		}
		
	}
	return false;

}

//-----------------------------------------------------------------------------------------------
//DisplePoolManager
//-----------------------------------------------------------------------------------------------
void DisplePoolManager::init( const std::string& filename )
{
	DisplePoolList::iterator itr = mDisplePoolList.begin();
	while(itr != mDisplePoolList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mDisplePoolList.clear();

	parse(filename, 1);
}

void DisplePoolManager::readline( std::stringstream& _stream )
{
	DisplePool* item = new DisplePool;
	item->readline(_stream);
	mDisplePoolList.insert(DisplePoolList::value_type(item->displeID, item));
}

const DisplePool * DisplePoolManager::getItemByID( unsigned int id )
{
	if( mDisplePoolList.find(id) != mDisplePoolList.end() )
	{
		return mDisplePoolList.find(id)->second;
	}
	return NULL;
}

DisplePoolManager* DisplePoolManager::getInstance()
{
	return DisplePoolManager::Get();
}

void PlayerLevelParamItem::readLine(std::stringstream& _stream)
{
	_stream 
		>> level
		>> exp
		>> discipleMax;
}

void PlayerLevelParamTableManager::init(const std::string& filename)
{
	PlayerLevelParamItemList::iterator it = mPlayerLevelParamItemList.begin();
	while (it != mPlayerLevelParamItemList.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}
	mPlayerLevelParamItemList.clear();
	parse(filename, 1);
}

void PlayerLevelParamTableManager::readline(std::stringstream& _stream)
{
	PlayerLevelParamItem* data = new PlayerLevelParamItem;
	data->readLine(_stream);
	mPlayerLevelParamItemList.insert(PlayerLevelParamItemList::value_type(data->level, data));
}

unsigned int PlayerLevelParamTableManager::getCurExp(unsigned int level, unsigned int totalExp)
{
	PlayerLevelParamItemList::const_iterator it = mPlayerLevelParamItemList.begin();
	PlayerLevelParamItemList::const_iterator end = mPlayerLevelParamItemList.end();
	for (; it != end; ++it)
	{	
		int val = totalExp - it->second->exp;
		if (val < 0)
		{
			return totalExp;
		}		
		totalExp = val;
		++level;
	}

	return totalExp;
}

const PlayerLevelParamItem* PlayerLevelParamTableManager::getPlayerLevelParamByLevel(unsigned int level)
{
	PlayerLevelParamItemList::const_iterator it = mPlayerLevelParamItemList.find(level);
	if (it != mPlayerLevelParamItemList.end())
	{
		return it->second;
	}

	return NULL;
}

//-----------------------------------------------------------------------------------------------
unsigned int PlayerLevelParamTableManager::getNeedPlayerLevelForBattleEnableSize(unsigned int battle_enabel_size)
{
	for (PlayerLevelParamItemList::iterator it = mPlayerLevelParamItemList.begin(); it != mPlayerLevelParamItemList.end(); it ++)
	{
		if (it->second->discipleMax >= battle_enabel_size)
			return it->first;
	}
	return 999999;
}

//-----------------------------------------------------------------------------------------------
//GradeTableManager
//------------------------------------------------------------------------------------------------
void PlayerGradeTableManager::init(const std::string& filename)
{
	GradeItemList::iterator it = mGradeItemList.begin();
	while (it != mGradeItemList.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}
	mGradeItemList.clear();
	parse(filename, 1);
}

void PlayerGradeTableManager::readline(std::stringstream& _stream)
{
	PlayerGradeItem* data = new PlayerGradeItem;
	data->readLine(_stream);
	mGradeItemList.insert(GradeItemList::value_type(data->ID, data));
}

const PlayerGradeItem* PlayerGradeTableManager::getGradeItemByID(unsigned int id)
{
	if (mGradeItemList.find(id) != mGradeItemList.end())
	{
		return mGradeItemList.find(id)->second;
	}

	return NULL;
}

PlayerGradeTableManager* PlayerGradeTableManager::getInstance()
{
	return PlayerGradeTableManager::Get();
}

//------------------------------------------------------------------------------------------------
//AnnouncementTableManager
//------------------------------------------------------------------------------------------------
void AnnouncementTableManager::init()
{
	clearAnnouncementItem();
	//parse(fileName, 1);
}

void AnnouncementTableManager::clearAnnouncementItem()
{
	AnnouncementItemList::iterator it = mAnnouncementItemList.begin();
	if(it != mAnnouncementItemList.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}

	mAnnouncementItemList.clear();
}
void AnnouncementTableManager::readline(Json::Value _stream)
{
	AnnouncementItem* item = new AnnouncementItem;
	item->readLine(_stream);
	mAnnouncementItemList.insert(AnnouncementItemList::value_type(item->itemId, item));
}

const AnnouncementItem* AnnouncementTableManager::getAnnouncementItemById(unsigned int id)
{
	if(mAnnouncementItemList.find(id) != mAnnouncementItemList.end())
	{
		return mAnnouncementItemList.find(id)->second;
	}

	return NULL;
}

AnnouncementTableManager* AnnouncementTableManager::getInstance()
{
	return AnnouncementTableManager::Get();
}

//------------------------------------------------------------------------------------------------
//UserPropertyTableManager
//------------------------------------------------------------------------------------------------
void UserPropertyTableManager::init(const std::string& filename)
{
	UserPropertyList::iterator it = mUserPropertyList.begin();
	while (it != mUserPropertyList.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}
	mUserPropertyList.clear();
	parse(filename, 1);
}
const UserPropertyItem * UserPropertyTableManager::getUserPropertyItemByID( unsigned int id )
{
	if(mUserPropertyList.find(id) != mUserPropertyList.end())
		return mUserPropertyList.find(id)->second;

	return NULL;
}
void UserPropertyTableManager::readline(std::stringstream& _stream)
{
	UserPropertyItem* item = new UserPropertyItem;
	item->readline(_stream);
	mUserPropertyList.insert(UserPropertyList::value_type(item->itemID, item));
}

UserPropertyTableManager* UserPropertyTableManager::getInstance()
{
	return UserPropertyTableManager::Get();
}


//------------------------------------------------------------------------------------------------
//ResPropertyTableManager
//------------------------------------------------------------------------------------------------
void ResPropertyTableManager::init(const std::string& filename)
{
	ResPropertyList::iterator it = mResPropertyList.begin();
	while (it != mResPropertyList.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}
	mResPropertyList.clear();
	parse(filename, 1);
}
const ResPropertyItem * ResPropertyTableManager::getResPropertyItemByID( unsigned int id )
{
	if(mResPropertyList.find(id) != mResPropertyList.end())
		return mResPropertyList.find(id)->second;

	return NULL;
}
void ResPropertyTableManager::readline(std::stringstream& _stream)
{
	ResPropertyItem* item = new ResPropertyItem;
	item->readline(_stream);
	mResPropertyList.insert(ResPropertyList::value_type(item->typeId, item));
}

ResPropertyTableManager* ResPropertyTableManager::getInstance()
{
	return ResPropertyTableManager::Get();
}

//------------------------------------------------------------------------------------------------
//NewActivePromptTableManager
//------------------------------------------------------------------------------------------------
void NewActivePromptTableManager::init(const std::string& filename)
{
	NewActivePromptList::iterator it = mNewActivePromptList.begin();
	while (it != mNewActivePromptList.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}
	mNewActivePromptList.clear();
	parse(filename, 1);
}
const NewActivePromptItem * NewActivePromptTableManager::getNewActivePromptItemByID( unsigned int id )
{
	if(mNewActivePromptList.find(id) != mNewActivePromptList.end())
		return mNewActivePromptList.find(id)->second;

	return NULL;
}
void NewActivePromptTableManager::readline(std::stringstream& _stream)
{
	NewActivePromptItem* item = new NewActivePromptItem;
	item->readline(_stream);
	mNewActivePromptList.insert(NewActivePromptList::value_type(item->itemID, item));
}

NewActivePromptTableManager::NewActivePromptList * NewActivePromptTableManager::getNewAcrivePromptItemByLevel(unsigned int level,unsigned int type)
{
	NewActivePromptList::iterator iterator = mNewActivePromptList.begin();
	NewActivePromptList * list = new NewActivePromptList;
	while(iterator != mNewActivePromptList.end())
	{
		NewActivePromptItem* newActivePromptItem = dynamic_cast<NewActivePromptItem*>(iterator->second);
		if(newActivePromptItem->level == level && newActivePromptItem->type == type)
		{
			list->insert(NewActivePromptList::value_type(newActivePromptItem->itemID,newActivePromptItem));
		}
		iterator++;
	}
	return list;
}


NewActivePromptTableManager::NewActivePromptList * NewActivePromptTableManager::getNewAcrivePromptItemByDropId(unsigned int dropId,unsigned int type)
{
	NewActivePromptList::iterator iterator = mNewActivePromptList.begin();
	NewActivePromptList * list = new NewActivePromptList;
	while(iterator != mNewActivePromptList.end())
	{
		NewActivePromptItem* newActivePromptItem = dynamic_cast<NewActivePromptItem*>(iterator->second);
		if(newActivePromptItem->type == type)
		{
			list->insert(NewActivePromptList::value_type(newActivePromptItem->itemID,newActivePromptItem));
		}
		iterator++;
	}
	return list;
}

NewActivePromptTableManager* NewActivePromptTableManager::getInstance()
{
	return NewActivePromptTableManager::Get();
}

//------------------------------------------------------------------------------------------------
//ArenaRewardRewardTableManager
//------------------------------------------------------------------------------------------------
void ArenaRewardTableManager::init(const std::string& filename)
{
	ArenaRewardList::iterator it = mArenaRewardList.begin();
	while (it != mArenaRewardList.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}
	mArenaRewardList.clear();
	parse(filename, 1);
}
const ArenaRewardItem * ArenaRewardTableManager::getArenaRewardItemByID( unsigned int id )
{
	if(mArenaRewardList.find(id) != mArenaRewardList.end())
		return mArenaRewardList.find(id)->second;

	return NULL;
}
void ArenaRewardTableManager::readline(std::stringstream& _stream)
{
	ArenaRewardItem* item = new ArenaRewardItem;
	item->readline(_stream);
	mArenaRewardList.insert(ArenaRewardList::value_type(item->itemID, item));
}

ArenaRewardTableManager* ArenaRewardTableManager::getInstance()
{
	return ArenaRewardTableManager::Get();
}

//------------------------------------------------------------------------------------------------
//FightEndRewardTableManager
//------------------------------------------------------------------------------------------------
void FightEndRewardTableManager::init(const std::string& filename)
{
	FightEndRewardList::iterator it = mFightEndRewardList.begin();
	while (it != mFightEndRewardList.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}
	mFightEndRewardList.clear();
	parse(filename, 1);
}
const FightEndRewardItem * FightEndRewardTableManager::getFightEndRewardItemByID( unsigned int id )
{
	if(mFightEndRewardList.find(id) != mFightEndRewardList.end())
		return mFightEndRewardList.find(id)->second;

	return NULL;
}
void FightEndRewardTableManager::readline(std::stringstream& _stream)
{
	FightEndRewardItem* item = new FightEndRewardItem;
	item->readline(_stream);
	mFightEndRewardList.insert(FightEndRewardList::value_type(item->itemID, item));
}

FightEndRewardTableManager* FightEndRewardTableManager::getInstance()
{
	return FightEndRewardTableManager::Get();
}

VaribleManager* VaribleManager::getInstance()
{
	return VaribleManager::Get();
}

void DestinyItem::readline(std::stringstream& _stream)
{
	std::string str;
	_stream
		>>itemID
		>>name
		>>consumeStar
		>>consumeSilver
		>>str
		>>iconPic;

	std::vector<unsigned int> vec;
	StringConverter::parseVector(str,vec);
	for (int i = 0;i < vec.size();i+=2)
	{
		addAttr.insert(std::make_pair(vec[i],vec[i+1]));
	}
}

void DestinyTableManager::init(const std::string& filename)
{
	DestinyList::iterator it = mDestinyList.begin();
	while (it != mDestinyList.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}
	mDestinyList.clear();
	parse(filename, 1);
}

void DestinyTableManager::readline(std::stringstream& _stream)
{
	DestinyItem* item = new DestinyItem;
	item->readline(_stream);
	mDestinyList.insert(DestinyList::value_type(item->itemID, item));
}

const DestinyItem* DestinyTableManager::getDestinyItemByID(unsigned int id)
{
	DestinyList::iterator it = mDestinyList.find(id);
	if (it != mDestinyList.end())
	{
		return it->second;
	}

	return NULL;
}

//------------------------------------------------------------------------------------------------
// class FortuneComeRewardTableManager
//------------------------------------------------------------------------------------------------
void FortuneComeRewardTableManager::init(const std::string& filename)
{
	FortuneComeRewardList::iterator it = mFortuneComeRewardList.begin();
	while (it != mFortuneComeRewardList.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}
	mFortuneComeRewardList.clear();
	parse(filename, 1);
}
const FortuneComeRewardItem * FortuneComeRewardTableManager::getFortuneComeRewardItemByID( unsigned int id )
{
	if(mFortuneComeRewardList.find(id) != mFortuneComeRewardList.end())
		return mFortuneComeRewardList.find(id)->second;

	return NULL;
}
void FortuneComeRewardTableManager::readline(std::stringstream& _stream)
{
	FortuneComeRewardItem* item = new FortuneComeRewardItem;
	item->readline(_stream);
	mFortuneComeRewardList.insert(FortuneComeRewardList::value_type(item->itemID, item));
}

FortuneComeRewardTableManager* FortuneComeRewardTableManager::getInstance()
{
	return FortuneComeRewardTableManager::Get();
}

//------------------------------------------------------------------------------------------------
// class VipDataTableManager
//------------------------------------------------------------------------------------------------
void VipDataTableManager::init(const std::string& filename)
{
	VipDataList::iterator it = mVipDataList.begin();
	while (it != mVipDataList.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}
	mVipDataList.clear();
	parse(filename, 1);
}
const VipDataItem * VipDataTableManager::getVipDataByID( unsigned int id )
{
	if(mVipDataList.find(id) != mVipDataList.end())
		return mVipDataList.find(id)->second;

	return NULL;
}
void VipDataTableManager::readline(std::stringstream& _stream)
{
	VipDataItem* item = new VipDataItem;
	item->readline(_stream);
	mVipDataList.insert(VipDataList::value_type(item->itemID, item));
}

//------------------------------------------------------------------------------------------------
// class ActivityPopTableManager
//------------------------------------------------------------------------------------------------
void ActivityPopTableManager::init(const std::string& filename)
{
	ActivityPopList::iterator it = mActivityPopList.begin();
	while (it != mActivityPopList.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}
	mActivityPopList.clear();
	parse(filename, 1);
}
const ActivityPopItem * ActivityPopTableManager::getActivityPopDataByID( unsigned int id )
{
	if(mActivityPopList.find(id) != mActivityPopList.end())
		return mActivityPopList.find(id)->second;

	return NULL;
}
void ActivityPopTableManager::readline(std::stringstream& _stream)
{
	ActivityPopItem* item = new ActivityPopItem;
	item->readline(_stream);
	mActivityPopList.insert(ActivityPopList::value_type(item->activityID, item));
}


//------------------------------------------------------------------------------------------------
// class AttentionWXTableManager
//------------------------------------------------------------------------------------------------
void PlatformRoleTableManager::init(const std::string& filename)
{
	PlatformRoleList::iterator it = mPlatformRoleList.begin();
	while (it != mPlatformRoleList.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}
	mPlatformRoleList.clear();
	parse(filename, 1);
}

const PlatformRoleItem * PlatformRoleTableManager::getPlatformRoleByID( unsigned int id )
{
	if(mPlatformRoleList.find(id) != mPlatformRoleList.end())
		return mPlatformRoleList.find(id)->second;
	return NULL;
}

const PlatformRoleItem * PlatformRoleTableManager::getPlatformRoleByName(std::string name)
{
	PlatformRoleList::iterator it=mPlatformRoleList.begin();
	for(;it!=mPlatformRoleList.end();++it)
	{
		if(it->second)
		{
			if(it->second->name==name)
			{
				return it->second;
			}
		}
	}
	return NULL;
}

void PlatformRoleTableManager::readline(std::stringstream& _stream)
{
	PlatformRoleItem* item = new PlatformRoleItem;
	item->readline(_stream);
	mPlatformRoleList.insert(PlatformRoleList::value_type(item->id, item));
}

PlatformRoleTableManager* PlatformRoleTableManager::getInstance()
{
	return PlatformRoleTableManager::Get();
}

//军团相关
void LeagueLevelParam::readline(std::stringstream& _stream)
{
	_stream 
		>> level		
		>> type
		>> upgradeNeed
		>> memberCount;
}

void LeagueLevelParamManager::init(const std::string filename)
{
	LeagueLevelParamList::iterator itr = mLeagueLevelParamList.begin();
	while ( itr != mLeagueLevelParamList.end() )
	{
		LeagueLevelParam* item = *itr;
		delete item;
	}
	mLeagueLevelParamList.clear();

	parse(filename, 1);
}

void LeagueLevelParamManager::readline(std::stringstream& _stream)
{
	LeagueLevelParam* leagueLevel = new LeagueLevelParam();
	leagueLevel->readline(_stream);
	mLeagueLevelParamList.push_back(leagueLevel);
}

const LeagueLevelParam* LeagueLevelParamManager::getParamByLevelAndType(unsigned int level,unsigned int type)
{
	LeagueLevelParamList::const_iterator it = mLeagueLevelParamList.begin();
	LeagueLevelParamList::const_iterator end = mLeagueLevelParamList.end();
	for (; it != end; ++it)
	{
		LeagueLevelParam* item = *it;
		if (item->level == level && item->type == type)
		{
			return item;
		}
	}

	return NULL;
}

LeagueLevelParamManager* LeagueLevelParamManager::getInstance()
{
	return LeagueLevelParamManager::Get();
}


void WorshipLevelParam::readline(std::stringstream& _stream)
{
	_stream
		>> level
		>> power
		>> reputation
		>> silver;
}

void WorshipLevelParamManager::init(const std::string filename)
{
	WorshipLevelParamList::iterator itr = mWorshipLevelParamList.begin();
	while ( itr != mWorshipLevelParamList.end() )
	{
		delete itr->second;
		itr->second = NULL;
		++itr;
	}
	mWorshipLevelParamList.clear();

	parse(filename, 1);
}

void WorshipLevelParamManager::readline(std::stringstream& _stream)
{
	WorshipLevelParam* worship = new WorshipLevelParam();
	worship->readline(_stream);
	mWorshipLevelParamList.insert(WorshipLevelParamList::value_type(worship->level, worship));
}

const WorshipLevelParam* WorshipLevelParamManager::getParamByLevel(unsigned int level)
{
	WorshipLevelParamList::const_iterator it = mWorshipLevelParamList.find(level);
	WorshipLevelParamList::const_iterator end = mWorshipLevelParamList.end();
	if (it != end)
	{
		return it->second;
	}

	return NULL;
}

WorshipLevelParamManager* WorshipLevelParamManager::getInstance()
{
	return WorshipLevelParamManager::Get();
}

//---------------------lineup system table----------------------
void CabinTableManager::init(const std::string filename)
{
	CabinItemList::iterator itr = mCabinItemList.begin();
	while ( itr != mCabinItemList.end() )
	{
		delete itr->second;
		itr->second = NULL;
		++itr;
	}
	mCabinItemList.clear();

	parse(filename, 1);
}

const CabinItem* CabinTableManager::getCabinById(unsigned int id)
{
	CabinItemList::iterator itr = mCabinItemList.begin();
	while ( itr != mCabinItemList.end() )
	{
		if ( itr->second && itr->second->id == id ) {
			return itr->second;
		}
		++itr;
	}
	return NULL;
}

void CabinTableManager::readline(std::stringstream& _stream)
{
	CabinItem* cabinItem = new CabinItem();
	cabinItem->readline(_stream);
	mCabinItemList.insert(CabinItemList::value_type(cabinItem->id, cabinItem));
}

void CabinSeatTableManager::init(const std::string filename)
{
	CabinSeatList::iterator itr = mCabinSeatList.begin();
	while ( itr != mCabinSeatList.end() )
	{
		delete itr->second;
		itr->second = NULL;
		++itr;
	}
	mCabinSeatList.clear();
	
	parse(filename, 1);
}

const CabinSeatItem* CabinSeatTableManager::getCabinSeatByCabinIdAndPosition(unsigned int cabinId, unsigned int position)
{
	CabinSeatList::iterator itr = mCabinSeatList.begin();
	while ( itr != mCabinSeatList.end() )
	{
		if ( itr->second && itr->second->cabinId == cabinId && itr->second->position == position )
		{
			return itr->second;
		}
		++itr;
	}
	return NULL;
}

void CabinSeatTableManager::readline(std::stringstream& _stream)
{
	CabinSeatItem* seatItem = new CabinSeatItem;
	seatItem->readline(_stream);
	mCabinSeatList.insert(CabinSeatList::value_type(seatItem->id, seatItem));
}
//---------------------------------------------------------------

void ReleaseCCBFileManager::init(const std::string& filename)
{
	CCBFileVec::iterator it = m_fileVec.begin();
	CCBFileVec::iterator end = m_fileVec.end();
	for ( ; it != end; ++it)
	{
		ReleaseCCBFile* d = *it;
		delete d;
	}
	m_fileVec.clear();

	parse(filename,1);
}

ReleaseCCBFile* ReleaseCCBFileManager::getRelaseCCBFileByName(std::string name)
{
	CCBFileVec::iterator it = m_fileVec.begin();
	CCBFileVec::iterator end = m_fileVec.end();
	for ( ; it != end; ++it)
	{
		ReleaseCCBFile* d = *it;
		if (d->name == name)
		{
			return d;
		}
	}

	return NULL;
}

void ReleaseCCBFileManager::readline( std::stringstream& _stream )
{
	ReleaseCCBFile* data = new ReleaseCCBFile;
	data->readline(_stream);
	m_fileVec.push_back(data);
}

void SoundEffectTableManager::init(const std::string& filename)
{
	SoundEffectList::iterator it = mSoundEffectList.begin();
	SoundEffectList::iterator end = mSoundEffectList.end();
	for ( ;it != end; ++it)
	{
		SoundEffectItem* d = it->second;
		delete d;
	}
	mSoundEffectList.clear();
	
	parse(filename,1);
}

void SoundEffectTableManager::readline(std::stringstream& _stream)
{
	SoundEffectItem* data = new SoundEffectItem;
	data->readline(_stream);
	mSoundEffectList[data->itemId] = data;
}

SoundEffectItem* SoundEffectTableManager::getSoundEffectById(unsigned int id)
{
	SoundEffectList::iterator it = mSoundEffectList.find(id);
	if (it != mSoundEffectList.end())
	{
		return it->second;
	}

	return NULL;
}

void PushPageItem::readline(std::stringstream& _stream)
{
	_stream
		>> itemId
		>> name
		>> isPushAni
		>> isBeforeEnter
		>> isPopAni
		>> isAfterExit
		>> blurType;
}

void PushPageTableManager::init(const std::string& filename)
{
	PushPageList::iterator it = mPushPageList.begin();
	PushPageList::iterator end = mPushPageList.end();
	for ( ;it != end; ++it)
	{
		PushPageItem* d = it->second;
		delete d;
	}
	mPushPageList.clear();

	parse(filename,1);
}

void PushPageTableManager::readline(std::stringstream& _stream)
{
	PushPageItem* data = new PushPageItem;
	data->readline(_stream);
	mPushPageList[data->itemId] = data;
}

PushPageItem* PushPageTableManager::getPushPageByName(const std::string name)
{
	PushPageList::iterator it = mPushPageList.begin();
	PushPageList::iterator end = mPushPageList.end();
	for ( ;it != end; ++it)
	{
		if (it->second->name == name)
		{
			return it->second;
		}		
	}

	return NULL;
}

void IOSDeviceItem::readline(std::stringstream& _stream)
{
	_stream
		>> itemId
		>> type
		>> name
		>> cfgType;
}

void IOSDeviceTableManager::init(const std::string& filename)
{
	IOSDeviceList::iterator it = mIOSDevicesList.begin();
	IOSDeviceList::iterator end = mIOSDevicesList.end();
	for ( ;it != end; ++it)
	{
		IOSDeviceItem* d = it->second;
		delete d;
	}
	mIOSDevicesList.clear();

	parse(filename,1);
}

void IOSDeviceTableManager::readline(std::stringstream& _stream)
{
	IOSDeviceItem* data = new IOSDeviceItem;
	data->readline(_stream);
	mIOSDevicesList[data->itemId] = data;
}

IOSDeviceItem* IOSDeviceTableManager::getIOSDeviceItemByName(const std::string name)
{
	IOSDeviceList::iterator it = mIOSDevicesList.begin();
	IOSDeviceList::iterator end = mIOSDevicesList.end();
	for ( ;it != end; ++it)
	{
		if (it->second->name == name)
		{
			return it->second;
		}		
	}

	return NULL;
}

void AndroidDeviceItem::readline(std::stringstream& _stream)
{
	_stream
		>> itemId
		>> cpus
		>> cpuHZ
		>> memory
		>> cfgType;
}

void AndroidDeviceTableManager::init(const std::string& filename)
{
	AndroidDeviceList::iterator it = mAndroidDevicesList.begin();
	AndroidDeviceList::iterator end = mAndroidDevicesList.end();
	for ( ;it != end; ++it)
	{
		AndroidDeviceItem* d = it->second;
		delete d;
	}
	mAndroidDevicesList.clear();

	parse(filename,1);
}

void AndroidDeviceTableManager::readline(std::stringstream& _stream)
{
	AndroidDeviceItem* data = new AndroidDeviceItem;
	data->readline(_stream);
	mAndroidDevicesList[data->itemId] = data;
}

int AndroidDeviceTableManager::getAndroidCfgType(unsigned int cpus,float cpuHZ,float memory)
{
	if (memory < 0.5)
	{
		return DeviceCfg_Low;
	}

	AndroidDeviceList::iterator it = mAndroidDevicesList.begin();
	AndroidDeviceList::iterator end = mAndroidDevicesList.end();
	for ( ;it != end; ++it)
	{
		AndroidDeviceItem* item = it->second;
		if (cpus == item->cpus && cpuHZ <= item->cpuHZ)
		{
			return item->cfgType;
		}
	}

	return DeviceCfg_Low;
}

//---------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
//ErrMsgTableManager
//-----------------------------------------------------------------------------------------------
void ErrMsgTableManager::init( const std::string& filename )
{
	ErrMsgList::iterator itr = mErrMsgList.begin();
	while(itr != mErrMsgList.end())
	{
		delete itr->second;
		itr->second =NULL;
		++itr;
	}
	mErrMsgList.clear();

	parse(filename, 1);
}

//-----------------------------------------------------------------------------------------------
void ErrMsgTableManager::readline( std::stringstream& _stream )
{
	ErrMsgItem* data = new ErrMsgItem;
	data->readline(_stream);
	mErrMsgList.insert(ErrMsgList::value_type(data->msgId, data));
}

//-----------------------------------------------------------------------------------------------
const ErrMsgItem * ErrMsgTableManager::getErrMsgItemByID( unsigned int id )
{
	if(mErrMsgList.find(id) != mErrMsgList.end())
		return mErrMsgList.find(id)->second;

	return NULL;
}

ErrMsgTableManager* ErrMsgTableManager::getInstance()
{
	return ErrMsgTableManager::Get();
}
