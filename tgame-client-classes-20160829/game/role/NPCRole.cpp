#include "NPCRole.h"
#include "Resources.h"
#include "CCColorUtil.h"
#include "GlobalTime.h"
#include "vlib/CVRand.h"
#include "defines.h"
#include "task/TaskControl.h"

#include "TaskCfgMaster.h"
#include "NPCCfgMaster.h"
#include "BubbleLibCfgMaster.h"

using namespace vmsg;

#define BUBBLE_TAG 202

NPCRole::NPCRole()
:m_stNPCName("")
,_resPath("")
,_cfgID(0)
,mArmature(NULL)
,m_pNameTxt(NULL)
,m_pTaskArmature(NULL)
,m_uBubbleID(0)
{
	m_uBubbleTime = GlobalTime::getTime();
	m_uLoopCnt = random_range(4, 10);
    m_iBubbleCnt = 0;
    m_iCheckInterval = random_range(100, 600);
}

NPCRole::~NPCRole(){

}

void NPCRole::onEnter(){
	CCNode::onEnter();
	scheduleUpdate();
}

void NPCRole::onExit(){
	unscheduleUpdate();
	CCNode::onExit();
	this->removeAllChildrenWithCleanup(true);
}

bool NPCRole::init(){
	return true;
}

void NPCRole::update(float delta){
    m_iBubbleCnt++;
    if (m_iBubbleCnt >= m_iCheckInterval)
    {
        updateBubbleDialog(); //速度控制
        m_iBubbleCnt = 0;
    }
	
}

void NPCRole::updateBubbleDialog(){
	unsigned int iNow = GlobalTime::getTime();
	const BubbleLibCfg* stCfg = BUBBLE_LIB_CFG->GetBubbleLibCfgByID(m_uBubbleID);

	//气泡cd时间
	if((iNow - m_uBubbleTime) >= (stCfg->cdtime() + CRandom::GetRandHaveMax(15)*4)){
		unsigned int roll = CRandom::GetRandHaveMax(10000);
		if(roll < stCfg->prob()){
			unsigned int idx = CRandom::GetRand(stCfg->words_size());
			CBubbleDialog* pBubble = CBubbleDialog::create();
			pBubble->setPosition(ccp(0, getContentSize().height - 10));
			pBubble->addBubbleEventListener(this, bubble_selector(NPCRole::bubbleEventHandler));
			addChild(pBubble, 8, BUBBLE_TAG);
			pBubble->setString(stCfg->words(idx));
		}

		m_uBubbleTime = iNow;
	}
}

void NPCRole::bubbleEventHandler(CCObject *pSender, BubbleEventType type){
	CBubbleDialog *pDialogLayout = dynamic_cast<CBubbleDialog*>(pSender);

	switch(type) {
	case BUBBLE_OVER:
		if(pDialogLayout->getParent() != NULL){
			pDialogLayout->removeFromParent();
		}
		break;
	default:
		break;
	}
}

void NPCRole::setCfgID(unsigned int var){
	_cfgID = var;
	NPCCfg* stCfg = NPC_CFG_MASTER->getNPCCfgByID(_cfgID);
	_resPath = stCfg->showid();
	m_stNPCName = stCfg->npcname();

	m_uBubbleTime = GlobalTime::getTime();
	m_uBubbleID = stCfg->bubbleid();
}

unsigned int NPCRole::getCfgID(){
	return _cfgID;
}

