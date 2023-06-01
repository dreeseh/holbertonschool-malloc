#ifndef _MALLOC_H_
#define _MALLOC_H_

/* libraries */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

/* macros */
#define BLOCK_SIZE 4096
#define ALIGN (2 * sizeof(void *))

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

#endif /* _MALLOC_H */
