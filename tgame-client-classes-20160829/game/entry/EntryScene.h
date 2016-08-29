#ifndef EntryScene_h__
#define EntryScene_h__

#include "basic/BaseScene.h"

class EntryScreenView;
class HeadLayout;

class EntryScene: public BaseScene
{
public:
	EntryScene();
	virtual ~EntryScene();

	static EntryScene* create();

	// override start

	virtual bool init();

	virtual void onEnter();

	virtual void onExit();

	virtual void loadRes();

	virtual void build();

	// override end

private:
	EntryScreenView *m_pScreenView;

};

#endif // EntryScene_h__