void NPCRole::LoadActionRes(){
	if(mArmature == NULL && strcmp(getResPath().c_str(), "null") != 0){
		mArmature = CCArmature::create(_resPath.c_str());
		setContentSize(mArmature->getContentSize());
		addChild(mArmature);
	}

	//CCLabelTTF* m_pNameTxt = CCLabelTTF::create(m_stNPCName.c_str(), "fonts/DroidSansFallback.ttf", 28);
    UILabel* m_pNameTxt = UILabel::create();
    m_pNameTxt->setFontName("fonts/tgamefont.ttf");
    m_pNameTxt->setFontSize(28);
   
    
    m_pNameTxt->setPosition(ccp(0, getContentSize().height + 10));
    m_pNameTxt->setColor(ccc3(0xFE, 0xF8, 0x97));
	//CCColorUtil::addStroke(m_pNameTxt, 1.0f, ccc3(0xFE, 0xF8, 0x97));
    CCColorUtil::addStroke(m_pNameTxt, 0.8f, ccc3(0xFF, 0x00, 0x00));
    CCColorUtil::addShadow(m_pNameTxt);
   
    m_pNameTxt->setText(m_stNPCName);
	addChild(m_pNameTxt, 2);

	//this->setScale(0.8);  //TODO

	mArmature->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(NPCRole::MovementCallback));

	if (mArmature->getAnimation()->getAnimationData()->getMovement("stand")){
		mArmature->getAnimation()->play("stand", -1, -1, 1);
	}else if (mArmature->getAnimation()->getAnimationData()->getMovement("stand1")){
		mArmature->getAnimation()->play("stand1", -1, -1, 1);
	}
	
}

void NPCRole::MovementCallback(CCArmature * armature, MovementEventType type, const char * org_name) {  
	if (type == LOOP_COMPLETE)  {  
		if (strcmp(org_name, "stand1") == 0){
			m_uLoopCnt--;
			if (m_uLoopCnt == 0){
				m_uLoopCnt = random_range(4, 10);
				if (mArmature->getAnimation()->getAnimationData()->getMovement("stand2")){
					mArmature->getAnimation()->play("stand2", -1, -1, 1);
				}
				
			}
		}else if (strcmp(org_name, "stand2") == 0 && mArmature->getAnimation()->getAnimationData()->getMovement("stand1")){
			mArmature->getAnimation()->play("stand1", -1, -1, 1);
		}
	}
}

void NPCRole::updateTaskState(){
	if(mArmature == NULL){
		return;
	}

	const CSTask &stTask = P_TASK_CTRL->getCurTask();
	unsigned int uMaxTaskID = P_TASK_CTRL->getMaxTaskID();

	if(!stTask.has_taskid() || !TASK_CFG_MASTER->isValid(stTask.taskid())) {
		if(m_pTaskArmature != NULL){
			m_pTaskArmature->removeFromParentAndCleanup(true);
			m_pTaskArmature = NULL;
		}
		return;
	}

	const TaskCfg *pTaskCfg = TASK_CFG_MASTER->getTaskCfgByID(stTask.taskid());
	unsigned int uTaskStat = stTask.taskstat();
	unsigned int uNPCID = 0;
	unsigned int uPreNPCID = 0;

	switch(stTask.taskstat()) {
	case vmsg::TASK_STAT_ORG:
		uNPCID = pTaskCfg->getnpcid();
		break;
	case vmsg::TASK_STAT_RECEIVED:
	case vmsg::TASK_STAT_FINISH:
	case vmsg::TASK_STAT_OVER:
		uNPCID = pTaskCfg->commitnpcid();
		break;
	default:;
	}

	if(uNPCID != _cfgID){
		if(m_pTaskArmature != NULL){
			m_pTaskArmature->removeFromParentAndCleanup(true);
			m_pTaskArmature = NULL;
		}
		return;
	}

	if(m_pTaskArmature == NULL){
		m_pTaskArmature = CCArmature::create("ut00020");
		float yy = getContentSize().height + 40;
		m_pTaskArmature->setPosition(ccp(0, yy));
		addChild(m_pTaskArmature);
	}

	if(uTaskStat == TASK_STAT_ORG){		//未接取
		m_pTaskArmature->getAnimation()->play("fly_t_xh", -1, -1, 1);
	}else if(uTaskStat == TASK_STAT_RECEIVED){	//已接取
		m_pTaskArmature->getAnimation()->play("fly_wh_xh", -1, -1, 1);
	}else if(uTaskStat == TASK_STAT_FINISH){	//已完成
		m_pTaskArmature->getAnimation()->play("fly_w_xh", -1, -1, 1);
	}else if(uTaskStat == TASK_STAT_OVER){	//已提交
		m_pTaskArmature->removeFromParentAndCleanup(true);
		m_pTaskArmature = NULL;
	}else{
		m_pTaskArmature->removeFromParentAndCleanup(true);
		m_pTaskArmature = NULL;
	}
}

