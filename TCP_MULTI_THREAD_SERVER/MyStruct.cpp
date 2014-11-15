#include "MyStruct.h"
MyStruct::MyStruct(void)
{
    strcpy(buf2, "");
    start = 0;
    finish = 0;
    HasData = false;
    END = false;
    FILE_START = false;
}
MyStruct::~MyStruct(void)
{
}
void MyStruct::setSequence(int seq)
{
    sequence = seq;

}
void MyStruct::setParity(short p)
{
    parity = p;
}
short MyStruct::getStart()
{
    return start;
}
int MyStruct::getSequence()
{
    return sequence;
}
short MyStruct::getParity()
{
    return parity;
}
short MyStruct::getFinish()
{
    return finish;
}
char* MyStruct::DataAccessor()
{
    return buf2;
}
bool MyStruct::IsData()
{
    return HasData;
}
void MyStruct::setBuf(char *buf)
{
    strcpy(buf2, buf);
    HasData = true;
}
bool& MyStruct::AccessorHasData()
{
    return HasData;
}
