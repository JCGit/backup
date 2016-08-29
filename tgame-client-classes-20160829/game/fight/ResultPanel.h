#ifndef ResultPanel_h__
#define ResultPanel_h__

#include "basic/BasePanel.h"

using namespace std;

class CResultPanel : public BasePanel
{
public:
    enum {
        TAG_WIN_EFF = 86, //win��Ч
        TAG_STAR_EFF = 77, //STAR��Ч

        TAG_IMG_FRAG = 43, //��Ƭͼ��
    };
	CResultPanel();
	virtual ~CResultPanel();

	CREATE_FUNC(CResultPanel);

	virtual bool init();

	void updateUI();

	void winAction();		//ʤ��ͼƬ�Ķ���
	void rollNumber();		//���ֹ���
	void txtAction();		//�����ı���������
	void updateCells();

	void resetAction();

	virtual void open();
	virtual void close();
protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void End();

	void shake();
    void showNextStarEff();
    void onStarEffDone(CCArmature* armature, MovementEventType type, const char * name);
    void onWinEffDone(CCArmature* armature, MovementEventType type, const char * name);

    UIButton* m_pSureBtn;

    UILayout* m_pInfoLayout;
    UILabel* m_pScoreTxt;
    UILabel* m_pExpTxt;
    UILabel* m_pTimeTxt;
    UILabel* m_pCoinTxt;

	ImageView* m_pDropLabel;

	ImageView* m_pWinBg;
	ImageView* m_pStar0;			//��һ����
	ImageView* m_pStar1;			//�ڶ�����
	ImageView* m_pStar2;			//��������

	CCArray *m_pUICellArray;
	CCArray *m_pCellArray;

	unsigned int m_uScore;
	unsigned int m_uMaxKill;
	float m_fInitX;
	unsigned int m_uCellTotalW;

    int m_iShowTotalStarNum; //��Ҫ��ʾ��������
    int m_iNowShowStarIdx; //��ǰ��ʾ�ڼ�������
};

#endif // ResultPanel_h__
