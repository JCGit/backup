#include "PopTipLayer.h"
#include "defines.h"
#include "BaseScene.h"
#include <string>
#include "ItemUtils.h"
#include "GlobalTime.h"
#include "NounHelper.h"
#include "CCColorUtil.h"

using namespace std;

CPopTipLayer::CPopTipLayer()
:m_pErrorNode(NULL)
{
    m_stTipLst = CCArray::create();
    m_stTipLst->retain();

    m_pErrorLabelLst = CCArray::create();
    m_pErrorLabelLst->retain();

	initUI();
}

CPopTipLayer::~CPopTipLayer(){
    CC_SAFE_RELEASE_NULL(m_stTipLst);
    CC_SAFE_RELEASE_NULL(m_pErrorLabelLst);
	CC_SAFE_RELEASE_NULL(m_pErrorNode);
}

void CPopTipLayer::initUI(){
	const CCSize globalSize = CCDirector::sharedDirector()->getVisibleSize(); 
	CCPoint stOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_pErrorNode = CCNode::create();
	m_pErrorNode->retain();
	m_pErrorNode->setAnchorPoint(ccp(0.5, 1));
	m_pErrorNode->setPosition(stOrigin + ccp(globalSize.width/2, globalSize.height*0.8));
}

void CPopTipLayer::ShowTipStr(const char* pStrTip, const int iType, const int iCfgID)
{
    if(NULL == pStrTip || sizeof(pStrTip) == 0) {
        return;
    }
    BaseScene *pScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    if(NULL == pScene) {
        CCLOG("[error]CPopTipLayer::showPopTip fail,running scene is null.");
        return;
    }

    UILayer *pPopLayer = dynamic_cast<UILayer*>(pScene->getLayer(LAYER_ID_POP));
    if(NULL == pPopLayer) {
        CCLOG("[error]CPopTipLayer::showPopTip fail,LAYER_ID_POP is null.");
        return;
    }
    CCLOG("[trace]ShowTipStrCallBack,pop layer child num=%d", pPopLayer->getChildrenCount());

    const CCSize globalSize = CCDirector::sharedDirector()->getVisibleSize(); 
    CCPoint stOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

    UIImageView* pImg = UIImageView::create(); //bg
    //pImg->setScale9Enabled(true);
    //pImg->setCapInsets(CCRectMake(40, 40, 40, 40));
    pImg->loadTexture("share/tip_bg.png", UI_TEX_TYPE_PLIST);
    //pImg->setSize(pLabel->getSize() + CCSize(60, 20));

    UILabel* pLabel = UILabel::create();
    pLabel->setFontName("fonts/tgamefont.ttf");
    pLabel->setFontSize(30);
    pLabel->setColor(ccc3(0xfe, 0xf8, 0x97)); //fef897
    pLabel->setTouchEnabled(false);
    pLabel->setAnchorPoint(ccp(0.5, 0.5));
	//CCColorUtil::addStroke(pLabel);
    pLabel->setText(pStrTip);

    if(iType > 0) {
        UIImageView* pIcon = UIImageView::create(); //icon
        if(vmsg::THING_TYPE_EXP == iType) {
            pIcon->loadTexture("share/icon_exp.png", UI_TEX_TYPE_PLIST);
        } else {
            pIcon->loadTexture(ItemUtils::getItemIcon(iType, iCfgID).c_str());
        }
        
        //int iTmpWidth = pImg->getSize().height - 40;
        //pIcon->setSize(CCSize(30, 30));
        pIcon->setScale(0.8f);
        int iTotalWidth = pIcon->getSize().width + pLabel->getSize().width;
        pIcon->setPositionX(pIcon->getSize().width/2 - iTotalWidth/2);
        pLabel->setPositionX( iTotalWidth/2 - pLabel->getSize().width/2);
        pImg->addChild(pIcon);
    }
    
    CCPoint pos;
    pos.x = (globalSize.width) / 2;
    pos.y = (globalSize.height/2);
    pos = pos + stOrigin;
    pImg->setPosition(pos);
    pImg->addChild(pLabel, 1, POP_TIP_STR);

    static int iPreCheckTime = 0;
    static int iPreTipTime = 0;
    const int iNow = GlobalTime::getTime();
    static int iDeltaYPos = 0;
    if(iNow - iPreTipTime > 1) {
        iPreTipTime = iNow; //间隔时间很长时，就无所谓了
        iDeltaYPos = 0;
    } else {
        iDeltaYPos++; //短时间内，如果连续弹出提示时，控制一下显示次序
    }
    
    if((iNow - iPreCheckTime) > 3) {
        //clear timeout tips
        ClearAllTips();  
    }
    iPreCheckTime = iNow;

    pPopLayer->addChild(pImg, 100 - iDeltaYPos);
    m_stTipLst->addObject(pImg);

    //飘字动画
    const float actionTime = 0.5f;
    CCActionInterval* actionTo = CCMoveTo::create(actionTime, ccp(pos.x, pos.y + 200 + 5*iDeltaYPos));
    CCActionInterval* toBig = CCScaleTo::create(0.8f, 1.3f, 1.3f);
    CCActionInterval* toSmall = CCScaleTo::create(actionTime, 0.5f, 0.5f);

    CCCallFuncND *actbackfun = CCCallFuncND::create(this, callfuncND_selector(CPopTipLayer::ShowTipStrCallBack),  (void *)0);

    //pImg->runAction(CCSequence::create(CCDelayTime::create(0.5* iDeltaYPos), actionTo, CCDelayTime::create(0.5f), actionOut, actbackfun, NULL));
    //pImg->runAction(CCSequence::create(CCDelayTime::create(0.3* (iDeltaYPos)), toBig, actionTo, toSmall, actbackfun, NULL));
    
    pImg->runAction(CCSequence::create(CCDelayTime::create(0.8* iDeltaYPos), 
        toBig, CCSpawn::create( actionTo, toSmall,  NULL),actbackfun, NULL));

}

