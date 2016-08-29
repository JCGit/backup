#ifndef __UpdateMgr_H_
#define __UpdateMgr_H_

//#include <thread>
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <condition_variable>
#include <queue>
#include <string>
#include <unordered_map>
#include <functional>

class TempFile;
struct DownloadPackage {
    DownloadPackage(){
    }
	DownloadPackage(const char * _pkg, const char * _sign, const char * _version, int nSize) :
		packageName(_pkg), sign(_sign), version(_version), nSize(nSize){
    }
    std::string packageName;
    std::string sign;
	std::string version;
	int		nSize;
};

struct DownloadApk
{
	DownloadApk(){
	}
	DownloadApk(const char * _apkName, const char * _apkURL) :
		apkName(_apkName), apkURL(_apkURL){
	}
	std::string apkName;
	std::string apkURL;
};

class iUpdateCallback
{
public:
	//virtual void setPercentage(float percent) = 0;
	//virtual void setNoticeWords(const char* szText) = 0;
	virtual void onChangeScene() = 0;
	virtual void onFail() = 0;
	virtual void onRestart() = 0;
};


class UpdateMgr {
public:
    enum ExceptionDef
    {
        //������
        ERROR_CURL_INIT_FAIL    = 1001, //curl��ʼ��ʧ��
        ERROR_CURL_DO_FAIL     , //curl_easy_performʧ��
        ERROR_CURL_DO_FAIL2    , //

        ERROR_JSON_PARSE_FAIL  , //JSON parse fail
        ERROR_JSON_FORMAT_ERROR, //JSON ��ʽ����
        ERROR_JSON_EXCEPTION   , //JsonException
		ERROR_JSON_NOT_EXIST   , //json �ļ�������

        ERROR_SIGN_WRONG       , //������md5У��ʧ��

        ERROR_ZIP_OPEN_FAIL    , //ZipFile openʧ��
        ERROR_ZIP_EXTRACT_FAIL , //��ѹʧ��
        ERROR_VER_FILE_FAIL    , //version�ļ���ʧ��

		ERROR_USER_CANCEL,		 //�û�ȡ������
    };


    enum Stage {
        CHECK_VERSION = 0,
		WAIT_CONFIRM,		//�ȴ��û�ȷ��
        DOWNLOAD_RESOURCE,
        EXTRACT_RESOURCE, //��ѹ�ļ�
		RESTART,
        DONE,
		UPDATE_FAIL, //����ʧ��
		LOAD,
		DOWNLOAD_APK,
		DOWNLOAD_APKFAIL,
		RELOAD_APK,
    };


    struct Status {
        Stage stage;
        int totalStep;
        int nowStep;
        int nowProgress;
        std::string filename;
        int errorCode;
        bool needRestartGame; //�Ƿ���Ҫ������Ϸ
    };
    
    UpdateMgr();
    virtual ~UpdateMgr();
	void setUpdateCallback(iUpdateCallback* callback) { m_callback = callback; };

	virtual void startUpdate();
    Status GetStatus();
	void SetStage(Stage stage);
    int OnProgress(float total, float now);
	void ThrowError(const char * filename, int what);

	bool checkNeedPackage();
	bool isUpdate(std::string serS, std::string locS);
	std::thread * getUpdateThread(){ return m_updateThread; }

	virtual std::string GetExtractPath() = 0;
	virtual void setResUrl(const char* pszUrl) = 0;

	bool DownloadServerStateFile(const char * url);

	int	getUpdateConfirm(){ return mUpdateConfirm; }
	void	 setUpdateConfirm(int nConfirm){ mUpdateConfirm = nConfirm; }

	void	setPromptLimit(int nLimit){ m_LimitPrompt = nLimit; }

private:

	// ��ʾ�û��Ƿ�������� 0:δ���� 1:ȡ�� 2:ȷ��
	bool waitCheckDownload();
	void PushNeedPackage(const char * pkg, const char * sign, const char * version, int nSize);
    bool GetNeedPackageList(const char * versionFile, const char * version);
    bool UpdateSinglePkg(const DownloadPackage & pkgInfo);
	bool DownloadFile(TempFile & file, const char * url, const char * filename);
    bool DownloadVersionFile(const char * path, const char * url, const char * filename);
    bool ExtractFile(TempFile & tmpFile, const char * pkgname);
	void loadGameRes();
	void downloadAPK(const char *apkName ,const char * url);
protected:
	virtual bool initUpdate(){ return true; };

    virtual std::string GetPkgUrl(const char * pkgName) = 0;
    virtual std::string GetTempDownloadPath(const char * pkgName) = 0;
	virtual std::string GetCurrentVersion() = 0;
    virtual bool UpdateVersion(std::string version) = 0;
    virtual bool IsApkNewest() = 0; //apk assets�汾�Ƿ�ȱ��ذ汾��
    virtual void delResDataDir();

    void doUpdate();



protected:
	iUpdateCallback* m_callback;

	Status m_status;
	std::vector<DownloadPackage> m_needPkg;
	DownloadApk m_needAPK;

	std::string m_resUrl;

	// �û�ȷ�ϸ�����Ϣ 1:ȡ�� 2:ȷ��
	int	mUpdateConfirm;

private:
    std::mutex m_mutex;
	std::thread* m_updateThread;
	bool b_downloadApk;
	int		m_LimitPrompt;	// ���°����ڸ�ֵ����ʾwifi����
};

#endif //__UpdateMgr_H_

