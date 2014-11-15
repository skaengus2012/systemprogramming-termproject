#ifndef SLIDEWINDOW_H_INCLUDED
#define SLIDEWINDOW_H_INCLUDED



#include <errno.h>

#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h> //for threading, link with lpthread
#include <iostream>


#include "SEQSTACK.h"
#include "MyStruct.h"
#include "CountOneBits.h"
#include <stdio.h>
#include <stdlib.h>



#define window_size 3
class SlideWindow
{
	MyStruct m1[window_size];//œœ¶óÀÌµå À©µµ¿ì Å©±â 3
	CountOneBits c1;
	SEQSTACK STACK_SEQ;
	char response[10];
public:
	char* AccessorResponse()
	{
		return response;
	}
	void loadData(int n, char *data)
	{
		m1[n].setBuf(data);
	}
	SEQSTACK& SEQSTACKAddress()
	{
		return STACK_SEQ;
	}
	MyStruct& MyStructAddress(int n)
	{
		return m1[n];
	}
	MyStruct* MyStructAccessor();
	bool sequence_check();
	bool parity_check();
	bool check_start_n_finish();
	void set_parity_bit();
	SlideWindow(void);
	~SlideWindow(void);
};




#endif // SLIDEWINDOW_H_INCLUDED
