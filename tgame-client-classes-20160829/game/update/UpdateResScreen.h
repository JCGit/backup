#ifndef UpdateResScreen_h__
#define UpdateResScreen_h__

#include "BaseLayout.h"
#include "UpdateMgrImp.h"

class UpdateResScreen: public BaseLayout
{
public:
	bool init();
    static UpdateResScreen* create() {
        UpdateResScreen *pItemCell = new UpdateResScreen();

        if(pItemCell != NULL && pItemCell->init()) {
            pItemCell->autorelease();
        } else {
            CC_SAFE_DELETE(pItemCell);
        }

        return pItemCell;
    }

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	virtual void onEnter();

	virtual void onExit();

	void setPercentage(float fPercentage);

	void setNoticeWords(const char *pszNoticeWords);
private:
    void checkUpdate(float dt);
    void onClickDialogRestartBtn();
    void onClickDialogEnterBtn();

    void queryDirConfig(); //����dir����ȡ���µ�ַ
    void onQueryDirConfigRsp(const std::string& strRspData, const int iRetCode);
    void doDirConfigDone(); //�ɹ���ȡdir���ã����Կ�ʼ����
    void reloadAllCfg();
private:
	UpdateResScreen();
	~UpdateResScreen();

	bool m_bIsAllReady;

	float m_fPercentage;

	UIImageView *m_pBgImg;
	UIImageView* m_pLoadIcon;

	ImageView* m_pLoadingBg;
	LoadingBar* m_pLoadBar;
    UILabel* m_pLoadTxt;
    UILabel* m_pTipTxt;

    UpdateMgrImp m_stUpdateMgr;

	CCPoint m_stInitPos;

};

#endif // UpdateResScreen_h__
