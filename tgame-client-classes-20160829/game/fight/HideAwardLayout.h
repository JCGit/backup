#ifndef HideAwardLayout_h__
#define HideAwardLayout_h__

#include "BaseLayout.h"

class CHideAwardLayout : public BaseLayout
{
public:
	CHideAwardLayout();
	virtual ~CHideAwardLayout();

	CREATE_FUNC(CHideAwardLayout);

	bool init();

	void openAwardBox();

protected:
private:
	void addOneArm();
	void addOneTxt(CCArmature* pArm);
	void nextArm(CCArmature* pArm);
	void handleArmComplete(CCArmature * armature, MovementEventType type, const char * name);

	void onShowEnd(float dt);

	UILayout* m_pLayer;

	CCArray* m_stAniArr;

	unsigned int m_uAwardSize;
	unsigned int m_uAwardIdx;
};

#endif // HideAwardLayout_h__
