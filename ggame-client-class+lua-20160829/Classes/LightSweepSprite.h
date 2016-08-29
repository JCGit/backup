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
	//扫光的目标
    void setTarget(CCSprite* sprite);
	//星星闪结束回调
	void onStarFlickerEnd(CCNode* node);
	void addStarPos(CCPoint pos);
	//随机得到一个星星闪的位置
	CCPoint getStarRandPos();
	void update(float dt);
	//设置星星素材
	void setStarSprite(const char* starFile);
	//设置扫光初始速度
	inline void setInitSpeed(float speed){m_posInitSpeed = speed; m_posSpeed = m_posInitSpeed;}
	//设置加速度
	inline void setAcceratedSpeed(float acc){m_posAcceleratedSpeed = acc;}
	//设置星星由小到大的时间
	inline void setScaleToMaxTime(float time){m_scaleToMaxTime = time;}
	//设置星星由大到小的时间
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
	@desc:普通logo扫光
	*/
    LightSweepSprite* createLightSweep(CCSprite* sprite);

	/*
	@desc:钻石扫光,区别在于需要不同的扫光素材和星星闪光的素材
	@param:需要扫光的sprite
	*/
	LightSweepSprite* createLightSweepForMoney(CCSprite* sprite);

	/*
	*@desc：钻石扫光，钻石分布的page比较多，把代码抽象出来，避免大量的重复
	@param:需要扫光的sprite
	*/
	void createLSforJewel(CCSprite* sprite);
}

#endif /* defined(__DevPro__LightSweepSprite__) */
