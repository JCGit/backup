#include "DialogLayout.h"

#include "Resources.h"

#include "Macros.pb.h"

#include "CfgMaster/HeroCfgMaster.h"
#include "CfgMaster/StoryCfgMaster.h"
#include "CfgMaster/NPCCfgMaster.h"

#include "basic/NounHelper.h"
#include "basic/ToolUtils.h"

#include "game/chat/AnimSprite.h"

#include "game/UserObj.h"
#include "game/task/TaskControl.h"

#include "PlotControl.h"

DialogLayout::DialogLayout()
:m_pBgWordsImg(NULL)
,m_pRoleHeadImg(NULL)
,m_pRoleNameTxt(NULL)
,m_pWordsTxt(NULL)
,m_pNextAnim(NULL)
,m_uStoryID(0)
,m_uDialogIdx(0)
,m_uShowIdx(0)
,m_pListener(NULL)
,m_pfnSelector(NULL)
,m_pBgLayout(NULL)
,m_bActivate(false)
{
}

DialogLayout::~DialogLayout()
{
}

DialogLayout* DialogLayout::create() {
	DialogLayout *pRet = new DialogLayout();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool DialogLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		CCDirector *pDirector = CCDirector::sharedDirector();
		CCPoint screenOrigin = pDirector->getVisibleOrigin(); // 屏幕原点
		CCSize screenSize = pDirector->getVisibleSize(); // 屏幕大小

		float largeOffset = 100.0f;
		CCSize largeSize = CCSizeMake(screenSize.width + largeOffset * 2, screenSize.height + largeOffset * 2);

		m_pBgLayout = UILayout::create();
		m_pBgLayout->setSize(largeSize);
		m_pBgLayout->setBackGroundColor(ccc3(0x00, 0x00, 0x00));
		m_pBgLayout->setBackGroundColorType(LAYOUT_COLOR_SOLID);
		m_pBgLayout->setBackGroundColorOpacity(0x40);
		addChild(m_pBgLayout);

		//setBackGroundColor(ccc3(0x00, 0x00, 0x00));
		//setBackGroundColorType(LAYOUT_COLOR_SOLID);
		//setBackGroundColorOpacity(0x40);

		setSize(screenSize);
		setPosition(screenOrigin);

		//initLayoutFromJsonFile("ui/DialogLayoutUI.json");
		initLayoutFromJsonFile("ui_v2/StoryScreenUI.ExportJson");
		addChild(m_pRootLayout);

		m_pBgWordsImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_words_img"));
		m_pBgWordsImg->setTouchEnabled(false);
		m_pBgWordsImg->addTouchEventListener(this, toucheventselector(DialogLayout::touchEventHandler));

		m_pNameBgImg = dynamic_cast<UIImageView*>(m_pBgWordsImg->getChildByName("name_bg"));
		m_pNameBgImg->setPosition(ccp(242, 231));

		m_pLightImg = dynamic_cast<UIImageView*>(m_pBgWordsImg->getChildByName("light_img"));
		m_pLightImg->setSize(CCSizeMake(m_pBgWordsImg->getSize().width - 235 ,35));
		m_pLightImg->setPosition(ccp(149, 18));

		m_pRoleHeadImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("role_head_img"));
		m_pRoleHeadImg->setPosition(ccp(-m_pRoleHeadImg->getSize().width, 0));

		m_pBgWordsImg->setSize(CCSizeMake(screenSize.width - 320, 256));
		m_pBgWordsImg->setPosition(ccp(getSize().width, 0));

		m_pRoleNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("role_name_txt"));
		m_pRoleNameTxt->setPosition(ccp(542, 230));

		m_pNextAnim = AnimSprite::create("ui_bg/next_anim.png", CCSizeMake(88, 88), 2, 0.8f);
		m_pNextAnim->setPosition(ccp(getSize().width - 300, 64));
		m_pRootLayout->addNode(m_pNextAnim);

		m_pWordsTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("words_txt"));
		m_pWordsTxt->setSize(CCSizeMake(m_pBgWordsImg->getSize().width - 140, 128));
		m_pWordsTxt->setPosition(ccp(423, 183));
		//m_pWordsTxt->setPosition(ccp(m_pBgWordsImg->getPositionX() - m_pBgWordsImg->getSize().width + 28, m_pBgWordsImg->getPositionY() + m_pBgWordsImg->getSize().height - 84));
		
		//m_pOptionTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("option_txt"));

		setName("dialog_layout");
		addTouchEventListener(this, toucheventselector(DialogLayout::touchEventHandler));

		bRet = true; 
	} while(0);

	return bRet;
}

