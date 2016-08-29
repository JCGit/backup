#ifndef VIPPanel_h__
#define VIPPanel_h__

#include "basic/BasePanel.h"
#include "VIPDetailLayout.h"

class ImageNumber;

class VIPPanel: public BasePanel
{
public:
	VIPPanel();
	virtual ~VIPPanel();
	
	static VIPPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

	void updateUI();

	void showVIPLv(const unsigned int uVIPLv);

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);
    
private:
    
	ImageNumber *m_pCurVIPLvImgNum;

	UILoadingBar *m_pVIPProgress;

	UILabel *m_pVIPProgressTxt;

	UILabel *m_pChargeTxt;

	UILabel *m_pChargeValTxt;

	UIImageView *m_pIconGoldImg;

	UILabel *m_pVIPLevelupTxt;
    
	ImageNumber *m_pNextVIPLvImgNum;

	UIButton *m_pChargeBtn;
    
	UIButton *m_pFlipLeftBtn;

	UIButton *m_pFlipRightBtn;

    unsigned int m_uMaxVIPLv;

	unsigned int m_uShowVIPLv;
    
    VIPDetailLayout *m_pDetailLayout;
};

#endif // VIPPanel_h__