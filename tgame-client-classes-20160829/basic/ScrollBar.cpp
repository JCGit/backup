#include "ScrollBar.h"

CScrollBar::CScrollBar()
: m_pTarget(NULL)
, m_pBg(NULL)
, m_pSlider(NULL)
, m_direction(DIR_NODIR)
, m_preContentSize(CCSizeZero)
, m_viewSize(CCSizeZero)
, m_sliderTouched(false)
, m_firstTouch(CCPointZero)
, m_sliderCurPos(CCPointZero)
, m_targetCurPos(CCPointZero)
{

}

CScrollBar::~CScrollBar(){

}

void CScrollBar::onEnter(){
	CCSprite::onEnter();
}

void CScrollBar::onExit(){
	CCSprite::onExit();

	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

CScrollBar* CScrollBar::create(const char* bar_bgFile, const char* bar_sliderFile, CCTableView* tableView, CSclBarDirection dir){
	CCScale9Sprite* pBg = CCScale9Sprite::create(bar_bgFile);
	CCScale9Sprite* pSlider = CCScale9Sprite::create(bar_sliderFile);

	return create(pBg, pSlider, tableView, dir);
}

CScrollBar * CScrollBar::create(CCScale9Sprite* bar_bg, CCScale9Sprite* bar_slider, CCTableView* tableView, CSclBarDirection dir){
	CScrollBar * pRet = new CScrollBar();
	if (pRet && pRet->myInit(bar_bg, bar_slider, tableView, dir)){
		pRet->autorelease();
	}else{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

/**
* ��ʼ����������
*/
bool CScrollBar::myInit(CCScale9Sprite* bar_bg, CCScale9Sprite* bar_slider, CCTableView* tableView, CSclBarDirection dir){
	if (!CCSprite::init()){
		return false;
	}

	m_pBg = bar_bg;
	m_pSlider = bar_slider;
	m_pTarget = tableView;
	m_direction = dir;
	m_preContentSize = m_pTarget->getContainer()->getContentSize();
	m_viewSize = m_pTarget->getViewSize();

	if (m_direction == DIR_VERTICAL){
		m_pBg->setContentSize(CCSize(m_pBg->getContentSize().width, m_viewSize.height));
		m_pBg->setPosition(ccp(m_pBg->getContentSize().width / 2,0));
		m_pSlider->setPositionX(m_pBg->getContentSize().width / 2);
	} else if (m_direction == DIR_HORIZENTAL){
		m_pBg->setContentSize(CCSize(m_viewSize.width, m_pBg->getContentSize().height));
		m_pBg->setPosition(ccp(0, -m_pBg->getContentSize().height/2));
		m_pSlider->setPositionY(-m_pBg->getContentSize().height/2);
	}

	this->addChild(m_pBg,0);
	this->updateSlider();
	this->addChild(m_pSlider,1);
	this->scheduleUpdate();

	//auto listenerT = EventListenerTouchOneByOne::create();
	//listenerT->onTouchBegan = CC_CALLBACK_2(ScrollBar::onTouchBegan,this);
	//listenerT->onTouchMoved = CC_CALLBACK_2(ScrollBar::onTouchMoved,this);
	//listenerT->onTouchEnded = CC_CALLBACK_2(ScrollBar::onTouchEnded,this);
	//listenerT->setSwallowTouches(false);
	//CCDirector::sharedDirector()->getTouchDispatcher()->addEventListenerWithSceneGraphPriority(listenerT,this);
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);

	return true;
}

void CScrollBar::registerWithTouchDispatcher(){  
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, true);//ʹ��int��Сֵ��������ȼ�,�����̵��¼�true  
}

void CScrollBar::updateSlider(){
	float ratio = 0.0;
	if (m_direction == DIR_VERTICAL){
		if(m_preContentSize.height == 0){
			return;
		}

		ratio = m_viewSize.height / m_preContentSize.height;
		m_pSlider->setContentSize(CCSize(m_pSlider->getContentSize().width, m_viewSize.height * ratio));
		//m_pSlider->setScaleY(m_pSlider->getContentSize().height/180);
	} else if (m_direction == DIR_HORIZENTAL){
		if(m_preContentSize.width == 0){
			return;
		}

		ratio = m_viewSize.width / m_preContentSize.width;
		m_pSlider->setContentSize(CCSize(m_viewSize.width * ratio, m_pSlider->getContentSize().height));
	}
	//���Ҫ��ʾ�����ݵĳߴ����ͼ��СС�������ػ���slider
	this->setVisible( !(ratio >= 1) );
}

void CScrollBar::update(float dt){
	//�жϵ�ǰ�����Ƿ�����������Ϊ���ݵ�������Ӱ��ContenSize,�Ӷ��޸�slider�Ĵ�С
	CCSize curContentSize = m_pTarget->getContainer()->getContentSize();
	if ( !(fabsf(curContentSize.height - m_preContentSize.height) <= 0.00001)  || 
		!(fabsf(curContentSize.width - m_preContentSize.width) <= 0.00001) ){
		m_preContentSize = curContentSize;
		this->updateSlider();
	}

	//����slider��λ��
	if (m_direction == DIR_VERTICAL){
		//���������λ��
		CCPoint curOffset = m_pTarget->getContentOffset() + (m_preContentSize - m_viewSize) / 2;
		float sliderOffset = curOffset.y / (m_viewSize.height - curContentSize.height) * 
			(m_viewSize.height - m_pSlider->getContentSize().height);

		//�жϻ����Ƿ񻬳�����
		if (fabsf(sliderOffset) > (m_viewSize.height - m_pSlider->getContentSize().height) / 2){
			return ;
		}

		m_pSlider->setPositionY(sliderOffset);
	}else if (m_direction == DIR_HORIZENTAL){
		CCPoint curOffset = m_pTarget->getContentOffset() - (m_preContentSize - m_viewSize) / 2;
		float sliderOffset = -curOffset.x / (m_viewSize.width - curContentSize.width) * 
			(m_viewSize.width - m_pSlider->getContentSize().width);

		if (fabsf(sliderOffset) > (m_viewSize.width - m_pSlider->getContentSize().width) / 2){
			return ;
		}

		m_pSlider->setPositionX(sliderOffset);
	}
}

bool CScrollBar::onTouchBegan(CCTouch* touch, CCEvent* pEvent){
	m_sliderCurPos = m_pSlider->getPosition();
	m_targetCurPos = m_pTarget->getContentOffset();
	CCPoint touchPoint = touch->getLocation();
	m_firstTouch = touchPoint;
	//��������תΪ�ڵ�ǰ�Ӳ��µ�����
	touchPoint = this->convertToNodeSpace(touchPoint);

	//ֻ��Ӧ����˻��鱳���Ĵ���
	if (!m_pBg->boundingBox().containsPoint(touchPoint)){
		return false;
	}

	//����ȵ���˻��飬�����ñ�־
	if (m_pSlider->boundingBox().containsPoint(touchPoint)){
		m_sliderTouched = true;
	}else{ //���û�е�����飬�������ǻ��鱳��ͼ
		if (m_direction == DIR_VERTICAL){
			//ͨ������m_pTarget��ƫ�ƣ��Ӷ������˻���slider��λ�ã���Ϊupdate������һֱ����m_pTarget��ƫ��
			float offset = touchPoint.y - m_sliderCurPos.y;
			if (touchPoint.y <= 0){
				offset += m_pSlider->getContentSize().height / 2;
			} else{
				offset -= m_pSlider->getContentSize().height / 2;
			}

			float newOff = m_targetCurPos.y + offset / (m_pSlider->getContentSize().height - m_viewSize.height) 
				* (m_preContentSize.height - m_viewSize.height);
			m_pTarget->setContentOffset(ccp(0, newOff));
		}else if (m_direction == DIR_HORIZENTAL){
			float offset = touchPoint.x - m_sliderCurPos.x;
			if (touchPoint.x <= 0){
				offset += m_pSlider->getContentSize().width / 2;
			} else{
				offset -= m_pSlider->getContentSize().width / 2;
			}
			float newOff = m_targetCurPos.x + offset / (m_viewSize.width - m_pSlider->getContentSize().width) 
				* (m_preContentSize.width - m_viewSize.width);
			m_pTarget->setContentOffset(ccp(newOff, 0));
		}
	}
	return true;
}

void CScrollBar::onTouchMoved(CCTouch* pTouch, CCEvent* pEvent){
	//ֻ��Ӧ����˻�����ƶ�
	if (m_sliderTouched){
		CCPoint offPos = pTouch->getLocation() - m_firstTouch;
		if (m_direction == DIR_VERTICAL){
			//ͨ������m_pTarget��ƫ�ƣ��Ӷ������˻���slider��λ�ã���Ϊupdate������һֱ����m_pTarget��ƫ��
			float newOff = m_sliderCurPos.y + offPos.y;
			//�жϻ����Ƿ񻬳�����
			if (fabsf(newOff) > (m_viewSize.height - m_pSlider->getContentSize().height) / 2){
				(newOff < 0 ? (newOff = (m_pSlider->getContentSize().height - m_viewSize.height) / 2) : 
					(newOff = (m_viewSize.height - m_pSlider->getContentSize().height) / 2));
			}
			newOff -= m_sliderCurPos.y;
			m_pTarget->setContentOffset(ccp(0,
				m_targetCurPos.y + newOff / (m_pSlider->getContentSize().height - m_viewSize.height) 
				* (m_preContentSize.height - m_viewSize.height)));
		}else if (m_direction == DIR_HORIZENTAL){
			float newOff = m_sliderCurPos.x + offPos.x;
			if (fabsf(newOff) > (m_viewSize.width - m_pSlider->getContentSize().width) / 2){
				(newOff < 0 ? (newOff = (m_pSlider->getContentSize().width - m_viewSize.width) / 2) : 
					(newOff = (m_viewSize.width - m_pSlider->getContentSize().width) / 2));
			}
			newOff -= m_sliderCurPos.x;
			m_pTarget->setContentOffset(ccp(m_targetCurPos.x + newOff / (m_viewSize.width - m_pSlider->getContentSize().width) 
				* (m_preContentSize.width - m_viewSize.width),0));
		}
	}
}

void CScrollBar::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	m_sliderTouched = false;
}