#ifndef MYSTRUCT_H_INCLUDED
#define MYSTRUCT_H_INCLUDED


#define FILE_BUFSIZE 19// char ¹è¿­ »çÀÌÁî 20¹ÙÀÌÆ®(+ NULL)
#include <iostream>
using namespace std;
#include <string.h>
class MyStruct
{
    bool FILE_START;//ÆÄÀÏÀÇ œÃÀÛÀÎÁö
    short start;//œÃÀÛ¹ÙÀÌÆ®
    bool HasData;//µ¥ÀÌÅÍÀÇ ÁžÀç¿©ºÎ
    int sequence;//ŒøŒ­¹øÈ£
    char buf2[FILE_BUFSIZE+1];//µ¥ÀÌÅÍ, NULL»çÀÌÁî + 1
    short parity;//ÆÐž®ÆŒ¹ÙÀÌÆ® 0(0x0000) ¶ÇŽÂ 1(0x0001)
    short finish;//ÁŸ·á¹ÙÀÌÆ®
    bool END;//ÆÄÀÏÀÇ ³¡
public:
    MyStruct(void);
    ~MyStruct(void);
    void setHasData(bool h)
    {
        HasData = h;
    }
    bool getEND();
    bool getFILE_START();
    void setSequence(int seq);
    void setParity(short p);
    short getStart();
    int getSequence();
    short getParity();
    short getFinish(){
        return finish;
    }
    char* DataAccessor();
    bool IsData();
    void setBuf(char *buf);

};





#endif // MYSTRUCT_H_INCLUDED
