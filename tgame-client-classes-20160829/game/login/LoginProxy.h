#ifndef LoginProxy_h__
#define LoginProxy_h__

#include "cocos2d.h"

#include "BasicTypes.h"
#include "QBaseProxy.h"
#include "PlayerLoginMsg.pb.h"
#include <string>

USING_NS_CC;

class LoginProxy: public QBaseProxy
{
public:
	LoginProxy();
	virtual ~LoginProxy();

	// override start

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	// override end

	virtual void fault(const vmsg::CSCommonErrMsg &err);

	
    void sendLoginRqst(const std::string & loginInfo); // 玩家登录
    void sendAppleGuestLoginRqst(const std::string & loginInfo); // apple 游客登录

	void sendQueryRoleListRqst(); // 查询玩家的角色列表

	void sendRegisterRqst(unsigned int uPlayerID, const char *pszNickName); // 注册一个角色

	void sendRoleLoginRqst(UIN_t uin); // 角色登入

	void sendRoleLogoutRqst(UIN_t uin); // 角色登出

	void sendQueryPlayerInfoRqst(UIN_t uin); // 查询玩家信息

	void sendHeartBeatRqst(); // 心跳

	void sendGetSvrListHttpRqst(const std::string &strURL); // 获取服务器列表

	void sendQueryGoldInfoRqst(); // 查询代币信息

	void sendQueryPlayerDetailInfoRqst(); // 查询玩家详细信息

	void sendQueryOtherPlayerInfoRqst(UIN_t uin); // 查询其他玩家信息

    void sendQuickLoginRqst(const bool bNeedOsdInfo = false);

    void sendDailySignInRqst(); //每日签到

	void sendSetSettingParamRqst(const std::string &strSettingParam);		//玩家设置改变

    void sendUmengDeviceTokenRqst(); //umeng上报device token
private:
    int OnLogin(const vmsg::CSPlayerLoginRsp & stRsp);

};

#endif // LoginProxy_h__
