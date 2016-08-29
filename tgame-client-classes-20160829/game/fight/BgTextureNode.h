#ifndef BgTextureNode_h__
#define BgTextureNode_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

class CBgTextureNode : public CCNode
{
public:
	CREATE_FUNC(CBgTextureNode);

	CBgTextureNode();
	virtual ~CBgTextureNode();

	void AddSubTextureName(string& stName, bool bIsTopAlign = false);	
	void ComposeTexture();

	void AddNewTexture(string& stName, bool bIsTopAlign = false);	
	void DelTextureByIdx(unsigned int index);
	void DelPreTexture();

	void UpdateTextures();
protected:
private:
	CC_SYNTHESIZE(CCArray*, _subTextures, SubTextures);
	CC_SYNTHESIZE(CCArray*, _subTextureNames, SubTextureNames);
	CC_SYNTHESIZE(CCSize, m_stTotalSize, TotalSize);

	bool m_bIsTopAlign;
};

#endif // BgTextureNode_h__
