#ifndef ItemUtils_h__
#define ItemUtils_h__

#include <string>
#include "BasicTypes.h"
using namespace std;

class ItemUtils
{
public:
	static string getItemIcon(const int iType, const int iCfgID);

	static string getItemName(const int iType, const int iCfgID);

	static string getItemDesc(const int iType, const int iCfgID);

	static unsigned int getItemColor(const int iType, const int iCfgID);

	static UINT64_t getEquipFightPower(const int iCfgID, const int iRefineLv, const int iPart);

	static UINT64_t getWeaponFightPower(const int iCfgID, const int iLv, const int iColor);

    //����װ��������ֵ(������ֵ��������������ֵ)
    static unsigned int getEquipAttrVal(const int iEquipCfgID, const int iEquipLv, const int iQuality);

    //��ȡװ������ʾ���� ���۳��������ԣ���������л�Ҫ����ϵ����
    static unsigned int getEquipShowAttrVal(const int iEquipCfgID, const int iEquipLv, const int iQuality);
    static string getColorBgImg(const int iEquipCfgID);
    static string getThingTypeName(const int iThingType);

	static string numbFormateW(int iNumb);
protected:
private:
};

#endif // ItemUtils_h__
