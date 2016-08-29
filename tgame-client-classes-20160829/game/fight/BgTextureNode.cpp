#include "BgTextureNode.h"
#include "ToolUtils.h"

CBgTextureNode::CBgTextureNode(){
	_subTextureNames = CCArray::create();
	_subTextureNames->retain();
	_subTextures = CCArray::create();
	_subTextures->retain();
	m_stTotalSize = CCSize(0, 0);
}

CBgTextureNode::~CBgTextureNode(){
	this->removeAllChildrenWithCleanup(true);

    if(_subTextureNames) {
        _subTextureNames->removeAllObjects();
    }
    if(_subTextures) {
        _subTextures->removeAllObjects();
    }
	CC_SAFE_RELEASE_NULL(_subTextures);
	CC_SAFE_RELEASE_NULL(_subTextureNames);
}

void CBgTextureNode::AddSubTextureName(string& stName, bool bIsTopAlign){
	CCString* pstNameObj = CCString::create((const string&)stName);

	if(CToolUtils::checkResContains(pstNameObj, _subTextureNames) == false){
		_subTextureNames->addObject(pstNameObj);
	}
	
	m_bIsTopAlign = bIsTopAlign;
}

void CBgTextureNode::ComposeTexture(){
	CCObject* obj;
	CCARRAY_FOREACH(_subTextureNames, obj){
		CCString* name = dynamic_cast<CCString*>(obj);
		if (!name){
			return;
		}
		
		//CCSprite* texture = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(name->getCString()));
        CCSprite* texture = CCSprite::create();
        texture->initWithSpriteFrameName(name->getCString());
		texture->setScale(1.5f);
        //texture->getTexture()->setAntiAliasTexParameters();
		addChild(texture);
		_subTextures->addObject(texture);

		if (!m_bIsTopAlign){
			this->setAnchorPoint(ccp(0,0));
			texture->setAnchorPoint(ccp(0, 0));
		}else{
			this->setAnchorPoint(ccp(0,1));
			texture->setAnchorPoint(ccp(0, 1));
		}

		texture->setPosition(ccp(m_stTotalSize.width, 0));
		m_stTotalSize.width += (texture->getContentSize().width*texture->getScale() - 1);
		if (m_stTotalSize.height < texture->getContentSize().height*texture->getScale()){
			m_stTotalSize.height = texture->getContentSize().height*texture->getScale();
		}

		setContentSize(m_stTotalSize);
	}
}

void CBgTextureNode::AddNewTexture(string& stName, bool bIsTopAlign){
	CCString* pstNameObj = CCString::create((const string&)stName);

	if(CToolUtils::checkResContains(pstNameObj, _subTextureNames) == false){
		_subTextureNames->addObject(pstNameObj);
	}

	CCSprite* texture = CCSprite::create();
    texture->initWithSpriteFrameName(stName.c_str());
	texture->setScale(1.5f);
	addChild(texture);


	if (!bIsTopAlign){
		texture->setAnchorPoint(ccp(0, 0));
	}else{
		texture->setAnchorPoint(ccp(0, 1));
	}

	_subTextures->addObject(texture);

	texture->setPosition(ccp(getContentSize().width, 0));

	CCSize curSize = CCSize(0, 0);
	m_stTotalSize.width += (texture->getContentSize().width*texture->getScale() - 1);
	curSize.width = getContentSize().width + texture->getContentSize().width*texture->getScale();
	if (m_stTotalSize.height < texture->getContentSize().height*texture->getScale()){
		m_stTotalSize.height = texture->getContentSize().height*texture->getScale();
	}
	curSize.height = m_stTotalSize.height;

	setContentSize(curSize);
}

void CBgTextureNode::DelTextureByIdx(unsigned int index){
	if(index >= _subTextures->count()){
		return;
	}

	CCSprite* texture = (CCSprite*)_subTextures->objectAtIndex(index);
	removeChild(texture);
	_subTextures->removeObjectAtIndex(index);

	UpdateTextures();
}

void CBgTextureNode::DelPreTexture(){
	if(_subTextures->count() == 0){
		return;
	}
	CCSprite* texture = (CCSprite*)_subTextures->objectAtIndex(0);
	removeChild(texture);
	_subTextures->removeObjectAtIndex(0);

	UpdateTextures();
}

void CBgTextureNode::UpdateTextures(){
	CCSprite* texture;
	float totalWith = 0;
	float fianlHeight = 0;
	for(unsigned int i = 0; i < _subTextures->count(); i++){
		texture = (CCSprite*)_subTextures->objectAtIndex(i);

		totalWith += (texture->getContentSize().width*texture->getScale() - 1);

		if (fianlHeight < texture->getContentSize().height*texture->getScale()){
			fianlHeight = texture->getContentSize().height*texture->getScale();
		}
	}

	m_stTotalSize.width = totalWith;
	m_stTotalSize.height = fianlHeight;
	//setContentSize(m_stTotalSize);
}