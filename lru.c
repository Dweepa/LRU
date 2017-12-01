/*
 *Dweepa - 01fb16ecs138
 *Lakshana - 01fb16ecs175
 *Ishita - 01fb16ecs143
 *Kusumanjali - 01fb16ecs173
 *Chandana - 01fb17ecs706
*/

#include <stdio.h>
#include <time.h>
#include<stdlib.h>

//Structure for the lru queue node.
typedef struct lru_queue_node
{
    int page_number;
    struct lru_queue_node* prev,*next;
}LRU;

//Structure for the lru queue.
typedef struct queue
{
    LRU* front,*rear;
    int occupied;
}Q;

//Structure for the hash table.
typedef struct hash
{
    LRU** arr;
}HASH;


int searchRAM(int* arr, int userinput);
void swap (int *a, int *b);
void printCache(Q* lru_queue);

/*CACHE FUNCTIONS*/

//makes a new LRU node.
LRU* makeNode(int page_number)
{
    LRU* new= (LRU*)malloc(sizeof(LRU));
    new->page_number=page_number;
    new->prev=new->next=NULL;
    return new;
}

//Creates and initailises a lru queue.
Q* makeQueue()
{
    Q* lru_queue=(Q*)malloc(sizeof(Q));
    lru_queue->front=lru_queue->rear=NULL;
    lru_queue->occupied=0;
    return lru_queue;
}

//Creates an initialises a hash table.
HASH* makeHash()
{
   HASH* lru_hash=(HASH*)malloc(sizeof(HASH));
    lru_hash->arr=(LRU**)malloc(10*sizeof(LRU));
    int i=0;
    for(i=0;i<10;i++)
    {
        lru_hash->arr[i]=NULL;
    }
    return lru_hash;
}

//Dequeues the least recently used element of the queue i.e. the last element of the queue.
void deQ(Q* lru_queue)
{
    if(lru_queue->occupied==0)
    {
        return;
    }
    else if(lru_queue->front==lru_queue->rear)
    {
        lru_queue->front=NULL;
    }
    LRU* temp=lru_queue->rear;
    lru_queue->rear=lru_queue->rear->prev;
    lru_queue->rear->next=NULL;
    free(temp);
    lru_queue->occupied--;
}

//Enqueues the element to the front of the queue.
void enQ(Q* lru_queue, HASH* lru_hash, int page_number, int capacity)
{
    //printf("occupied spaces: %d | ",lru_queue->occupied);
    LRU* temp= makeNode(page_number);
    int hash_index=page_number%10;

    if(lru_queue->occupied==0)
    {
        lru_queue->front=lru_queue->rear=temp;

    }
    else if(lru_queue->occupied==capacity)
    {
        int hash = (lru_queue->rear->page_number)%10;
        //printf("deleting: %d | ",lru_queue->rear->page_number);
        lru_hash->arr[hash]=NULL;
        deQ(lru_queue);
        lru_queue->front->prev=temp;
        temp->next=lru_queue->front;
        lru_queue->front=temp;
    }
    else
    {
        lru_queue->front->prev=temp;
        temp->next=lru_queue->front;
        lru_queue->front=temp;
    }
    lru_hash->arr[hash_index]=temp;
    lru_queue->occupied++;
}


//Gets the page that is referenced and reorders the cache based on the referenced page.
void getCache(Q* lru_queue, HASH* lru_hash, int page_number, int*RAM, int capacity)
{
    //printf("\n%d is referenced:\n", page_number);
    int hash_index= page_number%10;
    LRU* Page = lru_hash->arr[hash_index];
    
    //If page is not present in cache, put in the cache and queue from the ram.
    if (Page == NULL)
    {
        int ram = searchRAM(RAM,page_number);
        if(ram!=-1)
        {
            //printf("not present in cache... searching in ram :%d | ",ram);
            enQ( lru_queue, lru_hash, ram, capacity);
        }
        
    }
    //If page is present in queue but not in the front, put it in the front.
    else if (Page != lru_queue->front)
    {
        //printf("...page present in lru_queue | ");
        Page->prev->next = Page->next;
        if (Page->next)
        {
            Page->next->prev = Page->prev;
        }
        
        if (Page == lru_queue->rear)
        {
            lru_queue->rear = Page->prev;
            lru_queue->rear->next = NULL;
        }
        
        Page->next = lru_queue->front;
        Page->prev = NULL;
        Page->next->prev = Page;
        lru_queue->front = Page;
    }
    //printCache(lru_queue);
}

