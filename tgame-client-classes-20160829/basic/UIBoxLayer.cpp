#include "UIBoxLayer.h"

#include "basic/BasePanel.h"
#include "ToolUtils.h"

#include "game/guide/GuideControl.h"
#include "MoneyTopBar.h"
#include "PanelSeqMgr.h"

#include "game/city/CityControl.h"

UIBoxLayer::UIBoxLayer()
:m_pBgImg(NULL)
,m_pBgLayout(NULL)
,m_pContainerLayout(NULL)
,m_pBackBtn(NULL)
,m_bInitialized(false)
{
}

UIBoxLayer::~UIBoxLayer()
{
	CC_SAFE_RELEASE_NULL(m_pBackBtn);
    PANEL_SEQ_MGR->clearStack();
}

UIBoxLayer* UIBoxLayer::create(UIButton *pBackBtn) {
	UIBoxLayer *pRet = new UIBoxLayer();

	if(pRet != NULL && pRet->init(pBackBtn)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool UIBoxLayer::init(UIButton *pBackBtn) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!UILayer::init());

		m_stBoxes.clear();

		CCDirector *pDirector = CCDirector::sharedDirector();
		CCPoint screenOrigin = pDirector->getVisibleOrigin(); // 屏幕原点
		CCSize screenSize = pDirector->getVisibleSize(); // 屏幕尺寸
		float largeOffset = 100.0f;
		CCPoint largeOrigin = ccp(screenOrigin.x - largeOffset, screenOrigin.y - largeOffset);
		CCSize largeSize = CCSizeMake(screenSize.width + largeOffset * 2, screenSize.height + largeOffset * 2);


        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_bg/bg_svr_list.plist");
		m_pBgImg = UIImageView::create();
		m_pBgImg->setName("bg_img");
		m_pBgImg->loadTexture("ui_bg/bg_svr_list.png", UI_TEX_TYPE_PLIST);
		m_pBgImg->setPosition(ccp(largeOrigin.x + largeSize.width / 2, largeOrigin.y + largeSize.height / 2));
		float scaleX = largeSize.width / m_pBgImg->getSize().width;
		float scaleY = largeSize.height / m_pBgImg->getSize().height;
		float scale = scaleX > scaleY ? scaleX : scaleY;
		m_pBgImg->setScale(scale);
		m_pBgImg->setZOrder(-3);
		addWidget(m_pBgImg);

		m_pBgLayout = UILayout::create();
		m_pBgLayout->setName("bg_layout");
		m_pBgLayout->setSize(largeSize);
		m_pBgLayout->setPosition(largeOrigin);
		m_pBgLayout->setBackGroundColor(ccc3(0x00, 0x00, 0x00));
		m_pBgLayout->setBackGroundColorType(LAYOUT_COLOR_SOLID);
		m_pBgLayout->setBackGroundColorOpacity(0x40);
		m_pBgLayout->setZOrder(-2);
		addWidget(m_pBgLayout);
		m_pBgLayout->addTouchEventListener(this, toucheventselector(UIBoxLayer::touchEventHandler));

        //money top bar
        /*m_pMoneyTopBar = MoneyTopBar::create();
        addWidget(m_pMoneyTopBar);
        m_pMoneyTopBar->setEnabled(false);
        m_pMoneyTopBar->setVisible(false);
        m_pMoneyTopBar->setZOrder(5);*/

		m_pContainerLayout = UILayout::create();
		m_pContainerLayout->setName("container_layout");
		m_pContainerLayout->setSize(screenSize);
		m_pContainerLayout->setPosition(screenOrigin);
		m_pContainerLayout->setTouchEnabled(false);
		m_pContainerLayout->setZOrder(-1);
		addWidget(m_pContainerLayout);


		m_bInitialized = true;

		setBackBtn(pBackBtn);

		setBackBtnEnabled(false);
		setBgEnabled(false);
        setBgImgVisible(false);
        //setMoneyBarVisible(false);

		bRet = true;
	} while(0);

	return bRet;
}

