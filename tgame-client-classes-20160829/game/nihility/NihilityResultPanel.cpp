#include "NihilityResultPanel.h"
#include "UserObj.h"
#include "SceneManager.h"
#include "defines.h"
#include "CommonDialog.h"
#include "NounHelper.h"
#include "NihilityControl.h"
#include "IconCell.h"
#include "ToolUtils.h"
#include "weaponlib/WeaponLibControl.h"
#include "tip/TipControl.h"
#include "NihilityResultItem.h"

#include "HeroCfgMaster.h"
#include "FBShareCfgMaster.h"


#include "FaceBookSDK.h"
#include "FBShareCfgMaster.h"


using namespace vmsg;

#define NIHILITY_AWARD_MAX_SIZE 6

CNihilityResultPanel::CNihilityResultPanel()
:m_pSureBtn(NULL)
,m_pShareBtn(NULL)
,m_pRoleImg(NULL)
,m_pScoreTxt(NULL)
,m_pCoinTxt(NULL)
,m_pMaxTxt(NULL)
,m_pItemDemo(NULL)
,m_fSliderMinY(0)
,m_fSliderMaxY(0)
{

}

CNihilityResultPanel::~CNihilityResultPanel(){
	CC_SAFE_RELEASE_NULL(m_pItemDemo);
}

bool CNihilityResultPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/nihility_result_ui.ExportJson");
		addChild(m_pRootLayout);

		setClickOutToClose(false);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CNihilityResultPanel::initUI(){
	m_pSureBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("sure_btn"));
	m_pShareBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("friend_btn"));
	m_pScoreTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("score_txt"));
	m_pCoinTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("coin_txt"));
	m_pMaxTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("max_score_txt"));
	m_pRoleImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("role_img"));
	m_pSliderImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("slider_img"));

	m_pScoreLabel = ImageNumber::create(19);
	m_pScoreLabel->setNum("0");
	m_pRootLayout->addNode(m_pScoreLabel, 10);
	m_pScoreLabel->setPosition(m_pScoreTxt->getPosition() + ccp(0, m_pScoreLabel->getContentSize().height/2));
	m_pScoreTxt->setVisible(false);

	CNihilityResultItem* pItem = CNihilityResultItem::create();
	m_pItemDemo = pItem->getRootLayout();
	m_pItemDemo->retain();

	m_pAwardList = dynamic_cast<ScrollView*>(m_pRootLayout->getChildByName("award_list"));
	m_pAwardList->addEventListenerScrollView(this, scrollvieweventselector(CNihilityResultPanel::listScrollEvent));
	m_fSliderMinY = m_pSliderImg->getPositionY();
	m_fSliderMaxY = m_pSliderImg->getPositionY() + m_pAwardList->getSize().height - m_pSliderImg->getSize().height;

	m_pSureBtn->addTouchEventListener(this, toucheventselector(CNihilityResultPanel::touchEventHandler));
	m_pShareBtn->addTouchEventListener(this, toucheventselector(CNihilityResultPanel::touchEventHandler));
}

void CNihilityResultPanel::updateUI(){
	m_pCoinTxt->setText(to_string(P_NIHILITY_CTRL->getCoin()));
	m_pMaxTxt->setText(to_string(P_USER_OBJ->getUserInfo()->stNihilityInfo.score()));

	m_pScoreLabel->rollNumFromTo(0, P_NIHILITY_CTRL->getScore());

	m_pAwardList->removeAllChildren();

	const CSNothingLandEndRsp& stAwardInfo = P_NIHILITY_CTRL->getNihilityEndInfo();
	m_stAwardInfo.clear();

	bool bIsBigElt;
	vector<vmsg::CSAwardElement> stBigElts;
	for(int idx = 0; idx < stAwardInfo.awardlist_size(); idx++){
		const vmsg::CSAwardElement& elmt = stAwardInfo.awardlist(idx);
		bIsBigElt = CToolUtils::adjustIconBgSize(elmt.elementtype(), elmt.elementid(), NULL);
		if(bIsBigElt){
			stBigElts.push_back(elmt);
		}else{
			m_stAwardInfo.push_back(elmt);
		}
	}

	m_stAwardInfo.insert(m_stAwardInfo.begin(), stBigElts.begin(), stBigElts.end());

	CCSize stSize = m_pAwardList->getSize();
	float nowHight = stSize.height;
	unsigned int row = ceil(m_stAwardInfo.size()*1.0f / 3.0f);
	if(row > 0){
		nowHight = 100*row + (row - 1)*10;
	}

	bool bIsView = true;
	if(nowHight <= stSize.height){
		nowHight = stSize.height;
		bIsView = false;
	}
	m_pSliderImg->setVisible(bIsView);
	m_pAwardList->setInnerContainerSize(CCSizeMake(stSize.width, nowHight));
	m_pSliderImg->setPositionY(m_fSliderMinY);

	//update
	float itemX = 0;
	float itemY = 0;
	CNihilityResultItem* pItem = NULL;
	for(unsigned int i = 0; i < m_stAwardInfo.size(); i++){
		const CSAwardElement& elmt = m_stAwardInfo.at(i);
		pItem = CNihilityResultItem::create(m_pItemDemo);
		pItem->setInfo(elmt.elementtype(), elmt.elementcnt(), elmt.elementid());

		itemX = (pItem->getSize().width + 20) * (i % 3) ;
		itemY = m_pAwardList->getInnerContainer()->getSize().height - (pItem->getSize().height + 10) * (i / 3) - pItem->getSize().height;
		pItem->setPosition(ccp(itemX, itemY));
		m_pAwardList->addChild(pItem);
	}
}

bool CNihilityResultPanel::compareByType(const CSAwardElement& stInfoA, const CSAwardElement& stInfoB) {
	if(stInfoA.elementtype() == stInfoB.elementtype()) {
		if(THING_TYPE_WEAPON == stInfoA.elementtype()) {
			return true;
		}else{
			return false;
		}
	}else{
		return stInfoA.elementtype() > stInfoB.elementtype();
	}
}

void CNihilityResultPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget* pWidget = dynamic_cast<UIWidget*>(pSender);
	const char* pName = pWidget->getName();
	string strName = pWidget->getName();
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		if(strcmp(pName, "sure_btn") == 0){
			this->close();
			SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
		}else if(strcmp(pName, "friend_btn") == 0){

			CCLOG("[trace]CNihilityResultPanel::touchEventHandler share button clicked.");
			const FBShareCfg* pShareCfg = FBShare_CFG->GetFBShareCfg(1);
			if (!pShareCfg)
				break;

			FaceBookSDK::onShared(pShareCfg->name(), pShareCfg->link(), pShareCfg->picture(), pShareCfg->caption(), pShareCfg->description());
		}
		break;
	default:
		break;
	}
}

void CNihilityResultPanel::close(){
	BasePanel::close();
}

void CNihilityResultPanel::listScrollEvent(CCObject* obj, ScrollviewEventType type){
	CCPoint stPos = m_pAwardList->getInnerContainer()->getPosition();
	CCSize stSize = m_pAwardList->getSize();
	CCSize stInSize = m_pAwardList->getInnerContainerSize();
	float per;
	float disY;
	switch (type)
	{
	case SCROLLVIEW_EVENT_SCROLLING:
		per = stPos.y*1.0f/(stSize.height - stInSize.height);
		if(per > 1){
			per = 1;
		}else if(per < 0){
			per = 0;
		}

		disY = m_fSliderMinY - (1 - per)*(m_fSliderMaxY - m_fSliderMinY);
		m_pSliderImg->setPositionY(disY);
		break;
	}
}
