#include "FireArmsObj.h"
#include "ItemCfgMaster.h"
#include "WeaponExpItemCfgMaster.h"
#include "WeaponLevelCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "WeaponStarStepCfgMaster.h"
#include "ClientSvrMsg.pb.h"
#include "UserObj.h"
#include "ConstHelper.h"
#include "bag/BagControl.h"
CFireArmsObj::CFireArmsObj(){
	m_uPreWeaponGUID = 0;
    m_iLvUpMatLoginSize = 0;
}

CFireArmsObj::~CFireArmsObj(){

}

void CFireArmsObj::setWeaponBag(const vmsg::CSWeaponBag& info){
	weaponBag = info;

	setAllWeaponBag(info);
}

const vmsg::CSWeaponBag& CFireArmsObj::getWeaponBag(){
	return weaponBag;
}

void CFireArmsObj::setMaterialBag(const vmsg::CSBagInfo& info){
	m_stLvUpMaterials.clear();
	for(int i = 0; i < info.celllst_size(); i++){
		const vmsg::CSBagCellInfo& bagCellInfo = info.celllst(i);
		if(bagCellInfo.type() == vmsg::THING_TYPE_ITEM){
			const ItemCfg* itemCfg = ITEM_CFG_MASTER->GetItemCfgByID(bagCellInfo.iteminfo().itemcfgid());
			if(itemCfg->itemtype() == 1){  //枪械原料
				if(WEAPON_EXP_CFG->CheckItemIsExpItem(itemCfg->itemid())){
					m_stLvUpMaterials.push_back(bagCellInfo);
				}
			}
		}
	}

	sort(m_stLvUpMaterials.begin(), m_stLvUpMaterials.end(), compareMaterial);
}

bool CFireArmsObj::compareMaterial(const vmsg::CSBagCellInfo& first, const vmsg::CSBagCellInfo& second){
	unsigned int aCfgID = first.iteminfo().itemcfgid();
	unsigned int bCfgID = second.iteminfo().itemcfgid();
	return aCfgID > bCfgID;
}



const vector<vmsg::CSBagCellInfo>& CFireArmsObj::getLvUpMaterials(){
	return m_stLvUpMaterials;
}



void CFireArmsObj::setAllWeaponBag(const vmsg::CSWeaponBag& info){
    CCLOG("[trace]CFireArmsObj::setAllWeaponBag, update bag info.");
    P_USER_OBJ->updateWeaponInfo(info.playerweapon());
	m_stAllWeaponBag.clear();
    m_stWeaponNumMap.clear();
	m_stChangeWeaponLst.clear();

	for(int i =0; i < info.weapons_size(); i++){
		m_stAllWeaponBag.push_back(info.weapons(i));

        set<UINT64_t>& stOneSet = m_stWeaponNumMap[info.weapons(i).cfgid()];
        stOneSet.insert( info.weapons(i).weaponguid() );

		if(info.weapons(i).state() == vmsg::WEAPON_IDLE){   //处于空闲的枪械
			m_stChangeWeaponLst.push_back(info.weapons(i));
		}
	}

	sort(m_stChangeWeaponLst.begin(), m_stChangeWeaponLst.end(), compareByLevUp);
	sort(m_stAllWeaponBag.begin(), m_stAllWeaponBag.end(), compareByLev);
	std::reverse(m_stAllWeaponBag.begin(), m_stAllWeaponBag.end());
	//自己的枪械在最前面
	m_stAllWeaponBag.insert(m_stAllWeaponBag.begin(), info.playerweapon());
}

const vector<vmsg::CSWeapon>& CFireArmsObj::getAllWeaponBag(){
	return m_stAllWeaponBag;
}

const vector<vmsg::CSWeapon>& CFireArmsObj::getChangeWeaponLst(){
	return m_stChangeWeaponLst;
}

