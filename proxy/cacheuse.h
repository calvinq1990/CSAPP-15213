/*
 * This file is the interface of cacheuse header file, for 15-213 proxylab.
 *
 * The basic idea for the implement of the data structure of cache is the
 * double linked list (DLL). The double linked list is used as the priority
 * queue, with the least accessed node at the head of the list, and the most
 * recent accessed node at the end of the list.
 *
 * cacheuse.h and cacheuse.c files are used as the interface of functions to
 * handle the cache from the proxy.c.
 */

/* Header Files */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

/******* Interface of Cache Usage Structure *******/

/* Find a node in the linked list based on the url. */
node* find_node(cache* list, char* url);

/* Add a node to the linked list with the current data from the server. */
void add_node(cache* list, char* url, char* data, int length);

/* If the space is not enough, do eviction.
 * Delete the least accessed node to give out more space. */
void evict_node(cache* list, node* p);