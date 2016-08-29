
#include "stdafx.h"

#include "AboutPage.h"
#include "GameMessages.h"
#include "MessageBoxPage.h"
#include "AnimMgr.h"

REGISTER_PAGE("AboutPage",AboutPage);
USING_NS_CC;


AboutPage::AboutPage(void)
{
}


AboutPage::~AboutPage(void)
{
}

void AboutPage::Enter(MainFrame*)
{
	initContent();
}

void AboutPage::initContent()
{
	cocos2d::extension::CCScrollView* scrollContent=dynamic_cast<cocos2d::extension::CCScrollView*>(getVariable("mContent"));

	if(scrollContent)
	{
		autoAdjustResizeScrollview(scrollContent);
		cocos2d::CCNode* node = cocos2d::CCNode::create();
		CCSize size = CCSize(0,0);

		// added by sunyj
		// ��Ϊƽ̨���죬�����ǰitem�Ǳ�ƽ̨�ų�������Ϣ����ô�������item��������������΢�ţ�
		std::string _itemidStr="";
		std::vector<std::string> excludedIds;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		const PlatformRoleItem* roleItem=PlatformRoleTableManager::Get()->getPlatformRoleByID(libPlatformManager::getPlatform()->getPlatformId());
		if(roleItem)
		{
			excludedIds = GameMaths::tokenize(roleItem->excludedAboutIds,",");
		}
#else
	#ifdef WIN32
	#else
		const PlatformRoleItem* roleItem=PlatformRoleTableManager::Get()->getPlatformRoleByName(libPlatformManager::getPlatform()->getClientChannel());
		if(roleItem)
		{
			excludedIds = GameMaths::tokenize(roleItem->excludedAboutIds,",");
		}
	#endif
#endif
		// end sunyj

		AboutTableManager::AboutListIterator itr=AboutTableManager::Get()->getAboutIterator();
		int i=0;
		while(itr.hasMoreElements())
		{
			const AboutItem* item=itr.getNext();

			// added by sunyj
			// ��Ϊƽ̨���죬�����ǰitem�Ǳ�ƽ̨�ų�������Ϣ����ô�������item��������������΢�ţ�
			_itemidStr=StringConverter::toString(item->itemID);
			if ( std::find(excludedIds.begin(), excludedIds.end(), _itemidStr) != excludedIds.end()) {
				// �����excludedIds�б����ҵ���itemId����ô���˵����Id
				continue;
			}
			// end sunyj

			CCBContainer* content = CCBManager::Get()->loadCCbi("AboutContent.ccbi");
			CCLabelBMFont* itemName=dynamic_cast<CCLabelBMFont*>(content->getVariable(("mName")));
			if(item->name!="none")
			{
				if(itemName)
				{
					itemName->setString(item->name.c_str());
				}
			}
			
			CCLabelBMFont* itemDes=dynamic_cast<CCLabelBMFont*>(content->getVariable(("mDes")));
			if(itemDes)
			{
				itemDes->setString(item->describe.c_str());
			}

			CCSprite* face= dynamic_cast<CCSprite*>(content->getVariable("mIcon"));
			if(face)
			{
				//CCTexture2D*tex = CCTextureCache::sharedTextureCache()->addImage(item->iconPic.c_str());
				face->setTexture(item->iconPic.c_str());
			}

			node->addChild(content);
			content->setPosition(ccp((scrollContent->getViewSize().width-content->getContentSize().width)*0.5,content->getContentSize().height*i));
			size.width = content->getContentSize().width;
			size.height += content->getContentSize().height;
			++i;
		}
		excludedIds.clear();
		if(i==0)
		{
			MSG_BOX_LAN("@InvalidityAbout");
			MsgMainFrameChangePage msg;
			msg.pageName="SettingsPage";
			MessageManager::Get()->sendMessage(&msg);
		}
		node->setContentSize(size);
		scrollContent->setContainer(node);
		scrollContent->setContentSize(size);

		if(size.height<scrollContent->getViewSize().height)
		{
			scrollContent->setContentOffset(ccp(0,scrollContent->maxContainerOffset().y));
		}
		else
		{
			scrollContent->setContentOffset(ccp(0,scrollContent->minContainerOffset().y));
		}

		AnimMgr::Get()->nodeMoveIn(node,true);
	}
}

void AboutPage::Exit( MainFrame* frame)
{

}

void AboutPage::Execute( MainFrame* )
{

}

void AboutPage::load( void )
{
	loadCcbiFile("AboutPage.ccbi");
}


void AboutPage::onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender )
{
	MsgMainFrameChangePage msg;
	msg.pageName="SettingsPage";
	MessageManager::Get()->sendMessage(&msg);
}

void AboutPage::onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender, int tag)
{

}
