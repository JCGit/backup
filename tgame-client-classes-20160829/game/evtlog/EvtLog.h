#ifndef __EVT_LOG_H__
#define __EVT_LOG_H__

#include <string>
#include "vlib/base/CVSingleton.h"

using namespace std;

//事件定义
enum enumEvtLogID
{
    EVT_LOG_BOOT_GAME       = 10001, ///< 启动游戏 
    EVT_LOG_CHECK_UPDATE    = 10002, ///< 检查更新
    EVT_LOG_CHECK_UPDATE_RET    = 10003, ///< 检查更新返回结果
    EVT_LOG_CHECK_UPDATE_RET_CHECK    = 10004, ///< 检查结果(是否需要更新)
    EVT_LOG_START_DOWNLOAD    = 10005, ///< 开始下载更新
    EVT_LOG_END_DOWNLOAD      = 10006, ///< 完成下载更新
    EVT_LOG_START_UNZIP    = 10007, ///< 开始解压
    EVT_LOG_END_UNZIP      = 10008, ///< 完成解压
    EVT_LOG_NEED_REBOOT    = 10009, ///< 更新结束重启游戏
    EVT_LOG_LOAD_CFG_RES   = 10010, ///< 加载配置资源
    EVT_LOG_LOAD_CFG_RES_END    = 10011, ///< 结束加载配置资源
    EVT_LOG_SDK_LOGIN    = 10012, ///< 开始登录（渠道）
    EVT_LOG_SDK_LOGIN_END    = 10013, ///< 结束登陆，渠道返回token
    EVT_LOG_SDK_CHECK_TOKEN  = 10014, ///< 验证token
    EVT_LOG_SDK_CHECK_TOKEN_RET  = 10017, ///< 验证token结果
    EVT_LOG_GET_SVR_LIST  = 10018, ///< 拉取服务器列表
    EVT_LOG_CHOOSE_SVR    = 10019, ///< 选择服务器登陆

    //新手动画
    EVT_LOG_PLAY_START_ANI  = 10021, ///< 播放动画
    EVT_LOG_SKIP_START_ANI  = 10022, ///< 动画被跳过
    EVT_LOG_END_START_ANI   = 10023, ///< 动画播放完毕

    //创建角色
    EVT_LOG_CREATE_ROLE  = 10024, ///< 点击创建角色
    EVT_LOG_CREATE_ROLE_DONE  = 10026, ///< 创建角色成功

    //首场战斗指引
    EVT_LOG_DEMO_DIALOG  = 10027, ///< 进入首场演示战斗场景，出现对话，点击提示
    EVT_LOG_DEMO_ATK  = 10028, ///< 敌人出现，出现对话，点击，提示点击攻击按钮
    EVT_LOG_DEMO_MOV_UP  = 10029, ///< 完成攻击，出现下一波怪物，提示向上移动攻击
    EVT_LOG_DEMO_MOV_DOWN  = 10030, ///< 完成攻击，出现下一波怪物，提示向下移动攻击
    EVT_LOG_DEMO_SKILL  = 10031, ///< 完成攻击，进入下一阶段，出现敌人，提示放技能
    EVT_LOG_DEMO_SKILL_DONE  = 10032, ///< 完成放技能，点击提示进入下一阶段
    EVT_LOG_DEMO_END  = 10033, ///< 完成攻击，结束指引，进入同屏场景

    //首次进入同屏点
    EVT_LOG_GO_NPC_GET_TASK_FIRST  = 10134, ///< 点击任务快捷追踪（寻路接取第一个任务）
    EVT_LOG_GET_TASK_FIRST  = 10135, ///< 自动寻路到NPC，出现任务对话，点击接取
    EVT_LOG_FIRST_TASK_OPEN_WAR_PANEL  = 10136, ///< 点击任务快捷追踪，打开关卡选择面板
    EVT_LOG_FIRST_TASK_SEL_WAR  = 10137, ///< 点击提示选择第一个关卡，弹出关卡信息面板
    EVT_LOG_FIRST_TASK_ENTER_WAR  = 10138, ///< 点击进入关卡（第一关）
    EVT_LOG_FIRST_WAR_SKILL_NUM  = 10139, ///< 第一关中绝招，进化技，闪避技能的点击次数
    EVT_LOG_FIRST_WAR_RESULT  = 10140, ///< 第一关结束后弹出关卡结算面板
    EVT_LOG_FIRST_WAR_CARD  = 10141, ///< 第一关结束后弹出翻牌面板
    EVT_LOG_FIRST_WAR_END  = 10142, ///< 关卡胜利结束，回到同屏点（第一关结束）
    EVT_LOG_GO_NPC_COMMIT_TASK_FIRST = 10143, ///< 点击任务快捷追踪寻路（寻路提交第一个任务）
    EVT_LOG_COMMIT_FIRST_TASK  = 10144, ///< 自动寻路，点击对话，提交任务（提交第一个任务）
    EVT_LOG_UP_TO_LV_TWO  = 10142, ///< 升到2级

    //通用事件（带Ext字段）
    EVT_LOG_GET_TASK  = 11001, ///< 接取任务
    EVT_LOG_COMMIT_TASK  = 11002, ///< 提交任务
    EVT_LOG_LV_UP  = 11003, ///< 升级

}; //end enumEvtLog



class EvtLogControl {
public:
    void log(const int iEvtID, const int iExtID = 0);
    void log(const int iEvtID, const std::string& strExt = "");
    void logByLv(const int iEvtID, const std::string strExt = ""); //限制等级的上报

    //cyou
    void doCyouLogin();
    void doCyouMBILogLogin(); //上报登陆,uid和大区
    void doCyouMBILogEvt(const std::string &strEvtID);
    void doCyouOpenBugReport(); //cyou客服接口
    //cyou end

    //talking data log,begin
    void logTDAppReg(const std::string &strUserID);
    void logTDAppLogin(const std::string &strUserID);

    //iAmount 单位为：分
    //strCurrency,请使用国际标准组织ISO 4217中规范的3 位字母代码标记货币类型。
    //目前支持的货币种类有：人民币CNY，港元  HKD，台币：TWD，美元USD ；欧元EUR ；英镑 GBP，日元  JPY ；
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


