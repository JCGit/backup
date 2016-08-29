#ifndef CardDrawPanel_h__
#define CardDrawPanel_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "basic/BasePanel.h"
#include "CardCell.h"
#include "CardCfgMaster.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;
using namespace std;

const int DEFAULT_TIMES = 1;	
const int DOWN_TIME = 6;		//����ʱ

class CCardDrawPanel : public BasePanel
{
public:
	CCardDrawPanel();
	virtual ~CCardDrawPanel();

	CREATE_FUNC(CCardDrawPanel);

	virtual bool init();
	void open();
	virtual void close();

	void updateUI();

	void updateCurCard();

protected:
    void lockCards();
    void unLockCards();
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void handleOpenOtherEvent(const string& stEvent, CCObject* pstEventObj);

	void openCard(CCardCell* pCell);
	void openCostCard();
	void openOtherFreeCards();		//����ʣ�����ѿ�
	void openOtherCostCards();		//����ʣ��Ļ��ѿ�

	vector<CardElement> getOtherCardsInfo(bool isFree = true);	//�������л�ȡ�������Ƶ���Ϣ

	void downTime(float dt);				//����ʱ

	UILabel* timeTxt;
	UILabel* timeValueTxt;
	UIButton* sureBtn;
    UIImageView* m_pPanelBg;

	CCardCell* curCard;
	CCardCell* costCurCard;

	CCArray* pCards;		//����

	bool b_isOpenFreeCard;	//�Ƿ����ѿ�

	int totalTime;			//�ܵĵ���ʱ
	int freeTimes;			//��Ѵ���
	int costTimes;			//���ѵĴ���
};

#endif // CardDrawPanel_h__
