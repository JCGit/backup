#include "CCColorUtil.h"  

void CCColorUtil::addGray(CCNode* sp){  
	sp->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureGray));
}  

void CCColorUtil::removeGray(CCNode* sp){  
	 sp->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
} 

void CCColorUtil::addShadow(UILabel *pLabel){
#ifndef WIN32
	CCLabelTTF* pLabelTTF = dynamic_cast<CCLabelTTF*>(pLabel->getVirtualRenderer());
	pLabelTTF->enableShadow(CCSizeMake(2,-2), 1.0f, 1.0f);
#endif // !WIN32
}

void CCColorUtil::addShadow(CCLabelTTF *pLabel){
#ifndef WIN32
	pLabel->enableShadow(CCSizeMake(2,-2), 1.0f, 5.0f);
#endif // !WIN32
}

void CCColorUtil::addStroke(UILabel *pLabel, float strokeSize, const ccColor3B &strokeColor){
#ifndef WIN32
	CCLabelTTF* pLabelTTF = dynamic_cast<CCLabelTTF*>(pLabel->getVirtualRenderer());
	pLabelTTF->enableStroke(strokeColor, strokeSize);
#endif // !WIN32
}

void CCColorUtil::addStroke(CCLabelTTF *pLabel, float strokeSize, const ccColor3B &strokeColor){
#ifndef WIN32
	pLabel->enableStroke(strokeColor, strokeSize);
#endif // !WIN32
}


void CCColorUtil::setLabelColor(UILabel *pLabel, const ccColor3B &strokeColor) {
    if(!pLabel) {
        return;
    }
    CCLabelTTF* pLabelTTF = dynamic_cast<CCLabelTTF*>(pLabel->getVirtualRenderer());
    if(pLabelTTF) {
        pLabelTTF->setColor(strokeColor);
    }

}



