#include "malloc.h"

/**
 * naive_malloc - naive version of malloc
 * @size: size of memory to allocate
 * Return: is void*
 */
void *naive_malloc(size_t size)
{
	void *ptr = sbrk(0);
	void *request = sbrk(size);

	if (request == (void *) -1)
	{
		return (NULL);
	}
	else
	{
		return (ptr);
	}
}
