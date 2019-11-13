/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		sortedlist							  *
*		7/11/2019					  *
*									  *
************************************/

#include <stdio.h> /* printf */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "dllist.h"
#include "sortedlist.h"
#include "MyUtils.h" /* MAX2,MIN2 */

struct srt_list
{
	dl_list_t *dll;
	is_before_t is_before_ptr;
	void *is_before_param;
};

struct srt_iterator
{
	dll_iter_t iterator;
};

srt_list_t *SrtListCreate(void *param, is_before_t ptr)
{
	srt_list_t *new_srtlist = (srt_list_t *)malloc(sizeof(srt_list_t));
	if (NULL == new_srtlist)
	{
		return NULL;
	}	
	
	new_srtlist -> dll = DLListCreate();
	if (NULL == new_srtlist -> dll)
	{
		free(new_srtlist);
		
		return NULL;
	}
	
	assert(ptr);
	
	new_srtlist -> is_before_ptr = ptr;
	new_srtlist -> is_before_param = param;
	
	return new_srtlist;
}

void SrtListDestroy(srt_list_t *list)
{
	DLListDestroy(list -> dll);
	list -> is_before_ptr = NULL;
	list -> is_before_param = NULL;
	free(list);
}

srt_iter_t SrtListInsert(void *data, srt_list_t *list)
{
	dll_node_t *node = NULL;
	srt_iter_t srt_iter;
	
	for (node = DLListBegin(list -> dll);
		 node != DLListEnd(list -> dll);
		 node = DLListNext(node))
	{
		if (1 == (list -> is_before_ptr(data, DLListGetData(node), list -> is_before_param)))
		{
			break;
		}
	}
	
	DLListInsert(data, node, list -> dll);
	srt_iter.iterator = node;
	
	return srt_iter;
}






