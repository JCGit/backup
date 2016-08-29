#ifndef EquipInfoLayout_h__
#define EquipInfoLayout_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "EquipLibObj.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
using namespace std;
class CStarProgressBar;

class EquipInfoLayout : public CCObject{
public:
    enum 
    {
        LV_UP_EFF_TAG = 152,
        ADV_EFF_TAG = 153,
    };
    EquipInfoLayout(){
        m_pRefLayout = NULL;
        m_iRealExpPercent = 0;
    }
    virtual ~EquipInfoLayout() {
        CC_SAFE_RELEASE_NULL(m_pRefLayout);
    }

    void initWithLayout(UILayout *pRefLayout);
    UILayout* getRefLayout() {
        return m_pRefLayout;
    }
    void doUpdateUI(const bool bIsRsp);

    void previewLvUpExp(); //����ʱԤ��
    void resetUI();

    void playAdvEff(); //������Ч
private:
    void initUI();
    std::string getEquipPosStr(const int iEquipPos);

    //��Ч��ػص�
    void txtPreviewCallback(CCNode* pSender, void* pData);
    void txtRealCallback(CCNode* pSender, void* pData);

    void playLvUpEff(const int iDeltaLv);
    void playLvUpEffCallback(CCArmature* armature, MovementEventType type, const char * name);


    void onPlayAdvEffFrameEvt(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx);
    void playAdvEffCallback(CCArmature* armature, MovementEventType type, const char * name);
private:
    UILayout* m_pRefLayout; 


    UILabel* m_pNameTxt;
    UIImageView* m_pEquipImg;
    UIImageView* m_pImgInUse;
    UIImageView* m_pCellBg;

    UILabel* m_pPosTxt;
    UILabel* m_pDescTxt;
    UILabel* m_pMaxLvTxt;
    UILabel* m_pLvTxt;
    UILabel* m_pAttrNameTxt;
    UILabel* m_pAttrValTxt;
    
    UILoadingBar* m_pExpBar;
    UILabel* m_pExpBarValTxt;

    CStarProgressBar* m_pEquipStar;

    EquipLibOneInfo m_stEquipLibInfo;

    //Ԥ����������Ҫ�ı���
    string m_strRealMaxLvTxt; //δ����ʱ�����ȼ�
    string m_strPreviewMaxLvTxt; //����������ȼ�
    string m_strRealAttrTxt;  //δ����ʱ������
    string m_strPreviewAttrTxt;  //�����������
    int m_iRealExpPercent; //δ����ʱ�ľ���������

    int m_iLvUpEffPlayNum; //������Ч���Ŵ���
};


#endif // EquipInfoLayout_h__