void DialogLayout::setStoryID(unsigned int uStoryID) {
	m_uStoryID = uStoryID;
}

void DialogLayout::showDialog() {
	m_pBgWordsImg->setVisible(true);
	m_pRoleHeadImg->setVisible(true);
	m_pWordsTxt->setVisible(true);
	m_pNextAnim->setVisible(true);

	CCActionInterval *pBgAction = CCEaseBackOut::create(CCMoveTo::create(0.8f, ccp(getSize().width - m_pBgWordsImg->getSize().width, 0)));
	m_pBgWordsImg->setPosition(ccp(getSize().width, 0));
	m_pBgWordsImg->runAction(pBgAction);

	const StoryCfg *pStoryCfg = STORY_CFG_MASTER->getStoryCfg(m_uStoryID);

	if(m_uDialogIdx < pStoryCfg->storydialogs_size()) {
		if(m_pWordsTxt->numberOfRunningActions() > 0) {
			m_pWordsTxt->stopAllActions();
		}

		std::string url = "";

		if(pStoryCfg->storydialogs(m_uDialogIdx).roletype() == ROLE_TYPE_PLAYER) {
			unsigned int uAvatarID = P_USER_OBJ->getUserInfo()->showID;
			url = BIGHEAD_ICON_PATH + HERO_CFG_MASTER->GetHeroCfgByID(uAvatarID)->headicon() + ".png";
			m_pRoleNameTxt->setText(P_USER_OBJ->getUserInfo()->nick);
		} else if(pStoryCfg->storydialogs(m_uDialogIdx).roletype() == ROLE_TYPE_NPC) {
			unsigned int uNPCID = pStoryCfg->storydialogs(m_uDialogIdx).roleid();
			std::string showID = NPC_CFG_MASTER->getNPCCfgByID(uNPCID)->showid();
			url = BIGHEAD_ICON_PATH + showID + ".png";
			m_pRoleNameTxt->setText(NPC_CFG_MASTER->getNPCCfgByID(uNPCID)->npcname());
		}

		m_pRoleHeadImg->loadTexture(url.c_str());

	}

	CCActionInterval *pHeadAction = CCEaseBackOut::create(CCMoveTo::create(0.8f, ccp(0, 0)));
	m_pRoleHeadImg->setPosition(ccp(-m_pRoleHeadImg->getSize().width, 0));
	m_pRoleHeadImg->runAction(pHeadAction);

	CCActionInterval *pOptionAction = CCFadeIn::create(0.8f);
	m_pNextAnim->setOpacity(0x00);
	m_pNextAnim->runAction(CCSequence::create(CCDelayTime::create(0.8f), pOptionAction, NULL));

	CCActionInterval *pRoleNameAction = CCFadeIn::create(0.8f);
	m_pRoleNameTxt->setOpacity(0x00);
	m_pRoleNameTxt->getVirtualRenderer()->runAction(CCSequence::create(CCDelayTime::create(0.8f), pRoleNameAction, NULL));

	m_uDialogIdx = 0;
	CCAction *pFuncAction = CCCallFunc::create(this, callfunc_selector(DialogLayout::playDialog));
	this->runAction(CCSequence::create(CCDelayTime::create(0.8f), pFuncAction, NULL));
	//playDialog();
}

