#ifndef SEQSTACK_H_INCLUDED
#define SEQSTACK_H_INCLUDED


class SEQSTACK
{
	int seq[100000];
	int number;
public:
	int getSeq(int n1)
	{
		int temp = seq[n1];
		return temp;
	}
	void push(int n)
	{
		seq[number++] = n;
	}
	int pop()
	{
		int temp = number;
		number--;
		return seq[temp];
	}
	int peek()
	{
		return seq[number-1];
	}
	int getNumber()
	{
		return number;
	}
	SEQSTACK(void);
	~SEQSTACK(void);
};


#endif // SEQSTACK_H_INCLUDED
