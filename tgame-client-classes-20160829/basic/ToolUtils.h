#ifndef ToolUtils_h__
#define ToolUtils_h__

#include "cocos2d.h"
#include "cocos-ext.h"

#include <vector>
#include <string>

using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class CToolUtils
{
public:
	static void ArrayChangeOrder(CCArray* pstArr);					//�ı����������
    static bool checkResContains(CCString* str, CCArray* arr);		//����ַ����Ƿ���������

    static void removeArmatureFile(const char* pstrArmatureFile);
    static void removePlistTexFile(const char* pszPlist);

    static void loadArmatureRGBA4444(const string& strJsonPath); //����rgba4444��ʽ����Դ(�������Ͳ�����Ч)
    static void loadSpriteFrameRGBA4444(const string& strPlistPath); //����rgba4444��ʽ������

    static bool parseUTFStr(const string& sin, vector<string>& ret);	  //����utf8�ı�sin��ÿ���ַ���ret��(����Ϊ��Ӣ�Ļ���)

	static void parseUTF8(const string& sin, vector<string>& ret);

	static string splitChStr(const std::string& src, const int iOneLine);  //�������ַ����м���뻻�з�(����Ϊ������)

	static string getResType(const std::string& str);		//ͨ����Դ����ȡ��Դ������,��ͼƬ��Դ��������Դ��(.png, .ExportJson)

	static string splitUTFStr(const string& sin, const int iOneLine);		 //���ַ����м���뻻�з�(����Ϊ��Ӣ�Ļ���)

	static ccColor3B parseColorToCC3(const int color);						//����ɫת��Ϊr g b

	static string SecondToStr(int sec);   //ʱ��ת��
	static string AddZero(int time);

	static void setTimeScale(float scale);		//����ȫ�ֵļ��١�����


    //����iThingType����pBgImg�Ĵ�С
    //����true��ʾ��(166,88)�Ĵ�icon
    static bool adjustIconBgSize(const int iThingType, const int iThingCfgID, UIImageView* pBgImg = NULL);
    static void adjustFragIcon(const int iThingType, UIImageView* pBgImg); //�����Ƿ���ʾ��Ƭͼ��

	/**
     * @brief ��Widget����ӽǱ�
     * @param pWidget	Ҫ��ӽǱ��Widget
	 * @param uBadgeNum	��ʾ�ڽǱ��ϵ����ֵ�ֵ��Ϊ0ʱ���Ƴ����еĽǱ꣬�ﵽ����ֵʱ���ֻ���"!"
	 * @param offsetX	X���ϵ�ƫ����
	 * @param offsetY	Y���ϵ�ƫ����
     */
	static void badgeWidget(Widget *pWidget, unsigned int uBadgeNum, float offsetX = 0.0f, float offsetY = 0.0f);

	/**
     * @brief ��Widget����Ӳ������ֵĽǱ�
     * @param pWidget	Ҫ��ӽǱ��Widget
	 * @param uBadgeNum	����0����ʾ�Ǳ꣬������Ƴ����еĽǱ�
	 * @param offsetX	X���ϵ�ƫ����
	 * @param offsetY	Y���ϵ�ƫ����
     */
	static void badgeWidgetWithPoint(Widget *pWidget, unsigned int uBadgeNum, float offsetX = 0.0f, float offsetY = 0.0f);

	/**
     * @brief ������������Ч
     * @param pWidget	Ҫ��ӵ����
	 * @param bIsAdd	�Ƿ����
     */
	static void effectEntryIcon(Widget *pWidget, bool bIsAdd);

protected:
private:
	static float m_fTimeScale;
};

#endif // ToolUtils_h__
