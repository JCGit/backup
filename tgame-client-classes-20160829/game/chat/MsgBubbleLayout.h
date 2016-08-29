#ifndef MsgBubbleLayout_h__
#define MsgBubbleLayout_h__

#include "BaseLayout.h"
#include "ext/ExRichText.h"
#include "CSChatMsg.pb.h"

class MsgBubbleLayout: public BaseLayout
{
public:
	MsgBubbleLayout();
	virtual ~MsgBubbleLayout();
	
	static MsgBubbleLayout* create(UILayout *pRefLayout);

	// override start

	virtual bool init();

	// override end

	bool initWithLayout(UILayout *pRefLayout);

	std::string msg_to_rich_str(const std::string &msg);

	void setChatInfo(const vmsg::CSChatInfo &stChatInfo);

private:
	UIImageView *m_pBgChatMsgImg;

	ExRichText *m_pChatMsgTxt;

};

#endif // MsgBubbleLayout_h__