#include "LoginObj.h"

#include "cocos2d.h"
#include "defines.h"
#include "UserObj.h"
#include "QEnvSet.h"

USING_NS_CC;
using namespace std;

LoginObj::LoginObj()
{

}

LoginObj::~LoginObj()
{
}

vector<vmsg::CSPlayerRoleInfo>& LoginObj::getRoleInfoList() {
	return m_stRoleInfoList;
}

vector<SvrInfo>& LoginObj::getSvrInfoList() {
	return m_stSvrInfoList;
}

const SvrInfo& LoginObj::getCurSvrInfo() {
	if(strcmp(m_stCurSvrInfo.name.c_str(), "") == 0) {
		vector<SvrInfo> &stRecentLoginSvrInfos = getRecentLoginSvrInfos();

		if(stRecentLoginSvrInfos.size() > 0) {
			setCurSvrInfo(stRecentLoginSvrInfos.at(stRecentLoginSvrInfos.size()-1));
		} else if (getSvrInfoList().size() > 0 ) {
			setCurSvrInfo(getSvrInfoList().at(0));
		} else {
            CCLOG("[error]LoginObj::getCurSvrInfo, fail to get svr info!");
        }
	}

	if(QEnvSet::taddy()) {
		//m_stCurSvrInfo.zoneid = 1;
	}

	return m_stCurSvrInfo;
}


void LoginObj::setCurSvrInfo(const SvrInfo &stSvrInfo) {
	m_stCurSvrInfo = stSvrInfo;
    P_USER_OBJ->getMutableUserInfo()->zoneid = stSvrInfo.zoneid;
    P_USER_OBJ->getMutableUserInfo()->zoneName = stSvrInfo.name;

    CCLOG("[trace]LoginObj::setCurSvrInfo,zoneid=[%d],zonename=[%s]", stSvrInfo.zoneid, stSvrInfo.name.c_str());
}

const DirSvrInfo& LoginObj::getDirSvrInfo() {
	return m_stDirSvrInfo;
}

void LoginObj::setDirSvrInfo(const DirSvrInfo &stDirSvrInfo) {
	m_stDirSvrInfo = stDirSvrInfo;
}

vector<SvrInfo>& LoginObj::getRecentLoginSvrInfos() {
	m_stRecentLoginSvrInfos.clear();
	
	int len = CCUserDefault::sharedUserDefault()->getIntegerForKey("recent_login_svr_num", 0);

	if(len > 0) {
		for(int i = 1; i <= len; i++) {
			string key = string("recent_login_svr_") + intToString(i);
			string strSvrName = CCUserDefault::sharedUserDefault()->getStringForKey(key.c_str(), "");

			vector<SvrInfo>::const_iterator iter;

			for(iter = m_stSvrInfoList.begin(); iter != m_stSvrInfoList.end(); iter++) {
				if(iter->name == strSvrName) {
					m_stRecentLoginSvrInfos.push_back(*iter);
					break;
				}
			}
		}
	}

	return m_stRecentLoginSvrInfos;
}

void LoginObj::updateRecentLoginSvrInfo() {
	vector<SvrInfo> &stRecentLoginSvrInfos = getRecentLoginSvrInfos();
	vector<SvrInfo>::iterator iter;

	if(stRecentLoginSvrInfos.size() > 0) {
		for(iter = stRecentLoginSvrInfos.begin(); iter != stRecentLoginSvrInfos.end(); iter++) {
			if(iter->name == m_stCurSvrInfo.name) {
				iter = stRecentLoginSvrInfos.erase(iter);
				break;
			}
		}
	}
	
	if(stRecentLoginSvrInfos.size() == MAX_RECENT_LOGIN_SVR_NUM) {
		stRecentLoginSvrInfos.erase(stRecentLoginSvrInfos.begin());
	}

	stRecentLoginSvrInfos.push_back(m_stCurSvrInfo);
	
	int len = stRecentLoginSvrInfos.size();
	CCUserDefault::sharedUserDefault()->setIntegerForKey("recent_login_svr_num", len);

	for(int i = 1; i <= len; i++) {
		string key = string("recent_login_svr_") + to_string(i);
		CCUserDefault::sharedUserDefault()->setStringForKey(key.c_str(), stRecentLoginSvrInfos.at(i-1).name);
	}

	CCUserDefault::sharedUserDefault()->flush();
	CCUserDefault::purgeSharedUserDefault();
}