void DialogLayout::hideDialog() {
	const StoryCfg *pStoryCfg = STORY_CFG_MASTER->getStoryCfg(m_uStoryID);

	if(m_pBgWordsImg->numberOfRunningActions() > 0) {
		m_pBgWordsImg->stopAllActions();
	}

	if(m_pBgWordsImg->getScaleX() == 1.0f) {
		CCActionInterval *pBgAction = CCEaseBackIn::create(CCMoveTo::create(0.8f, ccp(getSize().width, 0)));
		m_pBgWordsImg->setPosition(ccp(getSize().width - m_pBgWordsImg->getSize().width, 0));
		m_pBgWordsImg->runAction(pBgAction);
	} else {
		CCActionInterval *pBgAction = CCEaseBackIn::create(CCMoveTo::create(0.8f, ccp(0, 0)));
		m_pBgWordsImg->setPosition(ccp(m_pBgWordsImg->getSize().width, 0));
		m_pBgWordsImg->runAction(pBgAction);
	}
	
	if(m_pRoleHeadImg->numberOfRunningActions() > 0) {
		m_pRoleHeadImg->stopAllActions();
	}

	if(pStoryCfg->storydialogs(m_uDialogIdx).roletype() == ROLE_TYPE_PLAYER) {
		CCActionInterval *pHeadAction = CCEaseBackIn::create(CCMoveTo::create(0.8f, ccp(getSize().width, 0)));
		m_pRoleHeadImg->setPositionX(getSize().width - m_pRoleHeadImg->getSize().width);
		m_pRoleHeadImg->runAction(pHeadAction);
	} else {
		CCActionInterval *pHeadAction = CCEaseBackIn::create(CCMoveTo::create(0.8f, ccp(-m_pRoleHeadImg->getSize().width, 0)));
		m_pRoleHeadImg->setPositionX(0.0f);
		m_pRoleHeadImg->runAction(pHeadAction);
	}

	if(m_pWordsTxt->numberOfRunningActions() > 0) {
		m_pWordsTxt->stopAllActions();
	}

	if(m_pWordsTxt->getVirtualRenderer()->numberOfRunningActions() > 0) {
		m_pWordsTxt->getVirtualRenderer()->stopAllActions();
	}

	CCActionInterval *pWordsAction = CCEaseBackIn::create(CCMoveTo::create(0.8f, ccp(423, 183 - 40)));
	m_pWordsTxt->setText("");
	m_pWordsTxt->setPosition(ccp(423, 183));
	m_pWordsTxt->setOpacity(0xff);
	m_pWordsTxt->runAction(pWordsAction);
	m_pWordsTxt->getVirtualRenderer()->runAction(CCFadeOut::create(0.8f));
	
	CCActionInterval *pSkipAction = CCFadeOut::create(0.8f);
	m_pNextAnim->setOpacity(0xff);
	m_pNextAnim->runAction(pSkipAction);

	CCActionInterval *pNameAction = CCFadeOut::create(0.8f);
	m_pRoleNameTxt->setOpacity(0xff);
	m_pRoleNameTxt->getVirtualRenderer()->runAction(pNameAction);

	scheduleOnce(schedule_selector(DialogLayout::onHideDialog), 0.8f);
}

void DialogLayout::addDialogEventListener(CCObject *pListener, SEL_DIALOG pSelector) {
	m_pListener = pListener;
	m_pfnSelector = pSelector;
}

void DialogLayout::playDialog() {
	m_bActivate = true;
	m_pWordsTxt->setText("");
	const StoryCfg *pStoryCfg = STORY_CFG_MASTER->getStoryCfg(m_uStoryID);

	if(m_uDialogIdx < pStoryCfg->storydialogs_size()) {
		if(m_pWordsTxt->numberOfRunningActions() > 0) {
			m_pWordsTxt->stopAllActions();
		}

		CToolUtils::parseUTFStr(pStoryCfg->storydialogs(m_uDialogIdx).words(), m_stWords);
		m_uShowIdx = 0;
		CCAction *pFuncAction = CCCallFunc::create(this, callfunc_selector(DialogLayout::literallyShow));

		m_pWordsTxt->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(0.02f), pFuncAction, NULL)));

		std::string option = "";

		if(m_uDialogIdx < pStoryCfg->storydialogs_size() - 1) {
			option = "Next";
		} else { // 最后一个对话
			switch(pStoryCfg->storytype()) {
				case STORY_RECV_TASK:
					option = NounStr(NOUN_RECV_TASK);
					break;
				case STORY_COMMIT_TASK:
					option = NounStr(NOUN_COMMIT_TASK);
					break;
				case STORY_GUIDE_STEP:
					option = "OK";
					break;
				default:;
			}
		}

		//m_pOptionTxt->setText(option);

		std::string url = "";

		if(pStoryCfg->storydialogs(m_uDialogIdx).roletype() == ROLE_TYPE_PLAYER) {
			unsigned int uAvatarID = P_USER_OBJ->getUserInfo()->showID;
			url = BIGHEAD_ICON_PATH + HERO_CFG_MASTER->GetHeroCfgByID(uAvatarID)->headicon() + ".png";
			m_pRoleNameTxt->setText(P_USER_OBJ->getUserInfo()->nick);
		} else if(pStoryCfg->storydialogs(m_uDialogIdx).roletype() == ROLE_TYPE_NPC) {
			unsigned int uNPCID = pStoryCfg->storydialogs(m_uDialogIdx).roleid();
			std::string showID = NPC_CFG_MASTER->getNPCCfgByID(uNPCID)->showid();
			url = BIGHEAD_ICON_PATH + showID + ".png";
			m_pRoleNameTxt->setText(NPC_CFG_MASTER->getNPCCfgByID(uNPCID)->npcname());
		}

		m_pRoleHeadImg->loadTexture(url.c_str());

		if(m_uDialogIdx > 0) {
			if(pStoryCfg->storydialogs(m_uDialogIdx).roletype() == ROLE_TYPE_PLAYER) {
				m_pBgWordsImg->setScaleX(-1.0f);
				m_pBgWordsImg->setPositionX(m_pBgWordsImg->getSize().width);
				m_pRoleHeadImg->setPositionX(getSize().width - m_pRoleHeadImg->getSize().width);
				m_pNameBgImg->setPositionX(780);
				m_pWordsTxt->setPositionX(38);
				m_pRoleNameTxt->setPositionX(179);
				m_pNextAnim->setPositionX(m_pBgWordsImg->getPositionX() - 260);
			} else {
				m_pBgWordsImg->setScaleX(1.0f);
				m_pBgWordsImg->setPositionX(getSize().width - m_pBgWordsImg->getSize().width);
				m_pRoleHeadImg->setPositionX(0.0f);
				m_pNameBgImg->setPositionX(242);
				m_pWordsTxt->setPositionX(423);
				m_pRoleNameTxt->setPositionX(542);
				m_pNextAnim->setPositionX(getSize().width - 300);
			}
		}

	}

}

