#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CustomPage.h"
#include "ContentBase.h"
#include "StateMachine.h"
#include "MainFrame.h"

class MessageBoxPage 
	: public CustomPage 
	, public State<MainFrame>
	, public Singleton<MessageBoxPage>
{
public:
	MessageBoxPage(void);
	~MessageBoxPage(void);

	virtual PAGE_TYPE getPageType() {return PT_CONTENT;}

	CREATE_FUNC(MessageBoxPage);
	
	//this will execute when the state is entered
	virtual void Enter(MainFrame*);
	//this is the states normal update function
	virtual void Execute(MainFrame*);
	//this will execute when the state is exited. (My word, isn't
	//life full of surprises... ;o))
	virtual void Exit(MainFrame*);

	virtual void load(void);

	void showMsg(std::string msgString);
	void showMsgByErrCode(unsigned int errId);

	static void Msg_Box(std::string msgString);
	static void Msg_Box_Lan(std::string msgString);	
private:

	
};

#define  MSG_BOX(msg) MessageBoxPage::Get()->showMsg(msg);
#define  MSG_BOX_LAN(msg) MSG_BOX(Language::Get()->getString(msg))