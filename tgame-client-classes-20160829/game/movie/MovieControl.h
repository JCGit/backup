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


	bool isPlayingMovie(); // �Ƿ����ڲ��Ŷ���

	void playMovie(CCObject* pTargetObj = NULL, SEL_MovieDoneCallFunc pCallbackFunc = NULL ); // ��ʾ������ͼ
	
private:
    void init(); // ��ʼ��
private:
	bool m_bInitialized;

	MovieView *m_pMovieView; // ������ͼ


};


#define P_MOVIE_CTRL CVLib::CVSingleton<MovieControl>::Instance()

#endif // MovieControl_h__