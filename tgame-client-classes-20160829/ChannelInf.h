#ifndef __ChannelInf_H_
#define __ChannelInf_H_

#include <string>

/*
�ӿڶ���ο���proj.android\chameleon\chameleoncb\include\chameleoncb\ChameleonChannelAPI.h
win32�º����ÿ�ʵ��
android�µ�ʵ��д��proj.android\jni\hellocpp\ChannelInfImp.cpp��

*/

class ChannelInf {
public:
    static void Init();
    static void Login();
    static std::string GetChannelName();

    static int Buy(const std::string & orderId, 
        const std::string & uidInGame, 
        const std::string & userNameInGame, 
        const std::string & serverId, 
        const std::string & productName, 
        const std::string & productId,
        const std::string & payInfo,
        int productCount,
        int realPayMoney);

    static std::string GetPayToken();
    static std::string GetSDKUid();
    static bool submitPlayerInfo(const std::string & roleId, 
        const std::string & roleName, 
        const std::string & roleLevel, 
        int zoneId,
        const std::string & zoneName);
    static void Logout();

    static void onLoginRsp(const std::string & strInfo);

    static void showToolbar(bool isVisible);

    static void doRunProtocol(int id, const std::string & protocol, const std::string & message);

	/**
     * ��ƽ̨�Ƿ�֧���˺��л�
     * @return {bool}, �Ƿ�֧���˺��л�
     */
	static bool isSupportSwtichAccount();

	/**
     * �����л��˻�������
     * @param {int} id, ��ʶ�������id
     * @return {int}, 0��ʾ�첽����ɹ��������ʾʧ��
     */
	static int switchAccount(int id);
};

#endif //__ChannelInf_H_

