#ifndef LoginScreenView_h__
#define LoginScreenView_h__

#include "basic/BaseLayout.h"
class CLoadingLayer;

class LoginScreenView: public BaseLayout
{
public:
	LoginScreenView();
	virtual ~LoginScreenView();

	static LoginScreenView* create();

	// override start

	virtual bool init();

	virtual void onEnter();
    void closeLoading();
	// override end

	void initUI();

	void updateUI();

    void onLoginInfoDone(const string& strLoginInfo); //得到logininfo

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void handleBtnEffComplete(CCArmature * armature, MovementEventType type, const char * name);

    void scheduleLoginAfterConnected(float dt);
    void scheduleLoadRoleRes(float dt);

    void doLoginIn();
    void releaseRoleRes();

    void delayGoSdk();

	void doLoginSDK(int iRetCode);

    //ios appstore 特殊处理，QQ授权登陆或者游客登陆
    void doAppleQQAuthLogin();
    void doAppleGuestLogin();

private:
	UIImageView *m_pBgImg;
    UIImageView* m_pGameLogoImg; //game_logo_img

	UIButton *m_pSettingBtn;

	UIButton *m_pNoticeBtn;

	UILabel *m_pSvrNameValTxt;

	UIButton *m_pSvrListBtn;

    UIButton *m_pLoginBtn;
    UILabel* m_pVerTxt;

	Button* m_pFaceBookLoginBtn;
	Button* m_pGuestLoginBtn;

	UITextField *m_pOpenIDInputTxt;
    
    CLoadingLayer* m_pLoadingLayer;
    static bool m_bIsNeedAutoLoginSDK;
};

#endif // LoginScreenView_h__