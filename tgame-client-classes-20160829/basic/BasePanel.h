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
	
	virtual void refresh(); // 刷新

	bool isOpen();
	virtual void open();			// add panel to uilayer of the scene
	virtual void close();			// remove panel from uilayer of the scene
    virtual void doGlobalBackClose();
    bool isInited() { return m_bIsInited; }

	virtual bool isMeetAutoOpen(); // 是否满足登录时打开

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

    bool isPanelOnTop(); //当前面板是否在最上层

    bool isShowMoneyTopBar(); //顶部显示钱
    void setShowMoneyTopBar(bool bIsShow);

    void openInSeq(BasePanel* pBackPanel); //按顺序打开面板.本面板关闭时，open pBackPanel
protected:
	void showLoading(CCObject* target, SEL_CallFunc callbackFun);
	void onClose();
    void closeOtherPanel(); //打开本面板时，关闭其他已经打开的面板
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

protected:
	bool m_bIsOpen;

	unsigned int m_uPopType;		//弹出方式
    bool m_bIsInited; //是否已经初始化了
	bool m_bIsInLoading; 

	bool m_bIsSwallowsTouches;	//原本UI层是否吞噬事件

	bool m_bClickOutToClose;

	bool m_bUseBackForClose;

	bool m_bOpenWithBgImg;

	BasePanel *m_pLinkedPanel;

	unsigned int m_uSoundType;		//面板音效类型

    bool m_bShowMoneyTopBar; //面板顶部公用的钱信息

private:
    static CLoadingLayer* m_pLoadingLayer; //加载提示
};

#endif // BasePanel_h__
