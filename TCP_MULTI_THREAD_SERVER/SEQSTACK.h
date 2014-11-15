#ifndef SEQSTACK_H_INCLUDED
#define SEQSTACK_H_INCLUDED

class SEQSTACK
{
    int seq[100000];
    int number;
public:
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
    SEQSTACK(void);
    ~SEQSTACK(void);
};

#endif // SEQSTACK_H_INCLUDED