void CPopTipLayer::ShowTipStrCallBack(CCNode* pSender, void* pData) {   
    UIImageView* pImg = dynamic_cast<UIImageView*>(pSender);
    if(pImg) {
        m_stTipLst->removeObject(pImg);
        pImg->removeFromParentAndCleanup(true);
    }
}




void CPopTipLayer::ShowNotifyStr(const char* pStrTip, const float iSeconds) {
    if(NULL == pStrTip) {
        return;
    }
    BaseScene *pScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    if(NULL == pScene) {
        CCLOG("[error]CPopTipLayer::showPopTip fail,running scene is null.");
        return;
    }
    UILayer *pPopLayer = dynamic_cast<UILayer*>(pScene->getLayer(LAYER_ID_POP));
    if(NULL == pPopLayer) {
        CCLOG("[error]CPopTipLayer::showPopTip fail,LAYER_ID_POP is null.");
        return;
    }
    CCLOG("[trace]ShowTipStrCallBack,pop layer child num=%d", pPopLayer->getChildrenCount());
    
    //如果当前正在显示，则放到buff里
    if(pPopLayer->getChildByTag(POP_SCROLL_STR)) {
        std::string str(pStrTip);
        m_stNotifyStrBuff.push_back(str);
        return;
    }

    const CCSize globalSize = CCDirector::sharedDirector()->getVisibleSize(); 
    CCPoint stOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

	ImageView* pLabelBg = ImageView::create();
	pLabelBg->loadTexture("share/server_notify_bg.png", UI_TEX_TYPE_PLIST);

	CCPoint pos;
	pos.x = (globalSize.width) / 2;
	pos.y = (globalSize.height) / 2 + 200;
	pLabelBg->setPosition(pos + stOrigin);
	pPopLayer->addChild(pLabelBg, 1, POP_SCROLL_STR);

	Layout* pClipLayout = Layout::create();
	pClipLayout->setSize(CCSize(1000, 70));
	pClipLayout->setClippingEnabled(true);

    UILabel* pLabel = UILabel::create();
    pLabel->setFontName("fonts/tgamefont.ttf");
    pLabel->setFontSize(40);
    pLabel->setColor(ccc3(0x00, 0xff, 0xf6));
    pLabel->setTouchEnabled(false);
    pLabel->setAnchorPoint(ccp(0.5, 0.5));
    CCColorUtil::addStroke(pLabel, 1.0f, ccBLACK);
    pLabel->setText(pStrTip);

	pClipLayout->setPosition(ccp(-pClipLayout->getSize().width/2, -pClipLayout->getSize().height/2));
	pClipLayout->addChild(pLabel);
	pLabel->setUserObject(pLabelBg);

	pLabelBg->addChild(pClipLayout);

	float xPos = 1000 + pLabel->getSize().width/2;
	float yPos = pClipLayout->getSize().height/2;
	pLabel->setPosition(ccp(xPos, yPos));

	float tempXPos = 500;
	float endXPos = xPos - 1000 - pLabel->getSize().width;
    CCActionInterval* actionTo = CCMoveTo::create(6.0f, ccp(tempXPos, yPos));
	CCActionInterval* actionTo2 = CCMoveTo::create(4.0f, ccp(endXPos, yPos));

    CCCallFuncND *actbackfun = CCCallFuncND::create(this, callfuncND_selector(CPopTipLayer::ShowNotifyStrCallBack),  (void *)100);
    pLabel->runAction(CCSequence::create(CCDelayTime::create(0.5f), actionTo,
		CCDelayTime::create(1.0f), actionTo2, CCDelayTime::create(0.5f), actbackfun, NULL));
}

