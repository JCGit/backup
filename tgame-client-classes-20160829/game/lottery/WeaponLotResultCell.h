#ifndef WeaponLotResultCell_h__
#define WeaponLotResultCell_h__

#include "basic/BaseLayout.h"

class IconCell;

//Ì½ÃØ½á¹ûcell
class WeaponLotResultCell : public BaseLayout
{
public:
    WeaponLotResultCell(){
        m_pCellBg = NULL;
        m_pNameTxt = NULL;
        m_pIconCell = NULL;
    }

    ~WeaponLotResultCell(){}


    static WeaponLotResultCell* create();
    virtual bool init();

	void hideName();

    void setInfo(const int iType, const int iCnt, const int iCfgID);
    UIImageView* getCellBg() {
        return m_pCellBg;
    }
    IconCell* getCell() {
        return m_pIconCell;
    }
private:
    void touchEventHandler(CCObject *pSender, TouchEventType type);
private:
    UIImageView* m_pCellBg;
    UILabel* m_pNameTxt;
    IconCell* m_pIconCell;

};

#endif // WeaponLotResultCell_h__
