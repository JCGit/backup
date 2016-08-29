#ifndef WeaponLibPanelNew_h__
#define WeaponLibPanelNew_h__

#include "BasePanel.h"
#include "BasicTypes.h"

#include "game/guide/GuideControl.h"

class CWeaponTypeItem;

class CWeaponLibPanelNew : public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate, public GuideContainer
{
public:
    enum 
    {
        INIT_WEAPON_NUM = 20,
    };
	CWeaponLibPanelNew();
	virtual ~CWeaponLibPanelNew();

	CREATE_FUNC(CWeaponLibPanelNew);

    bool init();

    virtual void open();
    virtual void close();
    
	virtual CCNode* getGuideNodeByName(const std::string &name);

    void updateUI();

	void updateWeaponList();
        
	void updateWeaponByChange();			//��ǹ��ĸ���

	int getWeaponInBagInfoIdxByGUID(UINT64_t weaponGUID);

	void setTypeIdx(unsigned int idx);
	unsigned int getTypeIdx();

	unsigned int getWeaponIdx();

	void clear();

	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
private:
	void initUI();
	void touchEventHandler(CCObject* pSender, TouchEventType type);
	void typeListViewEventHandler(CCObject *pSender, ListViewEventType type);
        
    UIListView* m_pBtnList;


	CCTableView* m_pTableView;
	UILayout* m_pWeaponDemo; //Ϊ�˷�ֹ�ظ�����json
	unsigned int m_uCellCount;
    
private:
	int m_iSelectTypeIdx;			//��ǰѡ���ǹе�����±�
	int m_uSelectWeapon;		//��ǰѡ���ǹе

	int m_uTypeSize;			//ǹе����������

	int m_uPlayerWeaponIdx;		//�������ǹе������

	unsigned int m_uWeaponTotalSize;	//�����͵�����ǹе�����������õ��ܺ� 
	unsigned int m_uWeaponInBag;		//�����и�����ǹе������
    
	CCArray* m_pTabBtnLst;
	CCTableViewCell* m_pSelectCell;
	CWeaponTypeItem* m_pTypeSelectItem;

	UIButton *m_pBackBtn;

};

#endif // WeaponLibPanelNew_h__
