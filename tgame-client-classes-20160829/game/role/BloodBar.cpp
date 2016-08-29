#include "BloodBar.h"

CBloodBar::CBloodBar()
:progress(NULL)
,progressBg(NULL)
{

}

CBloodBar::~CBloodBar(){

}

bool CBloodBar::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!CCNode::init());

		bRet = true;

	} while (0);

	return bRet;
}

CBloodBar* CBloodBar::createWithImage(const string progressName, const string progressBgName){
	CBloodBar* blood = CBloodBar::create();
	blood->initWithImage(progressName, progressBgName);
	return blood;
}


void CBloodBar::initWithImage(const string progressName, const string progressBgName){
    
    CCSprite* pSp = CCSprite::create();
    pSp->initWithSpriteFrameName(progressName.c_str());

	progress = CCProgressTimer::create(pSp);
	progressBg = CCSprite::create();
    progressBg->initWithSpriteFrameName(progressBgName.c_str());

	progress->setType(kCCProgressTimerTypeBar);
	progress->setPercentage(100);
	progress->setMidpoint(ccp(0, 1));		//设置运动方向
	progress->setBarChangeRate(ccp(1,0));

	addChild(progressBg, 0);
	addChild(progress, 1);
}

void CBloodBar::setPercentage(float fPercentage){
	float nowPer = progress->getPercentage();
	if(nowPer != fPercentage){
		//CCProgressFromTo* to = CCProgressFromTo::create(0.5, nowPer, fPercentage);
		progress->setPercentage(fPercentage);
	}
}

float CBloodBar::getPercentage(){
	return progress->getPercentage();
}