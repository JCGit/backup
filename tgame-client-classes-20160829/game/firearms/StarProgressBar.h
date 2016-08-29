#ifndef StarProgressBar_h__
#define StarProgressBar_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BaseLayout.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

class CStarProgressBar : public BaseLayout
{
public:
	CStarProgressBar();
	virtual ~CStarProgressBar();

    //pszSpSheetName,SpriteSheet�����֣�����CCSpriteBatchNode
	static CStarProgressBar* create(const char *pstBgFileName, const char *pstProgressFileName, 
        const char* pszSpSheetName, const int iMaxStarNum = 7);


	virtual void onSizeChanged();

	int getStarMargin() const;

	void setStarMargin(int nMargin);

    void setStarNum(int num, const int iTotalNum);
    void setStarNum(int num);

	int getStarNum();
protected:
    //pszSpSheetName,SpriteSheet�����֣�����CCSpriteBatchNode
    bool initWithTextures(const char *pstBgFileName, const char *pstProgressFileName, 
        const char* pszSpSheetName, const int iMaxStarNum);
private:
	void resizeStars();
    CCSpriteBatchNode* m_pBatchNode;

	int m_nStarTotalNum;	//����
	int m_nStarMargin;		//���
	int m_nStarNum;			//��ǰ����
	string m_strBgImgURL;
	string m_strProgressImgURL;
	vector<CCSprite*> m_stProgressImgList; 
};

#endif // StarProgressBar_h__
