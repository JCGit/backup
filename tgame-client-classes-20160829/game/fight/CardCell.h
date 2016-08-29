#ifndef CardCell_h__
#define CardCell_h__

#include "BaseLayout.h"
#include "vlib/QEventDispatcher.h"

class IconCell;

class CCardCell : public BaseLayout, public CQEventDispatcher
{
public:
	enum {
		FREE_CARD,
		COST_CARD
	};

	static const string OPEN_OTHER_CARDS_EVENT;

public:
	CCardCell();
	virtual ~CCardCell();

	CREATE_FUNC(CCardCell);

	virtual bool init();

	void updateUI();

	void openCard(bool isGeted = true);		//����

	void setItemInfo(unsigned int itemID, unsigned int itemType, unsigned int itemCnt);

	void lock();
	void unLock();

	bool getLocked(){
		return m_bLocked;
	}

private:
	void handleEffComplete(CCArmature * armature, MovementEventType type, const char * name);

	ImageView* m_pCardImg;
	ImageView* m_pIconBgImg;
	IconCell* m_pCell;
	ImageView* m_pGoldImg;
	Label* m_pCostTxt;


private:
	CC_SYNTHESIZE(unsigned int, _itemID, ItemID);		//���id
	CC_SYNTHESIZE(unsigned int, _itemType, ItemType);	//����
	CC_SYNTHESIZE(unsigned int, _itemCnt, ItemCnt);		//����
	CC_PROPERTY(unsigned int, _costType, CostType);		//�������ͣ���� OR ����
	CC_SYNTHESIZE(bool, _isOpen, IsOpen);				//�Ƿ��
	CC_SYNTHESIZE(bool, _isGeted, IsGeted);				//�Ƿ�õ�����Ʒ

	bool m_bLocked;			//�Ƿ���ס

};

#endif // CardCell_h__
