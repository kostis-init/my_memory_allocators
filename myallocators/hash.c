#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "hash.h"

#define TABLE_SIZE 4073

struct list
{
	uintptr_t key;	//key is the page's address
	uintptr_t value;	//value is the node's address
	struct list *next;
};

struct list *hash_table[TABLE_SIZE];

void hash_insert(uintptr_t key,uintptr_t value)
{
	int position = key % TABLE_SIZE;
	//printf("%d\n",position);
	//add the pair in that position's list
	if (hash_table[position]==NULL)//create list
	{
		hash_table[position]=malloc(sizeof(struct list));
		hash_table[position]->key=key;
		hash_table[position]->value=value;
		hash_table[position]->next=NULL;
	}
	else//list exists
	{
		struct list *temp = hash_table[position];
		//reach to end of the list
		while(temp->next!=NULL) temp=temp->next;
		temp->next=malloc(sizeof(struct list));
		temp->next->key=key;
		temp->next->value=value;
		temp->next->next=NULL;
	}
	//printf("inserted in %d\n",position);
	
}

uintptr_t hash_search(uintptr_t key)
{
	int position = key % TABLE_SIZE;
	uintptr_t value=0;
	//search into position's list
	struct list *temp = hash_table[position];
	while (temp!=NULL)
	{
		if (temp->key==key)
		{
			value = temp->value;
			break;
		}
		temp=temp->next;
	}
	return value;
}
