#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "../myallocators/myallocators.h"

#define SIZE 250

int main(void)
{
	int *array[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		array[i]=mymalloc(i*5*sizeof(int));
		printf("loop %3d: mymalloc returned address %" PRIuPTR " for size = %d\n",i,(uintptr_t)array[i],i*5*sizeof(int));	
	}
	
	
	myfree(array[50]);
	printf("freed array[50] = %" PRIuPTR "\n",(uintptr_t)array[50]);
	int *a=mymalloc(250*sizeof(int));
	printf("a \t\t= %" PRIuPTR "\n",(uintptr_t)a);
	myfree(a);
	array[50]=mymalloc(250*sizeof(int));
	printf("array[50] \t= %"PRIuPTR"\n",(uintptr_t)array[50]);
	
	
	for (int i = 0; i < SIZE; i++)
	{
		myfree(array[i]);
		//printf("freed %d\n",i);	
	}
	
	
	return 0;
}
