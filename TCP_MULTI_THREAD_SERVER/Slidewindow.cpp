#include "SlideWindow.h"


SlideWindow::SlideWindow(void)
{
    strcpy(response, "");
}
SlideWindow::~SlideWindow(void)
{
}
MyStruct* SlideWindow::MyStructAccessor()
{
    return m1;
}
bool SlideWindow::sequence_check()
{
//m1 << m2 << m3
    int compare = m1[0].getSequence();
    for(int i = 1; i<window_size; i++)
    {
        if(compare > m1[i].getSequence())
            return false;
        compare = m1[i].getSequence();
    }
    return true;
}
bool SlideWindow::parity_check()
{
//데이터가 있는 윈도우만 패리티 체크한다.
    for(int i=0; i<window_size; i++)
    {
        if(m1[i].IsData() == true)
        {
            int bits = 0;
            bits = c1.CountData(m1[i].DataAccessor());
            if(bits % 2 != 0)
                return false;
            else
                return true;
        }
    }
}
bool SlideWindow::check_start_n_finish()
{
    for(int i=0; i<window_size; i++)
    {
        if(m1[i].getStart() != 0 && m1[i].getFinish() != 0)
        {
            return false;
        }
    }
    return true;
}
void SlideWindow::set_parity_bit()
{
    int count = 0;
    for(int i=0; i<window_size; i++)
    {
        if(m1[i].IsData() == true)
        {
            count = c1.CountData(m1[i].DataAccessor());
            if(count % 2 == 0)
            {
                m1[i].setParity(0);
            }
            else
            {
                m1[i].setParity(1);
            }
        }
    }
}