const vmsg::CSWeapon& CFireArmsObj::getOneWeaponByGUID(UNIT64_t uID){
	unsigned int idx = 0;
	for(unsigned int i = 0; i < m_stAllWeaponBag.size(); i++){
		if(m_stAllWeaponBag.at(i).weaponguid() == uID){
			idx = i;
			break;
		}
	}

	return m_stAllWeaponBag.at(idx);
}
const vmsg::CSWeapon* CFireArmsObj::getWeaponByGUID(UNIT64_t uID) {
    unsigned int idx = 0;
    for(unsigned int i = 0; i < m_stAllWeaponBag.size(); i++){
        if(m_stAllWeaponBag.at(i).weaponguid() == uID){
            
            return &(m_stAllWeaponBag[i]);
        }
    }
    return NULL;
}

int CFireArmsObj::getWeaponNumByCfgID(const int iWeaponCfgID, UNIT64_t iIgnoreWeaponGuid) {
    if(m_stWeaponNumMap.count(iWeaponCfgID) == 0) {
        return 0;;
    } 
    set<UINT64_t>& stOneSet = m_stWeaponNumMap[iWeaponCfgID];
    int iIgnoreNum = stOneSet.count(iIgnoreWeaponGuid);

    return (stOneSet.size() - iIgnoreNum);
}

void CFireArmsObj::setPreWeaponGUID(UNIT64_t guid){
	m_uPreWeaponGUID = guid;
}

UNIT64_t CFireArmsObj::getPreWeaponGUID(){
	return m_uPreWeaponGUID;
}

void CFireArmsObj::updateWeaponBag(const vmsg::CSWeapon& info){
	//升级身上的枪械
	if(weaponBag.playerweapon().weaponguid() == info.weaponguid()){
		vmsg::CSWeapon* pstWeapon = weaponBag.mutable_playerweapon();
		*pstWeapon = info;
		setAllWeaponBag(weaponBag);
		return;
	}

	for(int i = 0; i < weaponBag.weapons_size(); i++){
		const vmsg::CSWeapon& pstWeapon = weaponBag.weapons(i);
		if(pstWeapon.weaponguid() == info.weaponguid()){
			vmsg::CSWeapon* pstWeapon = weaponBag.mutable_weapons(i);
			*pstWeapon = info;
			break;
		}
	}

	setAllWeaponBag(weaponBag);
}

void CFireArmsObj::resetUpWeapon(){
	m_stUpWeapon.set_weaponguid(0);
	m_stUpWeapon.set_cfgid(0);
	m_stUpWeapon.set_level(0);
	m_stUpWeapon.set_exp(0);
}

void CFireArmsObj::setUpWeapon(const vmsg::CSWeapon& info){
	m_stUpWeapon = info;
}

const vmsg::CSWeapon& CFireArmsObj::getUpWeapon(){
	return m_stUpWeapon;
}

void CFireArmsObj::resetUpMaterials(){
	m_stUpMaterials.clear();
    m_iLvUpMatLoginSize = 0;
}

void CFireArmsObj::addUpMaterial(UNIT64_t uID, unsigned int type, unsigned int idx){
	vmsg::CSWeaponLvUpConsume stLvUp;
	stLvUp.set_type(type);
	stLvUp.set_id(uID);
	stLvUp.set_cnt(1);
	m_stUpMaterials.insert(make_pair(idx, stLvUp));

    if(vmsg::THING_TYPE_WEAPON == type) {
        m_iLvUpMatLoginSize += LV_UP_MAT_SIZE_WEAPON;
    } else {
        m_iLvUpMatLoginSize += LV_UP_MAT_SIZE_ITEM;
    }
}

void CFireArmsObj::removeUpMaterial(UNIT64_t uID, unsigned int idx){
	map<UINT64_t, vmsg::CSWeaponLvUpConsume>::iterator iter;

	for(iter = m_stUpMaterials.begin(); iter != m_stUpMaterials.end(); iter++) {
		if(iter->second.id() == uID && iter->first == idx) {

            if(vmsg::THING_TYPE_WEAPON == iter->second.type() ) {
                m_iLvUpMatLoginSize -= LV_UP_MAT_SIZE_WEAPON;
            } else {
                m_iLvUpMatLoginSize -= LV_UP_MAT_SIZE_ITEM;
            }
            m_stUpMaterials.erase(iter);

			break;
		}
	}
}

