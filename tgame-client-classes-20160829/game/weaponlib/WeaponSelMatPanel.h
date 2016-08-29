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
	void selectEventHandle();		//选择材料
	bool checkIsFull();

	//void autoSelectEvent();

	unsigned int getSelectedNum();

	UIButton* m_pSureBtn;
	UIButton* m_pAutoBtn;
    UIImageView* m_pCloseBtn;

	vector<unsigned int> stAllItemType;					//记录所有item对应的类型
	map<unsigned int, unsigned int> stIDX2CfgID;		//材料索引对应cfg id
	vector<vmsg::CSWeapon> stMaterialWeapon;			//枪械做材料

	map<UINT64_t, vmsg::CSWeaponLvUpConsume> m_stUpMaterialCopy;		//拷贝一份打开前的升级材料，用于点击返回时重置升级材料

	unsigned int m_uCellCount;
	CCTableView* m_pTableView;

	UILayout* m_pMaterialDemo;
};

#endif // WeaponSelMatPanel_h__