void CPopTipLayer::ShowNotifyStrCallBack(CCNode* pSender, void* pData) {   
    UILabel* pLabel = dynamic_cast<UILabel*>(pSender);
    ImageView* pImg = dynamic_cast<ImageView*>(pLabel->getUserObject());
    if(NULL == pLabel) {
        return;
    }

    if(m_stNotifyStrBuff.size() > 0) {
        string str = m_stNotifyStrBuff.front();
        pLabel->setText(str);
        m_stNotifyStrBuff.erase(m_stNotifyStrBuff.begin());

		float xPos = 1000 + pLabel->getSize().width/2;
		float yPos = 70/2;
		pLabel->setPosition(ccp(xPos, yPos));

		float tempXPos = 500;
		float endXPos = xPos - 1000 - pLabel->getSize().width;
		CCActionInterval* actionTo = CCMoveTo::create(6.0f, ccp(tempXPos, yPos));
		CCActionInterval* actionTo2 = CCMoveTo::create(4.0f, ccp(endXPos, yPos));
        CCCallFuncND *actbackfun = CCCallFuncND::create(this, callfuncND_selector(CPopTipLayer::ShowNotifyStrCallBack),  (void *)100);
		pLabel->runAction(CCSequence::create(CCDelayTime::create(0.5f), actionTo,
			CCDelayTime::create(1.0f), actionTo2, CCDelayTime::create(0.5f), actbackfun, NULL));
        return;
    }
    pImg->removeFromParentAndCleanup(true);
}


void CPopTipLayer::ShowAwardTip(const int iType, const int iCfgID, const int iCnt) {
    if(iCnt <= 0) {
        CCLOG("[warn]CPopTipLayer::ShowAwardTip, cnt <= 0! type %d, cfgid %d", iType, iCfgID);
        return;
    }
    string str = ItemUtils::getItemName(iType, iCfgID);
    if(str.size() <= 0) {
        CCLOG("[warn]CPopTipLayer::ShowAwardTip, fail to get name! type %d, cfgid %d", iType, iCfgID);
        return;
    }
    CCLOG("[trace]CPopTipLayer::ShowAwardTip, type %d, cfgid %d, cnt %d", iType, iCfgID, iCnt);
    str = str + NounStr(MULTIPLY) + intToString(iCnt);
    ShowTipStr(str.c_str(), iType, iCfgID);
}

