#include "Solider.h"
#include "SoliderView.h"
#include "Constant.h"
#include "battle/Build.h"
#include "battle/BattleControl.h"
#include "GamePrecedure.h"

// #define PI 3.14
// #define MINFL 0.001f
// 
// #define HALF_PI Vec2::angle(Vec2(10,0),Vec2(0,10))
// 
// #define F_SOLDIER_SOLDIER 0.75 //ʿ����ʿ��ϵ��
// #define F_OTHER_BUILD 0.75     //ʿ������;����
// #define F_SRCTAR_BUILD 0.50    //ʿ������ʼ���յ�Ľ���
// #define DIS_SOLDIER_R 5      


Solider* Solider::create(unsigned int fid, AttribType attribType, eCampType srcCamp, eCampType dstCamp)
{
	Solider* pRet = new Solider();
	if (pRet && pRet->init(fid, attribType, srcCamp, dstCamp))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

Solider::Solider() : mStatus(Status::kNone)
, mFormationId(0)
, mSrcCamp(eCampType::ECAMP_TYPE_Invalid)
, mDstCamp(eCampType::ECAMP_TYPE_Invalid)
, mSlot(nullptr)
, mView(nullptr)
, mAttribType(AttribType::kNormal)
, mLevel(0)
, mSpeedScale(1.0f)
{
// 	mView = NULL;
// 	m_vPosition = Vec2(0,0);
// 	m_vFs = Vec2(0,0);
// 	m_vBuildF = Vec2(0,0);
// 	m_pTargetPosition = Vec2(0,0);
// 	m_bIsNearBuild = false;
// 	m_bIsLive = true;
// 	mBullet = 0;
}

Solider::~Solider()
{
	this->dispose();
}


bool Solider::init(unsigned int fid, AttribType attribType, eCampType srcCamp, eCampType dstCamp)
{
	if (eCampType::ECAMP_TYPE_Invalid == srcCamp)
		return false;

	if (eCampType::ECAMP_TYPE_Invalid == dstCamp)
		return false;

	mFormationId = fid;

	mSrcCamp = srcCamp;
	mDstCamp = dstCamp;

	mLevel = sgBattleControl()->getInstituteNum(mSrcCamp);
	if (mLevel > 4)
		mLevel = 4;
	mSpeedScale = 1.0f;

	mAttribType = attribType;

	mView = SoliderView::create(this);
	if (nullptr == mView)
		return false;

	mView->show(true);
	mView->retain();

	mView->refresh();

	return true;
}


void Solider::dispose()
{
	mSrcCamp = eCampType::ECAMP_TYPE_Invalid;
	mDstCamp = eCampType::ECAMP_TYPE_Invalid;

	mSlot = nullptr;

	if (mView)
		mView->release();
	mView = nullptr;

	mAttribType = AttribType::kNormal;
	mLevel = 0;
	mSpeedScale = 1.0f;
	mStatus = Status::kNone;
}


void Solider::setStatus(Status status)
{
	mStatus = status;
}


bool Solider::checkStatus(Status status) const
{
	return (mStatus & status) || (mStatus == status);
}


void Solider::addStatus(Status status)
{
	mStatus |= status;
}


void Solider::remvoeStatus(Status status)
{
	mStatus &= ~status;
}


void Solider::faceLeft()
{
	if (mView)
		mView->face(true);
}


void Solider::faceRight()
{
	if (mView)
		mView->face(false);
}


Point Solider::getPosition() const
{
	if (mView)
		return mView->getWorldPosition();

	return Point::ZERO;
}


void Solider::setPosition(const Point& position)
{
	this->setPosition(position.x, position.y);
}


void Solider::setPosition(float x, float y)
{
	if (mView)
		mView->setWorldPosition(x, y);
}


const Point& Solider::getLocalPosition() const
{
	if (mView)
		return mView->getPosition();

	return Point::ZERO;
}


void Solider::setLocalPosition(const Point& position)
{
	this->setLocalPosition(position.x, position.y);
}


void Solider::setLocalPosition(float x, float y)
{
	if (mView)
		mView->setPosition(x, y);
}

void Solider::setLocalZOrder(int zOrder)
{
	if (mView)
		mView->setLocalZOrder(zOrder);
}


int Solider::getLocalZOrder() const
{
	if (mView)
		return mView->getLocalZOrder();

	return 0;
}


void Solider::setLocalRotation(float angle)
{
	if (mView)
		mView->setRotation(angle);
}


float Solider::getLocalRotation() const
{
	if (mView)
		return mView->getRotation();

	return 0.0f;
}


void Solider::refreshView()
{
	if (mView)
		mView->refresh();
}


void Solider::doSpotted()
{
	this->addStatus(Status::kSpotted);
}


void Solider::doBombHit()
{

}


void Solider::doDead()
{
	if (mView)
	{
		mView->show(false);
		mView->playDeadAni();
	}

	this->addStatus(Status::kDead);
}


void Solider::doGradeLevel(int level)
{
	bool bUpgaradeAni = mLevel < level;
	mLevel = level;

	if (mView)
	{
		mView->refresh();

		if (bUpgaradeAni)
			mView->playUpgradeAni();
	}
}


void Solider::doSpeedScale(float scale)
{
	mSpeedScale = scale;

	if (mView)
		mView->refresh();
}


void Solider::doFreeze()
{
	this->addStatus(Status::kFreeze);
	if (mView)
		mView->setGray(true);
}


void Solider::doUnfreeze()
{
	this->remvoeStatus(Status::kFreeze);
	if (mView)
		mView->setGray(false);
}


//������ת90�ȣ�dir>0 ˳ʱ�룬dir<0 ��ʱ��
Vec2 Solider::rotateHalfPI(Vec2 pos,float dir)
{
// 	if (dir > 0)
// 	{
// 		if (pos.x > 0)
// 		{
// 			if (pos.y > 0)
// 			{
// 				return Vec2(pos.y,-pos.x);
// 			}
// 			else
// 			{
// 				return Vec2(-pos.y,pos.x);
// 			}
// 		}
// 		else
// 		{
// 			if (pos.y > 0)
// 			{
// 				return Vec2(-pos.y,pos.x);
// 			}
// 			else
// 			{
// 				return Vec2(pos.y,-pos.x);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if (pos.x > 0)
// 		{
// 			if (pos.y > 0)
// 			{
// 				return Vec2(-pos.y,pos.x);
// 			}
// 			else
// 			{
// 				return Vec2(pos.y,-pos.x);
// 			}
// 		}
// 		else
// 		{
// 			if (pos.y > 0)
// 			{
// 				return Vec2(pos.y,-pos.x);
// 			}
// 			else
// 			{
// 				return Vec2(-pos.y,pos.x);
// 			}
// 		}
// 	}

	return Vec2::ZERO;
}



//�����������ͨ�ù�ʽ����С���ٶ��йأ�ͬʱ�ܾ���Ӱ��
float Solider::calculate(float dis,float maxdis,float k,float speed)
{
//	return speed / k * ((dis-maxdis)/maxdis);
	return 0.0f;
}

//ʿ��֮�����
void Solider::addFs(Vec2 f,float dir,float speed)
{
// 	//ȷ���������� ����ʱ�Ĵ�С���������Ĵ�С
// 	Vec2 pVector = rotateHalfPI(m_pTargetPosition,-dir);
// 	
// 	//����ʿ��֮��ľ���
// 	float dis = f.distance(getPosition());
// 	//float pF = calculate(dis,DIS_SOLDIER_SOLDIER,F_SOLDIER_SOLDIER,speed);
// 
// 	//ȷ�������СԼ��
// 	float subdis = dis-DIS_SOLDIER_SOLDIER;
// 	if (subdis < -DIS_SOLDIER_SOLDIER)
// 	{
// 		subdis = -DIS_SOLDIER_SOLDIER;
// 	}
// 	else if(subdis > DIS_SOLDIER_SOLDIER*1.1)
// 	{
// 		subdis = DIS_SOLDIER_SOLDIER*1.1;
// 	}
// 	
// 	//�����С
// 	float pF = speed / F_SOLDIER_SOLDIER * (subdis/DIS_SOLDIER_SOLDIER);
// 
// 	//float pF = (1 - pVector.getLength()/TILED_FEET_SIZE)/30;
// 	m_vFs = m_vFs + Vec2(pVector.x/pVector.length() * pF , pVector.y/pVector.length() * pF);
// 	//m_vFs = m_vFs + Vec2(pVector.x * pF , pVector.y * pF);
}

//;�н������ʿ����������
void Solider::addBuildFs(Vec2 f,float speed)
{
// 	//����������û������ֱ����
// 	Vec2 pVector = getPosition() - f;
// 	float pF = speed/F_OTHER_BUILD;
// 	m_vBuildF = m_vBuildF + Vec2(pVector.x/pVector.length() * pF , pVector.y/pVector.length() * pF);
// 	
}

//�����յ㽨����ʿ����������
void Solider::addBuildInFs(Vec2 f,float speed)
{
// 	Vec2 pVector = f - getPosition();
// 
// 	if (Vec2::angle(pVector,m_pTargetPosition) < Vec2::angle(Vec2(10,0),Vec2(0,10)))
// 	{
// 		pVector.x = -pVector.x;
// 		pVector.y = -pVector.y;
// 	}
// 
// 	float pF = calculate(pVector.length(),DIS_SRCTAR_BUILD,F_SRCTAR_BUILD,speed);
// 	m_vFs = m_vFs + Vec2(pVector.x/pVector.length() * pF , pVector.y/pVector.length() * pF);
}

void Solider::addFormationFs(Vec2 pos, float speed)
{
	 
}

//�����ã��õ����յ���һ��������
Vec2 Solider::getRealNextPosition()
{
// 	//���ȣ�ȷ��һ�����ٶȴ�ֱ������p
// 	Vec2 pResule = Vec2(-m_pTargetPosition.y,m_pTargetPosition.x);
// 	
// 
// 	//�ڽ�����Χ�ڲ��ܵ�ʿ��֮�����
// 	if (m_bIsNearBuild)
// 	{
// 		m_vFs.x = 0;
// 		m_vFs.y = 0;
// 	}
// 
// 	//����������ĺ���
// 	m_vFs = m_vBuildF + m_vFs;
// 
// 	//��������p������ļн�
// 	float pAngle = Vec2::angle(pResule,m_vFs);
// 
// 	//����н��Ƕ۽ǣ���ת����p
// 	if(pAngle > HALF_PI)
// 	{
// 		pResule.x = -pResule.x;
// 		pResule.y = -pResule.y;
// 	}
// 
// 	//���������Ϊ����p�ı�׼����
// 	float x = pResule.x / pResule.length();
// 	float y = pResule.y / pResule.length();
// 
// 	//����������Ĵ�С
// 	float pFSLenght = m_vFs.length();
// 
// 
// 	
// 	//���������ഹֱ��������ӣ��ټ�������λ�ã��������һ��������
// 	m_pTargetPosition = m_pTargetPosition + Vec2(pFSLenght*x,pFSLenght*y) + getPosition();
// 
// 	/*m_pTargetPosition.x = m_pTargetPosition.x + m_vFs.x + getPositionX();
// 	m_pTargetPosition.y = m_pTargetPosition.y + m_vFs.y + getPositionY();*/
// 
// 
// 	m_vFs.x = 0;
// 	m_vFs.y = 0;
// 	m_vBuildF.x = 0;
// 	m_vBuildF.y = 0;
// 	return m_pTargetPosition;

	return Point::ZERO;
}

//����ʿ��ʩ�����Ľ���
void Solider::addWillCheckBuild(BaseBuild* build)
{
// 	if (build)
// 	{
// 		//֮ǰ����ͨ��id��Ϊkey��������Ӫ�ı��id��仯������ʹ��locationID��Ϊkey
// 		int k = build->getBuildInfo().locationID;
// 		m_vWillCheckBuilds.insert(std::make_pair(k,build));
// 	}
}

//�����б����
void Solider::addF(float speed)
{
// 	std::map<int,BaseBuild*>::iterator it = m_vWillCheckBuilds.begin();
// 
// 	m_bIsNearBuild = false;
// 	for(; it != m_vWillCheckBuilds.end() ;)
// 	{
// 		BaseBuild* pBuild = it->second;
// 
// 		if (NULL == pBuild)
// 		{
// 			continue;
// 		}
// 
// 		Vec2 pBuildPos = Vec2(pBuild->getBuildInfo().x,pBuild->getBuildInfo().y);
// 		float dis = getPosition().getDistance(pBuildPos);
// 		float fanwei = sqrt(pBuild->getBuildInfo().width * pBuild->getBuildInfo().width + pBuild->getBuildInfo().height * pBuild->getBuildInfo().height)/2;
// 
// 		//Ϊ�˻���
// 		if (dis > fanwei + DIS_SOLDIER_R)
// 		{
// 			it = m_vWillCheckBuilds.erase(it);
// 		}
// 		else if(dis > fanwei)
// 		{
// 			m_bIsNearBuild = true;
// 			it++;
// 		}
// 		else 
// 		{
// 			m_bIsNearBuild = true;
// 			addBuildFs(Vec2(pBuild->getBuildInfo().x,pBuild->getBuildInfo().y),speed);
// 			it++;
// 		}
// 
// 
// 	}
}

//��ʼ�������յ㽨�����������⴦��
void Solider::checkSrcAndTarBuild(BaseBuild* src,BaseBuild* tar,float speed)
{
// 	Vec2 pTar = Vec2(tar->getBuildInfo().x,tar->getBuildInfo().y);
// 	Vec2 pSrc = Vec2(src->getBuildInfo().x,src->getBuildInfo().y);
// 
// 	float pDis2Tar = pTar.distance(getPosition());
// 	float pDis2Src = pSrc.distance(getPosition());
// 
// 	/*if (pDis2Src < src->getCheckSoldierR() / 2)
// 	{
// 	addBuildInFs(pSrc,speed);
// 	}*/
// 
// 	if (pDis2Tar < DIS_SRCTAR_BUILD)
// 	{
// 		addBuildInFs(pTar,speed);
// 	}
}

void Solider::remove()
{
// 	if (mBullet)
// 	{
// 		mBullet->removeFromParent();
// 		mBullet = 0;
// 
// 		mView->doBombHit();
// 	}
// 	else
// 	{	
// 		if (mDstCamp != mSrcCamp)
// 		{
// 			sgGamePrecedure()->addDieEffect(mView);
// 		}
// 
// 		mView->removeFromParent();
// 	}
}

void Solider::add(Node* parent)
{
//	parent->addChild(mView);
}