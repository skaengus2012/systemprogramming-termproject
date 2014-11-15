#include "SlideWindow.h"
#include <iostream>
using namespace std;


SlideWindow::SlideWindow(void)
{
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
	for(int i = 1;i<window_size;i++)
	{
		if(compare > m1[i].getSequence())
			return false;
		compare = m1[i].getSequence();
	}
	return true;
}
bool SlideWindow::parity_check()
{
	//µ¥ÀÌÅÍ°¡ ÀÖŽÂ À©µµ¿ìžž ÆÐž®ÆŒ ÃŒÅ©ÇÑŽÙ.
	for(int i=0;i<window_size;i++)
	{
		if(m1[i].IsData() == true)
		{
			int bits = 0;
			bits = c1.CountData(m1[i].DataAccessor());
			if(bits % 2 != 0)
			{
				if(m1[i].getParity() == 1)
				{
					return true;
				}else
				{
					return false;
				}

			}else
			{
				if(m1[i].getParity() == 0)
				{
					return true;
				}else
				{
					return false;
				}
			}
		}else//µ¥ÀÌÅÍ°¡ ŸøŽÂ °æ¿ì
		{
			return true;
		}
	}
}
bool SlideWindow::check_start_n_finish()
{
	for(int i=0;i<window_size;i++)
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
	for(int i=0;i<window_size;i++)
	{
		if(m1[i].IsData() == true)
		{
			count = c1.CountData(m1[i].DataAccessor());
			if(count % 2 == 0)
			{
				m1[i].setParity(0);
			}else
			{
				m1[i].setParity(1);
			}
		}
	}
}
