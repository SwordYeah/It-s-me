#include "BoundedBuffer.h"
#include "system.h"

BoundedBuffer::BoundedBuffer(int max_size)
{
    maxsize = max_size;
    buffer =  (int *)malloc(sizeof(int)*(maxsize));
    in = out = 0;
    count = 0;
    lock = new Lock("bufferLock");
    WriteFull = new  Condition("Write");
    ReadEmpty = new Conditon("Read");
    
}

bool IsEmpty()
         {
	return (count == 0);
         }

bool IsFull()
         {
	return (count == maxsize);
         }

BounndedBuffer::~BoundedBuffer()
{
    free(buffer);
}

void  BoundedBuffer::Read(void *data,int size)
{
    int i,a;
    int *s = (int *)data;
    lock -> Acquire();
    for(i =0;i<size;i++)
        {
	while(IsEmpty())
		ReadEmpty -> Wait(lock);
	*(s + i) = *(buffer +out%maxsize);
	out = (out+1)%maxsize;
	count--;
	WriteFull -> Signal(lock);
        }
    *(s +1) = '\0'; 
    lock -> Release();
}

void  BoundedBuffer:Write(void *data,int size)
{
     int *s = (int *)data;
     int i;
     lock -> Acquire();
     for(i=0;i<size;,i++)
        {
	while(IsFull())
		WriteFull -> Wait(lock);
	*(buffer +in) = *(s + i);
	in = (in+1)%maxsize;
	count++;
	ReadEmpty -> Signal(lock);
        }
    *(s +1) = '\0'; 
    lock -> Release();
        }
}

void PrintBuffer()
{
    int i = out;
    int num =count;
    printf("\nCurrent buffer is :");
    while(num--)
    {
	printf("%d ",*(buffer+i));
	i = (i + 1)%maxsize;
    }