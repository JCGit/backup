#ifndef __CCRICHLABELTTF_H__
#define __CCRICHLABELTTF_H__

#include "cocos2d.h"
#include "ServerDateManager.h"

USING_NS_CC;

//string split information
struct split_info
{
	int					begin_pos;		//sub str begin pos
	int					end_pos;		//sub str end pos
	std::string			font_name;		//font name
	float				font_size;		//font size
	ccColor3B			color;			//font color
	bool				is_bold;		//font is bold
	bool				is_under_line;	//font with underline

	// parameters returned to handler
	//UserChatInfo::GotoPage goto_page;	//go to page
	bool				linkMakeFriend;	//show make friend page
	int					uId;			//user id
	std::string			uName;			//user name
	split_info() : begin_pos(0), end_pos(0), font_size(0.0f),
		color(ccBLACK), is_bold(false), is_under_line(false),
		uId(0),linkMakeFriend(false) {}
};

//Define the menu item clicked handler interface
class RichTTFMessageHandler
{
public:
	virtual void onReceiveMassage(unsigned int index, const split_info& info) = 0;
};

//Define the rich tabel class
class CCRichLabelTTF : public CCNode
{
public:
	CCRichLabelTTF();
	~CCRichLabelTTF();

	// create function
	static CCRichLabelTTF * create(void);

	// split string src_string, according with splitInfos[] array
	void splitString(const std::string& src_string, split_info splitInfos[], unsigned int size);

	// set all content color
	void setColor(const ccColor3B& color);

	// set one split sub-str color
	void setColor(int index, const ccColor3B& color);

	// a selector callback
	void menuCallback(CCObject* pSender);

	// set handler interface
	void setMessageHandler(RichTTFMessageHandler* handler) { m_messageHandler = handler; }

	// get content height
	float getContentHeight();

	// get content width
	float getContentWidth();

private:
	void createLabelTTF(const std::string& src_string, unsigned int index, float& x_pos);
	void onMenuItemClicked(int index);

private:
	split_info*   m_splitInfos;
	unsigned int  m_splitSize;
	std::map<CCObject*, unsigned int>  m_itemInfo;
	std::map<unsigned int, CCLabelTTF*>  m_LabelsInfo;
	RichTTFMessageHandler*  m_messageHandler;
};
#endif