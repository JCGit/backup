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

//�������ϸ��Ϣ
class WeaponInfoOne : public CCObject {
public:
    enum 
    {
        BG_EFF_TAG  = 141,
        ADV_EFF_TAG  = 151, //������Ч
        LV_UP_EFF_TAG  = 152, //������Ч
    };
    WeaponInfoOne(){
        m_uWeaponGuid = 0;
    }
    virtual ~WeaponInfoOne() {

    }

    void initWithLayout(UILayout *pRefLayout);
    void showWeaponInfo(const unsigned int cfgID, const UINT64_t weaponGUID);
    
    void showLvUpDoneAttr(); //ѡ�����������Ϻ���ʾ�����������
    void showDefaultAttr(); //չʾĬ�ϵ�����ֵ
    void playLvUpEff(const int iDeltaLv);
    void resetUI();

    void startBgEff();
    void stopBgEff();

    void playAdvEff(); //���Ž�����Ч

private:
    void playLvUpEffCallback(CCArmature* armature, MovementEventType type, const char * name);
    void playAdvEffCallback(CCArmature* armature, MovementEventType type, const char * name);
    void onPlayAdvEffFrameEvt(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx);

    //��Ч��ػص�
    void txtPreviewCallback(CCNode* pSender, void* pData);
    void txtRealCallback(CCNode* pSender, void* pData);

private:
    UILayout* m_pRefLayout; //base_info_layout

    UIImageView* m_pChangeWeaponTipImg;
    UIImageView* m_pEffBg;
    UIImageView* m_pInUseTipImg; //ͼƬ��Ŀǰװ�� in_use_tip_img
    UILabel* m_pNameTxt; //name_txt
    UILabel* m_pLvTxt; //lv_txt
    UILabel* m_pAttrLvTxt; //lv_val_txt
    UILabel* m_pAtkValTxt; //atk_val_txt
    UILabel* m_pAtkSpeedTxt; //atk_sp_txt 
    UILabel* m_pExpPerTxt; //100%
	UILabel* m_pBulletValTxt;	//��ҩ

    UIImageView* m_pWeaponIconBg;
    UIImageView* m_pWeaponImg;
    UIImageView* m_pWeaponColorImg; //ǹеƷ��

    CStarProgressBar* m_pStarBar;	//�Ǽ�
    UILoadingBar* m_pExpBar;
    UILabel* m_pHitValTxt; //����

    UINT64_t m_uWeaponGuid;
    int m_uWeaponCfgID;

    //��Ч��صı���
    string m_strRealAttrLvTxt; //δ����ʱ
    string m_strPreviewAttrLvTxt; //������

    string m_strRealAtkValTxt;  //δ����
    string m_strPreviewAtkValTxt;  //������
    int m_iRealExpPercent; //δ����ʱ�ľ���������

    int m_iLvUpEffPlayNum; //������Ч���Ŵ���
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
    void setForseClose(); //ֱ�ӹرգ�����ǹе��

	virtual CCNode* getGuideNodeByName(const std::string &name);

    void updateNowWeapon();
    void updateWeaponInfo(const unsigned int cfgID, const UINT64_t weaponGUID); //��ʾָ����ǹе
    void doSelMatResult(); //ѡ����ǹе����������
    void doUpdateUI(const vmsg::CSWeapon* info, const bool bIsPlayEff = false); //�ذ�ʱ���µ�ǰ����ʾ

    void showLvUpDoneAttr(); //ѡ�����������Ϻ���ʾ�����������

    void playAdvEffect();
    void playRefitEffect();
    void playLvUpEff(const int iDeltaLv);
    
    void doRspError(); //Э�鷵�ش���ʱ

private:
	void initUI();
	void touchEventHandler(CCObject* pSender, TouchEventType type);
    void showChildLayout(UILayout* pLayout, UIButton* pTabBtn);

    void playEffCallback(CCArmature* armature, MovementEventType type, const char * name);

    void selBtn(UIButton* pSelBtn);

    void resetUI();
private:
	UILayout* m_pOpenLayout; //��ǰ������ʾ��Layout
    CCArray* m_pOpBtnLst; //ǹе������ť
    
	UIButton* m_pDoUpBtn;
	UIButton* m_pDetailInfoBtn;
	UIButton* m_pAdvanceBtn;
	UIButton* m_pRefitBtn;
    UIButton* m_pCloseBtn;

    UIImageView* m_pWeaponImg; //�������ǹе

    WeaponInfoOne m_stInfoLayout; //�Ҳ�Ĵ����
    
    WeaponDetailInfoLayout m_stDetailInfoLayout; //��ϸ��Ϣ���
    WeaponAdvanceLayout m_stAdvanceLayout; //�������
    WeaponRefitLayout m_stRefitLayout; //ǹе��װ
    WeaponLvUpLayout m_stLvUpLayout; //ǹе����
    UIImageView* m_pSubPanelBg; //�����BG

    CC_SYNTHESIZE(UINT64_t, m_uWeaponGuid, WeaponGuid);
    CC_SYNTHESIZE(int, m_uWeaponCfgID, WeaponCfgID);

    bool m_bIsForseClose;
};

#endif // WeaponInfoPanel_h__