void UIBoxLayer::setBackBtn(UIButton *pBackBtn) {
	CCAssert(m_bInitialized, "Not initialized");

	if(pBackBtn != NULL) {
		if(m_pBackBtn != NULL && m_pBackBtn->getParent() != NULL) {
			m_pBackBtn->removeFromParent();
		}

		CC_SAFE_RETAIN(pBackBtn);
		CC_SAFE_RELEASE(m_pBackBtn);
		m_pBackBtn = pBackBtn;
		m_pBackBtn->setName("back_btn");

		CCDirector *pDirector = CCDirector::sharedDirector();
		CCPoint origin = pDirector->getVisibleOrigin(); // 屏幕原点
		CCSize size = pDirector->getVisibleSize(); // 屏幕尺寸

		const CCSize &btnSize = m_pBackBtn->getSize();
		const CCPoint &btnAnchor = m_pBackBtn->getAnchorPoint();

		float offsetX = 4.0f; // 距离屏幕边缘的偏移量X
		float offsetY = 4.0f; // 距离屏幕边缘的偏移量Y
		float posX = origin.x + btnSize.width * btnAnchor.x + offsetX;
		//float posX = origin.x + size.width - btnSize.width * (1 - btnAnchor.x) - offsetX;
		float posY = origin.y + size.height - btnSize.height * (1 - btnAnchor.y) - offsetY;
		m_pBackBtn->setPosition(ccp(posX, posY));
		addWidget(m_pBackBtn);
		m_pBackBtn->addTouchEventListener(this, toucheventselector(UIBoxLayer::touchEventHandler));
	}
}

void UIBoxLayer::addBox(Box *pBox) {
	CCAssert(m_bInitialized, "Not initialized");

	if(pBox != NULL && pBox->getParent() == NULL) {
		m_pContainerLayout->addChild(pBox);
		m_stBoxes.push_back(pBox);

		//先设置上方前是否显示
		setMoneyBarVisible(pBox->isShowMoneyTopBar());

		if(pBox->getLinkedPanel() != NULL) {
			CCSize aBoxSize = pBox->getSize();
			CCSize bBoxSize = pBox->getLinkedPanel()->getSize();

			CCDirector *pDirector = CCDirector::sharedDirector();
			CCPoint origin = CCPointZero; // m_pContainerLayout已添加到屏幕原点
			CCSize size = pDirector->getVisibleSize();

			float centerX = origin.x + size.width / 2;
			float centerY = origin.y + size.height / 2;

			float posX = origin.x + (size.width - aBoxSize.width - bBoxSize.width) / 2;
			float posY = pBox->getPositionY() + (aBoxSize.height - bBoxSize.height)/2;

			pBox->getLinkedPanel()->setPosition(ccp(posX, posY));
			pBox->setPositionX(posX + bBoxSize.width);
		}

		if(pBox->isOpenWithBgImg()) {
			setBgImgVisible(true);
		}

		setBackBtnEnabled(pBox->isUseBackForClose()); // 用返回按钮来实现关闭
		setBgEnabled(true);
	}
}

void UIBoxLayer::removeBox(Box *pBox) {
	CCAssert(m_bInitialized, "Not initialized");

	if(pBox != NULL && pBox->getParent() != NULL) {
		//m_stBoxes.pop_back(); // 若打开新面板后才关闭上一个面板，则要关闭的面板不是最后一个元素，故采用以下方式
		std::vector<Box*>::iterator iter = m_stBoxes.begin();

		while(iter != m_stBoxes.end()) {
			if(*iter == pBox) {
				iter = m_stBoxes.erase(iter);
				break;
			} else {
				iter++;
			}
		}

		if(pBox->getLinkedPanel() != NULL) {
			Box *pLinkedBox = pBox->getLinkedPanel();
			pBox->setLinkedPanel(NULL);
			removeBox(pLinkedBox);
        }

        m_pContainerLayout->removeChild(pBox); //先setLinkedPanel，然后再删除

		if(m_pContainerLayout->getChildrenCount() == 0) {
			setBackBtnEnabled(false);
			setBgEnabled(false);
			setBgImgVisible(false);
            setMoneyBarVisible(false);
		} else {
            if(m_stBoxes.size() > 0) {
                Box *pLastBox = m_stBoxes[m_stBoxes.size() - 1];
                if(pLastBox) {
					pLastBox->refresh();
                    setBackBtnEnabled(pLastBox->isUseBackForClose());
					setMoneyBarVisible(pLastBox->isShowMoneyTopBar());
                }
                
            }
			
			bool bShowBgImg = false;
			for(unsigned int i = 0; i < m_stBoxes.size(); i++) {
				if(m_stBoxes[i]->isOpenWithBgImg()) {
					bShowBgImg = true;
					break;
				}
			}

			setBgImgVisible(bShowBgImg);
		}
	}
}