//Prints the contents of the cache.
void printCache(Q* lru_queue)
{
    LRU* temp = lru_queue->front;
    printf("lru queue : ");
    while(temp->next!=NULL)
    {
        printf("%d ",temp->page_number);
        temp=temp->next;
    }
    printf("%d ",temp->page_number);
    printf("\n");
}


/*RAM FUNCTIONS*/

//Randomize and swap shuffle the RAM array
void randomize ( int arr[], int n )
{
    int i;
    srand ( time(NULL) );
    for ( i = n-1; i > 0; i--)
    {
        int j = rand() % (i+1);
        
        swap(&arr[i], &arr[j]);
    }
}
void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

//Generates a RAM array of 10000 data points.
void generateRAM(int arr[])
{
    int i=0;
    for(i=0;i<10000;i++)
    {
        arr[i]=i;
    }
    randomize(arr,10000);
}

//Search for the element in the array. Returns the element found, else returns -1.
int searchRAM(int* arr, int userinput)
{
    int i=0;
    for (i=0;i<10000;i++)
    {
        if(userinput== arr[i])
        {
            return userinput;
        }
    }
    return -1;
}

//Prints the RAM array
void print(int arr[], int n)
{
    printf("\n\n");
    int i=0;
    for(i=0;i<n;i++)
    {
        printf("%d   %d\n",i,arr[i]);
    }
    printf("i: %d",i);
}

