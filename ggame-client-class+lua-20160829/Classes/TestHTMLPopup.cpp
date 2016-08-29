
#include "TestHTMLPopup.h"
#include "stdafx.h"
#include <label_nodes/CCLabelBMFont.h>
#include <label_nodes/CCLabelTTF.h>
#include <textures/CCTexture2D.h>
#include <support/ccUTF8.h>
#include <platform/CCFileUtils.h>
#include <misc_nodes/CCClippingNode.h>

#include <cocos-ext.h>

REGISTER_PAGE("TestHTMLPopup",TestHTMLPopup);
static CCHTMLLabel* s_htmlLabel = NULL;
std::string tt;

TestHTMLPopup::TestHTMLPopup(void)
{

}

TestHTMLPopup::~TestHTMLPopup(void)
{
}

void TestHTMLPopup::Enter(MainFrame* frame)
{

	CCB_FUNC(this,"mShowMemTitle",CCLabelTTF,setString("DragonBone"));


	CCB_FUNC(this,"mTab1",CCMenuItemImage,setVisible(false));
	CCB_FUNC(this,"mTab2",CCMenuItemImage,setVisible(false));
	CCB_FUNC(this,"mTab3",CCMenuItemImage,setVisible(false));
	CCB_FUNC(this,"mTab1Tex",CCLabelBMFont,setVisible(false));
	CCB_FUNC(this,"mTab2Tex",CCLabelBMFont,setVisible(false));
	CCB_FUNC(this,"mTab3Tex",CCLabelBMFont,setVisible(false));

	_init();
}

void TestHTMLPopup::Execute(MainFrame* frame)
{

}

void TestHTMLPopup::unload(void)
{

}

void TestHTMLPopup::Exit(MainFrame* frame)
{
}

void TestHTMLPopup::load(void)
{
	loadCcbiFile("FairShowMemPopUp.ccbi");
}

void TestHTMLPopup::onMenuItemAction(const std::string& itemName,cocos2d::CCObject* sender)
{
	if ( itemName == "onClose")
	{
		MsgMainFramePopPage msg;
		msg.pageName="TestHTMLPopup";
		MessageManager::Get()->sendMessage(&msg);
	}
}

// on "init" you need to initialize your instance
bool TestHTMLPopup::_init()
{
	using namespace dfont;

	// font1
//	FontCatalog* font_catalog = NULL;
//	font_catalog = FontFactory::instance()->create_font(
//		"font1", "Helvetica.font", 0xffffffff, 32, e_plain, 0.0f, 0xffffffff, 0);
//	// font2
////	font_catalog = FontFactory::instance()->create_font(
////		"font2", "simkai.ttf", 0xffffffff, 24, e_shadow, 1.0f, 0xff000000, 0);
////	font_catalog->add_hackfont("htmltest/Marker Felt.ttf", latin_charset(), -1);
//	// font3
//	font_catalog = FontFactory::instance()->create_font(
//		"font3", "Helvetica.font", 0xffffffff, 20, e_border, 1.0f, 0xff000000, 0);
//	font_catalog->add_hackfont("Helvetica.font", latin_charset(), 5);


	//////////////////////////////////////////////////////////////////////////
	
	CCSize vsize = CCDirector::sharedDirector()->getVisibleSize();
	CCString* str_utf8 = CCString::createWithContentsOfFile("html.htm");
	if (str_utf8)
	{
		CCHTMLLabel* htmllabel = CCHTMLLabel::createWithString(str_utf8->getCString(), 
			CCSize(vsize.width*0.8f, vsize.height), "default");
		htmllabel->setAnchorPoint(ccp(0.5f,0.5f));
		htmllabel->setPosition(ccp(vsize.width*0.5f, vsize.height*0.5f));
		addChild(htmllabel);

		s_htmlLabel = htmllabel;

		htmllabel->registerListener(this, &TestHTMLPopup::onHTMLClicked, &TestHTMLPopup::onHTMLMoved );
	}
	
	FontFactory::instance()->dump_textures();

    return true;
}

void TestHTMLPopup::onHTMLClicked(
	IRichNode* root, IRichElement* ele, int _id)
{
	CCLog("[On Clicked] id=%d", _id);

	if ( !s_htmlLabel )
	{
		return;
	}
	else if ( _id == 1002 ) // close
	{
		s_htmlLabel->setVisible(false);
	}
	else if ( _id == 2000 ) //reload
	{
		CCString* str_utf8 = CCString::createWithContentsOfFile("htmltest/html.htm");
		if (str_utf8)
		{
			s_htmlLabel->setString(str_utf8->getCString());	
		}
		
		
	}
}

void TestHTMLPopup::onHTMLMoved(
	IRichNode* root, IRichElement* ele, int _id,
	const CCPoint& location, const CCPoint& delta)
{
	CCLog("[On Moved] id=%d", _id);

	if ( !s_htmlLabel )
	{
		return;
	}
	else if ( _id == 1001 )
	{
		s_htmlLabel->setPosition(ccpAdd(delta, s_htmlLabel->getPosition()));
	}
}


bool TestHTMLPopup::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

 void TestHTMLPopup::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
 {
 }
