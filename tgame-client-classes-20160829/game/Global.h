#ifndef Global_h__
#define Global_h__

#include "vlib/base/CVSingleton.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
using namespace std;
USING_NS_CC_EXT;
USING_NS_CC;

struct GPopEffInfo {

    GPopEffInfo() {
        target = NULL;
        callFunc = NULL;
        bIsLoop = false;
        strAniName = "fly"; //默认播放fly
        fYPosPercent = 0.66f;
    }

    //必填参数
    string strJsonPath; //必填参数.json文件路径,如 effect/ut00001/ut00001.ExportJson
    string strArmatureName; //必填参数.create 时传入的动作名，如 ut00001
    string strSoundName; //必填参数.音效名字，如 ut00001.ogg

    //选填参数
    string strAniName; //动作名，如fly
    bool   bIsLoop; //是否循环
    CCObject *target; //播放完后的回调
    SEL_MovementEventCallFunc callFunc; //播放完后的回调

    double fYPosPercent; //特效y坐标相对位置，默认0.66
};

/*
typedef void (CCObject::*SEL_AdvItemEffDoneEvtCallFunc)();
#define advEffDoneEvent_selector(_SELECTOR) (SEL_AdvItemEffDoneEvtCallFunc)(&_SELECTOR)
*/
class Global : public CCObject
{
public:
    enum 
    {
        POP_EFFECT_TAG  = 76, //弹出层特效
        POP_FP_UP_TAG   = 77, //战力提升

        POP_EFFECT_Z    = 20, //特效的z order
        POP_FP_UP_Z     = 21, //战力提升的z order
    };
	Global();
	~Global();

    unsigned int getSvrTime() const; //上一次心跳时的服务器时间
    unsigned int getSvrTimeRelative(); //根据当前时间修正过的服务器时间
	void setSvrTime(unsigned int uSvrTime);

    //屏幕中间显示特效,传入特效名字，如ut00001,播放一次自动删除
    void showPopEff(const char* strEffName); 

    //屏幕中间显示特效,
    //如果没有回调，则播放一次自动删除；如果有，则调用回调函数
    void showPopEff(const GPopEffInfo& stEffInfo); 
    void clearPopEff(); //清除

    //pParentNode上播放特效strArmatureName
    //传入strFullPath为完全路径
    //iLoop: 0  不循环，>0 循环
    void showEffOnNode(CCNode* pParentNode, const char* strArmatureName, 
        const char* strFullPath, 
        const CCPoint& stArmaturePos, 
        const CCPoint& stArmatureAnchorPt, 
        const int iLoop = 0);

    void rmEffOnNode(CCNode* pParentNode);

    void showAdvItemEff(const int iType, const int iCfgID, 
        CCObject *target = NULL, SEL_MovementEventCallFunc callFunc = NULL); //高级物品全屏特效
    void clearAdvItemEff(); //清除掉该特效

    CCLayer* getScenePopLayer();

    void showFpUpEff(const int iFromVal, const int iToVal); //战力提升

    //全局相关函数
    void releaseUnusePanel(); //退出city时释放不常用的面板资源
    void resetDataOnReconnect(); //断线重新登陆时重置玩家数据
    void onLoginPlayerInfoInited(); //登陆时拉取玩家信息后，需要更新的信息放在这里
    void onNewDayRefresh(); //0点后刷新
private:
    void onPopEffDone(CCArmature* armature, MovementEventType type, const char * name);

    void onAdvItemEffFrameEvt(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx);

    void onRollFpDone();
    void onCleanFpEff();
private:
	unsigned int m_uSvrTime;
    int m_iDeltaTime; //服务器时间差值

};

#define P_GLOBAL CVLib::CVSingleton<Global>::Instance()

#endif // Global_h__
