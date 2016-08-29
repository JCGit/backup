#include "Enemy.h"
#include "GlobalTime.h"
#include "vlib/CVRand.h"
#include "control/AudioManager.h"

#include "BubbleLibCfgMaster.h"

#define BUBBLE_TAG 202

CEnemy::CEnemy()
:_monsterObj(NULL)
,m_stHeadRes("")
{

}

CEnemy::~CEnemy(){
	CC_SAFE_RELEASE_NULL(_monsterObj);
}

bool CEnemy::init()
{
	setRoleParty(ROLE_PARTY_HOSTILE);
	SetIsControlledByAI(true);

	_isBubbleOn = true;
	m_uBubbleTime = GlobalTime::getTime();
	m_uBubbleID = 0;

	m_iBubbleCnt = 0;
	m_iCheckInterval = random_range(100, 600);

	return true;
}

void CEnemy::setScale(float fScale){
	CQStageEntity::setScale(fScale);
}

void CEnemy::setScaleX(float fScaleX){
	float paramScale = 1.0f;
	if(m_uInitScale == 1){
		paramScale = -1.0f;
	}
	CQStageEntity::setScaleX(fScaleX*paramScale);
}

float CEnemy::getScaleX(){
	float paramScale = 1.0f;
	if(m_uInitScale == 1){
		paramScale = -1.0f;
	}
	float realScaleX = CQStageEntity::getScaleX();
	return paramScale*realScaleX;
}

void CEnemy::SleepAct(){
	getBlood()->setVisible(false);
	setIsActivate(false);
	SetIsControlledByAI(false);

	//播放休眠动作
	if(getArmature() && getArmature()->getAnimation()->getAnimationData()->getMovement("sleep")){
		getArmature()->getAnimation()->play("sleep", -1, -1, -1);
		setShadowVisiable(false);
	}
}

void CEnemy::Active(){
	//播放激活动作
	if(getArmature()){
		getArmature()->getAnimation()->play("activate", -1, -1, 0);
	}

	
}

void CEnemy::MovementCallback(CCArmature * armature, MovementEventType type, const char * name){
	CQStageEntity::MovementCallback(armature, type, name);

	if(type == COMPLETE){
		if(strcmp(name, "activate") == 0){
			if(_monsterObj->type != APPEAR_SLEEP){  //雕像怪已经加入舞台
				DispatchEvent(ACTIVE_COMPLETE_EVENT, this);  //加入舞台数组
			}

			setIsActivate(true);
			getBlood()->setVisible(true);
			SetIsControlledByAI(true);
			setShadowVisiable(true);
		}
	}
}

void CEnemy::update(float dt)
{
	CQStageEntity::update(dt);


	m_iBubbleCnt++;
	if (_isBubbleOn && m_iBubbleCnt >= m_iCheckInterval && getIsActivate())
	{
		updateBubbleDialog(); //速度控制
		m_iBubbleCnt = 0;
	}

	//to do AI or else
}

void CEnemy::updateBubbleDialog(){
	return;

	unsigned int iNow = GlobalTime::getTime();
	const BubbleLibCfg* stCfg = BUBBLE_LIB_CFG->GetBubbleLibCfgByID(m_uBubbleID);
	if(stCfg == NULL){
		return;
	}

	//气泡cd时间
	if(iNow - m_uBubbleTime >= 10 + CRandom::GetRandHaveMax(15)*4){
		unsigned int roll = CRandom::GetRandHaveMax(10000);
		if(roll < stCfg->prob()){
			unsigned int idx = CRandom::GetRand(stCfg->words_size());
			CBubbleDialog* pBubble = CBubbleDialog::create();
			pBubble->setPosition(ccp(0, getContentSize().height - 10));
			pBubble->addBubbleEventListener(this, bubble_selector(CEnemy::bubbleEventHandler));
			addChild(pBubble, 8, BUBBLE_TAG);
			pBubble->setString(stCfg->words(idx));
		}

		m_uBubbleTime = iNow;
	}
}

void CEnemy::bubbleEventHandler(CCObject *pSender, BubbleEventType type){
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

void CEnemy::setMonsterObj(CEnemyData* var){
	_monsterObj = var;
	CC_SAFE_RETAIN(_monsterObj);
	if(_monsterObj->id != 0){
		//m_uBubbleID = MONSTER_CFG_MASTER->GetMonsterCfgByID(_monsterObj->id)->bubbleid();
	}else{ //无尽模式中合成的怪物配置，不会出气泡
		m_uBubbleID = 0;
		_isBubbleOn = false;
	}

	//暂时关闭
	_isBubbleOn = false;
}

CEnemyData* CEnemy::getMonsterObj(){
	return _monsterObj;
}

void CEnemy::HurtWithDamage(CHurtDamageObj stObj){
	CQStageEntity::HurtWithDamage(stObj);
}

void CEnemy::HurtWithDamage(float fDamage){
	if(_monsterObj->type == APPEAR_SLEEP && getIsActivate() == false){  //雕像怪受伤就被激活
		Active();
		return;
	}

	CQStageEntity::HurtWithDamage(fDamage);
}

void CEnemy::ShowHurt(int fDamage)
{
	DispatchEvent(ENEMY_HURT_EVENT, this);				//发出受伤时的事件，展示界面上怪物血条
    if(fDamage > 0 && _monsterObj->type != APPEAR_AFTER_DIE) {  //死亡后不停刷怪机制输出的怪血量伤害不计入总伤害
        CCString* strValue = CCString::createWithFormat("%d", fDamage);
        DispatchEvent(HERO_HARMVALUE_CHANGE, strValue);		//怪物受伤的数值上报，计入总伤害 
    }
	
}

void CEnemy::HandleEntityDeath(){
	this->DispatchEvent(CQStageEntity::DIE_COMPLETE_EVENT, this);
}

void CEnemy::HPChange(){
	if(getRoleType() == ROLE_TYPE_JUNIOR_BOSS || getRoleType() == ROLE_TYPE_SENIOR_BOSS){  //boss
		
	}else{
		if(_state == kActionStateDie || _state == kActionStateDying){
			return;
		}

		if(_blood == NULL){
			return;
		}

		float per;
		if(getHP() <= 0){
			per = 0;
		}else{
			per = 100.0*getHP()/getMaxHP();
		}

		_blood->setPercentage(per);
	}
}

void CEnemy::OtherHurt(){
	m_stHurtAnimation = _obj->hurtResPath;
	if (m_stHurtAnimation.length() && m_stHurtAnimation != "null"){
		CCArmature* pstHurtAnimation = CCArmature::create(m_stHurtAnimation.c_str());
		if (!pstHurtAnimation){
			return;
		}

		pstHurtAnimation->getAnimation()->play("fly", -1, -1, 0);
		getArmature()->addChild(pstHurtAnimation);
		pstHurtAnimation->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CQStageEntity::HandleBeatenEffActionEvent));
	}
}

void CEnemy::Emerge(){
	string emergeStr = m_stMovements.emergeAct;
	if (mArmature->getAnimation()->getAnimationData()->getMovement(emergeStr.c_str())){
		mArmature->getAnimation()->play(emergeStr.c_str(), -1, -1, 0);
		_state = kActionStateEmerge;
	}else{
		Idle();
	}

	if (getEmergeVoice().length() && strcmp(getEmergeVoice().c_str(), "null")){
		AudioManager::playEffect(getEmergeVoice().c_str());
	}

	_isEmerge = true;
}

void CEnemy::WalkToDstDoneCallback() {
	CQStageEntity::WalkToDstDoneCallback();
	DispatchEvent("walk_done", this);
}
