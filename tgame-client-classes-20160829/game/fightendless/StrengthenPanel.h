#ifndef StrengthenPanel_h__
#define StrengthenPanel_h__

#include "BasePanel.h"
#include "CSInfiniteMsg.pb.h"

class CStrengthenPanel : public BasePanel
{
public:
	CStrengthenPanel();
	virtual ~CStrengthenPanel();

	CREATE_FUNC(CStrengthenPanel);

	bool init();

	void updateUI();

	void updateItemByIdx(unsigned int index, const vmsg::CSGen& info);

	void updateRandDesc(const vmsg::CSGen& info);

	virtual void open();
	virtual void close();
protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	UIButton* m_pCloseBtn;
	UIButton* m_pEnterBtn;
	UILabel* m_pDescTxt;
	UIPanel* m_pLayer;
	ListView* m_pList;
};

#endif // StrengthenPanel_h__
