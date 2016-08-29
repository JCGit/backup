#ifndef WeaponSelItem_h__
#define WeaponSelItem_h__

#include "CSWeaponMsg.pb.h"
#include "firearms/StarProgressBar.h"
#include "BasicTypes.h"

using namespace gui;
using namespace std;

class WeaponSelItem : public BaseLayout
{
public:
    WeaponSelItem();
    virtual ~WeaponSelItem();

    CREATE_FUNC(WeaponSelItem);

    static WeaponSelItem* create(UILayout *pRefLayout);

    virtual bool init();

    bool initWithLayout(UILayout *pRefLayout);

    void updateUI();

    void setInfo(const vmsg::CSWeapon& stInfo);
    const vmsg::CSWeapon& getInfo(){
		return m_stInfo;
	}

protected:
private:
    void initUI();
    
    vmsg::CSWeapon m_stInfo;

	ImageView* m_pCellBg;
	ImageView* m_pIconImg;
	Label* m_pNameTxt;
	Label* m_pLvTxt;
    
    CStarProgressBar* m_pStarBar;	//ÐÇ¼¶
};

#endif // WeaponSelItem_h__
