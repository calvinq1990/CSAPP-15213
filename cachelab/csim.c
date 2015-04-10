/*Name: Jiaqi Luo
 * ID: jiaqiluo
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "cachelab.h"


 struct line{
	int valid;
	int tag;
	int LRU;
};

int analyze(struct line **cache, unsigned addr, char oper, int E, int s, int b ,int *hits, int *misses, int *evictions, int k, int *HITM, int *MISS, int *HIT, int *EVICTION);
int gettagbits(unsigned addr, int b, int s);
int getsetbits(unsigned addr, int b, int s);

int main(int argc, char **argv){

	int opt, E, b, s, S;
	char *trace;
	int hits = 0;
	int misses = 0;
	int evictions = 0;
	int vflag = 0;
	
	while(-1 != (opt = getopt(argc, argv, "vs:E:b:t:"))){
		switch(opt){
			case 's' :
				s = atoi(optarg);
				break;
			case 'E' :
				E = atoi(optarg);
				break;	
			case 'b' :
				b = atoi(optarg);
				break;
			case 'v' :
				vflag = 1;
				break;
			case 't' :
				trace = optarg;
 				break;
			default: 
				printf("wrong argument\n");		
				exit(-1);
		}
	}
	//printf("s is %d,E is %d,b is %d\n",s,E,b);
	S = (2 << s);

	struct line **cache = (struct line**)malloc(sizeof(struct line *) * S);

	for(int i = 0 ; i < S; i++){
		cache[i] = (struct line*)malloc(sizeof(struct line) * E);
	}
	
	FILE *tracefile;
	tracefile = fopen (trace, "r");
	if (!tracefile){
		fprintf(stderr, "Error: Can not open the file!\n");
		return -1;
	}

	char oper;
	unsigned addr;	
	int size, v; 
	int k = 0;



	
	while (fscanf(tracefile, "%c %x, %d", &oper, &addr, &size) > 0 ){
		int HITM = 0;
		int MISS = 0;
		int HIT = 0;
		int EVICTION = 0;
		if (oper == 'M' || oper == 'L' || oper == 'S'){
			analyze(cache, addr, oper, E, s, b, &hits, &misses, &evictions, k, &HITM, &MISS, &HIT, &EVICTION);
		
			k++;
			if (vflag == 1){
				v = MISS + EVICTION + HIT + HITM;
			//printf("misses:%d, hits:%d, eviction:%d\n\n",misses, hits, evictions);
				printf("%c %x, %d , NO:%d ", oper, addr, size, k);
				switch (v){
					case 2:
						if (HITM == 1 && HIT == 1)
							printf("hit hit\n\n");
						if (HITM == 1 && MISS == 1 )
							printf("miss hit\n\n");
						if (MISS == 1 && EVICTION == 1)
							printf("miss eviction\n\n");
						break;
					case 3:
						printf("miss eviction hit\n\n");
						break;
					case 1:
						if (MISS == 1)	printf("miss\n\n");
						 else printf("hit\n\n");
						break;
				}
			}
		}
	}
	
	for (int i = 0; i < E; i++)
		free(cache[i]);
	free(cache);	
	fclose(tracefile);
	printSummary(hits, misses, evictions);
}

int getsetbits(unsigned addr, int b, int s){
	int mask;
	addr = addr >> b;
	mask = ~((-1) << s);
	//printf("s=%d,b=%d,getsetbits:%d, mask = %x\n",s,b,(mask&addr),mask);
	return (addr & mask);
}
	
int gettagbits(unsigned addr, int b, int s){
	int mask;
	mask = ~((-1) << (64 - s - b));
	addr = addr >> (s+b);
	//printf("gettagbits:%x\n",(addr & mask));
	return (addr & mask);
}

int analyze(struct line **cache, unsigned addr, char oper, int E ,int s, int b, int *hits, int *misses, int *evictions, int k, int *HITM, int *MISS, int *HIT, int *EVICTION){
	int setbit, tagbit, j ;
	
	
	setbit = getsetbits(addr, b, s);
	tagbit = gettagbits(addr, b, s);
	printf("setbit:%x, tagbit: %x\n",setbit,tagbit);
	//printf("E=%d\n",E);

	if ('M' == oper){
	(*hits)++;
	*HITM = 1;
	}
	for (j = 0; j < E; j++){
		if(cache[setbit][j].valid == 1 && cache[setbit][j].tag == tagbit){
			(*hits)++;
			*HIT = 1;
			cache[setbit][j].LRU = k;
			return 0;
			
		}
	}
	(*misses)++;
	*MISS = 1;
	for (j = 0; j < E; j++){
		if (cache[setbit][j].valid == 0){
			cache[setbit][j].valid = 1;
			cache[setbit][j].tag = tagbit;
			cache[setbit][j].LRU = k;
			
			printf("new line: %d\n",j);
			return 0;
		}
	}
	
	(*evictions)++;
	*EVICTION = 1;
	int least = 0;
	
	for (j = 1; j < E; j++){
		if (cache[setbit][j].LRU <  cache[setbit][least].LRU)
			least = j;
	}
	cache[setbit][least].tag = tagbit;
	cache[setbit][least].LRU = k;
	printf("evic line:%d\n",least);
	//printf("%x,%x\n",cache[1][0].valid,cache[1][0].tag);
	
	//printf("k = %d\n",k);
	return 0;
	
}
	

