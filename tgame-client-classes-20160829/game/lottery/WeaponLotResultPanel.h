#ifndef WeaponLotResultPanel_h__
#define WeaponLotResultPanel_h__

#include "basic/BasePanel.h"

#include "CSEquipItemMsg.pb.h"
namespace vmsg {
    class CSWeaponLotRsp;
}

class HBox;
class WeaponLotResultCell;

class WeaponLotResultPanel: public BasePanel
{
public:
    WeaponLotResultPanel();
    ~WeaponLotResultPanel();

    CREATE_FUNC(WeaponLotResultPanel);

    enum  {
        MAX_CELL_NUM = 10,
    };

    // override
    virtual bool init();
    virtual void open();
    virtual void close();

    void showResult(const vmsg::CSWeaponLotRsp& stRsp);
    void showSingleResult(const vmsg::CSWeaponLotRsp& stRsp);

private:
    void touchEventHandler(CCObject *pSender, TouchEventType type);
    void HandleBaoxiangEffEvent(CCArmature * armature, MovementEventType type, const char * name);
    void HandleCellEffEvent(CCArmature * armature, MovementEventType type, const char * name);
    void showNextElmt(); //展示下一个物品
    const std::string getLastCellAniName(const bool isLoopAni);
    const std::string getColorString(const int iColor);
	const std::string getWeaponColorString(const int iColor);

    void HandleAdvEffDoneEvent(CCArmature * armature, MovementEventType type, const char * name);
    void doShowElmt();

    void onFlyBallArrived();
    void resetUI();
private:
    UIButton *m_pCloseBtn; //close btn
    UIButton *m_pLotAgainBtn; 
    UIImageView* m_pBg;
    
    HBox *m_pCellHBox;

    WeaponLotResultCell* m_pSingleCell;
    
    //CCPoint m_stTipTxtInitPt;
    CCArmature* m_pBaoxiangAni; //宝箱特效
    WeaponLotResultCell* m_pCurCell;
    CCArmature* m_pCurCellAni; //cell特效
    CCArmature* m_pFlyBallAni; //飞行小球
    
    std::vector<vmsg::CSAwardElement> m_stElmtDataLst;

	unsigned int m_uLotteryType;

};

#endif // WeaponLotResultPanel_h__
