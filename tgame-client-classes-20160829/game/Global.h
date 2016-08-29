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
        strAniName = "fly"; //Ĭ�ϲ���fly
        fYPosPercent = 0.66f;
    }

    //�������
    string strJsonPath; //�������.json�ļ�·��,�� effect/ut00001/ut00001.ExportJson
    string strArmatureName; //�������.create ʱ����Ķ��������� ut00001
    string strSoundName; //�������.��Ч���֣��� ut00001.ogg

    //ѡ�����
    string strAniName; //����������fly
    bool   bIsLoop; //�Ƿ�ѭ��
    CCObject *target; //�������Ļص�
    SEL_MovementEventCallFunc callFunc; //�������Ļص�

    double fYPosPercent; //��Чy�������λ�ã�Ĭ��0.66
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
        POP_EFFECT_TAG  = 76, //��������Ч
        POP_FP_UP_TAG   = 77, //ս������

        POP_EFFECT_Z    = 20, //��Ч��z order
        POP_FP_UP_Z     = 21, //ս��������z order
    };
	Global();
	~Global();

    unsigned int getSvrTime() const; //��һ������ʱ�ķ�����ʱ��
    unsigned int getSvrTimeRelative(); //���ݵ�ǰʱ���������ķ�����ʱ��
	void setSvrTime(unsigned int uSvrTime);

    //��Ļ�м���ʾ��Ч,������Ч���֣���ut00001,����һ���Զ�ɾ��
    void showPopEff(const char* strEffName); 

    //��Ļ�м���ʾ��Ч,
    //���û�лص����򲥷�һ���Զ�ɾ��������У�����ûص�����
    void showPopEff(const GPopEffInfo& stEffInfo); 
    void clearPopEff(); //���

    //pParentNode�ϲ�����ЧstrArmatureName
    //����strFullPathΪ��ȫ·��
    //iLoop: 0  ��ѭ����>0 ѭ��
    void showEffOnNode(CCNode* pParentNode, const char* strArmatureName, 
        const char* strFullPath, 
        const CCPoint& stArmaturePos, 
        const CCPoint& stArmatureAnchorPt, 
        const int iLoop = 0);

    void rmEffOnNode(CCNode* pParentNode);

    void showAdvItemEff(const int iType, const int iCfgID, 
        CCObject *target = NULL, SEL_MovementEventCallFunc callFunc = NULL); //�߼���Ʒȫ����Ч
    void clearAdvItemEff(); //���������Ч

    CCLayer* getScenePopLayer();

    void showFpUpEff(const int iFromVal, const int iToVal); //ս������

    //ȫ����غ���
    void releaseUnusePanel(); //�˳�cityʱ�ͷŲ����õ������Դ
    void resetDataOnReconnect(); //�������µ�½ʱ�����������
    void onLoginPlayerInfoInited(); //��½ʱ��ȡ�����Ϣ����Ҫ���µ���Ϣ��������
    void onNewDayRefresh(); //0���ˢ��
private:
    void onPopEffDone(CCArmature* armature, MovementEventType type, const char * name);

    void onAdvItemEffFrameEvt(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx);

    void onRollFpDone();
    void onCleanFpEff();
private:
	unsigned int m_uSvrTime;
    int m_iDeltaTime; //������ʱ���ֵ

};

#define P_GLOBAL CVLib::CVSingleton<Global>::Instance()

#endif // Global_h__
