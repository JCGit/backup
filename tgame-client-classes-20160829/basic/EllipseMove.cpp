#include "EllipseMove.h"

EllipseMove::EllipseMove()
{
}

EllipseMove::~EllipseMove()
{
}

EllipseMove* EllipseMove::create(float duration, const EllipseInfo &info) {
	EllipseMove *pRet = new EllipseMove();

	if(pRet != NULL && pRet->initWithDuration(duration, info)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EllipseMove::initWithDuration(float duration, const EllipseInfo &info) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!CCActionInterval::initWithDuration(duration));

		m_stEllipseInfo = info;

		bRet = true;
	} while(0);

	return bRet;
}

void EllipseMove::startWithTarget(CCNode *pTarget) {
	CCActionInterval::startWithTarget(pTarget);
}

void EllipseMove::stop() {
	CCActionInterval::stop();
}

void EllipseMove::update(float time) {
	if(m_pTarget != NULL) {
		float x = 0.0f;
		float y = 0.0f;

		float startRadian = m_stEllipseInfo.startRadian;
		float deltaRadian = m_stEllipseInfo.deltaRadian;

		if(m_stEllipseInfo.clockwise) {
			x = m_stEllipseInfo.a * cos(startRadian - deltaRadian * time);
			y = m_stEllipseInfo.b * sin(startRadian - deltaRadian * time);
		} else {
			x = m_stEllipseInfo.a * cos(startRadian + deltaRadian * time);
			y = m_stEllipseInfo.b * sin(startRadian + deltaRadian * time);
		}

		m_pTarget->setPosition(ccpAdd(m_stEllipseInfo.center, ccp(x, y)));
		float scale = 0.4f - 0.6f * (y - m_stEllipseInfo.b) / (2 * m_stEllipseInfo.b);
		m_pTarget->setScale(scale);
		m_pTarget->setZOrder(m_stEllipseInfo.b - y);
	}
}

CCActionInterval* EllipseMove::reverse() {
	EllipseInfo info;
	info.center = m_stEllipseInfo.center;
	info.a = m_stEllipseInfo.a;
	info.b = m_stEllipseInfo.b;
	info.startRadian = m_stEllipseInfo.startRadian + m_stEllipseInfo.deltaRadian;
	info.deltaRadian = m_stEllipseInfo.deltaRadian;
	info.clockwise = !m_stEllipseInfo.clockwise;
	return EllipseMove::create(m_fDuration, info);
}
