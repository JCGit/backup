/**
* 文件类，用于文件内容读取和写入
* @author will
*/
#ifndef __CT_FILE_H__
#define __CT_FILE_H__ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace CVLib
{

class CVFile
{
public:
    CVFile()
        :m_pstFile(NULL)
    {

    }

    virtual ~CVFile()
    {
        if (m_pstFile)
        {
            fclose(m_pstFile);
        }
    }
    
    bool HasOpened()
    {
        return (m_pstFile != NULL);
    }

    int Pos()
    {
        if (m_pstFile)
        {
            return ftell(m_pstFile);
        }

        return 0;
    }

    int Seek(int iPos)
    {
        if (m_pstFile)
        {
            return fseek(m_pstFile, iPos, SEEK_SET);
        }
        return -1;
    }

protected:
    FILE* m_pstFile;
};

class CVReadFile : public CVFile
{
public:
    CVReadFile()
    {
        m_iBufLen = 1;
        m_sBuf = (char*)malloc(m_iBufLen);
    }

    virtual ~CVReadFile()
    {
        free(m_sBuf);
    }

    int Open(const char* szFilePos)
    {
        if (m_pstFile)
        {
            fclose(m_pstFile);
        }

        m_pstFile = fopen(szFilePos, "r");
        if (m_pstFile == NULL)
        {
            return -1;
        }
        
        return 0; 
    }

    //读取一行，以delim结尾
    int Read(char delim)
    {
        if (m_pstFile == NULL)
        {
            return -1;
        }

        int iNowPos = Pos();  //记录当前位置

        m_iReadSize = (int)getdelim(&m_sBuf, &m_iBufLen, delim, m_pstFile); 
        if (m_iReadSize < 0)
        {
            m_iReadSize = 0;
            return -2;
        }

        if (m_sBuf[m_iReadSize - 1] != (char)delim) //一行没读完文件就结束了
        {
            Seek(iNowPos);
            return -3;
        }
        
        return 0;
    }

    char* GetBuf()
    {
        return m_sBuf;
    }

    int ReadSize()
    {
        return (int)m_iReadSize; 
    }

    //读取一行，以\n结尾
    char* ReadLine()
    {
        int iRet = Read('\n'); 
        if (iRet != 0)
        {
            return NULL;
        }

        char* sTemp = GetBuf();
        if (sTemp[ReadSize()-1] != '\n')
        {
            return NULL;
        }
        
        sTemp[ReadSize()-1] = '\0';
        return sTemp;
    }


private:
    size_t m_iBufLen;
    char* m_sBuf;
    size_t m_iReadSize;
};

class CVWriteFile : public CVFile
{
public:
    CVWriteFile()
    {

    }

    virtual ~CVWriteFile()
    {

    }

    int Open(const char* szFilePos, bool bAppend = false)
    {
        if (m_pstFile)
        {
            return 0;
        }
        
        if (bAppend)
        {
            m_pstFile = fopen(szFilePos, "a");
        }
        else
        {
            m_pstFile = fopen(szFilePos, "w");
        }
        
        if (m_pstFile == NULL)
        {
            return -1;
        }

        return 0; 
    }

    int Write(const char* szBuf, int iLen, char delim)
    {
        if (szBuf == NULL || iLen <= 0 || m_pstFile == NULL)
        {
            return -1;
        }
        
        int iWriteSize = fwrite(szBuf, sizeof(char), iLen, m_pstFile);
        if (iWriteSize != iLen)
        {
            return -2;
        }
        
        iWriteSize = fwrite(&delim, sizeof(char), 1, m_pstFile);
        if (iWriteSize != 1)
        {
            return -3;
        }

        fflush(m_pstFile);
        return 0;
    }

    int WriteLine(const char* szLine)
    {
        return Write(szLine, strlen(szLine), '\n');
    }
};

}

#endif
