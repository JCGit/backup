#ifndef EquipSelMatItem_h__
#define EquipSelMatItem_h__

#include "basic/BaseLayout.h"
#include "CSEquipItemMsg.pb.h"
#include "bag/ItemCell.h"
#include "EquipLibObj.h"

class CStarProgressBar;

class EquipSelMatItem : public BaseLayout
{
public:
	EquipSelMatItem();
	virtual ~EquipSelMatItem();

	static EquipSelMatItem* create();

	static EquipSelMatItem* create(UILayout *pRefLayout);

	// override 
	virtual bool init();

	bool initWithLayout(UILayout *pRefLayout);

	void initUI();

	virtual std::string getDescription() const;
	
    void setInfo(const EquipSelMatOneInfo& info, const int iListIdx);
    const EquipSelMatOneInfo& getEquipInfo() {return m_stMatInfo;}

    void setSelect(bool isSel);
    bool isSelect();
    int getListIdx() {
        return m_iListIdx;
    }
private:
    void listViewEventHandler(CCObject *pSender, TouchEventType type);
private:
    UILabel *m_pEquipNameTxt;

    UILabel *m_pExpTxt;

    UIImageView *m_pIcon;
    ItemCell *m_pEquipCell;

    UIImageView *m_pSelStatBg; //选中状态时的背景
    UIImageView *m_pSelStatIcon; //选中状态小图标

    CStarProgressBar* m_pEquipStar;

    
    EquipSelMatOneInfo m_stMatInfo;
    int m_iListIdx;
};

#endif // EquipSelMatItem_h__
