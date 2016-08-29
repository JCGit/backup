#include "ImageNumber.h"
#include "defines.h"
#include "Resources.h"
#include "vlib/CVRand.h"
#include "control/AudioManager.h"

using namespace CVLib;

ImageNumber::ImageNumber(){
	m_iNum = "";
	m_stFileStr = "";

	m_fInterX = 0;
	m_iStartNum = 0;
	m_iEndNum = 0;
	m_iRollNum = 0;
	m_iRollNum = 0;
	m_iBitNum = 0;
	m_iReapeatNum = 0;
    m_iRemainRepeatNum = 0;

    m_pOnRollDoneTarget = NULL;
    m_pOnRollDoneCallFunc = NULL;
}

ImageNumber::~ImageNumber(){

}

ImageNumber* ImageNumber::create(unsigned int numIdx){
	ImageNumber *pRet = new ImageNumber();

	if(pRet != NULL && pRet->initWithNum(numIdx)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ImageNumber::initWithNum(unsigned int numIdx){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!CCNode::init());

		setAnchorPoint(ccp(0.5, 0.5));

		m_stFileStr = "number_" + intToString(numIdx);
		string plistFile = NUM_RES_PATH + m_stFileStr + PLIST_TYPE_NAME;
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistFile.c_str());

		string fileStr = NUM_RES_PATH + m_stFileStr + PNG_TYPE_NAME;
		m_pNode = CCSpriteBatchNode::create(fileStr.c_str());
		addChild(m_pNode);

		setNum("0");

		bRet = true;
	} while (0);

	return bRet;
}

void ImageNumber::setNum(const string number, float interX /* = 0 */, const string operatorStr /* = "" */, unsigned int operatorIdx/* = 0*/){
	m_pNode->removeAllChildrenWithCleanup(true);
	m_iNum = number;

	if(number == ""){
		return;
	}

	float width = 0;
	float height = 0;

	unsigned int numLen = number.length();
	if(operatorStr != "" && operatorIdx < number.length()){
		numLen += 1;
	}

	//符号加在前面
	string fileStr;
	unsigned int numIdx = 0;
	float startX = 0;
	for (unsigned int i = 0; i < numLen; i++){
		if(operatorStr != ""){
			if(operatorIdx == i){
				fileStr = m_stFileStr + "/" + operatorStr + PNG_TYPE_NAME;
			}else{
				string oneNumber =  number.substr(numIdx,1);
				fileStr = m_stFileStr + "/" + oneNumber + PNG_TYPE_NAME;
				numIdx++;
			}
		}else{
			string oneNumber =  number.substr(numIdx,1);
			fileStr = m_stFileStr + "/" + oneNumber + PNG_TYPE_NAME;
			numIdx++;
		}
		
		CCSprite* oneNumSp = CCSprite::createWithSpriteFrameName(fileStr.c_str());
		oneNumSp->setAnchorPoint(ccp(0, 0.5));
		oneNumSp->setPosition(ccp(startX, 0));
		m_pNode->addChild(oneNumSp);
		startX += oneNumSp->getContentSize().width + (interX)*i;

		height = oneNumSp->getContentSize().height;
		width += oneNumSp->getContentSize().width;
	}

	width += (numLen - 1)*interX;
	setContentSize(CCSize(width, height));

	//CCLOG("the ImageNumber is number === %s, width === %f, height === %f", number.c_str(), width, height);
}

void ImageNumber::showNumWithAction(unsigned int number, float interX, int reapeatNum){
	m_iRollNum = 0;
	m_iEndNum = number;
	m_iReapeatNum = reapeatNum;
	m_fInterX = interX;

	m_iBitNum = (int)ceil(log10(number));			//10为底的对数
	m_iRandNum = (int)::pow(10, m_iBitNum - 1);			//最小值
	m_iStartNum = (int)::pow(10, m_iBitNum) - 1;			//10的幂次方后 -1，即9999等

	this->setNum(intToString(number), interX);
	this->schedule(schedule_selector(ImageNumber::rollNum), 0.03f);
}

void ImageNumber::rollNum(float dt){
	m_iRollNum++;
	int randNum = 0;;
	for(int i = 0; i < m_iBitNum; i++){
		randNum += CRandom::GetRandHaveMax(9)*(::pow(10, i));
	}

	AudioManager::playEffect("roll_number.ogg");

	if(m_iRollNum < m_iReapeatNum){
		this->setNum(intToString(randNum), m_fInterX);
	}else if(m_iRollNum == m_iReapeatNum){
		this->setNum(intToString(m_iEndNum), m_fInterX);
	}else{
		this->unschedule(schedule_selector(ImageNumber::rollNum));
	}
}

void ImageNumber::rollNumFromTo(int fromNum, int toNum, float interX, int reapeatNum){
	if(this->numberOfRunningActions() > 0){
		this->stopAllActions();
	}

	m_fInterX = interX;
	m_iRollNum = 0;
	m_iStartNum = fromNum;
	m_iEndNum = toNum;
	m_iRandNum = fromNum;
	m_iReapeatNum = reapeatNum;
	m_iBitNum = (int)ceil(log10(m_iEndNum));			//10为底的对数

	this->setNum(intToString(m_iStartNum), m_fInterX);

	CCFiniteTimeAction* rollFun = CCCallFunc::create(this, callfunc_selector(ImageNumber::reapeatRollNum));
    CCCallFunc *actbackfun = CCCallFunc::create(this, callfunc_selector(ImageNumber::onRollDoneCallBack));

    m_iRemainRepeatNum = m_iReapeatNum;

    CCRepeat* rep = CCRepeat::create(CCSequence::create(CCDelayTime::create(0.03f), rollFun, actbackfun, NULL), m_iReapeatNum);
    this->runAction(rep);
	//this->runAction(CCSequence::create(rep, actbackfun, NULL));
}

void ImageNumber::reapeatRollNum(){
	m_iRollNum++;
	m_iRandNum += (m_iEndNum - m_iStartNum)/m_iReapeatNum;

	AudioManager::playEffect("roll_number.ogg");

	if(m_iRollNum >= m_iReapeatNum - 1){
		if(this->numberOfRunningActions() > 0){
			this->stopAllActions();
		}

		this->setNum(intToString(m_iEndNum), m_fInterX);
		return;
	}

	this->setNum(intToString(m_iRandNum), m_fInterX);
}

void ImageNumber::onRollDoneCallBack() {
    m_iRemainRepeatNum--;
    if(m_iRemainRepeatNum == 1) {
        CCLOG("ImageNumber::onRollDoneCallBack");
        if(m_pOnRollDoneTarget && m_pOnRollDoneCallFunc) {
            (m_pOnRollDoneTarget->*m_pOnRollDoneCallFunc)();
        }
    }
}

