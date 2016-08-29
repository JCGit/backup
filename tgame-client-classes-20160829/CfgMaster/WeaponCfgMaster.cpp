#include "WeaponCfgMaster.h"
#include "WeaponRefitAdditionCfgMaster.h"
#include "Macros.pb.h"
#include "ClientSvrMsg.pb.h"
#include "TextHelper.h"
#include "NounHelper.h"
#include "BasicTypes.h"
#include "UserObj.h"
#include <set>
#include "vlib/CVRand.h"

using std::set;

int CWeaponCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.weaponcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.weaponcfg(iIdx).cfgid();
		if(stItemIDSet.insert(uiCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//id重复
		}
		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int CWeaponCfgMaster::SetCfg()
{
	int iRet = 0;

	map<unsigned int , int>::iterator stLvIter;
	map<int, vector<int> >::iterator stIter;

	unsigned int uWeaponID = 0;
	unsigned int uStar = 0;
	unsigned int uStep = 0;
	for(int iIdx = 0; iIdx < m_stCfg.weaponcfg_size(); iIdx++)
	{
		uWeaponID = m_stCfg.weaponcfg(iIdx).weaponid();
		uStar = m_stCfg.weaponcfg(iIdx).initstar();
		uStep = m_stCfg.weaponcfg(iIdx).initstep();
		unsigned int uKey = MakeKey(uWeaponID, uStar, uStep);

		unsigned int uiCfgID = m_stCfg.weaponcfg(iIdx).cfgid();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//id重复
		}

		if(m_stKEYTOIDX.insert(make_pair(uKey, iIdx)).second == false){
			CV_ERROR(("%s, uKey = %u is dup cfg id ! idx %u, %u, %u",
				__PRETTY_FUNCTION__, uKey, uWeaponID, uStar, uStep));
			return -1;//id重复
		}

		unsigned int type = m_stCfg.weaponcfg(iIdx).weapontype();
		stIter = m_stTypeToIDX.find(type);

		if(stIter == m_stTypeToIDX.end()){
			m_stTypeToIDX[type].clear();
			m_stTypeToIDX[type].push_back(iIdx);
		}else{
			m_stTypeToIDX[type].push_back(iIdx);
		}
	}

	return iRet;
}

const WeaponCfg* CWeaponCfgMaster::GetWeaponCfgByID(const unsigned int uiCfgID)
{
    if(uiCfgID <= 0) {
        return NULL;
    }
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
		return NULL; //找不到该物品配置
	}

	return &(m_stCfg.weaponcfg(it->second));
}

