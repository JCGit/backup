#ifndef SecretChatPanel_h__
#define SecretChatPanel_h__

#include "BasePanel.h"
#include "basic/BasicTypes.h"
#include "CSChatMsg.pb.h"

class PlayerChatItem;
class MsgBubbleLayout;

class SecretChatPanel: public BasePanel
{
public:
	SecretChatPanel();
	virtual ~SecretChatPanel();
	
	static SecretChatPanel* create();

	// override start

	virtual bool init();

	void open();

	void close();

	// override end

	void updateUI();

	void updateChatRecords();

	void pushChatRecord(UIN_t uin, const vmsg::CSChatInfo &stChatInfo);

	UIN_t getCurChatPlayerUin();

	MsgBubbleLayout* getBubbleLayout();

	void cacheBubbles();

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

private:
	UIButton *m_pCloseBtn;

	UIListView *m_pPlayerListView;

	UIButton *m_pExpressionBtn;

	UITextField *m_pChatInputTxt;

	UIButton *m_pSendBtn;

	UIScrollView *m_pChatRecordsScrollView;

	UILayout *m_pExpressionLayout;

	int m_nPlayerSelectedIdx;

	std::vector<PlayerChatItem*> m_stPlayerItems;

	std::vector<MsgBubbleLayout*> m_stMsgBubbles;

	UIN_t m_uCurChatPlayerUin;

	UILayout *m_pInnerLayout;  

};

#endif // SecretChatPanel_h__