#ifndef EquipLibListItem_h__
#define EquipLibListItem_h__

#include "BasicTypes.h"
#include "BaseLayout.h"
#include "EquipLibObj.h"

class CStarProgressBar;

class EquipLibListItemOne {
public:
    EquipLibListItemOne();
    ~EquipLibListItemOne() {}

    void initWithLayout(UIImageView *pRefLayout);

    void setEquipInfo(const EquipLibOneInfo& stInfo);
    void hide(); //不显示
    //bool hitTest(const CCPoint& stGlobalTouchPt);
    UIImageView* getRefLayoutImg() {
        return m_pRefLayoutImg;
    }
    void onTouch(); //点击

	const EquipLibOneInfo& getEquipLibOneInfo(){return m_stEquipInfo;}
private:

private:
    UIImageView *m_pRefLayoutImg;

    UILabel* m_pNameTxt;
    UILabel* m_pLvTxt;
    
    UIImageView* m_pCellBg;
    UIImageView* m_pEquipImg;


    CStarProgressBar* m_pStarBar;	//星级

    EquipLibOneInfo m_stEquipInfo;
};

class EquipLibListItem : public BaseLayout
{
public:
	EquipLibListItem();
	virtual ~EquipLibListItem();

	CREATE_FUNC(EquipLibListItem);

	static EquipLibListItem* create(UILayout *pRefLayout);

	virtual bool init();

	bool initWithLayout(UILayout *pRefLayout);

	void initUI();

	void setEquipInfo(const int iItemIdx, const EquipLibOneInfo& stInfo);
    void hideSecondItem();
    bool onItemTouched(const CCPoint& stGlobalTouchPt);

    CCNode* getFirstItemNode();

   const vmsg::CSEquipInfo& getEquipInfo();
private:
    EquipLibListItemOne m_stOne;
    EquipLibListItemOne m_stTwo;

};

#endif // EquipLibListItem_h__
