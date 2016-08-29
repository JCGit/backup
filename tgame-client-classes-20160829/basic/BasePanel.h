#ifndef BasePanel_h__
#define BasePanel_h__

#include "BaseLayout.h"

using namespace std;
class CLoadingLayer;

typedef enum {
	POP_LEFT,
	POP_CENTER,
	POP_RIGHT
} PopType;

class BasePanel: public BaseLayout
{
public:
	BasePanel();
	virtual ~BasePanel();

	// override start
	virtual bool init();		
	virtual std::string getDescription() const;

	virtual void onEnterTransitionDidFinish();

	virtual void onExit();

	virtual void initLayoutFromJsonFile(const char *pszFileName);
	// override end
	
	virtual void refresh(); // ˢ��

	bool isOpen();
	virtual void open();			// add panel to uilayer of the scene
	virtual void close();			// remove panel from uilayer of the scene
    virtual void doGlobalBackClose();
    bool isInited() { return m_bIsInited; }

	virtual bool isMeetAutoOpen(); // �Ƿ������¼ʱ��

	bool getIsInLoading() const;

	void setPopType(unsigned int uPopType);

	bool isClickOutToClose();

	void setClickOutToClose(bool bClickOutToClose);

	bool isUseBackForClose();

	void setUseBackForClose(bool bUseBackForClose);

	bool isOpenWithBgImg();

	void setOpenWithBgImg(bool bOpenWithBgImg);

	BasePanel* getLinkedPanel();

	void setLinkedPanel(BasePanel *pLinkedPanel);

    void showLoading();
    void closeLoading();

    bool isPanelOnTop(); //��ǰ����Ƿ������ϲ�

    bool isShowMoneyTopBar(); //������ʾǮ
    void setShowMoneyTopBar(bool bIsShow);

    void openInSeq(BasePanel* pBackPanel); //��˳������.�����ر�ʱ��open pBackPanel
protected:
	void showLoading(CCObject* target, SEL_CallFunc callbackFun);
	void onClose();
    void closeOtherPanel(); //�򿪱����ʱ���ر������Ѿ��򿪵����
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

protected:
	bool m_bIsOpen;

	unsigned int m_uPopType;		//������ʽ
    bool m_bIsInited; //�Ƿ��Ѿ���ʼ����
	bool m_bIsInLoading; 

	bool m_bIsSwallowsTouches;	//ԭ��UI���Ƿ������¼�

	bool m_bClickOutToClose;

	bool m_bUseBackForClose;

	bool m_bOpenWithBgImg;

	BasePanel *m_pLinkedPanel;

	unsigned int m_uSoundType;		//�����Ч����

    bool m_bShowMoneyTopBar; //��嶥�����õ�Ǯ��Ϣ

private:
    static CLoadingLayer* m_pLoadingLayer; //������ʾ
};

#endif // BasePanel_h__
