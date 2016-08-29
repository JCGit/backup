#ifndef EquipInfoPanel_h__
#define EquipInfoPanel_h__

#include "basic/BasePanel.h"
#include "CSEquipItemMsg.pb.h"

#include "game/guide/GuideControl.h"

class EquipInfoCell;

class EquipInfoPanel: public BasePanel, public GuideContainer
{
public:
	enum EquipStatus {
		EQUIP_IN_BAG,   // 在背包里
		EQUIP_ON_ROLE,  // 在角色身上
        EQUIP_INFO,     // 展示装备信息
	};
public:
	EquipInfoPanel();
	virtual ~EquipInfoPanel();
	
	static EquipInfoPanel* create();

	// override start

	virtual bool init();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	// override end

	void updateInfo(const vmsg::CSEquipInfo &stEquipInfo, unsigned int uEquipStatus);

    void updateSimpleEquipInfo(const int iEquipCfgID); //展示该配置id的装备信息
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void onSaleDialog();

private:
	UIButton *m_pCloseBtn;

	UIListView *m_pEquipListView;

	UIButton *m_pSaleBtn;

	UIButton *m_pEquipBtn;
	UIButton *m_pCultureBtn;
    
	EquipInfoCell *m_pDressedInfoCell;

	EquipInfoCell *m_pSelectedInfoCell;

	vmsg::CSEquipInfo m_stEquipInfo;

};

#endif // EquipInfoPanel_h__