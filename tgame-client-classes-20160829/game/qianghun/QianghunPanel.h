#ifndef QianghunPanel_h__
#define QianghunPanel_h__

#include "BasePanel.h"
#include "QianghunCard.h"
#include "vlib/CVArray.h"
using namespace CVLib;

class IconCell;

class CQianghunPanel : public BasePanel
{
public:
	enum {
		MAX_LEVEL = 4,
		MAX_CARD_NUM = 9,
	};

public:
	CQianghunPanel();
	virtual ~CQianghunPanel();

	CREATE_FUNC(CQianghunPanel);

	bool init();

	void updateUI();

	void updateInfoLst();

	void updateCard(unsigned int pos);

	void updateAwards(int floor);

	void updateBtn();

	void checkFlopFree();		//����´η����Ƿ����

	void preLayer();
	void nextLayer(); 
	void gotoLayer(unsigned int uLayer);
	void toLayer();

	void lock();
	void unLock();

	void reset();

	void open();
	virtual void close();

	void playShuffle();
	void showAllCard(float dt);
	void hideAllCard(float dt);
	void shuffleCard(float dt);		//ϴ��
	void dealDone(float dt);
protected:
private:
	void nextFun();
	void flopFun();			//����
	void touchEventHandler(CCObject* pSender, TouchEventType type);

	std::string getCountDesc(unsigned int count); 

private:
	UIButton* m_pCloseBtn;
	UIButton* m_pPrevBtn;
	UIButton* m_pNextBtn;
	ImageView *m_pFloorImg;

	UIPanel* m_pCardLayer;
	UIPanel* m_pInfoLayer;

	UILabel* m_pLayerNumTxt;
	UILabel* m_pNumTxt;

	CVArray<CQianghunCard*, MAX_CARD_NUM> m_stItemLst;
	CVArray<ImageView*, MAX_LEVEL> m_stLevLst;
	CVArray<IconCell*, 2> m_stCellLst;

	unsigned int m_uCurLayer;			//��ǰ����
	unsigned int m_uMaxLayer;			//������
	unsigned int m_uGotoLayer;			//��Ծ�Ĳ���
	unsigned int m_uFreeNum;			//��Ѵ���
	unsigned int m_uCurFlopNum;			//��ǰ���ƴ���
	unsigned int m_uCardIdx;

	int m_nWholeScrollDirection; 

	CCPoint m_stInitCellPos;
	CCPoint m_stMidCellPos;

	vector<CCPoint> m_stCellPos;
};

#endif // QianghunPanel_h__
