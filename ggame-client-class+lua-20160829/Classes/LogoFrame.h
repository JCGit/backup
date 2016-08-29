#pragma once


#include "cocos2d.h"
#include "cocos-ext.h"
#include "StateMachine.h"
#include "GamePrecedure.h"
#include "libPlatform.h"
#include "libOS.h"


using namespace cocos2d;
class LogoFrame
	: public State<GamePrecedure>
    , public platformListener
	, public libOSListener
{

public:
	LogoFrame(void){m_bLogoFinished = false;}
	~LogoFrame(void){}

public:
	/* interface from State */
	virtual void	Enter(GamePrecedure*);
	virtual void	Execute(GamePrecedure*);
	virtual void	Exit(GamePrecedure*);

	/* interface from libOSListener */
	virtual void	onPlayMovieEndMessage();

	/* interface from platformListener */
	virtual void	onPlayMovieEnd();

	//
	bool			isLogoFinished() const{return m_bLogoFinished;}

private:
	bool			m_bLogoFinished;

};

