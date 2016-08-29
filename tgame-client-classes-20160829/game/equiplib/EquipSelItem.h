#ifndef EquipSelItem_h__
#define EquipSelItem_h__

#include "basic/BaseLayout.h"
#include "CSEquipItemMsg.pb.h"
#include "bag/ItemCell.h"

class CStarProgressBar;

class EquipSelItem : public BaseLayout
{
public:
	EquipSelItem();
	virtual ~EquipSelItem();

	static EquipSelItem* create();

	static EquipSelItem* create(UILayout *pRefLayout);

	// override 
	virtual bool init();

	bool initWithLayout(UILayout *pRefLayout);

	void initUI();

	virtual std::string getDescription() const;
	
    void updateEquipInfo(const vmsg::CSEquipInfo& stEquip, bool bIsWear, const int iBagIdx);
    
    const vmsg::CSEquipInfo& getEquipInfo() {return m_stEquipInfo;}
    const int getEquipBagIdx() {
        return m_iEquipBagIdx;
    }
    void setSelect(bool isSel);

private:
    UILabel *m_pEquipNameTxt;
    UILabel *m_pPartTxt;
    UILabel *m_pAttrTxt;

    UIImageView *m_pIcon;
    ItemCell *m_pEquipCell;

    UIImageView *m_pSelStatBg; //选中状态时的背景
    UIImageView *m_pSelStatIcon; //选中状态小图标
    UIImageView *m_pInUsePic; //使用中

    CStarProgressBar* m_pEquipStar;
    
    vmsg::CSEquipInfo m_stEquipInfo;
    int m_iEquipBagIdx; //default is -1
};

#endif // EquipSelItem_h__
