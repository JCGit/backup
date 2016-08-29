#ifndef FragListItem_h__
#define FragListItem_h__

#include "basic/BaseLayout.h"

class IconCell;

class FragListItem: public BaseLayout
{
public:
    enum 
    {
        WEAPON_CELL_TYPE_NUM = 2, 
        EQUIP_CELL_TYPE_NUM = 4, 

        ICON_CELL_TAG = 88,
    };
	FragListItem();
	virtual ~FragListItem();
	
	static FragListItem* create();
    virtual bool init();


    void setCellTypeNum(const int iNum);

    void onTouched(const CCPoint& stGlobalPt);
    void setInfoByIdx(const int iLogicIdx, const int iBagIdx);
    void resetCellByIdx(const int iLogicIdx);
    void setNotOwnTip(bool bIsShow);

    void setInfoByCfgID(const int iLogicIdx, const int iCfgID); //我没有的
private:

private:
    int m_iShowCellNum; //当前显示几个Cell

    UIImageView* m_stEquipCell[EQUIP_CELL_TYPE_NUM];

    IconCell* m_stIconCellLst[EQUIP_CELL_TYPE_NUM];

    int m_iCellOriX; //显示枪械时的x坐标偏移

    bool m_bIsMyFrag; //是否是我拥有的
};

#endif // FragListItem_h__