void DialogLayout::onHideDialog(float dt) {
	m_pBgWordsImg->setVisible(false);
	m_pRoleHeadImg->setVisible(false);
	m_pWordsTxt->setVisible(false);
	m_pNextAnim->setVisible(false);
	
	m_pBgWordsImg->setScaleX(1.0f);
	m_pBgWordsImg->setPosition(ccp(getSize().width, 0));
	m_pRoleHeadImg->setScaleX(1.0f);
	m_pRoleHeadImg->setPosition(ccp(-m_pRoleHeadImg->getSize().width, 0));
	m_pNextAnim->setPositionX(getSize().width - 300);
	m_pWordsTxt->setPosition(ccp(423, 183));
	m_pWordsTxt->setOpacity(0xff);

	P_PLOT_CTRL->hideDialog();
	
	// 对话结束
	/*if(m_pListener != NULL && m_pfnSelector != NULL) {
		(m_pListener->*m_pfnSelector)(this, m_uStoryID);
	}*/
	P_PLOT_CTRL->onStoryEnd();
}

void DialogLayout::literallyShow() {
	std::string words = "";

	if(m_uShowIdx < m_stWords.size()) {
		for(unsigned int i = 0; i <= m_uShowIdx; i++) {
			words.append(m_stWords[i]);
		}

		m_uShowIdx++;
	} 
	
	if(m_uShowIdx >= m_stWords.size()) {
		m_pWordsTxt->stopAllActions();
	}

	m_pWordsTxt->setText(words);
}

void DialogLayout::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, getName()) == 0) {
				if(m_bActivate) {
					m_bActivate = false;

					const StoryCfg *pStoryCfg = STORY_CFG_MASTER->getStoryCfg(m_uStoryID);

					if(m_uDialogIdx < pStoryCfg->storydialogs_size() - 1) {
						m_uDialogIdx++;
						m_pWordsTxt->setText("");
						playDialog();
					} else if(m_uDialogIdx == pStoryCfg->storydialogs_size() - 1) { // 最后一句
						hideDialog();
					}
				}
			} else if(std::strcmp(pszWidgetName, m_pBgWordsImg->getName()) == 0) {
				const StoryCfg *pStoryCfg = STORY_CFG_MASTER->getStoryCfg(m_uStoryID);

				if(m_uDialogIdx < pStoryCfg->storydialogs_size() - 1) {
					m_uDialogIdx++;
					m_pWordsTxt->setText("");
					playDialog();
				} else if(m_uDialogIdx == pStoryCfg->storydialogs_size() - 1) { // 最后一句
					hideDialog();
				}
				
			}
			break;
		default:
			break;
	}
}
