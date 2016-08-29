#include "RollNumber.h"
#include "defines.h"
#include "vlib/CVRand.h"
#include "Resources.h"
#include "control/AudioManager.h"
#include "SimpleAudioEngine.h"
using namespace CVLib;
CRollNumber::CRollNumber(){
	m_iStartNum = 0;
	m_iEndNum = 0;
	m_iRollNum = 0;
	m_iRollNum = 0;
	m_iBitNum = 0;
	m_iReapeatNum = 0;
}

CRollNumber::~CRollNumber(){

}

CRollNumber* CRollNumber::createWithProperty(const string& stringValue, const string& charMapFile, 
											 int itemWidth, int itemHeight, const string& startCharMap){
	CRollNumber* widget = new CRollNumber();
    if (widget && widget->initWithProperty(stringValue, charMapFile, itemWidth, itemHeight, startCharMap)){
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool CRollNumber::initWithProperty(const string& stringValue, const string& charMapFile, int itemWidth, int itemHeight, const string& startCharMap){
	bool bRet = false;

	do{
		CC_BREAK_IF(!Widget::init());

		m_pNumLabel = LabelAtlas::create();	
		m_pNumLabel->setProperty(stringValue, charMapFile, itemWidth, itemHeight, startCharMap);
		this->addChild(m_pNumLabel);

		string fullPath = SOUND_RES_PATH + "roll_number.ogg";
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(fullPath.c_str());
		AudioManager::preloadEffect(fullPath.c_str());

		bRet = true;
	}while(0);

	return bRet;
}

void CRollNumber::setLabelAnchorPoint(const CCPoint &pt){
	m_pNumLabel->setAnchorPoint(pt);
}

void CRollNumber::showNumWithAction(unsigned int number, int reapeatNum){
	m_iRollNum = 0;
	m_iEndNum = number;
	m_iReapeatNum = reapeatNum;

	m_iBitNum = (int)ceil(log10(number));			//10为底的对数
	m_iRandNum = (int)::pow(10, m_iBitNum - 1);			//最小值
	m_iStartNum = (int)::pow(10, m_iBitNum) - 1;			//10的幂次方后 -1，即9999等

	m_pNumLabel->setStringValue(intToString(m_iStartNum));

	this->schedule(schedule_selector(CRollNumber::rollNum), 0.03f);
}

void CRollNumber::rollNum(float dt){
	m_iRollNum++;
	int randNum = 0;;
	for(int i = 0; i < m_iBitNum; i++){
		randNum += CRandom::GetRandHaveMax(9)*(::pow(10, i));
	}

	AudioManager::playEffect("roll_number.ogg");

	if(m_iRollNum < m_iReapeatNum){
		m_pNumLabel->setStringValue(intToString(randNum));
	}else if(m_iRollNum == m_iReapeatNum){
		m_pNumLabel->setStringValue(intToString(m_iEndNum));
	}else{
		this->unschedule(schedule_selector(CRollNumber::rollNum));
	}
}

void CRollNumber::showNumDirect(unsigned int number){
	m_pNumLabel->setStringValue(intToString(number));
}

void CRollNumber::rollNumFromTo(int fromNum, int toNum, int reapeatNum){
	if(this->numberOfRunningActions() > 0){
		this->stopAllActions();
	}

	m_iRollNum = 0;
	m_iStartNum = fromNum;
	m_iEndNum = toNum;
	m_iRandNum = fromNum;
	m_iReapeatNum = reapeatNum;
	m_iBitNum = (int)ceil(log10(m_iEndNum));			//10为底的对数

	m_pNumLabel->setStringValue(intToString(m_iStartNum));

	CCFiniteTimeAction* rollFun = CCCallFunc::create(this, callfunc_selector(CRollNumber::reapeatRollNum));
	this->runAction(CCRepeat::create(CCSequence::create(CCDelayTime::create(0.03f), rollFun, NULL), m_iReapeatNum));
}

void CRollNumber::reapeatRollNum(){
	m_iRollNum++;
	m_iRandNum += (m_iEndNum - m_iStartNum)/m_iReapeatNum;

	AudioManager::playEffect("roll_number.ogg");

	if(m_iRollNum >= m_iReapeatNum - 1){
		if(this->numberOfRunningActions() > 0){
			this->stopAllActions();
		}

		m_pNumLabel->setStringValue(intToString(m_iEndNum));
		return;
	}

	m_pNumLabel->setStringValue(intToString(m_iRandNum));
}

