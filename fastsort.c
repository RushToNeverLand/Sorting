#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sort.h"
#define inf 0xFFFFFFFF

typedef long long ll;

void usage(char *prog){
    fprintf(stderr, "usage: %s <-i file>\n", prog);
    exit(1);
}

//Merge Sort to achieve a stable sort algorithm
//Merge stage
void Merge(rec_t *num,ll p,ll q,ll r)
{
    ll i,j,n1=q-p+1,n2=r-q;
    rec_t *lef=(rec_t *)malloc(sizeof(rec_t)*(n1+1));
    rec_t *rig=(rec_t *)malloc(sizeof(rec_t)*(n2+1));
    for(i=0;i<n1;i++)
        lef[i]=num[p+i];
    lef[n1].key=inf;
    for(i=0;i<n2;i++)
        rig[i]=num[q+i+1];
    rig[n2].key=inf;
    i=j=0;
    for(ll k=p;k<=r;k++)
    {
        if(lef[i].key<=rig[j].key)
            num[k]=lef[i++];
        else
            num[k]=rig[j++];
    }
    free(lef);
    free(rig);
    return ;
}
//Sort stage
void Merge_Sort(rec_t *num,ll p,ll r){
    if(p<r){
        int q=(p+r)/2;
        Merge_Sort(num,p,q);
        Merge_Sort(num,q+1,r);
        Merge(num,p,q,r);
    }
    return ;
}

int main(int argc, char *argv[])
{
    // set the arguments
    char *inFile=strdup(argv[1]);
    char *outFile=strdup(argv[2]);

    //variables
    int c,i,j;

    // open input file and create output file
    int fd = open(inFile, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "open %s error!\n",inFile);
        exit(1);
    }

    int fd2=open(outFile,O_RDWR|O_CREAT,0777);
    if(fd2<0){
        fprintf(stderr, "open %s error!\n",outFile);
        exit(1);
    }

    //malloc for huge file
    //get the information of inputfile
    struct stat buf;
    fstat (fd, &buf);
    rec_t r;
    int cnt=0;
    //caculate the size 
    ll _count=(ll)(buf.st_size+sizeof(rec_t)-1)/sizeof(rec_t);
    rec_t *vec;
    vec=(rec_t*)malloc(sizeof(rec_t)*(_count+1));
    if(vec==NULL){
        fprintf(stderr, "malloc error!\n");
        exit(1);
    }
    while (1) {
        int rc;
        rc = read(fd, &r, sizeof(rec_t));
        if (rc == 0){ // 0 indicates EOF
            break;
        }
        if (rc < 0) {
            fprintf(stderr, "read error!\n");
            exit(1);
        }
        vec[cnt++]=r;
    }

    //sort the data by index
    Merge_Sort(vec,0,cnt-1);

    //write the answer to the file
    for(i=0;i<cnt;i++){
        write(fd2,&vec[i],sizeof(rec_t));
    }

    //free the memory
    free(vec);
    //close the input file and output file
    (void) close(fd);
    (void) close(fd2);

    return 0;
}