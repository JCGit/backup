#include "PlotControl.h"

#include "SimpleAudioEngine.h"

#include "Macros.pb.h"

#include "CfgMaster/TaskCfgMaster.h"
#include "CfgMaster/StoryCfgMaster.h"

#include "basic/BaseScene.h"
#include "basic/NounHelper.h"
#include "control/AudioManager.h"

#include "game/task/TaskControl.h"
#include "game/guide/GuideControl.h"

using namespace CocosDenshion;

PlotControl::PlotControl()
:m_pDialogLayout(NULL)
,m_uStoryID(0)
{
}

PlotControl::~PlotControl()
{
	CC_SAFE_RELEASE_NULL(m_pDialogLayout);
}

DialogLayout* PlotControl::getDialogLayout() {
	if(m_pDialogLayout == NULL) {
		m_pDialogLayout = DialogLayout::create();
		m_pDialogLayout->retain();
	}

	return m_pDialogLayout;
}

void PlotControl::setStoryID(unsigned int uStoryID) {
	m_uStoryID = uStoryID;
	
	if(m_pDialogLayout != NULL) {
		m_pDialogLayout->setStoryID(uStoryID);
	}
}

void PlotControl::onStoryEnd() {
	const StoryCfg *pStoryCfg = STORY_CFG_MASTER->getStoryCfg(m_uStoryID);
	const vmsg::CSTask &stTask = P_TASK_CTRL->getCurTask();

	if(pStoryCfg != NULL) {
		std::string url = "";

		switch(pStoryCfg->storytype()) {
			case STORY_RECV_TASK:
				if(stTask.taskstat() == vmsg::TASK_STAT_RECEIVED) {
					P_TASK_CTRL->autoTask();

					if(P_GUIDE_CTRL->isGuideDuring()) {
						P_GUIDE_CTRL->execStep();
					}
				} else {
					url = SOUND_RES_PATH + "ut00028.ogg";
					//SimpleAudioEngine::sharedEngine()->preloadEffect(url.c_str());
					//SimpleAudioEngine::sharedEngine()->playEffect(url.c_str());
					AudioManager::preloadEffect(url.c_str());
					AudioManager::playEffect(url.c_str());
					P_TASK_CTRL->sendRecvTaskRqst();

					if(P_GUIDE_CTRL->isGuideDuring()) {
						P_GUIDE_CTRL->execStep();
					}
				}
				break;
			case STORY_COMMIT_TASK:
				url = SOUND_RES_PATH + "ut00029.ogg";
				//SimpleAudioEngine::sharedEngine()->preloadEffect(url.c_str());
				//SimpleAudioEngine::sharedEngine()->playEffect(url.c_str());
				AudioManager::preloadEffect(url.c_str());
				AudioManager::playEffect(url.c_str());
				P_TASK_CTRL->sendCommitTaskRqst();

				if(P_GUIDE_CTRL->isGuideDuring()) {
					P_GUIDE_CTRL->execStep();
				}
				break;
			case STORY_GUIDE_STEP:
				if(P_GUIDE_CTRL->isGuideDuring() && P_GUIDE_CTRL->isStoryStep()) {
					P_GUIDE_CTRL->nextStep();
				}
				break;
			default:;
		}
	}
}

void PlotControl::showDialog() {
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCScene *pScene = pDirector->getRunningScene();

	if(pScene != NULL) {
		CCLayer *pLayer = dynamic_cast<BaseScene*>(pScene)->getLayer(LAYER_ID_TOP);
		UILayer *pTopLayer = dynamic_cast<UILayer*>(pLayer);

		if(m_pDialogLayout != NULL && m_pDialogLayout->getParent() == NULL) {
			pTopLayer->addWidget(m_pDialogLayout);
			m_pDialogLayout->showDialog();
		}
	}
}

void PlotControl::hideDialog() {
	if(m_pDialogLayout != NULL && m_pDialogLayout->getParent() != NULL) {
		m_pDialogLayout->removeFromParent();
	}
}
