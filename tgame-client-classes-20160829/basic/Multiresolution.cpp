#include "Multiresolution.h"

static CCSize s_stInnerSizeInPoints = CCSizeZero;

static float s_fContentScaleFactor = 0.0f;

Multiresolution::Multiresolution()

{
}

Multiresolution::~Multiresolution()
{
}

void Multiresolution::setDesignResolutionSize(float width, float height) {
	s_stInnerSizeInPoints.setSize(width, height);
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCEGLView *pEGLView = pDirector->getOpenGLView();
	CCSize frameSize = pEGLView->getFrameSize();
	
	float scaleX = frameSize.width / width;
	float scaleY = frameSize.height / height;

	float scale = 0.0f;

	if(scaleX > scaleY) {
		scale = scaleX / scaleY;
	} else {
		scale = scaleY / scaleX;
	}

	s_fContentScaleFactor = scale;
	pEGLView->setDesignResolutionSize(width * scale, height * scale, kResolutionNoBorder);
}

const CCSize& Multiresolution::getInnerSize() {
	return s_stInnerSizeInPoints;
}

CCSize Multiresolution::getInnerSizeInPixels() {
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCEGLView *pEGLView = pDirector->getOpenGLView();
	CCSize frameSize = pEGLView->getFrameSize();
	CCSize visibleSize = pEGLView->getVisibleSize();

	float scale = frameSize.width / visibleSize.width;

	return CCSizeMake(s_stInnerSizeInPoints.width * scale, s_stInnerSizeInPoints.height * scale);
}

CCPoint Multiresolution::getInnerOrigin() {
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCSize visibleSize = pDirector->getVisibleSize();
	CCPoint visibleOriginPos = pDirector->getVisibleOrigin();

	float posX = visibleOriginPos.x + (visibleSize.width - s_stInnerSizeInPoints.width) / 2;
	float posY = visibleOriginPos.y + (visibleSize.height - s_stInnerSizeInPoints.height) / 2;

	return ccp(posX, posY);
}
