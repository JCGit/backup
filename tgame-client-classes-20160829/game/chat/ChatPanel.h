#ifndef ChatPanel_h__
#define ChatPanel_h__

#include "basic/BasePanel.h"
#include "ext/ExRichText.h"

class ChatPanel: public BasePanel
{
public:
	ChatPanel();
	virtual ~ChatPanel();
	
	static ChatPanel* create();

	// override start

	virtual bool init();

	void open();

	void close();

	// override end

	void updateUI();

	std::string msg_to_rich_str(const std::string &nick, const std::string &msg);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	void elmtTouchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	UIListView *m_pChannelListView;

	UIButton *m_pExpressionBtn;

	UITextField *m_pChatInputTxt;

	UIButton *m_pSendBtn;

	UIScrollView *m_pChatRecordsScrollView;

	ExRichText *m_pChatRecordsTxt;

	UILayout *m_pExpressionLayout;

	int m_nChannelSelectedIdx;

	unsigned int m_uChannelType;

};

#endif // ChatPanel_h__