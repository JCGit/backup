
#include "AnimMgr.h"
#include "effects/CCGridInverseColor.h"


AnimMgr::AnimMgr()
{
	
}

AnimMgr::~AnimMgr()
{

}

void AnimMgr::fadeInAndOut(CCNode* node,float time)
{
	node->stopAllActions();

	CCFadeOut* out = CCFadeOut::create(time);
	CCFadeIn* in = CCFadeIn::create(time);
	CCArray* array = CCArray::create();
	array->addObject(out);
	array->addObject(in);
	CCSequence* seq = CCSequence::create(array);
	CCRepeatForever* forever = CCRepeatForever::create(seq);
	node->runAction(forever);
}

void AnimMgr::scrollViewMoveIn(CCScrollView* scrollView)
{
	if (!scrollView)
	{
		return;
	}
	CCArray* children = scrollView->getContainer()->getChildren();
	int count = 1;
	if (!children || children->count() == 0)
	{
		return;
	}
	for (int i = children->count()-1; i >= 0; --i)
	{
		CCNode* child = dynamic_cast<CCNode*>(children->objectAtIndex(i));

		if (child)
		{
			if (!child->getActionByTag(SCROLL_MOVE_IN_TAG))
			{
				CCPoint end = child->getPosition();
				CCPoint start = CCPoint(640,end.y);
				CCDelayTime* waiting = CCDelayTime::create(count*ANIM_DELAY_TIME);

				child->setPosition(start);
				CCActionInterval* move = CCMoveTo::create(ANIM_MOVE_TIME,end);
				CCSequence* sequence = CCSequence::create(waiting,move,NULL);

				CCEaseSineInOut* sine = CCEaseSineInOut::create(sequence);
				sine->setTag(SCROLL_MOVE_IN_TAG);
				child->runAction(sine);
				++count;
			}
			else
			{
				return;
			}
		}	
	}
}

void AnimMgr::nodeMoveIn(CCNode* node,bool order)
{
	if (!node)
	{
		return;
	}

	CCArray* children = node->getChildren();
	int count = 1;
	if (!children || children->count() == 0)
	{
		return;
	}

	if (!order)
	{
		for (int i = children->count()-1; i >= 0; --i)
		{
			CCNode* child = dynamic_cast<CCNode*>(children->objectAtIndex(i));

			if (child)
			{
				if (!child->getActionByTag(SCROLL_MOVE_IN_TAG))
				{
					CCPoint end = child->getPosition();
					CCPoint start = CCPoint(640,end.y);
					CCDelayTime* waiting = CCDelayTime::create(count*ANIM_DELAY_TIME);

					child->setPosition(start);
					CCActionInterval* move = CCMoveTo::create(ANIM_MOVE_TIME,end);
					CCSequence* sequence = CCSequence::create(waiting,move,NULL);

					CCEaseSineInOut* sine = CCEaseSineInOut::create(sequence);
					sine->setTag(SCROLL_MOVE_IN_TAG);
					child->runAction(sine);
					++count;
				}	
				else
				{
					return;
				}
			}	
		}
	}
	else
	{
		for (int i = 0;i < children->count(); ++i)
		{
			CCNode* child = dynamic_cast<CCNode*>(children->objectAtIndex(i));

			if (child)
			{
				if (!child->getActionByTag(SCROLL_MOVE_IN_TAG))
				{
					CCPoint end = child->getPosition();
					CCPoint start = CCPoint(640,end.y);
					CCDelayTime* waiting = CCDelayTime::create(count*ANIM_DELAY_TIME);

					child->setPosition(start);
					CCActionInterval* move = CCMoveTo::create(ANIM_MOVE_TIME,end);
					CCSequence* sequence = CCSequence::create(waiting,move,NULL);

					CCEaseSineInOut* sine = CCEaseSineInOut::create(sequence);
					sine->setTag(SCROLL_MOVE_IN_TAG);
					child->runAction(sine);
					++count;
				}		
				else
				{
					return;
				}
			}	
		}
	}	
}

AnimMgr* AnimMgr::getInstance()
{
	return AnimMgr::Get();
}

void AnimMgr::scaleToShow(CCNode* node,float start,float end,CCCallFuncND* func)
{
	node->setScale(start);
	CCScaleTo* max = CCScaleTo::create(0.2f, end*1.1f);
	CCScaleTo* back = CCScaleTo::create(0.05f, end*1.0f);
	CCScaleTo* back2 = CCScaleTo::create(0.05f, end*1.0f);
	
	CCSequence* seq = NULL;
	if (func)
	{
		seq = CCSequence::create(max,back,back2,func,NULL);
	}
	else
	{
		seq = CCSequence::create(max,back,back2,NULL);
	}
	node->runAction(seq);
}

void AnimMgr::scaleToHide(CCNode* node,float start,CCCallFuncND* func)
{
	CCFadeOut* out = CCFadeOut::create(0.2f);
	CCScaleTo* max = CCScaleTo::create(0.05,start*1.1f);
	CCScaleTo* min = CCScaleTo::create(0.15f,0.0f);
	CCScaleTo* back = CCScaleTo::create(0.0,start);

	CCSequence* seq = NULL;
	if (func)
	{
		seq = CCSequence::create(max,min,back,func,NULL);
	}
	else
	{
		seq = CCSequence::create(max,min,back,NULL);
	}

	node->runAction(seq);
}



namespace InverseColorEffect
{

	CCSprite* generateInverseColorSpriteByNode(CCNode* node,CCPoint anchorPoint){

		CCGridInverseColor* inverseColor = CCGridInverseColor::create(CCSizeMake(10,10));
		if (inverseColor)
		{
			node->setGrid(inverseColor);
			inverseColor->retain();
			inverseColor->setActive(true);
		}
		CCPoint anPoint = node->getAnchorPoint();

		CCRenderTexture* rt = CCRenderTexture::create(node->getContentSize().width ,node->getContentSize().height);
		rt->clear(0,0,0,0);
		rt->begin();
		node->setAnchorPoint(anchorPoint);
		node->visit();

		rt->end();

		inverseColor->setActive(false);
		node->setAnchorPoint(anPoint);
		CCTexture2D * rtTexture = rt->getSprite()->getTexture();
		CCSprite* inverseColorSprite = new CCSprite;
		inverseColorSprite->initWithTexture(rtTexture,CCRect(0.0,0.0,rtTexture->getContentSize().width,rtTexture->getContentSize().height));
		//since the render texture 
		inverseColorSprite->setFlipY(true);
		inverseColorSprite->autorelease();

		return inverseColorSprite;

	}


}