bool CFireArmsObj::isInUpMaterial(unsigned int uIdx){
	
    return m_stUpMaterials.count(uIdx) > 0;
    /*
    bool bRet = false;
	map<UINT64_t, vmsg::CSWeaponLvUpConsume>::const_iterator iter;
    
	for(iter = m_stUpMaterials.begin(); iter != m_stUpMaterials.end(); iter++) {
		if(iter->first == uIdx) {
			bRet = true;
			break;
		}
	}

	return bRet;
    */
}

int CFireArmsObj::getFullLvNeedExp(const int iWeaponCfgID, const int iWeaponLv) {
    const WeaponCfg* weaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(iWeaponCfgID);
    if(NULL == weaponCfg) {
        return 0;
    }
    const int maxLv = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(weaponCfg->initstar(), weaponCfg->initstep())->needlevel();
    const int iDeltaLv = maxLv - iWeaponLv;
    if(iDeltaLv <= 0) {
        return 0;
    }
    int uTotalExp = 0;
    unsigned int uAddExp = WEAPON_CFG_MASTER->GetWeaponCfgByID(iWeaponCfgID)->expmulti();
    for(int i = 0; i < iDeltaLv; i++){
        const WeaponLevelCfg* pCfg = WEAPON_LEVEL_CFG->GetWeaponLevelCfgByID(iWeaponLv + i);
        if(pCfg) {
            uTotalExp += pCfg->exp();
        }

    }
    uTotalExp = (unsigned int)(1.0*uTotalExp*uAddExp/10000);

    return uTotalExp;
}

const map<UINT64_t, vmsg::CSWeaponLvUpConsume>& CFireArmsObj::getUpMaterials(){
	return m_stUpMaterials;
}

int CFireArmsObj::getUpMatLogicSize() {
    /*int iSize = 0;
    
    for(auto stIter = m_stUpMaterials.begin(); stIter != m_stUpMaterials.end(); stIter++){
        const vmsg::CSWeaponLvUpConsume& oneItem = stIter->second;
        if(vmsg::THING_TYPE_WEAPON == oneItem.type() ) {
            iSize += 2;
        } else {
            iSize += 1;
        }
    }*/
    return m_iLvUpMatLoginSize;
}

bool CFireArmsObj::canAddUpMat(const int iType) {
    int iNeedSize = 0;
    if(vmsg::THING_TYPE_WEAPON == iType) {
        iNeedSize = LV_UP_MAT_SIZE_WEAPON;
    } else {
        iNeedSize = LV_UP_MAT_SIZE_ITEM;
    }
    int iEmptySize = LV_UP_MAT_MAX_SIZE - m_iLvUpMatLoginSize;
    if(iEmptySize >= iNeedSize) {
        return true;
    } else {
        return false;
    }
}

const vector<vmsg::CSWeaponLvUpConsume> CFireArmsObj::getSortUpMaterials(){
	vector<vmsg::CSWeaponLvUpConsume> stAllMaterials;
	map<UINT64_t, vmsg::CSWeaponLvUpConsume>::const_iterator stIter;
	stAllMaterials.clear();
	for(stIter = m_stUpMaterials.begin(); stIter != m_stUpMaterials.end(); stIter++){
		stAllMaterials.push_back(stIter->second);
	}
	sort(stAllMaterials.begin(), stAllMaterials.end(), compareUpMaterial);

	return stAllMaterials;
}

bool CFireArmsObj::compareUpMaterial(vmsg::CSWeaponLvUpConsume first, vmsg::CSWeaponLvUpConsume second){
	return first.type() > second.type(); //枪械排列在前面
}

unsigned int CFireArmsObj::getExpByTwoLevels(unsigned int fromLev, unsigned int toLev, unsigned int uID){
	if(fromLev >= toLev){
		return 0;
	}

	unsigned int uTotalExp = 0;
	unsigned int uAddExp = WEAPON_CFG_MASTER->GetWeaponCfgByID(uID)->expmulti();
	for(unsigned int i = 0; i < (toLev - fromLev); i++){
		WeaponLevelCfg* pCfg = WEAPON_LEVEL_CFG->GetWeaponLevelCfgByID(fromLev + i);
		uTotalExp += pCfg->exp();
	}

	uTotalExp = (unsigned int)(1.0*uTotalExp*uAddExp/10000);
	return uTotalExp;
}

