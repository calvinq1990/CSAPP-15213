/*
 * This file is the interface of cache header file, for 15-213 proxylab.
 *
 * The basic idea for the implement of the data structure of cache is the
 * double linked list (DLL). The double linked list is used as the priority
 * queue, with the least accessed node at the head of the list, and the most
 * recent accessed node at the end of the list.
 *
 * cache.h and cache.c files are used as the basic structure as well as the
 * interface of basic maintainance of the data structure.
 */

/* Header Files */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

/* Recommended max cache and object sizes */

#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* Data Structure of Double Linked List for Cache System */

typedef struct cache_node node;
struct cache_node
{
    int size;
    char* url;
    char* data;
    node *next;
    node *prev;
    
};

typedef struct cache_header cache;
struct cache_header
{
    node *front;
    node *end;
    int empty_size;
    sem_t mutex_r, mutex_w;
    int readcnt
};

/******* Interface of Cache Structure *******/

/* Initialize the cache data structure. */
cache* init_cache();

/* Malloc a new node, and store the corresponding data. */
node* new_node(char* url, char* data, int length);

/* Detele a node from the double linked list. */
void delete_node(cache* list, node* p);

/* Free the space which is malloc to the node. */
void free_node(node* p);

/* Search a node from the linked list by the requested url. */
node* search_node(cache* list, char* url);

/* Insert a node to the front of the linked list. */
void insert_node_front(cache* list, node* p);

/* Insert a node to the end of the linked list. */
void insert_node_end(cache* list, node* p);
