#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"

#include <string>
#include <map>

USING_NS_CC;
USING_NS_CC_EXT;

class CCBContainer : public cocos2d::CCNode
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
{
private:
	static unsigned long containterCount;
public:
	class CCBContainerListener
	{
	public:
		virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender, int tag){};
		virtual void onAnimationDone(const std::string& animationName){};
	};

	CREATE_FUNC(CCBContainer);

	CCBContainer(void);
	CCBContainer(CCBContainerListener*, int tag = 0);
	virtual ~CCBContainer(void);

	virtual void onAnimationDone(const std::string& animationName){};
	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender){};
	virtual void unload();
	virtual bool getLoaded();

	virtual void loadCcbiFile(const std::string& filename, bool froceLoad = false);
	void runAnimation(const std::string& actionname);
	void playAutoPlaySequence();
	cocos2d::CCObject* getVariable(const std::string& variablename);
	void setListener(CCBContainerListener* listener, int tag = 0);
	void setAllChildColor(unsigned char r, unsigned char g, unsigned char b);
	
	
	CCNode* getCCNodeFromCCB(const std::string& variablename);
	CCSprite* getCCSpriteFromCCB(const std::string& variablename);
	CCLabelBMFont* getCCLabelBMFontFromCCB(const std::string& variablename);
	CCLabelTTF* getCCLabelTTFFromCCB(const std::string& variablename);
	//CCBFileNew* getCCBFileFromCCB(const std::string& variablename);
	CCMenuItemImage* getCCMenuItemImageFromCCB(const std::string& variablename);
	CCScrollView* getCCScrollViewFromCCB(const std::string& variablename);
	CCMenuItemCCBFile* getCCMenuItemCCBFileFromCCB(const std::string& variablename);

	static void updateRichTextNode(cocos2d::CCNode* node);

	virtual bool init();

	virtual std::string dumpInfo();

	friend class cocos2d::extension::CCNodeLoader;
	friend class cocos2d::extension::CCBAnimationManager;
private:
	//Do NOT use the fellow functions
	/** use this to cancel the selector method*/
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;}

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode) ;
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelectorWithSender(cocos2d::CCObject * pTarget, const char* pSelectorName, cocos2d::CCNode* sender);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;}
	void _animationDone();
	void _menuItemClicked(cocos2d::CCObject * pSender);

private:
	cocos2d::extension::CCBAnimationManager *mActionManager;
	typedef std::map<std::string, cocos2d::CCObject*> VARIABLE_MAP;
	VARIABLE_MAP mVariables;

	typedef std::map<cocos2d::CCObject*, std::string> MENUITEM_MAP;
	MENUITEM_MAP mMenus;

	CCBContainerListener* mCCBContainerListener;


	int mCCBTag;
	std::string mLoadedCCBFile;
};

/**
example : 
CCBContainer *page = some_ccb_file;
CCB_FUNC(page,"mSkillBig",CCNode,removeAllChildren());
*/
#define CCB_FUNC(_ccb_,_var_,_type_,_func_) { \
	CCObject* __obj__ = _ccb_->getVariable(_var_); \
	_type_* __node__ = __obj__?dynamic_cast<_type_*>(__obj__):0; \
	if(__node__) __node__-> _func_;}

#define CCB_FUNC_R(_ccb_,_var_,_type_,_func_) { \
	CCObject* __obj__ = _ccb_.getVariable(_var_); \
	_type_* __node__ = __obj__?dynamic_cast<_type_*>(__obj__):0; \
	if(__node__) __node__-> _func_;}

#define CCB_FUNC_RETURN(_ccb_,_var_,_type_,_func_,_ret_) { \
	CCObject* __obj__ = _ccb_->getVariable(_var_); \
	_type_* __node__ = __obj__?dynamic_cast<_type_*>(__obj__):0; \
	if(__node__) _ret_ = __node__-> _func_;}

