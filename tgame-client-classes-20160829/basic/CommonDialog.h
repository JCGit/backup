#ifndef __COMMON_DIALOG_H__
#define __COMMON_DIALOG_H__

#include "basic/BasePanel.h"
#include "vlib/base/CVSingleton.h"
#include "game/guide/GuideControl.h"
using namespace CVLib;
using namespace std;

typedef void (CCObject::*SEL_BtnEvent)();
#define dialogbtneventselector(_SELECTOR) (SEL_BtnEvent)(&_SELECTOR)

class CommonDialog: public BasePanel, public GuideContainer
{
public:
	CommonDialog();
	~CommonDialog();

	// override
	virtual bool init();
	virtual void close();

	virtual std::string getDescription() const;

	virtual CCNode* getGuideNodeByName(const std::string &name);

    void show(const string& strDescTxt, CCObject *target, 
        const string& strYesBtnTxt, const string& strNoBtnTxt = "",
		SEL_BtnEvent yesEvent = NULL, bool bRet2Login=false);
    void show(const string& strDescTxt); //��򵥵���ʾһ�����֣�һ��ȷ����ť


	void reset();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);
private:
	UIButton    *m_pYesBtn;
	UIButton    *m_pNoBtn;
	UIButton	*m_pCloseBtn;
    UILabel     *m_pDescTxt;


    CCObject*    _btnEventListener;
    SEL_BtnEvent _btnYesEventSelector;

    bool m_bIsInit;
	unsigned int m_uBtnNum;			//��ť��

	float m_fInitPosX;
	bool	m_bRet2Login;	// �ر�ʱ�Ƿ񷵻ص���½����
};


#define COMMON_DIALOG CVLib::CVSingleton<CommonDialog>::Instance()

#endif // __COMMON_DIALOG_H__
