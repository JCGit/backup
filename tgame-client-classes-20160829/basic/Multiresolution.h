#ifndef Multiresolution_h__
#define Multiresolution_h__

#include "cocos2d.h"

USING_NS_CC;

class Multiresolution
{
public:
	Multiresolution();
	~Multiresolution();

	static void setDesignResolutionSize(float fWidth, float fHeight);

	static const CCSize& getInnerSize();

	static CCSize getInnerSizeInPixels();

	static CCPoint getInnerOrigin();

private:

};

#endif // Multiresolution_h__
