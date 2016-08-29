#ifndef WeaponArrInfoPanel_h__
#define WeaponArrInfoPanel_h__

#include "BasePanel.h"
#include "CSWeaponMsg.pb.h"
#include "game/guide/GuideContainer.h"

class CStarProgressBar;

using namespace vmsg;

class CWeaponArrInfoPanel : public BasePanel , public GuideContainer
{
public:
	CWeaponArrInfoPanel();
	virtual ~CWeaponArrInfoPanel();

	CREATE_FUNC(CWeaponArrInfoPanel);

	bool init();

	void updateUI();

	void open();
	void close();

	void setInfo(const CSWeapon& stInfo, int iIdx);
	
	void doClickDone();
	void doUnloadDone();
	virtual CCNode* getGuideNodeByName(const std::string &name);
protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	ImageView* m_pIconImg;
	ImageView* m_pUseImg;
	ImageView* m_pChangeImg;
	Button* m_pCloseBtn;
	Button* m_pChangeBtn;
	Button* m_pUnloadBtn;

	Label* m_pNameTxt;
	Label* m_pLevTxt;
	Label* m_pAtkTxt;
	Label* m_pAtkSpeedTxt;
	Label* m_pHitTxt;
	Label* m_pBulletTxt;

	CStarProgressBar* m_pStarBar;	//�Ǽ�

	CSWeapon m_stInfo;
	int m_iIDx;				//������Ϊ-1,������ʾ����������λ�±�
};

#endif // WeaponArrInfoPanel_h__
