#ifndef __UpdateHelper_H_
#define __UpdateHelper_H_

#include <string>

#include "support/zip_support/unzip.h"
#include "md5.h"
#include "cocos2d.h"

using namespace std;
USING_NS_CC;



class TempFile {
public:
    TempFile(const std::string & path):
        m_fp(NULL)
    {
            m_path = path;
    }

    ~TempFile();

    const std::string& GetPath() const {
        return m_path;
    }
    FILE * GetFD() {
        return m_fp;
    }

    bool Open(const char * mode);
    void Close();
    const std::string GetSign();
    size_t Write(char *ptr, size_t size, size_t nmemb);

private:
    std::string m_path;
    FILE * m_fp;
    char m_sign[32];
};



class ZipFile {
public:
    ZipFile(const char * path, int* iProgress) {
        m_file = unzOpen(path);
        m_bIsHaveSo = false;
        m_piProgress = iProgress;
        *m_piProgress = 0;
    }
    ~ZipFile() {
        unzClose(m_file);
    }

    bool IsOpen() {
        return m_file != NULL;
    }
    bool getIsHaveSo() {
        return m_bIsHaveSo;
    }

    int ExtractTo(const char * root);
private:
    unzFile m_file;
    bool m_bIsHaveSo; //是否有so在里面
    int* m_piProgress; //当前处理的进度百分比

};


class UpdateHelper {
public:
    static std::string GetPath(const char * filepath);
    static std::string ConvertWinPath(const string& str);

    static size_t saveDataToFD(char *ptr, size_t size, size_t nmemb, void *userdata);
    static size_t saveData(char *ptr, size_t size, size_t nmemb, void *userdata);

    static void HexMd5(const unsigned char * input, char * output);

    static int MakeDirs(const char * path);
    static int RemoveDir(const char *path);
};

#endif //__UpdateHelper_H_

