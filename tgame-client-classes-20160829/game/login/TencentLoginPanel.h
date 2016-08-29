#ifndef TencentLoginPanel_h__
#define TencentLoginPanel_h__

#include "basic/BasePanel.h"

class TencentLoginPanel: public BasePanel
{
public:
	TencentLoginPanel();
	virtual ~TencentLoginPanel();

	static TencentLoginPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end
    bool isSelQQ(){
        return m_bIsSelQQ;
    }
    bool isSelWX(){
        return m_bIsSelWX;
    }
private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIImageView *m_pBgImg;

	UIButton *m_pWeiXinLoginBtn;

	UIButton *m_pQQLoginBtn;

    bool m_bIsSelQQ;
    bool m_bIsSelWX;
};

#endif // TencentLoginPanel_h__