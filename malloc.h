#ifndef _MALLOC_H_
#define _MALLOC_H_

/* libraries */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

/* macros */
#define ALIGN (2 * sizeof(void *))

#define HEADER_SZ (sizeof(size_t) * 2)
#define BLK_SZ(size) (HEADER_SZ + size)
#define PAYLOAD_SZ(size) (size - HEADER_SZ)
#define BLK_PAYLOAD(ptr) ((void *)((uint8_t *)ptr + HEADER_SZ))
#define BLK_HEADER(ptr) ((block_t *)((uint8_t *)ptr - HEADER_SZ))

/* structures */
/**
 * struct block_s - heap allocation block header
 *
 * @prev_size: size of previous block in bytes, if free
 * @size: size of this block in bytes, including overhead
 * @next: in free blocks, points to next member of free list
 * @prev: in free blocks, points to previous member of free list
 */
typedef struct block_s
{
	size_t prev_size;
	size_t size;
	/* payload begins after size in allocated blocks */
	struct block_s *next;
	struct block_s *prev;
/*
 *	struct block_s *next_size_class;
 *	struct block_s *prev_size_class;
 */
} block_t;

/* prototypes */
void *naive_malloc(size_t size);

void print_free_list(char *prefix);
block_t *new_free_block(size_t algnd_pyld_sz);
void *_malloc(size_t size);

void free_list_remove(block_t *blk);
void free_list_add(block_t *blk);
block_t *free_block_split(block_t *free_blk, size_t size);
void free_blocks(void);

void _free(void *ptr);

#endif /* _MALLOC_H */
