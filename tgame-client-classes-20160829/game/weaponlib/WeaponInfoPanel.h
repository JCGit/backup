#ifndef WeaponInfoPanel_h__
#define WeaponInfoPanel_h__

#include "BasePanel.h"
#include "WeaponDetailInfoLayout.h"
#include "BasicTypes.h"
#include "WeaponAdvanceLayout.h"
#include "WeaponRefitLayout.h"
#include "WeaponLvUpLayout.h"

#include "game/guide/GuideControl.h"

namespace vmsg {
    class CSWeapon;
};

class CStarProgressBar;

//子面板详细信息
class WeaponInfoOne : public CCObject {
public:
    enum 
    {
        BG_EFF_TAG  = 141,
        ADV_EFF_TAG  = 151, //进阶特效
        LV_UP_EFF_TAG  = 152, //升级特效
    };
    WeaponInfoOne(){
        m_uWeaponGuid = 0;
    }
    virtual ~WeaponInfoOne() {

    }

    void initWithLayout(UILayout *pRefLayout);
    void showWeaponInfo(const unsigned int cfgID, const UINT64_t weaponGUID);
    
    void showLvUpDoneAttr(); //选择完升级材料后，显示升级后的属性
    void showDefaultAttr(); //展示默认的属性值
    void playLvUpEff(const int iDeltaLv);
    void resetUI();

    void startBgEff();
    void stopBgEff();

    void playAdvEff(); //播放进阶特效

private:
    void playLvUpEffCallback(CCArmature* armature, MovementEventType type, const char * name);
    void playAdvEffCallback(CCArmature* armature, MovementEventType type, const char * name);
    void onPlayAdvEffFrameEvt(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx);

    //特效相关回调
    void txtPreviewCallback(CCNode* pSender, void* pData);
    void txtRealCallback(CCNode* pSender, void* pData);

private:
    UILayout* m_pRefLayout; //base_info_layout

    UIImageView* m_pChangeWeaponTipImg;
    UIImageView* m_pEffBg;
    UIImageView* m_pInUseTipImg; //图片：目前装备 in_use_tip_img
    UILabel* m_pNameTxt; //name_txt
    UILabel* m_pLvTxt; //lv_txt
    UILabel* m_pAttrLvTxt; //lv_val_txt
    UILabel* m_pAtkValTxt; //atk_val_txt
    UILabel* m_pAtkSpeedTxt; //atk_sp_txt 
    UILabel* m_pExpPerTxt; //100%
	UILabel* m_pBulletValTxt;	//弹药

    UIImageView* m_pWeaponIconBg;
    UIImageView* m_pWeaponImg;
    UIImageView* m_pWeaponColorImg; //枪械品级

    CStarProgressBar* m_pStarBar;	//星级
    UILoadingBar* m_pExpBar;
    UILabel* m_pHitValTxt; //命中

    UINT64_t m_uWeaponGuid;
    int m_uWeaponCfgID;

    //特效相关的变量
    string m_strRealAttrLvTxt; //未升级时
    string m_strPreviewAttrLvTxt; //升级后

    string m_strRealAtkValTxt;  //未升级
    string m_strPreviewAtkValTxt;  //升级后
    int m_iRealExpPercent; //未升级时的经验条进度

    int m_iLvUpEffPlayNum; //升级特效播放次数
};

class WeaponInfoPanel : public BasePanel, public GuideContainer
{
public:
    enum 
    {
        INIT_WEAPON_NUM = 20,
    };
	WeaponInfoPanel();
	virtual ~WeaponInfoPanel();

	CREATE_FUNC(WeaponInfoPanel);

    bool init();

    virtual void open();
    virtual void close();
    virtual void refresh();
    void setForseClose(); //直接关闭，不打开枪械库

	virtual CCNode* getGuideNodeByName(const std::string &name);

    void updateNowWeapon();
    void updateWeaponInfo(const unsigned int cfgID, const UINT64_t weaponGUID); //显示指定的枪械
    void doSelMatResult(); //选择完枪械升级材料了
    void doUpdateUI(const vmsg::CSWeapon* info, const bool bIsPlayEff = false); //回包时更新当前的显示

    void showLvUpDoneAttr(); //选择完升级材料后，显示升级后的属性

    void playAdvEffect();
    void playRefitEffect();
    void playLvUpEff(const int iDeltaLv);
    
    void doRspError(); //协议返回错误时

private:
	void initUI();
	void touchEventHandler(CCObject* pSender, TouchEventType type);
    void showChildLayout(UILayout* pLayout, UIButton* pTabBtn);

    void playEffCallback(CCArmature* armature, MovementEventType type, const char * name);

    void selBtn(UIButton* pSelBtn);

    void resetUI();
private:
	UILayout* m_pOpenLayout; //当前正在显示的Layout
    CCArray* m_pOpBtnLst; //枪械操作按钮
    
	UIButton* m_pDoUpBtn;
	UIButton* m_pDetailInfoBtn;
	UIButton* m_pAdvanceBtn;
	UIButton* m_pRefitBtn;
    UIButton* m_pCloseBtn;

    UIImageView* m_pWeaponImg; //点击更换枪械

    WeaponInfoOne m_stInfoLayout; //右侧的大面板
    
    WeaponDetailInfoLayout m_stDetailInfoLayout; //详细信息面板
    WeaponAdvanceLayout m_stAdvanceLayout; //进阶面板
    WeaponRefitLayout m_stRefitLayout; //枪械改装
    WeaponLvUpLayout m_stLvUpLayout; //枪械升级
    UIImageView* m_pSubPanelBg; //子面板BG

    CC_SYNTHESIZE(UINT64_t, m_uWeaponGuid, WeaponGuid);
    CC_SYNTHESIZE(int, m_uWeaponCfgID, WeaponCfgID);

    bool m_bIsForseClose;
};

#endif // WeaponInfoPanel_h__
