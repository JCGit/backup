#ifndef LoginControl_h__
#define LoginControl_h__

#include "LoginProxy.h"

#include "LoginScreenView.h"
#include "ChooseSvrPanel.h"
#include "ChooseRolePanel.h"
#include "CreateRolePanel.h"
#include "TencentLoginPanel.h"
#include "NoticePanel.h"
#include "GameExitPanel.h"

class LoginControl
{
public:
	LoginControl();
	~LoginControl();
	
	LoginProxy* getProxy();

	LoginScreenView* getLoginScreenView();

	ChooseSvrPanel* getChooseSvrPanel();

	ChooseRolePanel* getChooseRolePanel();

	CreateRolePanel* getCreateRolePanel();
    
	TencentLoginPanel* getTencentLoginPanel();

	NoticePanel* getNoticePanel();

	GameExitPanel* getGameExitPanel();

    void onLoginGetData(); //��½�ɹ�����ȡ����

    void finalize();

	void sendSetSettingParamRqst(std::string str);

    void forceCloseLoginPanels(); //�رս�ɫѡ��ȸ���½��ص����
    void doAfterSDKInited();
private:

private:
	LoginProxy *m_pLoginProxy;

	LoginScreenView *m_pLoginScreenView;

	ChooseSvrPanel *m_pChooseSvrPanel;

	ChooseRolePanel *m_pChooseRolePanel;

	CreateRolePanel *m_pCreateRolePanel;
    
	TencentLoginPanel *m_pTencentLoginPanel;

	NoticePanel *m_pNoticePanel;

	GameExitPanel* m_pGameExitPanel;

};

#define P_LOGIN_CTRL CVLib::CVSingleton<LoginControl>::Instance()

#endif // LoginControl_h__
