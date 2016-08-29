#ifndef EquipLibPanel_h__
#define EquipLibPanel_h__

#include "BasePanel.h"
#include "BasicTypes.h"
#include "EquipLvUpLayout.h"
#include "EquipAdvLayout.h"
#include "EquipMixLayout.h"
#include "game/guide/GuideControl.h"
#include "EquipInfoLayout.h"

class EquipLibPanel : public BasePanel, public GuideContainer
{
public:
	EquipLibPanel();
	virtual ~EquipLibPanel();

	CREATE_FUNC(EquipLibPanel);

    bool init();

	virtual CCNode* getGuideNodeByName(const std::string &name);

    virtual void open();
    virtual void close();
    virtual void refresh();

    void doUpdateUI(const bool bIsRsp);
    void updateSelMat();

    void previewLvUp();
    void playAdvEffect();
    void playMixEffect();
private:
	void initUI();
	void touchEventHandler(CCObject* pSender, TouchEventType type);
    void showChildLayout(UILayout* pLayout, UIButton* pTabBtn);
    
    void selBtn(UIButton* pSelBtn);
    void playEffCallback(CCArmature* armature, MovementEventType type, const char * name);
private:
	UILayout* m_pOpenLayout; //��ǰ������ʾ��Layout
    CCArray* m_pOpBtnLst; //������ť
    
    UIButton* m_pCloseBtn;

	UIButton* m_pLvUpBtn;   //����
	UIButton* m_pAdvanceBtn; //����
	UIButton* m_pMixBtn;    //�ں�

    UILabel* m_pCoinTxt;
    UILabel* m_pGoldTxt;

    EquipInfoLayout     m_stEquipInfoLayout;

    EquipLvUpLayout     m_stLvUpLayout; //�������
    EquipAdvLayout      m_stAdvLayout;  //�������
    EquipMixLayout      m_stMixLayout;  //�ں����


};

#endif // EquipLibPanel_h__
