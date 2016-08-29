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

	
    void sendLoginRqst(const std::string & loginInfo); // ��ҵ�¼
    void sendAppleGuestLoginRqst(const std::string & loginInfo); // apple �ο͵�¼

	void sendQueryRoleListRqst(); // ��ѯ��ҵĽ�ɫ�б�

	void sendRegisterRqst(unsigned int uPlayerID, const char *pszNickName); // ע��һ����ɫ

	void sendRoleLoginRqst(UIN_t uin); // ��ɫ����

	void sendRoleLogoutRqst(UIN_t uin); // ��ɫ�ǳ�

	void sendQueryPlayerInfoRqst(UIN_t uin); // ��ѯ�����Ϣ

	void sendHeartBeatRqst(); // ����

	void sendGetSvrListHttpRqst(const std::string &strURL); // ��ȡ�������б�

	void sendQueryGoldInfoRqst(); // ��ѯ������Ϣ

	void sendQueryPlayerDetailInfoRqst(); // ��ѯ�����ϸ��Ϣ

	void sendQueryOtherPlayerInfoRqst(UIN_t uin); // ��ѯ���������Ϣ

    void sendQuickLoginRqst(const bool bNeedOsdInfo = false);

    void sendDailySignInRqst(); //ÿ��ǩ��

	void sendSetSettingParamRqst(const std::string &strSettingParam);		//������øı�

    void sendUmengDeviceTokenRqst(); //umeng�ϱ�device token
private:
    int OnLogin(const vmsg::CSPlayerLoginRsp & stRsp);

};

#endif // LoginProxy_h__
