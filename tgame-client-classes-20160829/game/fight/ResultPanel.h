#ifndef ResultPanel_h__
#define ResultPanel_h__

#include "basic/BasePanel.h"

using namespace std;

class CResultPanel : public BasePanel
{
public:
    enum {
        TAG_WIN_EFF = 86, //win特效
        TAG_STAR_EFF = 77, //STAR特效

        TAG_IMG_FRAG = 43, //碎片图标
    };
	CResultPanel();
	virtual ~CResultPanel();

	CREATE_FUNC(CResultPanel);

	virtual bool init();

	void updateUI();

	void winAction();		//胜利图片的动画
	void rollNumber();		//数字滚动
	void txtAction();		//数字文本划出动画
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
	ImageView* m_pStar0;			//第一颗星
	ImageView* m_pStar1;			//第二颗星
	ImageView* m_pStar2;			//第三颗星

	CCArray *m_pUICellArray;
	CCArray *m_pCellArray;

	unsigned int m_uScore;
	unsigned int m_uMaxKill;
	float m_fInitX;
	unsigned int m_uCellTotalW;

    int m_iShowTotalStarNum; //需要显示几个星星
    int m_iNowShowStarIdx; //当前显示第几个星星
};

#endif // ResultPanel_h__
