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

    void setInfoByCfgID(const int iLogicIdx, const int iCfgID); //��û�е�
private:

private:
    int m_iShowCellNum; //��ǰ��ʾ����Cell

    UIImageView* m_stEquipCell[EQUIP_CELL_TYPE_NUM];

    IconCell* m_stIconCellLst[EQUIP_CELL_TYPE_NUM];

    int m_iCellOriX; //��ʾǹеʱ��x����ƫ��

    bool m_bIsMyFrag; //�Ƿ�����ӵ�е�
};

#endif // FragListItem_h__