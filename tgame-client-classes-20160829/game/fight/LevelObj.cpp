#include "LevelObj.h"
#include "UserObj.h"
#include "FightObj.h"
#include "bag/BagControl.h"

CLevelObj::CLevelObj(){
	_enterLevelID = 1;
	m_IsHasFriend = false;
	m_uMaxEliteWarMapID = 0;
}

CLevelObj::~CLevelObj(){

}

void CLevelObj::setMapInfo(const vmsg::CSWarMapQueryRsp& info){
	mapInfo = info;
}

vmsg::CSWarMapQueryRsp& CLevelObj::getMapInfo(){
	return mapInfo;
}

void CLevelObj::updateNewWarMapId(){
	if(P_FIGHT_OBJ->getFightMapID() > mapInfo.newestwarmapid()){  //战斗的关卡在最新通关之后，更新玩家的数据
		P_USER_OBJ->updateNewestWarMap(P_FIGHT_OBJ->getFightMapID());
	}
}

void CLevelObj::setCardInfo(const vmsg::CSWarMapCardRsp& info){
	cardInfo = info;
}

vmsg::CSWarMapCardRsp& CLevelObj::getCardInfo(){
	return cardInfo;
}

void CLevelObj::setEnterLevelID(unsigned int var){
	_enterLevelID = var;
}

unsigned int CLevelObj::getEnterLevelID(){
	return _enterLevelID;
}

void CLevelObj::setFriendDetailInfo(const vmsg::CSPlayerDetailInfo& stMsg){
	friendDetailInfo = stMsg;
	m_IsHasFriend = true;
}

vmsg::CSPlayerDetailInfo& CLevelObj::getFriendDetailInfo(){
	return friendDetailInfo;
}

void CLevelObj::resetFriendDetailInfo(){
	friendDetailInfo.Clear();
}

bool CLevelObj::getIsHasFriend(){
	return m_IsHasFriend;
}

void CLevelObj::setIsHasFriend(bool var){
	m_IsHasFriend = var;
}

void CLevelObj::setSweepInfo(const vmsg::CSSweepWarRsp& stMsg){
	m_stSweepRsp = stMsg;

	//更新背包信息
	P_BAG_CTRL->setBagInfo(stMsg.baginfo());

	//手动更新扫荡关卡的剩余次数信息
	updateMapInfoByLvID(_sweepLevelID, _sweepNum);
}

const vmsg::CSSweepWarRsp& CLevelObj::getSweepInfo(){
	return m_stSweepRsp;
}

void CLevelObj::updateMapInfoByLvID(unsigned int levelID, unsigned int sweepNum){
	int idx = levelID - P_LEVEL_OBJ->getMapInfo().fromid();
	if(idx >= (int)mapInfo.infos_size() || idx < 0){
		return;
	}

	vmsg::CSWarMapInfo* stMapInfo = mapInfo.mutable_infos(idx);
	unsigned int preCnt = stMapInfo->fightcnt();
	stMapInfo->set_fightcnt(preCnt + sweepNum);
}

void CLevelObj::doBuyFightCntRsp(const vmsg::CSBuyInstanceRsp& stRsp){
	int idx = stRsp.warid() - P_LEVEL_OBJ->getMapInfo().fromid();
	if(idx >= (int)mapInfo.infos_size() || idx < 0){
		return;
	}

	vmsg::CSWarMapInfo* stMapInfo = mapInfo.mutable_infos(idx);
	stMapInfo->set_fightcnt(stRsp.usedcnt());
	stMapInfo->set_totalcnt(stRsp.totalcnt());
	stMapInfo->set_boughtcnt(stRsp.boughtcnt());
}

unsigned int CLevelObj::getMaxEliteWarMapID() {
	return m_uMaxEliteWarMapID;
}

void CLevelObj::setMaxEliteWarMapID(unsigned int uMaxEliteWarMapID) {
	m_uMaxEliteWarMapID = uMaxEliteWarMapID;
}

const vmsg::CSEliteWarInfo& CLevelObj::getEliteWarInfo() {
	return m_stEliteWarInfo;
}

void CLevelObj::setEliteWarInfo(const vmsg::CSEliteWarInfo& stEliteWarInfo) {
	m_stEliteWarInfo = stEliteWarInfo;
}

const vmsg::CSSweepEliteWarRsp& CLevelObj::getEliteSweepInfo() {
	return m_stEliteSweepInfo;
}

void CLevelObj::setEliteSweepInfo(const vmsg::CSSweepEliteWarRsp& stEliteSweepInfo) {
	m_stEliteSweepInfo = stEliteSweepInfo;
}
