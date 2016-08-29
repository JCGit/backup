#include "SceneBuild.h"

CSceneBuild::CSceneBuild(){

}

CSceneBuild::~CSceneBuild(){

}

bool CSceneBuild::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!CCNode::init());

		bRet = true;
	} while (0);

	return bRet;
}

void CSceneBuild::createWithTexture(CCTexture2D *pTexture){
	picture = CCSprite::createWithTexture(pTexture);
	addChild(picture);
	setContentSize(picture->getContentSize());
}