void CPopTipLayer::ClearAllTips() {
    for(unsigned int i = 0; i < m_stTipLst->count(); ++i) {
        UIImageView* pImg = (UIImageView*)m_stTipLst->objectAtIndex(i);
        if(pImg) {
            pImg->stopAllActions();
            pImg->removeFromParentAndCleanup(true);
        }
    }
    m_stTipLst->removeAllObjects(); //clear

    //错误提示
    for(unsigned int i = 0; i < m_pErrorLabelLst->count(); ++i) {
        UILabel* pLabel = (UILabel*)m_pErrorLabelLst->objectAtIndex(i);
        if(pLabel) {
            pLabel->removeFromParentAndCleanup(true);
        }
    }
    m_pErrorLabelLst->removeAllObjects(); //clear
}

void CPopTipLayer::ShowErrorStr(const char* pStrError) {
    if(NULL == pStrError) {
        return;
    }

	if(NULL == m_pErrorNode) {
		CCLOG("[error]CPopTipLayer::ShowErrorStr fail,error node is null.");
		return;
	}

	if(m_pErrorNode->getParent() == NULL){
		BaseScene *pScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
		if(NULL == pScene) {
			CCLOG("[error]CPopTipLayer::ShowErrorStr fail,running scene is null.");
			return;
		}
		UILayer *pPopLayer = dynamic_cast<UILayer*>(pScene->getLayer(LAYER_ID_POP));
		if(NULL == pPopLayer) {
			CCLOG("[error]CPopTipLayer::ShowErrorStr fail,LAYER_ID_POP is null.");
			return;
		}

		pPopLayer->addChild(m_pErrorNode, 2, POP_ERROR_STR);
	}

	while (m_pErrorLabelLst->count() >= POP_ERROR_MAX_NUM){
		UILabel* pFirst = dynamic_cast<UILabel*>(m_pErrorLabelLst->objectAtIndex(0));
		if(pFirst){
			pFirst->removeFromParentAndCleanup(true);
		}
		m_pErrorLabelLst->removeObjectAtIndex(0);
	}

    CCLOG("[trace]CPopTipLayer::ShowErrorStr, error=[%s]. tips num %d", pStrError, m_pErrorLabelLst->count() );
    const CCSize globalSize = CCDirector::sharedDirector()->getVisibleSize(); 
    CCPoint stOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

	UILabel* pLast = dynamic_cast<UILabel*>(m_pErrorLabelLst->lastObject());
	float yPos = 0;
	if(pLast){
		yPos = pLast->getPositionY() - 32;
	}

    UILabel* mylabel = UILabel::create();
    mylabel->setPosition(ccp(0, yPos));
    mylabel->setFontName("fonts/tgamefont.ttf");
    mylabel->setFontSize(42);
    CCLabelTTF* pttf = (CCLabelTTF*)mylabel->getVirtualRenderer();
    pttf->setColor(ccRED); //红色
    //CCColorUtil::addShadow(pttf);
    CCColorUtil::addStroke(mylabel, 1.0f, ccBLACK);
    mylabel->setText(pStrError);
	mylabel->setScale(0.8f);

	float yNodePos = globalSize.height*0.5 - yPos + stOrigin.y;
	m_pErrorNode->setPositionY(yNodePos);
    
    m_pErrorNode->addChild(mylabel);
    m_pErrorLabelLst->addObject(mylabel);

    CCCallFuncND *fun1111 = CCCallFuncND::create(this, callfuncND_selector(CPopTipLayer::ShowErrorStrCallBack),  (void *)0 );
	CCEaseElasticOut* backIn = CCEaseElasticOut::create(CCScaleTo::create(0.8f, 1.0f));
    CCSequence* seq = CCSequence::create(backIn, CCDelayTime::create(1.5f), fun1111, NULL );
    mylabel->runAction(seq);
}

void CPopTipLayer::ShowErrorStrCallBack(CCNode* pSender, void* pData) {

    UILabel* plabel = dynamic_cast<UILabel*>(pSender);
    if(plabel) {
        m_pErrorLabelLst->removeObject(plabel);
        plabel->removeFromParentAndCleanup(true);
    }
}

