#ifndef COUNTONEBITS_H_INCLUDED
#define COUNTONEBITS_H_INCLUDED

#include <string.h>
#define FILE_BUFSIZE 19
class CountOneBits
{
	int check;
public:
	int CountData(char* data2)
	{
		char data[FILE_BUFSIZE+1];//data »çÀÌÁîžŠ 4¹ÙÀÌÆ®(32ºñÆ®)·Î ÇÏÀÚ
		char temp;
		strcpy(data, data2);

		int count = 0;
		for(int i=0; i<FILE_BUFSIZE+1;i++)
		{
			temp = data[i];
			for(int i=0;i<8;i++, check = check << 1)
			{
				data[i] &= check;
				if((int)temp == 0)
				{
					data[i] = temp;
					continue;
				}
				data[i] = temp;
				count++;
			}
			check = 1;
		}
		return count;
	}
	CountOneBits(void);
	~CountOneBits(void);
};


#endif // COUNTONEBITS_H_INCLUDED
