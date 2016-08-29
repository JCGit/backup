#ifndef SweepPanel_h__
#define SweepPanel_h__

#include "BasePanel.h"

class CSweepPanel : public BasePanel
{
public:
	CSweepPanel();
	virtual ~CSweepPanel();

	CREATE_FUNC(CSweepPanel);

	bool init();

	void updateUI();

	void open();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void setSweepNum(bool isAdd);		//����ɨ������

	void updateSweepInfo();

	UIButton* m_pCloseBtn;
	UIButton* m_pStartBtn;
	UIButton* m_pAddBtn;
	UIButton* m_pDownBtn;

	UILabel* m_pNumTxt;
	UILabel* m_pTimeTxt;
	UILabel* m_pManualTxt;
	UILabel* m_pTipTxt;

	CC_PROPERTY(unsigned int, _levelID, LevelID);		//�ؿ�id

	unsigned int m_uSweepNum;			//���õ�ɨ������
	unsigned int m_uCanSweepNum;		//����ս�Ĵ���
};

#endif // SweepPanel_h__
