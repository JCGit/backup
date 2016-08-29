#ifndef SignInControl_h__
#define SignInControl_h__

#include "vlib/base/CVSingleton.h"
#include "SignInPanel.h"

class SignInPanel;

class SignInControl 
{
public:
	SignInControl();
	~SignInControl();

	SignInPanel* getSignInPanel();
    
    void updateSignInRsp(bool bIsRsp);

    void finalizePanel();
    void closeSigninPanel();

    void checkNeedAutoOpen(); //检查是否需要自动弹出签到提醒
private:
    SignInPanel* m_pSignInPanel;
    bool m_bNeedAutoOpen; //没签到时登陆后自动弹出提醒

	void notifySignIn();
	bool isCanSignIn();
};

#define P_SIGN_IN_CTRL CVLib::CVSingleton<SignInControl>::Instance()

#endif // SignInControl_h__