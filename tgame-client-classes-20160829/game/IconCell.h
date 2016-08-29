#ifndef IconCell_h__
#define IconCell_h__

#include "basic/BaseLayout.h"
class CStarProgressBar;
class IconCell: public BaseLayout
{
public:
	IconCell();
	virtual ~IconCell(){}

    static IconCell* create() {
        IconCell *pItemCell = new IconCell();

        if(pItemCell != NULL && pItemCell->init()) {
            pItemCell->autorelease();
        } else {
            CC_SAFE_DELETE(pItemCell);
        }

        return pItemCell;
    }

	// override start

	virtual bool init();

    virtual std::string getDescription() const{
        return "IconCell";
    }
    
	UIImageView* getIconImg(){return m_pIconImg;}

	void setOpacity(GLubyte opacity);

    void setInfo(const int iType, const int iNum, const int iCfgID, ccColor3B color = ccGREEN);

	void setNum(const int iNum, ccColor3B color = ccWHITE);

    void clearCell();

    void showNumTxt();

	void showNumTxt(bool bIsShow);

    UILabel* getNumTxt() {
        return  m_pNumTxt;
    }

    int getItemType() {return m_iType;}
    int getItemNum() {return m_iNum;}
    int getItemCfgid() {return m_iCfgID;}

protected:
	UIImageView *m_pIconImg;
    UILabel *m_pNumTxt;
	ccColor3B m_stColor;

    int m_iType;
    int m_iNum;
    int m_iCfgID;

    bool m_bIsShowNumTxt; //默认不显示数量文本

	CStarProgressBar* m_pStarBar;
};

#endif // IconCell_h__
