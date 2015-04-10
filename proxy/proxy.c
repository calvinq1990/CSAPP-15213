/*
 * The proxy lab for 15-213.
 * This is a simple proxy with cache which can support most of the websites.
 *
 * Name: Hua Zheng (Hale)            Name: Jiaqi Luo
 * Andrew ID: huazheng               Andrew ID: jiaqiluo
 * Email: halezheng@cmu.edu          Email: jiaqiluo@andrew.cmu.edu
 * Date: Apr. 24th, 2014
 */


/*
 * This proxy runs by multi-thread, and therefore can be fast when used by
 * several client. It has the switch to turn off the cache usage by adding
 * "-off" in the command line when running proxy.
 *
 * Normally, this proxy receives the requests from the browser, and then modify
 * the requests into the required form. Use the url as the key to look up in the
 * cache linked list.
 *
 * If cache hits, then forward the cache data directly to the browser without
 * connecting the server.
 * If cache misses, then send the requests to the server and receive data from
 * the server. Forward the data from server to the browser, and at same time,
 * if the data size is not too big, storage the data into the cache.
 *
 * The basic idea for the implement of the data structure of cache is the
 * double linked list (DLL). The double linked list is used as the priority
 * queue, with the least accessed node at the head of the list, and the most
 * recent accessed node at the end of the list.
 *
 * To avoid the race condition between threads, we use P() and V() functions to
 * block the read or write of the proxy as well as the cache.
 *
 * cache.h and cache.c files are used as the basic structure as well as the
 * interface of basic maintainance of the data structure.
 * cacheuse.h and cacheuse.c files are used as the interface of functions to
 * handle the cache from the proxy.c.
 *
 * This proxy, including this script and the related files, is developed for
 * the Proxy Lab Assignment of 15-213 by Hua Zheng and Jiaqi Luo from Chemical
 * Engineering, Carnegie Mellon University, Apr. 24th, 2014.
 */




/* Header Files */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <float.h>
#include "csapp.h"
#include "cache.h"
#include "cacheuse.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* Helper Functions for the Proxy System */
void doit(void* connfd);
int execute_proxy(int client_fd, int* server_fd);
void parse_port(char* host, char* remote_port);
void parse_host(char* host, char* remote_host);
void url_to_uri(char* url, char* uri);
void forward_from_cache(int client_fd, node* p);
void forward_client(char* buf, int client_fd, int* server_fd, char* url);
void data_union(char* data, int* length, char* buf, int n);


/* You won't lose style points for including these long lines in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *accept_hdr = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
static const char *accept_encoding_hdr = "Accept-Encoding: gzip, deflate\r\n";
static const char *connection_hdr = "Connection: close\r\n";
static const char *proxy_connection_hdr = "Proxy-Connection: close\r\n";

/* Global Variables */

sem_t mutex_w;
cache* cache_list;
int cache_off;

/* Main Function */
int main(int argc, char **argv)
{
    int listenfd, port, clientlen;
    int* connfd;
    struct sockaddr_in clientaddr;
    pthread_t tid;
    Sem_init(&mutex_w, 0, 1);

    Signal(SIGPIPE, SIG_IGN);

    /* Check command line args */
    if (argc != 2 && argc != 3)
    {
        fprintf(stderr, "Usage: %s <port> [cache]\n", argv[0]);
        fprintf(stderr, "  [cache] \"-off\" to Disable Caching\n");
        fprintf(stderr, "  Default or other string will Enable Caching\n");
        exit(1);
    }
    port = atoi(argv[1]);
    if (port <= 1024 || port >= 65535)
    {
        fprintf(stderr, "Unauthorized Port %d\n", port);
        exit(1);
    }

    if (argc == 3 && strcmp(argv[2], "-off") == 0)
        cache_off = 1;
    else
    {
        cache_off = 0;
        cache_list = init_cache();
    }

    if ((listenfd = Open_listenfd(port)) < 0)
    {
        fprintf(stderr, "Cannot listen to Port %d\n", port);
        exit(1);
    }

    while (1)
    {
        clientlen = sizeof(clientaddr);
        connfd = malloc(sizeof(int));
        *connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *)&clientlen);
        Pthread_create(&tid, NULL, (void*) doit, connfd);
    }
    return 0;
}

/* This function handles the main part of sending and receiving data */
void doit(void* connfd)
{
    //printf("enter doit\n");
    Pthread_detach(Pthread_self());
    int server_fd = -1, client_fd = *(int*) connfd;
    free(connfd);

    execute_proxy(client_fd, &server_fd);

    if (server_fd >= 0)
        close(server_fd);
    if (client_fd >= 0)
        close(client_fd);

    return;
}