void LoginObj::setRecentLoginRoleName(std::string nameStr){
	CCLOG("set the recent name is %s", nameStr.c_str());
	CCUserDefault::sharedUserDefault()->setStringForKey("recent_login_role_name", nameStr);
	CCUserDefault::sharedUserDefault()->flush();
}

std::vector<vmsg::CSPlayerRoleInfo> LoginObj::getRecentRoleInfoList(){
	std::string recentName = CCUserDefault::sharedUserDefault()->getStringForKey("recent_login_role_name");
	CCLOG("you can get recentName is %s", recentName.c_str());
	if(recentName == "" || checkInRoleList(recentName) == false){
		return m_stRoleInfoList;
	}


	vector<vmsg::CSPlayerRoleInfo> stRecentRoleList;
	vector<vmsg::CSPlayerRoleInfo>::iterator stIter;
	vmsg::CSPlayerRoleInfo stRecentRoleInfo;
	for(stIter = m_stRoleInfoList.begin(); stIter != m_stRoleInfoList.end(); stIter++){
		if(stIter->nick() == recentName){
			stRecentRoleInfo = *stIter;
			continue;;
		}

		stRecentRoleList.push_back(*stIter);
	}

	stRecentRoleList.insert(stRecentRoleList.begin(), stRecentRoleInfo);

	return stRecentRoleList;
}

bool LoginObj::checkInRoleList(std::string nameStr){
	bool isInList = false;
	vector<vmsg::CSPlayerRoleInfo>::iterator stIter;
	for(stIter = m_stRoleInfoList.begin(); stIter != m_stRoleInfoList.end(); stIter++){
		if(stIter->nick() == nameStr){
			isInList = true;
			break;
		}
	}

	return isInList;
}

const vmsg::CSPlayerInfo& LoginObj::getOtherPlayerInfo() {
	return m_stOtherPlayerInfo;
}

void LoginObj::setOtherPlayerInfo(const vmsg::CSPlayerInfo &stPlayerInfo) {
	m_stOtherPlayerInfo = stPlayerInfo;
}

const vmsg::CSPlayerDetailInfo& LoginObj::getOtherPlayerDetailInfo() {
	return m_stOtherPlayerDetailInfo;
}

void LoginObj::setOtherPlayerDetailInfo(const vmsg::CSPlayerDetailInfo &stPlayerDetailInfo) {
	m_stOtherPlayerDetailInfo = stPlayerDetailInfo;
}


void LoginObj::setStartMovieDone(){
    CCLOG("[trace]LoginObj,setStartMovieDone");
    CCUserDefault::sharedUserDefault()->setIntegerForKey("is_start_movie_done", 1);
    CCUserDefault::sharedUserDefault()->flush();
}


bool LoginObj::getIsStartMovieDone(){
    
    const int iIsDone = CCUserDefault::sharedUserDefault()->getIntegerForKey("is_start_movie_done");
    return (iIsDone > 0);
}



void LoginObj::resetOnConnClose() {
    //断线时重置数据
    CCLOG("[trace]LoginObj,reset data on connection close.");
    m_stRoleInfoList.clear();
    m_stSvrInfoList.clear();

    //m_stCurSvrInfo = stCurSvrInfo; //!!!这个不重置

    m_stRecentLoginSvrInfos.clear();
    m_stOtherPlayerInfo.Clear();
    m_stOtherPlayerDetailInfo.Clear();

}

