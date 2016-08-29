#ifndef __UpdateMgrImp_H_
#define __UpdateMgrImp_H_

#include "UpdateMgr.h"


class UpdateMgrImp : public UpdateMgr 
{
public:

	static UpdateMgrImp* getInstance();

	virtual void setResUrl(const char* pszUrl);

	//-1��ʾ����ʧ�ܣ�0 ��ʾ����Ҫ���� 1��ʾ��Ҫ����
	void updateSearchPath();
	void startUpdate();

	void setIsUpdateSceneShow(bool isShow)
	{
		m_isShowUpdateScene = isShow;
	}

	bool getIsUpdateSceneShow()
	{
		return m_isShowUpdateScene;
	}
public:
	virtual std::string GetExtractPath();
	virtual std::string GetCurrentVersion();
protected:

	UpdateMgrImp();
	//~UpdateMgrImp();

	bool initUpdate();
	bool queryDirConfig();

    virtual std::string GetPkgUrl(const char * pkgName);
    virtual std::string GetTempDownloadPath(const char * pkgName);
    virtual bool UpdateVersion(std::string version);
    virtual bool IsApkNewest();
    virtual void delResDataDir();


	std::string getJsonVer(const char* strFilePath);

	//�Ƚϰ汾���Ƿ����

private:
	bool m_isShowUpdateScene;

	static UpdateMgrImp* s_updatMgrImp;

    std::string m_strResUrl;
	std::string m_writableFolder;

	//�Ƿ���Ը�������·��(Ĭ��true,���apk�汾���ߣ���Ϊfalse)
	//bool m_bCanUpdateSearchPath; 
};

#endif //__UpdateMgrImp_H_
