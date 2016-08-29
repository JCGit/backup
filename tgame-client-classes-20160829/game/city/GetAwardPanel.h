#ifndef GetAwardPanel_h__
#define GetAwardPanel_h__

#include "basic/BasePanel.h"

typedef struct {
	string desc;
	string icon;
	bool plisticon;
	unsigned int cnt;
} AwardInfo_t;

class ImageNumber;

class GetAwardPanel: public BasePanel
{
public:
	GetAwardPanel();
	virtual ~GetAwardPanel();

	static GetAwardPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void close();

	virtual bool isMeetAutoOpen();

	// override end

	unsigned int getAwardCnt();

	void popAward();

	void claimed(); // 已领取

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	UILabel *m_pAwardDescTxt;

	UIImageView *m_pAwardIconImg;

	ImageNumber *m_pAwardCntImgTxt;

	UIButton *m_pGetBtn;

	vector<AwardInfo_t> m_stAwardInfos;

	bool m_bIsGetAwardBeforeClose; // 关闭之前是否发出领奖

};

#endif // GetAwardPanel_h__