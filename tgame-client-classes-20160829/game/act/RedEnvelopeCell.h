#ifndef RedEnvelopeCell_h__
#define RedEnvelopeCell_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class RedEnvelopeCell: public Widget
{
public:
	enum EnvelopeStat {
		ENVELOPE_CLOSED,
		ENVELOPE_RECEIVED,
		ENVELOPE_OPENED,
	};
public:
	RedEnvelopeCell();
	virtual ~RedEnvelopeCell();

	static RedEnvelopeCell* create();

	// override start

	virtual bool init();

	// override end

	unsigned int getDay();

	void setDay(unsigned int uDay);

	void setStat(EnvelopeStat stat);

	void openEnvelope();

private:
	void onEnvelopeOpened(CCArmature *pArmature, MovementEventType type, const char *pszActionName);

private:
	UILabel *m_pDayTxt;

	CCArmature *m_pEnvelope;

	EnvelopeStat m_stat;

	unsigned int m_uDay;

};

#endif // RedEnvelopeCell_h__