unsigned int CFireArmsObj::getLevelsByExp(const vmsg::CSWeapon& stWeapon, unsigned int uiExpAdd){
    //neil,从后台拿的代码，修改一下
    int uLevels = 0;

    uint64 u64ExpHas = uiExpAdd + stWeapon.exp();
    uint64 u64ExpAll = 0;
    unsigned int uiMaxLevel = WEAPON_LEVEL_CFG->GetMaxLevel();
    const WeaponCfg* pstWeaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid());
    if (pstWeaponCfg == NULL)
    {
        return 0;
    }
    const WeaponStarStepCfg* pstStarStepCfg = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(pstWeaponCfg->initstar(), pstWeaponCfg->initstep());
    if (pstStarStepCfg == NULL)
    {
        return 0;
    }

    //pstWeapon->m_ui64TotalExp -= iMyExp;
    int iMyLv = stWeapon.level();
    int iMyExp = stWeapon.exp();

    for (unsigned int uiLevel = iMyLv; uiLevel <= uiMaxLevel; uiLevel++)
    {

        const WeaponLevelCfg* pstLvCfg = WEAPON_LEVEL_CFG->GetWeaponLevelCfgByID(uiLevel);
        if (pstLvCfg == NULL)
        {
            CV_WARNING(("%s, weapon level cfg cant find", __PRETTY_FUNCTION__));
            return 0;
        }

        if (uiLevel < uiMaxLevel)
        {
            const WeaponLevelCfg* pstNextLvCfg = WEAPON_LEVEL_CFG->GetWeaponLevelCfgByID(uiLevel+1);
            if (pstNextLvCfg == NULL)
            {
                CV_WARNING(("%s, weapon level cfg cant find", __PRETTY_FUNCTION__));
                return 0;
            }
            if (pstStarStepCfg->needlevel() < uiLevel + 1)
            {
                if (iMyExp > 0)
                {
                    iMyLv = uiLevel;
                    iMyExp = 0;
                    uLevels = iMyLv - stWeapon.level();
                }
                return uLevels;
            }
        }

        uint64 uiExpLv = static_cast<uint64>(pstLvCfg->exp() * 1.0 * pstWeaponCfg->expmulti() / EGAME_DENOM);

        u64ExpAll += uiExpLv;

        if (u64ExpAll > u64ExpHas)//只能升到当前等级啦
        {
            iMyLv = uiLevel;
            iMyExp = u64ExpHas + uiExpLv - u64ExpAll;
            //pstWeapon->m_ui64TotalExp += iMyExp;
            break;
        }

        //满级满经验
        if (uiLevel == uiMaxLevel
            && u64ExpAll <= u64ExpHas)
        {
            iMyLv = uiMaxLevel;
            iMyExp = uiExpLv;
            //pstWeapon->m_ui64TotalExp += uiExpLv;
            break;
        }

        iMyLv = uiLevel;
        iMyExp = uiExpLv;
        //pstWeapon->m_ui64TotalExp += iMyExp;
    }

    uLevels = iMyLv - stWeapon.level();
	return uLevels;
}

unsigned int CFireArmsObj::getUpLeftExp(const vmsg::CSWeapon& stWeapon, unsigned int addExp){
	const WeaponCfg* pCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid());
	const WeaponStarStepCfg* pStarCfg = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(pCfg->initstar(), pCfg->initstep());
	unsigned int expMulti = pCfg->expmulti();
	if(addExp < WEAPON_LEVEL_CFG->GetWeaponLevelCfgByID(stWeapon.level())->exp()*(1.0*expMulti/10000) - stWeapon.exp()){
		return addExp;
	}

	unsigned int uLeftExp = addExp - (WEAPON_LEVEL_CFG->GetWeaponLevelCfgByID(stWeapon.level())->exp()*(1.0*expMulti/10000) - stWeapon.exp());

	unsigned int uLevels = 1;
	unsigned int uTempLev = stWeapon.level() + 1;
	while (uLeftExp >= (WEAPON_LEVEL_CFG->GetWeaponLevelCfgByID(uTempLev)->exp()*(1.0*expMulti/10000)))
	{
		uLevels++;
		uTempLev++;
		if(uLevels + stWeapon.level() >= pStarCfg->needlevel()){
			break;
		}
		unsigned int temp = WEAPON_LEVEL_CFG->GetWeaponLevelCfgByID(uTempLev)->exp()*(1.0*expMulti/10000);
		uLeftExp = uLeftExp - WEAPON_LEVEL_CFG->GetWeaponLevelCfgByID(uTempLev)->exp()*(1.0*expMulti/10000);
	}

	return uLeftExp;
}


