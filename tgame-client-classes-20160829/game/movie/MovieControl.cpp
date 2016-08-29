#include "MovieControl.h"
#include "BaseScene.h"

MovieControl::MovieControl()
:m_bInitialized(false)
,m_pMovieView(NULL)
{
}

MovieControl::~MovieControl()
{
	if(m_bInitialized) {
		finalize();
	}
}

void MovieControl::init() {
	if(!m_bInitialized) {

		// Òýµ¼ÊÓÍ¼
		m_pMovieView = MovieView::create();
        CC_SAFE_RETAIN(m_pMovieView);

        CCLOG("[trace]MovieControl::init");
		m_bInitialized = true;
	}
}

void MovieControl::finalize() {
	if(m_bInitialized) {
		CC_SAFE_RELEASE_NULL(m_pMovieView);
		

		CCLOG("[trace]MovieControl::finalize.");
		m_bInitialized = false;
	}
}

bool MovieControl::isPlayingMovie() {
	return m_pMovieView && m_pMovieView->getParent();
}

void MovieControl::playMovie(CCObject* pTargetObj, SEL_MovieDoneCallFunc pCallbackFunc ) {
	if(m_pMovieView == NULL) {
        init();
	}

    m_pMovieView->playMovie(MovieView::MOVIE_ENTER_GAME, pTargetObj, pCallbackFunc);

}


