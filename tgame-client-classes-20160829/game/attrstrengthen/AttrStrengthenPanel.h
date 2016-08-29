#ifndef AttrStrengthenPanel_h__
#define AttrStrengthenPanel_h__

#include "basic/BasePanel.h"

class AttrStrengthenPanel: public BasePanel
{
public:
	AttrStrengthenPanel();
	virtual ~AttrStrengthenPanel();
	
	static AttrStrengthenPanel* create();

	// override start

	virtual bool init();

	// override end

	void setType(unsigned int uAttrType, unsigned int uMagicType);

	void updateUI();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIImageView *m_pAttrIconImg;

	UILabel *m_pAttrNameTxt;

	UILabel *m_pAttrLevelValTxt;

	UILabel *m_pAttrAddValTxt;

	UIImageView *m_pBgCostCoinImg;

	UIImageView *m_pBgCostItemImg;

	UIButton *m_pStrengthenBtn;

	unsigned int m_uAttrType;

	unsigned int m_uMagicType;

};

#endif // AttrStrengthenPanel_h__