bool CFireArmsObj::compareByLev(const vmsg::CSWeapon& first, const vmsg::CSWeapon& second){
	if(first.level() < second.level()){
		return true;
	}else if(first.level() == second.level()){
		if(first.exp() < second.exp()){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

bool CFireArmsObj::compareByLevUp(const vmsg::CSWeapon& first, const vmsg::CSWeapon& second){
	if(first.level() > second.level()){
		return true;
	}else if(first.level() == second.level()){
		const WeaponCfg* cfgA = WEAPON_CFG_MASTER->GetWeaponCfgByID(first.cfgid());
		const WeaponCfg* cfgB = WEAPON_CFG_MASTER->GetWeaponCfgByID(second.cfgid());

		if(cfgA->initstar() > cfgB->initstar()){
			return true;
		}else if(cfgA->initstar() == cfgB->initstar()){
			return (cfgA->initstep() > cfgB->initstep());
		}else{
			return false;
		}
	}else{
		return false;
	}
}

bool CFireArmsObj::checkCanStarWeapon(unsigned int uCfgID, UINT64_t uWeaponGUID/* = 0*/){
	//bool isEnough = true;
	const WeaponCfg* pWeaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(uCfgID);
    if(!pWeaponCfg) {
        return false;
    }
	const WeaponStarStepCfg* pStarCfg = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(pWeaponCfg->initstar(), pWeaponCfg->initstep());
    if(!pStarCfg) {
        return false;
    }

    if(pStarCfg->needweaponcnt() > 0) {
		int iBagWeaponCnt = uWeaponGUID > 0 ? getWeaponIdleNumByCfgID(uCfgID, uWeaponGUID) : getWeaponNumByCfgID(uCfgID);
        if(iBagWeaponCnt < (int)pStarCfg->needweaponcnt()) {
            return false;
        }
    }

    int iBagCnt = 0;
    int iNeedCnt = 0;
    for(int i = 0; i < pStarCfg->needs_size(); i++){
        iNeedCnt = pStarCfg->needs(i).cfgcnt();
        iBagCnt = P_BAG_CTRL->getItemCnt(pStarCfg->needs(i).cfgid());
        if(iBagCnt < iNeedCnt) {
            return false;
        }
    }

	return true;
}

int CFireArmsObj::getWeaponIdx(UNIT64_t weaponGUID){
	int bagIdx = 0;
	if(weaponGUID == weaponBag.playerweapon().weaponguid()){
		bagIdx = -1;
	}else{
		google::protobuf::RepeatedPtrField<vmsg::CSWeapon> weaponLst = weaponBag.weapons();
		google::protobuf::RepeatedPtrField<vmsg::CSWeapon>::iterator stIter;
		unsigned int idx = 0;
		for(stIter = weaponLst.begin(); stIter != weaponLst.end(); stIter++) {
			if(stIter->weaponguid() == weaponGUID){
				bagIdx = idx;
				break;
			}
			idx++;
		}
	}

	return bagIdx;
}



void CFireArmsObj::getWeaponsInCfg(vector<int>& stAllWeaponInCfg, 
                                   const unsigned int type, 
                                   const vector<vmsg::CSWeapon>& stMyWeapons){
	unsigned int cfgId;
    //过滤出 stAllWeaponInCfg 里我没有的装备

	vector<int>::iterator stIter;
	for(unsigned int i = 0; i < stMyWeapons.size(); i++){
		cfgId = stMyWeapons.at(i).cfgid();
		stIter = find(stAllWeaponInCfg.begin(), stAllWeaponInCfg.end(), cfgId);

		if(stIter != stAllWeaponInCfg.end()){
			stAllWeaponInCfg.erase(stIter);
		}
	}

	sort(stAllWeaponInCfg.begin(), stAllWeaponInCfg.end(), compareWeaponByLv);
}

void CFireArmsObj::getWeaponsInBagByType(const unsigned int type, vector<vmsg::CSWeapon>& stWeapons){

	unsigned int cfgID;
    const UINT64_t uPlayerWeaponGuid = getWeaponBag().playerweapon().weaponguid();

	for(unsigned int i = 0; i < m_stAllWeaponBag.size(); i++){
        if(uPlayerWeaponGuid == m_stAllWeaponBag[i].weaponguid()) {
            continue; //已经装备的永远在前面
        }
		cfgID = m_stAllWeaponBag.at(i).cfgid();
		if(WEAPON_CFG_MASTER->GetWeaponCfgByID(cfgID)->weapontype() == type){
			stWeapons.push_back(m_stAllWeaponBag.at(i));
		}
	}

    //根据枪械评级排序
	sort(stWeapons.begin(), stWeapons.end(), compareWeaponInfoByLv);

    //已经装备的永远在前面
    int iMyWeaponCfgID = getWeaponBag().playerweapon().cfgid();
    const WeaponCfg* pCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(iMyWeaponCfgID);
    if(pCfg && pCfg->weapontype() == type) {
        stWeapons.insert(stWeapons.begin(), getWeaponBag().playerweapon() );
    }
    

}

bool CFireArmsObj::compareWeaponByLv(const int cfgIDA, const int cfgIDB){
	const WeaponCfg* cfgA = WEAPON_CFG_MASTER->GetWeaponCfgByID(cfgIDA);
	const WeaponCfg* cfgB = WEAPON_CFG_MASTER->GetWeaponCfgByID(cfgIDB);

	return (cfgA->weaponlv() > cfgB->weaponlv());
}

bool CFireArmsObj::compareWeaponInfoByLv(const vmsg::CSWeapon& weaponA, const vmsg::CSWeapon& weaponB){
	const WeaponCfg* cfgA = WEAPON_CFG_MASTER->GetWeaponCfgByID(weaponA.cfgid());
	const WeaponCfg* cfgB = WEAPON_CFG_MASTER->GetWeaponCfgByID(weaponB.cfgid());

	return (cfgA->weaponlv() > cfgB->weaponlv());
}


bool CFireArmsObj::isWeaponBagFull() {
    int iMaxNum = ConstHelper(WEAPON_BAG_MAX_NUM);
    if(iMaxNum > 0 && weaponBag.weapons_size() >= iMaxNum) {
        return true;
    }

    return false;
}

void CFireArmsObj::setWeaponInBattleInfo(const vmsg::CSWeaponInBattleRsp& stInfo){
	m_stWeaponInBattle = stInfo;
}

void CFireArmsObj::setOldWeapon(UNIT64_t uGUID, int iArrayIdx){
	m_stOldWeapon.Clear();

	m_stOldWeapon.set_weaponguid(uGUID);
	m_stOldWeapon.set_arrayidx(iArrayIdx);

	int uBagIdx = getWeaponIdx(uGUID);
	m_stOldWeapon.set_bagidx(uBagIdx);
}

int CFireArmsObj::getWeaponIdleNumByCfgID(unsigned int uCfgID, UNIT64_t iIgnoreWeaponGuid){
	if(m_stWeaponNumMap.count(uCfgID) == 0) {
		return 0;
	} 
	set<UINT64_t>& stOneSet = m_stWeaponNumMap[uCfgID];

	int iIgnoreNum = 0;
	set<UINT64_t>::iterator stIter;
	for(stIter = stOneSet.begin(); stIter != stOneSet.end(); stIter++){
		UINT64_t uGUID = *stIter;
		if(getOneWeaponByGUID(uGUID).state() != vmsg::WEAPON_IDLE || uGUID == iIgnoreWeaponGuid){   //武器不再空闲中
			iIgnoreNum++;
		}
	}

	return (stOneSet.size() - iIgnoreNum);
}

