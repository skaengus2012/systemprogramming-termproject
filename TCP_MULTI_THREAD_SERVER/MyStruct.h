#ifndef MYSTRUCT_H_INCLUDED
#define MYSTRUCT_H_INCLUDED


#define FILE_BUFSIZE 19// char 배열 사이즈 20바이트(+ NULL)
#include <iostream>
using namespace std;
#include <string.h>
class MyStruct
{
    bool FILE_START;//파일의 시작인지
    short start;//시작바이트
    bool HasData;//데이터의 존재여부
    int sequence;//순서번호
    char buf2[FILE_BUFSIZE+1];//데이터, NULL사이즈 + 1
    short parity;//패리티바이트 0(0x0000) 또는 1(0x0001)
    short finish;//종료바이트
    bool END;
public:
    void setFILE_START(bool temp)
    {
        FILE_START = temp;
    }
    void setEND()
    {
        END = true;
    }
    bool& AccessorHasData();
    void setSequence(int seq);
    void setParity(short p);
    short getStart();
    bool getFILE_START()
    {
        return FILE_START;
    }
    bool getEND()
    {
        return END;
    }
    int getSequence();
    short getParity();
    short getFinish();
    char* DataAccessor();
    bool IsData();
    void setBuf(char *buf);
    MyStruct(void);
    ~MyStruct(void);
};



#endif // MYSTRUCT_H_INCLUDED
