#ifndef WeaponSelMatItem_h__
#define WeaponSelMatItem_h__

#include "BaseLayout.h"
#include "firearms/StarProgressBar.h"
#include "CSWeaponMsg.pb.h"

using namespace std;
using namespace vmsg;
class CStarProgressBar;

class WeaponSelMatItem : public BaseLayout
{
public:
	WeaponSelMatItem();
	virtual ~WeaponSelMatItem();

	CREATE_FUNC(WeaponSelMatItem);

	static WeaponSelMatItem* create(UILayout *pRefLayout);

	virtual bool init();

	bool initWithLayout(UILayout *pRefLayout);

	void setType(unsigned int var);

	unsigned int getType(){
		return m_uType;
	}

	void setItemCfgID(unsigned int var);

	unsigned int getItemCfgID(){
		return m_uItemCfgID;
	}

	void setWeaponInfo(const CSWeapon& stInfo);

	const CSWeapon& getWeaponInfo(){
		return m_stWeaponInfo;
	}

	void updateMaterial();
	void updateWeapon();

	void setSelected(bool var);
	bool getSelected();

	void setIsEnable(bool var);
	bool getIsEnable();
protected:
private:
	unsigned int m_uType; //当前显示的是枪械还是道具
	unsigned int m_uItemCfgID;
	CSWeapon m_stWeaponInfo;
private:
	void initUI();
    
	UIImageView* m_pCellBg;	//资源背景
	UILabel* m_pNameTxt;		//名
	UILabel* m_pExpTxt;			//经验
    UICheckBox* m_pSelectBtn; //选中框

    UIImageView* m_pCellIcon;

	CStarProgressBar* m_pStarBar;	//星级

    bool _selected;
    bool _isEnable;
};

#endif // WeaponSelMatItem_h__
