#include "AnimationUtils.h"

CCFrameData* CAnimationUtils::getFrameDataByFrameIdx(CCArmature* pArm, const string stActName, const string stBoneName, const int frameIdx){
	CCMovementData* movData = pArm->getAnimation()->getAnimationData()->getMovement(stActName.c_str());
	if(movData == NULL){
		return NULL;
	}

	CCMovementBoneData *movBoneData = movData->getMovementBoneData(stBoneName.c_str());
	if(movBoneData == NULL){
		return NULL;
	}

	CCFrameData* stFrameData = NULL;
	CCFrameData* pData;
	for(unsigned int i = 0; i < movBoneData->frameList.count(); i++){
		pData = movBoneData->getFrameData(i);
		if(pData->frameID == frameIdx){
			stFrameData = pData;
			break;
		}
	}

	return stFrameData;
}

const CCRect CAnimationUtils::getColliderRect(CCContourData* contour, CCPoint pos, float scale_x, float scale_y){
	CCContourVertex2* LeftUp = (CCContourVertex2*)contour->vertexList.objectAtIndex(0);
	CCContourVertex2* LeftDown = (CCContourVertex2*)contour->vertexList.objectAtIndex(1);
	CCContourVertex2* RightDown = (CCContourVertex2*)contour->vertexList.objectAtIndex(2);
	CCContourVertex2* RightUp = (CCContourVertex2*)contour->vertexList.objectAtIndex(3);

	CCPoint aPos = ccp((RightUp->x + LeftUp->x) / 2, (LeftUp->y + LeftDown->y) / 2);

	aPos.x *= scale_x;
	aPos.y *= scale_y;

	aPos = ccpAdd(aPos, pos);
	CCSize aSize = CCSizeMake(RightUp->x - LeftUp->x, RightUp->y - RightDown->y);

	return CCRect(aPos.x - aSize.width/2, aPos.y - aSize.height/2, aSize.width, aSize.height);
}

const CCRect CAnimationUtils::getBlockRect(CCArmature* pArm, const string stBoneName){
	CCBone* pstBone = pArm->getBone(stBoneName.c_str());
	if(pstBone == NULL){
		return CCRect();
	}

	CCArray* coliArray = pstBone->getColliderBodyList();
	if(!coliArray || coliArray->count() == 0){
		return CCRect();
	}

	ColliderBody* body = (ColliderBody*)coliArray->objectAtIndex(0);
	return getColliderRect(body->getContourData(), pstBone->getPosition());
}

CCFrameData* CAnimationUtils::getFrameDataByEvent(CCArmature* pArm, const string stActName, const string stEventName){
	CCMovementData* movData = pArm->getAnimation()->getAnimationData()->getMovement(stActName.c_str());
	if(movData == NULL){
		return NULL;
	}

	CCDictionary* stDataDic = &(movData->movBoneDataDic);
	CCDictElement *element = NULL;
	CCDICT_FOREACH(stDataDic, element){
		CCMovementBoneData *movBoneData = movData->getMovementBoneData(element->getStrKey());
		CCFrameData* stFrameData = NULL;
		for(unsigned int i = 0; i < movBoneData->frameList.count(); i++){
			stFrameData = movBoneData->getFrameData(i);
			if(stFrameData->strEvent == stEventName){
				return stFrameData;
			}
		}
	}

	return NULL;
}

