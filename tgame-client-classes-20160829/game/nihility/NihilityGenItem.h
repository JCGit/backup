#ifndef NihilityGenItem_h__
#define NihilityGenItem_h__

#include "BaseLayout.h"
#include "ClientSvrMsg.pb.h"
using namespace vmsg;

class CNihilityGenItem : public BaseLayout
{
public:
	CNihilityGenItem();
	virtual ~CNihilityGenItem();

	CREATE_FUNC(CNihilityGenItem);

	static CNihilityGenItem* create(UILayout *pRefLayout);

	bool init();

	bool initWithLayout(UILayout *pRefLayout);

	void setObj(unsigned int idx, const vmsg::CSGen& info);

	const vmsg::CSGen& getObj(){
		return m_stInfo;
	}

	unsigned int getIDX(){
		return m_uIDX;
	}
	Button* getGeneBtn(){return m_pBtn;}
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	ImageView* m_pIcon;
	Label* m_pNameTxt;
	Label* m_pDescTxt;
	Label* m_pCostTxt;
	Label* m_pNumTxt;
	Button* m_pBtn;

	CSGen m_stInfo;
	unsigned int m_uIDX;
};

#endif // NihilityGenItem_h__
