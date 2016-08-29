#ifndef RecordItem_h__
#define RecordItem_h__

#include "BaseLayout.h"
#include "CSArenaMsg.pb.h"

using namespace vmsg;

class CRecordItem : public BaseLayout
{
public:
	CRecordItem();
	virtual ~CRecordItem();

	CREATE_FUNC(CRecordItem);

	bool init();

	void setInfo(const CSArenaOneRecord& stInfo);

	const CSArenaOneRecord& getInfo(){
		return m_stInfo;
	}

	void updateUI();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	UILabel* m_pRecordTxt;
	UILabel* m_pTimeTxt;
	UIButton* m_pFightBtn;

	CSArenaOneRecord m_stInfo;
};

#endif // RecordItem_h__