void UIBoxLayer::removeAllBoxes() {
    
    int iAllChildNum = m_pContainerLayout->getChildren()->count();
    while(iAllChildNum > 0) {
        Box* pBox = (Box*)(m_pContainerLayout->getChildren()->objectAtIndex(iAllChildNum - 1) );
        if(pBox) {
            CCLOG("[trace]UIBoxLayer::removeAllBoxes,close [%s]", (typeid(*pBox).name()) );
            pBox->close(); //因为有的面板逻辑在此
        } else {
            m_pContainerLayout->getChildren()->removeLastObject();
        }

        iAllChildNum = m_pContainerLayout->getChildren()->count();
    }
    
    //removeAll时没有调用close函数，所以有些逻辑可能处理不到
    
    m_stBoxes.clear();
	m_pContainerLayout->removeAllChildren();

	setBackBtnEnabled(false);
	setBgEnabled(false);
    setMoneyBarVisible(false);

    PANEL_SEQ_MGR->clearStack();
}

bool UIBoxLayer::isBgImgVisible() {
	return m_pBgImg->isVisible();
}

void UIBoxLayer::setBgImgVisible(bool visible) {
	m_pBgImg->setVisible(visible);
}

bool UIBoxLayer::isBgEnabled() {
	CCAssert(m_bInitialized, "Not initialized");

	return m_pBgLayout->isVisible();
}

void UIBoxLayer::setBgEnabled(bool enabled) {
	CCAssert(m_bInitialized, "Not initialized");

	m_pBgLayout->setVisible(enabled);
	m_pBgLayout->setTouchEnabled(enabled);
}

bool UIBoxLayer::isBackBtnEnabled() {
	return m_pBackBtn->isVisible();
}

void UIBoxLayer::setBackBtnEnabled(bool enabled) {
	CCAssert(m_bInitialized, "Not initialized");

	m_pBackBtn->setVisible(enabled);
	m_pBackBtn->setTouchEnabled(enabled);
}

UIButton* UIBoxLayer::getBackBtn() {
	return m_pBackBtn;
}

void UIBoxLayer::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pBgLayout->getName()) == 0) { // 点击了box以外非返回按钮的区域
				/*if(m_stBoxes.size() > 0) {
					Box *pLastBox = m_stBoxes[m_stBoxes.size() - 1];

					if(pLastBox->isClickOutToClose()) {
                        pLastBox->close();
					}
				}*/
			} else if(std::strcmp(pszWidgetName, m_pBackBtn->getName()) == 0) { // 返回
				GUIDE_TOUCH(m_pBackBtn);

				if(m_stBoxes.size() > 0) {
					Box *pLastBox = m_stBoxes[m_stBoxes.size() - 1];

					if(pLastBox->isUseBackForClose()) {
                        pLastBox->doGlobalBackClose();
					}
				}
			}
			break;
		default:;
	}
}


bool UIBoxLayer::isBoxOnTop(Box *pBox) {
    if(pBox && pBox->getParent() == m_pContainerLayout && m_stBoxes.size() > 0) {
        Box* pLastBox = *(m_stBoxes.rbegin());
        return (pBox == pLastBox);
    }
    return false;
}


void UIBoxLayer::setMoneyBarVisible(bool bIsShow) {
    CityScreenView *pCityScreenView = P_CITY_CTRL->getCityScreenView();

	if(pCityScreenView != NULL) {
		if(bIsShow) {
			pCityScreenView->bringAssetsToTop();
		} else {
			pCityScreenView->bringAssetsToBottom();
		}
	}

	/*if(bIsShow) {
        CCDirector *pDirector = CCDirector::sharedDirector();
        CCPoint origin = pDirector->getVisibleOrigin(); // 屏幕原点
        CCSize size = pDirector->getVisibleSize(); // 屏幕尺寸

        const CCSize &btnSize = m_pMoneyTopBar->getSize();
        const CCPoint &btnAnchor = m_pMoneyTopBar->getAnchorPoint();

        float offsetX = 4.0f; // 距离屏幕边缘的偏移量X
        float offsetY = 4.0f; // 距离屏幕边缘的偏移量Y
        float posX = origin.x + size.width - btnSize.width * (1 - btnAnchor.x ) - offsetX;
        float posY = origin.y + size.height - btnSize.height * (1 - btnAnchor.y) - offsetY;

        m_pMoneyTopBar->setPosition(ccp(posX, posY));

        m_pMoneyTopBar->setEnabled(true);
        m_pMoneyTopBar->setVisible(true);
    } else {
		if(m_pContainerLayout && m_pContainerLayout->getChildrenCount() == 0) {
			m_pMoneyTopBar->setVisible(false);
			m_pMoneyTopBar->setEnabled(false);
		}
    }*/
}

void UIBoxLayer::updateMoneyBar() {
    /*if(m_pMoneyTopBar && m_pMoneyTopBar->isVisible()) {
        m_pMoneyTopBar->updateUI();
    }*/
}