int execute_proxy(int client_fd, int* server_fd)
{
    //printf("enter execute proxy\n");
    char buf[MAXLINE], method[MAXLINE], url[MAXLINE], version[MAXLINE],
         request_line[MAXLINE], remote_host[MAXLINE], remote_port[MAXLINE],
         uri[MAXLINE], temp_hostname[MAXLINE], url_id[MAXLINE];
    rio_t client_rio;
    node* p;
    memset(remote_port, 0, MAXLINE); // initialize remote_port

    Rio_readinitb(&client_rio, client_fd);
    if (Rio_readlineb(&client_rio, buf, MAXLINE) < 0)
    {
        fprintf(stderr, "Error in Reading from Client\n");
        return 0;
    }
    //printf("read 151: %s\n", buf);

    sscanf(buf, "%s %s %s", method, url, version);
    if (strlen(url) == 0)
        return 0;

    strcpy(version, "HTTP/1.0");
    strcpy(url_id, url);
    if (strncasecmp(method, "GET", 3) != 0)
        return 0;

    strcpy(temp_hostname, url);
    parse_port(url, remote_port);
    parse_host(temp_hostname, remote_host);
    url_to_uri(url, uri);
    
    sprintf(request_line, "%s %s %s\r\nHost: %s\r\n", method, uri, version,
            remote_host);
    strcat(request_line, user_agent_hdr);
    strcat(request_line, accept_hdr);
    strcat(request_line, accept_encoding_hdr);
    strcat(request_line, proxy_connection_hdr);
    strcat(request_line, connection_hdr);

    while(rio_readlineb(&client_rio, buf, MAXLINE) > 0)
    {
        //printf("read 186: %s\n", buf);
        if (strcmp(buf, "\r\n") == 0)
        {
            strcat(request_line, "\r\n");
            break;
        }
        if (strstr(buf, "User-Agent:") != NULL)
            continue;
        if (strstr(buf, "Accept-Encoding:") != NULL)
            continue;
        if (strstr(buf, "Accept:") != NULL)
            continue;
        if (strstr(buf, "Connection:") != NULL)
            continue;
        if (strstr(buf, "Proxy Connection:") != NULL)
            continue;
        if (strncasecmp(buf, "Host:", 5) == 0)
        {
            if (*remote_port == 0)
                parse_port(buf, remote_port);
            if (*remote_port == 0)
                strcpy(remote_port, "80");
            continue;
        }

        strcat(request_line, buf);
        memset(buf, 0, MAXLINE);
    }


    //printf("request_line:\n%s\n", request_line);

    if (cache_off == 0 && (p = find_node(cache_list, url_id)) != NULL)
    {
        //printf("HIT!\n");
        forward_from_cache(client_fd, p);
    }
    else
    {
        //printf("MISS!\n");
        *server_fd = open_clientfd_r(remote_host, atoi(remote_port));
        //printf("open it: %s %s\n", remote_host, remote_port);
        if (*server_fd == -1)
        {
            fprintf(stderr, "Error in Open_clientfd_r\n");
            return 0;
        }
        //P(&mutex_w);
        rio_writen(*server_fd, request_line, strlen(request_line));
        //V(&mutex_w);
        forward_client(buf, client_fd, server_fd, url_id);
    }

    return 1;
}

void parse_port(char* host, char* remote_port)
{
    //printf("enter port parse\n");
    char temp[MAXLINE];
    char* port_tmp, *tmp;
    if (strncasecmp(host, "Host:", 5) == 0)
    {
        sscanf(host + 5, " %s", temp);
        if ((port_tmp = strstr(temp, ":")) != NULL)
            strcpy(remote_port, port_tmp + 1);
    }
    else
    {
        if ((port_tmp = strstr(host, "//")) != NULL)
        {
            strcpy(temp, port_tmp);
            if ((port_tmp = strstr(temp, ":")) != NULL)
            {
                if ((tmp = strstr(port_tmp, "/")) != NULL)
                    *tmp = '\0';
                strcpy(remote_port, port_tmp + 1);
            }
        }
    }

    return;
}

void parse_host(char* host, char* remote_host)
{
    //printf("enter parse url\n");
    char other1[MAXLINE], other2[MAXLINE];
    char* port_tmp;

    if (strstr(host, "://") != NULL)
        sscanf(host, "%[^:]://%[^/]%s", other1, remote_host, other2);
    else
        sscanf(host, "%[^/]%s", remote_host, other2);
    if ((port_tmp = strstr(remote_host, ":")) != NULL)
        *port_tmp = '\0';

    return;
}

void url_to_uri(char* url, char* uri)
{
    //printf("enter url to uri\n");

    char* tmp, *uri_tmp;
    if ((tmp = strstr(url, "://")) != NULL)
    {
        uri_tmp = strstr(tmp + 3, "/");
    }
    else if (strstr(url, "/") != NULL)
        uri_tmp = strstr(url, "/");
    else
    {
        strcpy(uri, "/");
        return;
    }
    strcpy(uri, uri_tmp);

    return;
}

void forward_from_cache(int client_fd, node* p)
{ 
    if (p == NULL)
        return;

    //P(&mutex_w);
    rio_writen(client_fd, p->data, p->size);
    //printf("-----------begin----------\n%s\n------end-----\n",p->data);
    //printf("--------length--------\n%d", p->size);
    //V(&mutex_w);

    return;
} 

void forward_client(char* buf, int client_fd, int* server_fd, char* url)
{
    //printf("enter forward client\n");
    rio_t server_rio;
    size_t n;
    int length = 0;
    char data[MAX_OBJECT_SIZE];
    memset(data, 0, MAX_OBJECT_SIZE);

    memset(buf, 0, MAXLINE);
    Rio_readinitb(&server_rio, *server_fd);
    while ((n = rio_readnb(&server_rio, buf, MAXLINE)) > 0)
    {
        //printf("%s", buf);
        //P(&mutex_w);
        rio_writen(client_fd, buf, n);
        //V(&mutex_w);
        if (cache_off == 0 && length != -1)
            data_union(data, &length, buf, n);
        memset(buf, 0, MAXLINE);
    }

    if (cache_off == 0 && length != -1)
        add_node(cache_list, url, data, length);
    //printf("%s", data);

    return;
}

void data_union(char* data, int* length, char* buf, int n)
{
    if (data == NULL || buf == NULL)
        return;
    if (*length + n > MAX_OBJECT_SIZE)
    {
        *length = -1;
        return;
    }

    char* p = data + *length;
    memcpy(p, buf, n);
    *length += n;

    return;
}