//*******************************************************
// Justin Sanchez COP4530
// 10/7/2016 MW 11:00-12:15
//*******************************************************

#include <cassert>
#include <iostream>
#include "dlUtils.h"
#include "MemoryManager.h"

MemoryManager::MemoryManager(unsigned int memtotal) : memsize(memtotal)
{
	baseptr = new unsigned char[memsize];
	blockdata originalBlock(memsize, true, baseptr);
	firstBlock = new dlNode<blockdata>(originalBlock, NULL, NULL);
}

void MemoryManager::showBlockList()
{
	printDlList(firstBlock, "->");
}

void MemoryManager::splitBlock(dlNode<blockdata> *p, unsigned int chunksize)
{ // Put your code below
	unsigned char* ch = new unsigned char[100]();
	int num = p->info.blocksize - chunksize;
	blockdata temp = {num, true, ch};
	//Add node after p
	p->next = new dlNode<blockdata>(temp, p, p->next);
	p = p->next;
	if (p->next != NULL)
		p->next->prev = p;

}

unsigned char * MemoryManager::malloc(unsigned int request)
{ // Put your code below
	dlNode<blockdata> *temp = firstBlock;
	while (temp != NULL) 
	{
		//Check if the block is free
		if (temp->info.free == true)
		{
			//Check block size
			if (temp->info.blocksize == request || temp->info.blocksize > (int)request) 
			{
				if (temp->info.blocksize == request) 
				{
					//if they are equal then change to false
					temp->info.free = false;
					temp->info.blocksize = request;
				}
				else 
				{
					//splitBlock
					temp->info.free = false;
					splitBlock(temp, request);
					temp->info.blocksize = request;
				}
				//return original location
				return temp->info.blockptr;
			}
		}
		temp = temp->next;
	}
	//If no block is big enough, return NULL
	return NULL;
}

void MemoryManager::mergeForward(dlNode<blockdata> *p)
{ // Put your code below
	if (p->next != NULL)
	{
		if (p->next->info.free == true)
		{
			dlNode<blockdata> *temp = p->next;
			//merges the two blocks
			p->info.blocksize += p->next->info.blocksize;
			//Delete the node
			if (p->next->next != NULL) {
				p->next->next->prev = p;
				p->next = temp->next;
				delete temp;
			}
			else
			{
				p->next = NULL;
				delete temp;
			}
		}
	}
}

void MemoryManager::mergeBackward(dlNode<blockdata> *p)
{ // Put your code below
	if (p->prev != NULL)
	{
		if (p->prev->info.free == true)
		{
			dlNode<blockdata> *temp = p->prev;
			//merges the two blocks
			p->info.blocksize += p->prev->info.blocksize;
			//Delete the node
			if (p->prev->prev != NULL) {
				p->prev->prev->next = p;
				p->prev = temp->prev;
				delete temp;
			}
			else
			{
				p->prev = NULL;
				delete temp;
			}
		}
	}
}

void MemoryManager::free(unsigned char *ptr2block)
{ // Put your code below
	dlNode<blockdata> *temp = firstBlock;
	unsigned char* tem;
	//looks for block with blockptr == ptr2block
	while (temp != NULL) 
	{
		if (temp->info.blockptr == ptr2block) 
		{
			//set bool value to free
			temp->info.free = true;
			tem = ptr2block;
			//check if block can be merged
			if (temp->next != NULL && temp->next->info.free) 
			{
				mergeForward(temp);
			}

			if (temp->prev != NULL && temp->prev->info.free)
			{
				mergeBackward(temp->prev);
			}

		}
		temp = temp->next;
	}
	if (tem != ptr2block)
		exit(0);
}
