#include "BaseSceneItem.h"

CBaseSceneItem::CBaseSceneItem(){
	_armature = NULL;
	_currentFrameIdx = 0;
}

CBaseSceneItem::~CBaseSceneItem(){

}

void CBaseSceneItem::onEnter(){
	CCNode::onEnter();
}

void CBaseSceneItem::onExit(){
	CCNode::onExit();
}


