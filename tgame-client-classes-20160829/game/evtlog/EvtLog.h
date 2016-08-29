#ifndef __EVT_LOG_H__
#define __EVT_LOG_H__

#include <string>
#include "vlib/base/CVSingleton.h"

using namespace std;

//�¼�����
enum enumEvtLogID
{
    EVT_LOG_BOOT_GAME       = 10001, ///< ������Ϸ 
    EVT_LOG_CHECK_UPDATE    = 10002, ///< ������
    EVT_LOG_CHECK_UPDATE_RET    = 10003, ///< �����·��ؽ��
    EVT_LOG_CHECK_UPDATE_RET_CHECK    = 10004, ///< �����(�Ƿ���Ҫ����)
    EVT_LOG_START_DOWNLOAD    = 10005, ///< ��ʼ���ظ���
    EVT_LOG_END_DOWNLOAD      = 10006, ///< ������ظ���
    EVT_LOG_START_UNZIP    = 10007, ///< ��ʼ��ѹ
    EVT_LOG_END_UNZIP      = 10008, ///< ��ɽ�ѹ
    EVT_LOG_NEED_REBOOT    = 10009, ///< ���½���������Ϸ
    EVT_LOG_LOAD_CFG_RES   = 10010, ///< ����������Դ
    EVT_LOG_LOAD_CFG_RES_END    = 10011, ///< ��������������Դ
    EVT_LOG_SDK_LOGIN    = 10012, ///< ��ʼ��¼��������
    EVT_LOG_SDK_LOGIN_END    = 10013, ///< ������½����������token
    EVT_LOG_SDK_CHECK_TOKEN  = 10014, ///< ��֤token
    EVT_LOG_SDK_CHECK_TOKEN_RET  = 10017, ///< ��֤token���
    EVT_LOG_GET_SVR_LIST  = 10018, ///< ��ȡ�������б�
    EVT_LOG_CHOOSE_SVR    = 10019, ///< ѡ���������½

    //���ֶ���
    EVT_LOG_PLAY_START_ANI  = 10021, ///< ���Ŷ���
    EVT_LOG_SKIP_START_ANI  = 10022, ///< ����������
    EVT_LOG_END_START_ANI   = 10023, ///< �����������

    //������ɫ
    EVT_LOG_CREATE_ROLE  = 10024, ///< ���������ɫ
    EVT_LOG_CREATE_ROLE_DONE  = 10026, ///< ������ɫ�ɹ�

    //�׳�ս��ָ��
    EVT_LOG_DEMO_DIALOG  = 10027, ///< �����׳���ʾս�����������ֶԻ��������ʾ
    EVT_LOG_DEMO_ATK  = 10028, ///< ���˳��֣����ֶԻ����������ʾ���������ť
    EVT_LOG_DEMO_MOV_UP  = 10029, ///< ��ɹ�����������һ�������ʾ�����ƶ�����
    EVT_LOG_DEMO_MOV_DOWN  = 10030, ///< ��ɹ�����������һ�������ʾ�����ƶ�����
    EVT_LOG_DEMO_SKILL  = 10031, ///< ��ɹ�����������һ�׶Σ����ֵ��ˣ���ʾ�ż���
    EVT_LOG_DEMO_SKILL_DONE  = 10032, ///< ��ɷż��ܣ������ʾ������һ�׶�
    EVT_LOG_DEMO_END  = 10033, ///< ��ɹ���������ָ��������ͬ������

    //�״ν���ͬ����
    EVT_LOG_GO_NPC_GET_TASK_FIRST  = 10134, ///< ���������׷�٣�Ѱ·��ȡ��һ������
    EVT_LOG_GET_TASK_FIRST  = 10135, ///< �Զ�Ѱ·��NPC����������Ի��������ȡ
    EVT_LOG_FIRST_TASK_OPEN_WAR_PANEL  = 10136, ///< ���������׷�٣��򿪹ؿ�ѡ�����
    EVT_LOG_FIRST_TASK_SEL_WAR  = 10137, ///< �����ʾѡ���һ���ؿ��������ؿ���Ϣ���
    EVT_LOG_FIRST_TASK_ENTER_WAR  = 10138, ///< �������ؿ�����һ�أ�
    EVT_LOG_FIRST_WAR_SKILL_NUM  = 10139, ///< ��һ���о��У������������ܼ��ܵĵ������
    EVT_LOG_FIRST_WAR_RESULT  = 10140, ///< ��һ�ؽ����󵯳��ؿ��������
    EVT_LOG_FIRST_WAR_CARD  = 10141, ///< ��һ�ؽ����󵯳��������
    EVT_LOG_FIRST_WAR_END  = 10142, ///< �ؿ�ʤ���������ص�ͬ���㣨��һ�ؽ�����
    EVT_LOG_GO_NPC_COMMIT_TASK_FIRST = 10143, ///< ���������׷��Ѱ·��Ѱ·�ύ��һ������
    EVT_LOG_COMMIT_FIRST_TASK  = 10144, ///< �Զ�Ѱ·������Ի����ύ�����ύ��һ������
    EVT_LOG_UP_TO_LV_TWO  = 10142, ///< ����2��

    //ͨ���¼�����Ext�ֶΣ�
    EVT_LOG_GET_TASK  = 11001, ///< ��ȡ����
    EVT_LOG_COMMIT_TASK  = 11002, ///< �ύ����
    EVT_LOG_LV_UP  = 11003, ///< ����

}; //end enumEvtLog



class EvtLogControl {
public:
    void log(const int iEvtID, const int iExtID = 0);
    void log(const int iEvtID, const std::string& strExt = "");
    void logByLv(const int iEvtID, const std::string strExt = ""); //���Ƶȼ����ϱ�

    //cyou
    void doCyouLogin();
    void doCyouMBILogLogin(); //�ϱ���½,uid�ʹ���
    void doCyouMBILogEvt(const std::string &strEvtID);
    void doCyouOpenBugReport(); //cyou�ͷ��ӿ�
    //cyou end

    //talking data log,begin
    void logTDAppReg(const std::string &strUserID);
    void logTDAppLogin(const std::string &strUserID);

    //iAmount ��λΪ����
    //strCurrency,��ʹ�ù��ʱ�׼��֯ISO 4217�й淶��3 λ��ĸ�����ǻ������͡�
    //Ŀǰ֧�ֵĻ��������У������CNY����Ԫ  HKD��̨�ң�TWD����ԪUSD ��ŷԪEUR ��Ӣ�� GBP����Ԫ  JPY ��
    void logTDGamePay(const string& strUserID, const string& strOrderID, int iAmount, const string& strCurrency, const string& strPayType);
    void logTDGameAddRole(const std::string &strRoleName);
    //talking data log,end
private:
    void initSysInfo();
    void sendEvtID(const std::string &strInfo);
    static void* pthreadSendEvtID(void *pStr);
private:
    std::string m_strSysInfo;
};


#define P_EVT_LOG CVLib::CVSingleton<EvtLogControl>::Instance()

//use this to add one evt log :)
#define ADD_EVT_LOG(iEvtID) P_EVT_LOG->log(iEvtID, 0)

#endif //__EVT_LOG_H__


