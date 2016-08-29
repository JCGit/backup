#include "SetSysControl.h"
#include "UserObj.h"
#include "login/LoginControl.h"

const string SetSysControl::INIT_SET = "0111000000000000000000000000000000000000";  //40

SetSysControl::SetSysControl(){
	m_strSys = INIT_SET;
	m_bIsInited = false;
}

SetSysControl::~SetSysControl(){

}

void SetSysControl::initSet(){
	if(m_bIsInited){
		return;
	}

	m_strSys = P_USER_OBJ->getUserInfo()->strSettingParam;
	m_bIsInited = true;

	updateLocalData();
}

bool SetSysControl::isSet(unsigned int uType){
	string str = m_strSys.substr(uType, 1);
	if(uType >= m_strSys.length()) {
		return false;
	}
	return (str == "1");
}

void SetSysControl::setOpStat(unsigned int uType, bool bSel, bool isSendUpdate /* = true */){
	const char* ch = bSel ? "1" : "0";
	if(uType >= m_strSys.length()) {
		string newstr = add0str(m_strSys, uType + 1);
		if(m_strSys.length() == newstr.length()) {
			CCLOG("SetSysControl::setOpStat,Error! uType = %u, paramLen = %u", uType, m_strSys.length());
			return;
		}
		m_strSys = newstr;
	}
	string str = setCharByIdx(m_strSys, ch, uType);
	if(str == "") {
		return;
	}

	m_strSys = str; //¸³Öµ
	updateLocalData();

	//update svr
	if(isSendUpdate) {
		P_LOGIN_CTRL->sendSetSettingParamRqst(m_strSys);
	}
}

string SetSysControl::setCharByIdx(string str, const char* ch, unsigned int charIdx){
	if(charIdx >= str.length()) {
		return "";
	}
	string lstr = str.substr(0, charIdx);
	string rstr = str.substr(charIdx + 1);

	return lstr + ch + rstr;
}

string SetSysControl::add0str(string str, unsigned int newLen){
	unsigned int num = newLen - str.length();
	if(num <= 0) {
		return str;
	}
	string tmpstr = "";
	for (unsigned int i = 0; i < num; ++i) {
		tmpstr += "0";
	}
	str += tmpstr;
	return str;
}

void SetSysControl::doSetRsp(){
	P_USER_OBJ->getMutableUserInfo()->strSettingParam = m_strSys;

	updateLocalData();
}

void SetSysControl::updateLocalData(){
	bool isBgSoundOn = isSet(BACKGROUND_SOUND_TYPE);
	bool isEffectOn = isSet(SOUND_EFFECT_TYPE);
	CCUserDefault::sharedUserDefault()->setBoolForKey("is_tgame_bg_sound", isBgSoundOn);
	CCUserDefault::sharedUserDefault()->setBoolForKey("is_tgame_sound_effect", isEffectOn);
	CCUserDefault::sharedUserDefault()->flush();
}

CSetPanel* SetSysControl::getSetPanel(){
	if(!m_stSetPanel.isInited()){
		m_stSetPanel.init();
	}

	return &m_stSetPanel;
}

