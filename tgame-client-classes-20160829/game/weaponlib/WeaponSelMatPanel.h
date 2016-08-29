#ifndef WeaponSelMatPanel_h__
#define WeaponSelMatPanel_h__

#include "BasePanel.h"
#include "CSWeaponMsg.pb.h"
#include "vlib/CVArray.h"
#include "BasicTypes.h"

#include "game/guide/GuideControl.h"

using namespace CVLib;
using namespace std;

class WeaponSelMatPanel : public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate, public GuideContainer
{
public:
    
	WeaponSelMatPanel();
	virtual ~WeaponSelMatPanel();

	CREATE_FUNC(WeaponSelMatPanel);

	bool init();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	void updateUI(); 

	void open();
	void close();

	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
protected:
private:
	void initUI();
	void touchEventHandler(CCObject* pSender, TouchEventType type);

	void backFun();

	void sureEvent();
	void selectEventHandle();		//ѡ�����
	bool checkIsFull();

	//void autoSelectEvent();

	unsigned int getSelectedNum();

	UIButton* m_pSureBtn;
	UIButton* m_pAutoBtn;
    UIImageView* m_pCloseBtn;

	vector<unsigned int> stAllItemType;					//��¼����item��Ӧ������
	map<unsigned int, unsigned int> stIDX2CfgID;		//����������Ӧcfg id
	vector<vmsg::CSWeapon> stMaterialWeapon;			//ǹе������

	map<UINT64_t, vmsg::CSWeaponLvUpConsume> m_stUpMaterialCopy;		//����һ�ݴ�ǰ���������ϣ����ڵ������ʱ������������

	unsigned int m_uCellCount;
	CCTableView* m_pTableView;

	UILayout* m_pMaterialDemo;
};

#endif // WeaponSelMatPanel_h__
