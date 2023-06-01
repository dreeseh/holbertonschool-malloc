#include "malloc.h"

static block_t *first_free_blk;

/**
 * free_list_remove - removes a block from the free list
 *
 * @blk: block to remove from free list
 *
 * Return: is void
 */
void free_list_remove(block_t *blk)
{
	if (blk == NULL)
	{
		fprintf(stderr, "free_list_remove: blk is NULL\n");
		return;
	}

	if (blk == first_free_blk)
		first_free_blk = blk->next;

	if (blk->prev)
		blk->prev->next = blk->next;

	if (blk->next)
		blk->next->prev = blk->prev;
}


/**
 * free_list_add - adds a block to the free list, deallocating it from heap
 * memory available to the user
 *
 * @blk: block to add to free list
 *
 * Return: is void
 */
void free_list_add(block_t *blk)
{
	if (!blk)
	{
		fprintf(stderr, "freeListAdd: blk is NULL\n");
		return;
	}

	/* LIFO free list, always add to head */
	if (first_free_blk != NULL)
		first_free_blk->prev = blk;
	blk->next = first_free_blk;
	blk->prev = NULL;
	first_free_blk = blk;
}

/**
 * free_block_split - splits free block into allocated block and remainder,
 * remainder becomes new free block
 *
 * @free_blk: unused block from which to carve out allocation
 * @size: block size in bytes, including header, to allocate from free block
 *
 * Return: pointer to first byte in a contiguous region of `size`
 * bytes in the heap; aligned for any kind of variable
 */
block_t *free_block_split(block_t *free_blk, size_t size)
{
	block_t *new_free_blk;

	if (!free_blk)
	{
		fprintf(stderr, "splitFreeBlock: free_blk is NULL\n");
		return (NULL);
	}

	if (size < sizeof(block_t) || size % ALIGN)
	{
		fprintf(stderr, "splitFreeBlock: size not aligned\n");
		return (NULL);
	}

	new_free_blk = (block_t *)((uint8_t *)free_blk + size);

	new_free_blk->next = free_blk->next;
	new_free_blk->prev = free_blk->prev;
	new_free_blk->size = free_blk->size - size;
	free_blk->size = size;

	return (new_free_blk);
}


/**
 * free_blocks - traversal of free list, merging any block with
 * any other free block that is next in the list and also in an contiguous
 * address space of heap
 *
 * Return: is void
 */
void free_blocks(void)
{
	block_t *curr;
	void *pgm_brk;
	long page_sz;

	pgm_brk = sbrk(0);
	if (pgm_brk == (void *)-1)
	{
		perror("free_blocks: sbrk");
		return;
	}
	page_sz = sysconf(_SC_PAGESIZE);
	if (page_sz == -1)
	{
		perror("free_blocks: sysconf");
		return;
	}

	/* if free block is contiguous to the next in the list, merge them */
	for (curr = first_free_blk; curr; curr = curr->next)
	{
		if ((uint8_t *)curr + curr->size == (uint8_t *)curr->next)
		{
			curr->size += curr->next->size;
			curr->next = curr->next->next;
			if (curr->next)
				curr->next->prev = curr;
		}
	}
	/* check free list for large unused block at the end of the heap */
	for (curr = first_free_blk; curr; curr = curr->next)
	{
		if ((uint8_t *)curr + curr->size == pgm_brk &&
		    curr->size >= (size_t)page_sz)
		{
			free_list_remove(curr);
			if (brk(curr) != 0)
				perror("free_blocks: brk");
			break;
		}
	}
}
