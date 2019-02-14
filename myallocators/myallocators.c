#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include "myallocators.h"
#include "hash.h"

struct dll
{
	struct dll *next,*prev;
	void *page;
	char bit_vector[128/CHAR_BIT];
	int class_size;							//in bytes i.e. 32/64/128/.../4096
};

//global variables
struct dll *class[8];						//class[0]->32 bytes, class[1]->64 bytes, ..., class[7]->4096 bytes
void *curptr_pool=NULL,*endptr_pool=NULL;	//pointers for 1MB pool

void *search_address(struct dll *tempptr, int class_num);	//search all "pages" of this class for free space

void *mymalloc(size_t cbytes)
{
	if (cbytes<=0)
		return NULL;
	if (cbytes>4096)
		return malloc(cbytes);
	
	void *return_address=NULL;
	
	//find appropriate class, by rounding to next multiple of 2 (right shift)
	char class_num=0;
	if(cbytes>32)//if it is less or equal than 32, no need for change, class_num=0
	{
		int temp=(cbytes-1)>>5;
		while(temp)
		{
			temp = temp >> 1;
			class_num++;
		}
	}
	
	if (curptr_pool==NULL || curptr_pool==endptr_pool) //take memory for pages if there is no left at the pool
	{
		uintptr_t temp = (uintptr_t)malloc(1000000);//cast to uintptr_t because we need to make it multiple of 4096
		uintptr_t end_temp = temp + 1000000;
		if(temp%4096!=0) // rare case that it is already a multiple - do nothing
		{
			temp >>=12;
			temp++;
			temp <<=12;
		}
		curptr_pool = (void *)temp;
		if(end_temp%4096!=0) // rare case that it is already a multiple - do nothing
		{
			end_temp >>=12;
			end_temp <<=12;
		}
		endptr_pool = (void *)end_temp;
	}
	
	if (class[class_num]==NULL)//no pages exist for this class, make first page
	{
		class[class_num]=malloc(sizeof(struct dll));
		class[class_num]->prev=NULL;
		class[class_num]->next=NULL;
		class[class_num]->page=curptr_pool;
		curptr_pool+=4096;
		class[class_num]->class_size=2<<(4+class_num);
		
		hash_insert((uintptr_t)class[class_num]->page,(uintptr_t)class[class_num]);////put it in hash table!!!!!!!!!
	}
	
	//search all "pages" of this class for free space
	return_address = search_address(class[class_num], class_num);
	
	if (return_address==NULL)//if space not found, create new "page"
	{
		//go to the last "page"
		struct dll *tempptr = class[class_num];
		while(tempptr->next!=NULL) tempptr=tempptr->next;
		
		tempptr->next = malloc(sizeof(struct dll));
		tempptr->next->prev=tempptr;
		tempptr->next->next=NULL;
		tempptr->next->page=curptr_pool;
		curptr_pool+=4096;
		tempptr->next->class_size=2<<(4+class_num);
		
		hash_insert((uintptr_t)tempptr->next->page,(uintptr_t)tempptr->next);
		
		return_address = search_address(class[class_num], class_num);
		
	}
	
	return return_address;
}


void myfree(void *address)
{
	uintptr_t add = (uintptr_t)address;
	//round address at prev multiple of 4096
	if(add%4096!=0) // rare case that it is already a multiple - do nothing
	{
		add >>=12;
		add <<=12;
	}
	
	struct dll *target_node = (void *)hash_search(add);	//search hash table to take the address' node
	if (target_node==NULL)//not ours, call system's free
		free(address);
	else//it is ours, change appropriate bit
	{
		int dif =(uintptr_t)address - add;
		char bit_to_change = dif/target_node->class_size;
		target_node->bit_vector[bit_to_change/CHAR_BIT] ^= 1<<(CHAR_BIT - bit_to_change%CHAR_BIT -1);
	}
}


void *search_address(struct dll *curptr,int class_num)
{
	void *return_address=NULL;
	while (curptr!=NULL)
	{
		for (int i = 0; i < 4096/(curptr->class_size); i++)//search bit vector until it finds one free slot
		{
			if(((curptr->bit_vector[i/CHAR_BIT] >> (CHAR_BIT - i%CHAR_BIT - 1)) % 2) == 0)//if free
			{
				curptr->bit_vector[i/CHAR_BIT] |= 1<<(CHAR_BIT - i%CHAR_BIT - 1);		//change bit
				return_address = curptr->page + i * curptr->class_size;					//save address
				//put the node first
				while (curptr->prev!=NULL)
				{
					struct dll *temp = curptr->prev;
					curptr->prev = temp->prev;
					if(curptr->next!=NULL)curptr->next->prev=temp;
					temp->prev=curptr;
					temp->next=curptr->next;
					curptr->next=temp;
				}
				class[class_num] = curptr;
				break;
			}
		}
		if(return_address!=NULL) break;
		curptr=curptr->next;
	}
	return return_address;
}