WeaponCfg* CWeaponCfgMaster::GetMutableWeaponCfgByID(const unsigned int uiCfgID)
{
    map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

    if(it == m_stID2IDX.end())
    {
        CV_ERROR(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
        return NULL; //找不到该物品配置
    }

    return (m_stCfg.mutable_weaponcfg(it->second));
}

unsigned int CWeaponCfgMaster::MakeKey(const unsigned int uWeaponID,
											const unsigned int uStar,
											const unsigned int uStep)const
{
	return (uWeaponID & 0xFF) << 16 
		| (uStar & 0xFF) << 8
		| (uStep & 0xFF);
}

int CWeaponCfgMaster::GetWeaponCfgByStarAndStep(const unsigned int uWeaponID,
												const unsigned int uStar, 
												const unsigned int uStep, 
												WeaponCfg& stCfg)
{
	int iRet = ERET_OK;

	unsigned int uKey = MakeKey(uWeaponID, uStar, uStep);
	 map<unsigned int, int>::iterator it = m_stKEYTOIDX.find(uKey);

	 if (it == m_stKEYTOIDX.end())
	 {
		 CV_WARNING(("%s, cant find weapon cfg,weaponID %d,star %d, step %d", __PRETTY_FUNCTION__, uWeaponID, uStar, uStep));
		 return -1;
	 } 

	 stCfg = m_stCfg.weaponcfg(it->second);

	 return iRet;
}

int CWeaponCfgMaster::GetWeaponAtkAdd(const unsigned int uiCfgID, unsigned int color, unsigned int level){
	const WeaponCfg* stCfg = GetWeaponCfgByID(uiCfgID);
    if(!stCfg) {
        CCLOG("[warn]CWeaponCfgMaster::GetWeaponAtkAttrAdd,fail to get cfg, cfg id %d,lv %d, color %d", uiCfgID, level, color);
        return 0;
    }
	int iLevelAddAtkVal = stCfg->atkadd() * (level - 1); //强化攻击力
    const WeaponRefitAdditionCfg* pRefitCfg = WEAPONREFITADDITION_CFG_MASTER->GetCfg(color);
    if(NULL == pRefitCfg) {
        CCLOG("[warn]CWeaponCfgMaster::GetWeaponAtkAttrAdd,fail to get WeaponRefitAdditionCfg, cfg id %d,lv %d, color %d", uiCfgID, level, color);
        return 0;
    }
	int iAtkVal = 1.0 * (stCfg->atk() + iLevelAddAtkVal) * pRefitCfg->multi() / EGAME_DENOM;
	iAtkVal = (iAtkVal*stCfg->shotspeed())/1270.0f;  //按秒伤计算
	iAtkVal = iAtkVal * (1 + 1.0*P_USER_OBJ->getUserInfo()->atkAddEff/EGAME_DENOM);		//百分比加成

	return iAtkVal;
}

int CWeaponCfgMaster::GetWeaponAtkAttrAdd(const unsigned int uiCfgID, unsigned int level, unsigned int color){
	const WeaponCfg* stCfg = GetWeaponCfgByID(uiCfgID);
    if(!stCfg) {
        CCLOG("[warn]CWeaponCfgMaster::GetWeaponAtkAttrAdd,fail to get cfg, cfg id %d,lv %d, color %d", uiCfgID, level, color);
        return 0;
    }
	int iLevelAddAtkAttrVal = stCfg->atkattrvaladd() * (level - 1); //强化攻击力
    const WeaponRefitAdditionCfg* pRefitCfg = WEAPONREFITADDITION_CFG_MASTER->GetCfg(color);
    if(NULL == pRefitCfg) {
        CCLOG("[warn]CWeaponCfgMaster::GetWeaponAtkAttrAdd,fail to get WeaponRefitAdditionCfg, cfg id %d,lv %d, color %d", uiCfgID, level, color);
        return 0;
    }
	int atkAttr = 1.0 * (stCfg->atkattrval() + iLevelAddAtkAttrVal) * pRefitCfg->multi() / EGAME_DENOM;

	return atkAttr;
}

const string CWeaponCfgMaster::getAtkAttrTypeDesc(const unsigned int atkAttrType){
	string str = "";

	switch (atkAttrType)
	{
	case NULL_MAGIC:
		str = TextStr(WEAPON_ATTR_TYPE_NONE);
		break;
	case ICE_MAGIC:
		str = TextStr(WEAPON_ATTR_TYPE_ICE);
		break;
	case FLAME_MAGIC:
		str = TextStr(WEAPON_ATTR_TYPE_FIRE);
		break;
	case POISON_MAGIC:
		str = TextStr(WEAPON_ATTR_TYPE_DRUG);
		break;
	case BIOTECH_MAGIC:
		str = TextStr(WEAPON_ATTR_TYPE_BIO);
		break;
	case LIGHTNING_MAGIC:
		str = TextStr(WEAPON_ATTR_TYPE_THUNDER);
        break;
	default:
		break;
	}

	return str;
}

vector<int> CWeaponCfgMaster::getWeaponsByType(const unsigned int type){
	map<int, vector<int> >::iterator stIter = m_stTypeToIDX.find(type);

    vector<int> stVec;
	if(stIter == m_stTypeToIDX.end()){
		CV_ERROR(("%s, can not find type = %u weapon", __PRETTY_FUNCTION__, type));
		return stVec;
	}
    
    stVec.clear();
	for(unsigned int i = 0; i < stIter->second.size(); i++){
		stVec.push_back(m_stCfg.weaponcfg(stIter->second.at(i)).cfgid());
	}


    return stVec;
}



string CWeaponCfgMaster::getName(const unsigned int uiCfgID) {
    const WeaponCfg* cfg = GetWeaponCfgByID(uiCfgID);
    if(cfg) {
        return cfg->weaponname();
    }
    return "";
}

string CWeaponCfgMaster::getIcon(const unsigned int uiCfgID) {
    const WeaponCfg* cfg = GetWeaponCfgByID(uiCfgID);
    if(cfg) {
        return cfg->weaponicon();
    }
    return "";
}


string CWeaponCfgMaster::getWeaponDesc(const unsigned int uiCfgID) {
    const WeaponCfg* cfg = GetWeaponCfgByID(uiCfgID);
    if(cfg) {
        return cfg->weapondesc();
    }
    return "";
}


const string CWeaponCfgMaster::getWeaponColorDesc(const unsigned int color){
	string str = "";

	switch (color)
	{
	case vmsg::WEAPON_COLOR_WHITE:
		str = NounStr(NOUN_COLOR_WHITE);
		break;
	case vmsg::WEAPON_COLOR_GREEN:
		str = NounStr(NOUN_COLOR_GREEN);
		break;
	case vmsg::WEAPON_COLOR_BLUE:
		str = NounStr(NOUN_COLOR_BLUE);
		break;
	case vmsg::WEAPON_COLOR_PURPLE:
		str = NounStr(NOUN_COLOR_PURPLE);
		break;
	case vmsg::WEAPON_COLOR_GOLD:
		str = NounStr(NOUN_COLOR_GOLDEN);
		break;
	default:
		break;
	}

	str += NounStr(NOUN_WEAPON_TXT);

	return str;
}


int CWeaponCfgMaster::getWeaponLv(const unsigned int uiCfgID) {
    const WeaponCfg* cfg = GetWeaponCfgByID(uiCfgID);
    if(cfg) {
        return cfg->weaponlv();
    }
    return 0;
}

unsigned int CWeaponCfgMaster::getWeaponRandByStarAndStep(unsigned int uStar, unsigned int uStep, unsigned int uWeaponID){
	vector<unsigned int> stVec;
	for(int iIdx = 0; iIdx < m_stCfg.weaponcfg_size(); iIdx++){
		unsigned int uiCfgID = m_stCfg.weaponcfg(iIdx).cfgid();
		if(m_stCfg.weaponcfg(iIdx).initstar() == uStar && m_stCfg.weaponcfg(iIdx).initstep() == uStep
			&& m_stCfg.weaponcfg(iIdx).weaponid() >= uWeaponID){
			stVec.push_back(uiCfgID);
		}
	}

	unsigned int uSize = stVec.size();
	unsigned int uIdx = CRandom::GetRand(uSize);
	return stVec.at(uIdx);
}

