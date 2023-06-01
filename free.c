#include "malloc.h"

/**
 * _free - frees a memory space
 *
 * @ptr: a pointer to the memory space to be freed
 *
 * Return: is void
 */
void _free(void *ptr)
{
	if (!ptr)
		return;

	free_list_add(BLK_HEADER(ptr));
	free_blocks();
}
