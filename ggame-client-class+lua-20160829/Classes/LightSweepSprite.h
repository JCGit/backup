//
//  LightSweepSprite.h
//  DevPro
//
//  Created by long on 13-12-18.
//
//

#ifndef __DevPro__LightSweepSprite__
#define __DevPro__LightSweepSprite__

#include <string>
#include "cocos2d.h"

USING_NS_CC;

#define START_SPEED 7.0
#define ACCELERATED_SPEED 0.7f
#define END_POS 15
#define FREQUENCE  120
#define LIGHT_SWEEP_TAG 11001

class LightSweepSprite : public CCSprite
{
protected:
    virtual char* getShaderFile();
    virtual char* getVertShaderFile();
    virtual void initUniforms();
    virtual void setUniforms();
    
public:
    LightSweepSprite();
	LightSweepSprite(float speed, float accelerate);
    ~LightSweepSprite();
    
    static LightSweepSprite* create(const char *pszFileName);
    bool initWithTexture(CCTexture2D* texture, const CCRect& rect);
    bool initWithFile(const char *pszFilename);
	void initData();
    void draw();
    void initProgram();
	//ɨ���Ŀ��
    void setTarget(CCSprite* sprite);
	//�����������ص�
	void onStarFlickerEnd(CCNode* node);
	void addStarPos(CCPoint pos);
	//����õ�һ����������λ��
	CCPoint getStarRandPos();
	void update(float dt);
	//���������ز�
	void setStarSprite(const char* starFile);
	//����ɨ���ʼ�ٶ�
	inline void setInitSpeed(float speed){m_posInitSpeed = speed; m_posSpeed = m_posInitSpeed;}
	//���ü��ٶ�
	inline void setAcceratedSpeed(float acc){m_posAcceleratedSpeed = acc;}
	//����������С�����ʱ��
	inline void setScaleToMaxTime(float time){m_scaleToMaxTime = time;}
	//���������ɴ�С��ʱ��
	inline void setScaleToMinTime(float time){m_scaleToMinTime = time;}
private:
    GLuint  sampler;
    GLuint  pos;
    GLuint  texSize;
	GLuint  texRect;
    GLuint  targetSize;
	GLuint  targetTexSize;

	CCSprite*			m_pTarget;
    CCSize              m_targetSize;
    CCPoint             m_srcPosition;
    CCTexture2D*		m_pTargetTexture;

    float     m_posSpeed;
	float	  m_posInitSpeed;
	float	  m_posAcceleratedSpeed;
    float     m_end;
    int       m_frequence;
	int		  m_frequenceMax;
	float	  m_scaleToMaxTime;
	float	  m_scaleToMinTime;

	CCSprite*		m_pStar;
	std::vector<CCPoint>	m_starPosVec;
};

namespace LightSweep
{
	/*
	@desc:��ͨlogoɨ��
	*/
    LightSweepSprite* createLightSweep(CCSprite* sprite);

	/*
	@desc:��ʯɨ��,����������Ҫ��ͬ��ɨ���زĺ�����������ز�
	@param:��Ҫɨ���sprite
	*/
	LightSweepSprite* createLightSweepForMoney(CCSprite* sprite);

	/*
	*@desc����ʯɨ�⣬��ʯ�ֲ���page�Ƚ϶࣬�Ѵ���������������������ظ�
	@param:��Ҫɨ���sprite
	*/
	void createLSforJewel(CCSprite* sprite);
}

#endif /* defined(__DevPro__LightSweepSprite__) */
