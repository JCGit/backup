#ifndef MovieControl_h__
#define MovieControl_h__

#include "vlib/base/CVSingleton.h"
#include "cocos2d.h"
#include "MovieView.h"

USING_NS_CC;
class MovieView;

class MovieControl 
{
public:
	
public:
	MovieControl();
	~MovieControl();


	void finalize(); // free


	bool isPlayingMovie(); // 是否正在播放动画

	void playMovie(CCObject* pTargetObj = NULL, SEL_MovieDoneCallFunc pCallbackFunc = NULL ); // 显示引导视图
	
private:
    void init(); // 初始化
private:
	bool m_bInitialized;

	MovieView *m_pMovieView; // 引导视图


};


#define P_MOVIE_CTRL CVLib::CVSingleton<MovieControl>::Instance()

#endif // MovieControl_h__