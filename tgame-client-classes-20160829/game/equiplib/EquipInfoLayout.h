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

    void previewLvUpExp(); //升级时预览
    void resetUI();

    void playAdvEff(); //进阶特效
private:
    void initUI();
    std::string getEquipPosStr(const int iEquipPos);

    //特效相关回调
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

    //预览升级所需要的变量
    string m_strRealMaxLvTxt; //未升级时的最大等级
    string m_strPreviewMaxLvTxt; //升级后的最大等级
    string m_strRealAttrTxt;  //未升级时的属性
    string m_strPreviewAttrTxt;  //升级后的属性
    int m_iRealExpPercent; //未升级时的经验条进度

    int m_iLvUpEffPlayNum; //升级特效播放次数
};


#endif // EquipInfoLayout_h__