//A function to mimic page referencing.
void functionTest(Q* lru_queue, HASH* lru_hash, int*RAM, int capacity)
{
    printf("\n");
    
    getCache(lru_queue,lru_hash,234, RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,88,RAM,capacity);
    getCache(lru_queue,lru_hash,95,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,234, RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,999, RAM,capacity);
    getCache(lru_queue,lru_hash,10,RAM,capacity);
    getCache(lru_queue,lru_hash,981,RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,999, RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,88,RAM,capacity);
    getCache(lru_queue,lru_hash,95,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    
    getCache(lru_queue,lru_hash,234, RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,88,RAM,capacity);
    getCache(lru_queue,lru_hash,95,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,234, RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,999, RAM,capacity);
    getCache(lru_queue,lru_hash,10,RAM,capacity);
    getCache(lru_queue,lru_hash,981,RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,999, RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,88,RAM,capacity);
    getCache(lru_queue,lru_hash,95,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    
    getCache(lru_queue,lru_hash,234, RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,88,RAM,capacity);
    getCache(lru_queue,lru_hash,95,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,234, RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,999, RAM,capacity);
    getCache(lru_queue,lru_hash,10,RAM,capacity);
    getCache(lru_queue,lru_hash,981,RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,999, RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,88,RAM,capacity);
    getCache(lru_queue,lru_hash,95,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    
    getCache(lru_queue,lru_hash,234, RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,88,RAM,capacity);
    getCache(lru_queue,lru_hash,95,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,234, RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,999, RAM,capacity);
    getCache(lru_queue,lru_hash,10,RAM,capacity);
    getCache(lru_queue,lru_hash,981,RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,999, RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,88,RAM,capacity);
    getCache(lru_queue,lru_hash,95,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    
    getCache(lru_queue,lru_hash,234, RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,88,RAM,capacity);
    getCache(lru_queue,lru_hash,95,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,234, RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,999, RAM,capacity);
    getCache(lru_queue,lru_hash,10,RAM,capacity);
    getCache(lru_queue,lru_hash,981,RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,999, RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,88,RAM,capacity);
    getCache(lru_queue,lru_hash,95,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    
    getCache(lru_queue,lru_hash,234, RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,88,RAM,capacity);
    getCache(lru_queue,lru_hash,95,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,234, RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,999, RAM,capacity);
    getCache(lru_queue,lru_hash,10,RAM,capacity);
    getCache(lru_queue,lru_hash,981,RAM,capacity);
    getCache(lru_queue,lru_hash,346,RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,999, RAM,capacity);
    getCache(lru_queue,lru_hash,23,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    getCache(lru_queue,lru_hash,88,RAM,capacity);
    getCache(lru_queue,lru_hash,95,RAM,capacity);
    getCache(lru_queue,lru_hash,7,RAM,capacity);
    
}


int main()
{
    int i=0;
    clock_t start, start1,start2, start3, start4, end, end1, end2, end3, end4;
    double cpu_time_used;
    start=clock();
    
    /*PROGRAM*/
    int RAM[10000];
    
    generateRAM(RAM);
    randomize(RAM,10000);
    //print(RAM,10000);
    int pages[]={234,346,23,7,88,95,7,234,346,23,999,10,981,346,23,999,23,7,88,95,7,234,346,23,7,88,95,7,234,346,23,999,10,981,346,23,999,23,7,88,95,7,234,346,23,7,88,95,7,234,346,23,999,10,981,346,23,999,23,7,88,95,7,234,346,23,7,88,95,7,234,346,23,999,10,981,346,23,999,23,7,88,95,7,234,346,23,7,88,95,7,234,346,23,999,10,981,346,23,999,23,7,88,95,7,234,346,23,7,88,95,7,234,346,23,999,10,981,346,23,999,23,7,88,95,7,234,346,23,7,88,95,7,234,346,23,999,10,981,346,23,999,23,7,88,95,7,234,346,23,7,88,95,7,234,346,23,999,10,981,346,23,999,23,7,88,95,7};
    printf("\nFollowing pages are referenced: \n\n");
    for(i=0;i<126;i++)
    {
        printf("%-4d ",pages[i]);
    }
    
    HASH* lru_hash= makeHash();
    Q* lru_queue=makeQueue();
    printf("\n");
    printf("-----------------------------------------------");
    start1=clock();
    printf("\nCACHE REFERENCE FOR CACHE CAPACITY 3");
    lru_hash= makeHash();
    lru_queue=makeQueue();
    functionTest(lru_queue,lru_hash,RAM,3);
    end1=clock();
    
    printf("\nTime taken when cache capacity is 3: ");
    cpu_time_used=((double)(end1-start1))/CLOCKS_PER_SEC;
    printf("\nseconds: %20lfs\n",cpu_time_used);
    printf("-----------------------------------------------");
    
    
    printf("\nCACHE REFERENCE FOR CACHE CAPACITY 5");
    lru_hash= makeHash();
    lru_queue=makeQueue();
    start2=clock();
    functionTest(lru_queue,lru_hash,RAM,5);
    end2=clock();
    
    printf("\nTime taken when cache capacity is 5: ");
    cpu_time_used=((double)(end2-start2))/CLOCKS_PER_SEC;
    printf("\nseconds: %20lfs\n",cpu_time_used);
    
    
    printf("-----------------------------------------------");
    printf("\nCACHE REFERENCE FOR CACHE CAPACITY 10");
    lru_hash= makeHash();
    lru_queue=makeQueue();
    start3=clock();
    functionTest(lru_queue,lru_hash,RAM,10);
    end3=clock();
    
    printf("\nTime taken when cache capacity is 10: ");
    cpu_time_used=((double)(end3-start3))/CLOCKS_PER_SEC;
    printf("\nseconds: %20lfs\n",cpu_time_used);
    printf("-----------------------------------------------");
    
    printf("\nSEARCHING PAGES LINEARLY IN RAM");
    start4=clock();
    i=0;
    for(i=0;i<126;i++)
    {
        searchRAM(RAM,pages[i]);
    }
    end4=clock();
    
    printf("\n\nTime taken for linear searching in RAM: ");
    cpu_time_used=((double)(end4-start4))/CLOCKS_PER_SEC;
    printf("\nseconds: %20lfs\n",cpu_time_used);
    printf("-----------------------------------------------");
    
    end=clock();
    printf("\n\nTOTAL TIME TAKEN \n");
    cpu_time_used=((double)(end-start))/CLOCKS_PER_SEC;
    printf("\nseconds: %20lfs\n",cpu_time_used);
    printf("-----------------------------------------------\n");

    printCache(lru_queue);
    printf("\n");
}



