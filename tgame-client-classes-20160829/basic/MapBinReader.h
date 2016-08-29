#ifndef __MAP_BIN_READER_H__
#define __MAP_BIN_READER_H__

#include "MapBin.pb.h"

using namespace std;

class MapBinReader
{
public:    
    static int Init(CMapBin& stMapBin, const char* pstrMapBinPath)
    {
        stMapBin.Clear();
        int iRet = 0;

        unsigned long iLen = 0;
        unsigned char* pFile = CCFileUtils::sharedFileUtils()->getFileData(pstrMapBinPath, "rb", &iLen);
        CCLog("[trace]=============MapBinReader,file=%s,file_len=%lu", pstrMapBinPath, iLen);

        if (! stMapBin.ParseFromArray(pFile, iLen))
        {
            CCLog("[error]=============MapBinReader::Init,read config %s fail 2!", pstrMapBinPath);
            if(pFile) {
                delete [] pFile;
                pFile = NULL;
            }
            return -1;
        }
        if(pFile) {
            delete [] pFile;
            pFile = NULL;
        }

        
        return iRet;
    }
    

};


#endif

