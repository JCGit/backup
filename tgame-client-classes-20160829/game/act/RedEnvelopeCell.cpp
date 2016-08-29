#include "RedEnvelopeCell.h"

#include "basic/TextHelper.h"

RedEnvelopeCell::RedEnvelopeCell()
:m_pDayTxt(NULL)
,m_pEnvelope(NULL)
,m_stat(ENVELOPE_CLOSED)
,m_uDay(0)
{
}

RedEnvelopeCell::~RedEnvelopeCell()
{
}

RedEnvelopeCell* RedEnvelopeCell::create() {
	RedEnvelopeCell *pRet = new RedEnvelopeCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool RedEnvelopeCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!Widget::init());

		ignoreContentAdaptWithSize(false);
		setSize(CCSizeMake(80, 132));

		m_pEnvelope = CCArmature::create("ut00087");
		addNode(m_pEnvelope);

		m_pDayTxt = UILabel::create();
		m_pDayTxt->setFontName("fonts/tgamefont.ttf");
		m_pDayTxt->setFontSize(24);
		m_pDayTxt->setColor(ccc3(0xff, 0xf0, 0x00)); //fff000
		m_pDayTxt->setTouchEnabled(false);
		m_pDayTxt->setAnchorPoint(ccp(0.5, 0.5));
		m_pDayTxt->setPositionY(72);
		dynamic_cast<CCLabelTTF*>(m_pDayTxt->getVirtualRenderer())->enableShadow(CCSizeMake(2, 2), 0.4f, 1.0f);
		addChild(m_pDayTxt);

		setStat(m_stat);

		bRet = true;
	} while(0);

	return bRet;
}

unsigned int RedEnvelopeCell::getDay() {
	return m_uDay;
}

void RedEnvelopeCell::setDay(unsigned int uDay) {
	m_uDay = uDay;

	m_pDayTxt->setText(TextStr(TEXT_ACT_SPRING_DAY_1 + uDay - 1));
}

void RedEnvelopeCell::setStat(EnvelopeStat stat) {
	m_stat = stat;

	if(m_stat == ENVELOPE_CLOSED) {
		setTouchEnabled(false);
		m_pEnvelope->getAnimation()->play("fly_3", -1, -1, 0);
	} else if(m_stat == ENVELOPE_RECEIVED) {
		setTouchEnabled(true);
		m_pEnvelope->getAnimation()->play("fly_0", -1, -1, 1);
	} else if(m_stat == ENVELOPE_OPENED) {
		setTouchEnabled(false);
		m_pEnvelope->getAnimation()->play("fly_2", -1, -1, 0);
	}
}

void RedEnvelopeCell::openEnvelope() {
	m_pEnvelope->getAnimation()->play("fly_1", -1, -1, 0);
	m_pEnvelope->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(RedEnvelopeCell::onEnvelopeOpened));
}

void RedEnvelopeCell::onEnvelopeOpened(CCArmature *pArmature, MovementEventType type, const char *pszActionName) {
	if(type == COMPLETE) {
		if(pArmature->getName() == "ut00087") {
			setStat(ENVELOPE_OPENED);
		}
	}
}
