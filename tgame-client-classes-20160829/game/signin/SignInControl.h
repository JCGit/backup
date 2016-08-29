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

    void checkNeedAutoOpen(); //����Ƿ���Ҫ�Զ�����ǩ������
private:
    SignInPanel* m_pSignInPanel;
    bool m_bNeedAutoOpen; //ûǩ��ʱ��½���Զ���������

	void notifySignIn();
	bool isCanSignIn();
};

#define P_SIGN_IN_CTRL CVLib::CVSingleton<SignInControl>::Instance()

#endif // SignInControl_h__