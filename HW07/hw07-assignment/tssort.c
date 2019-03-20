//Written by Andrew Ricci

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "float_vec.h"
#include <sys/time.h>
#include <float.h>
#include <sys/types.h>
#include <sys/wait.h>

void
seed_rng()
{
    struct timeval tv;
    gettimeofday(&tv, 0);

    long pid = getpid();
    long sec = tv.tv_sec;
    long usc = tv.tv_usec;

    srandom(pid ^ sec ^ usc);
}

int cmp(const void *i, const void *j){
    float *first = (float *)i;
    float *second = (float *)j;
    return *first < *second ? -1 : *second < *first;
}

float* floats_sort(float* data, long len){
    qsort(data, len, sizeof(float), cmp);
    return data;
}

int main(int argc, char* argv[]){

   //printf("Number of processes is: %s\n", argv[1]);	
   //printf("File to open is: %s\n", argv[2]);
   

    int p = atoi(argv[1]);
    int samples = 3*(p-1);
    floats* sample_vec = make_floats(0);
    seed_rng();

    long count = 0;

    char* filename = argv[2];
    FILE *data = fopen(filename, "r");
    fread(&count, sizeof(float), 1, data);
    floats* all = make_floats(0);

    for (long i = 0; i < count; i++){
	float num = 0.0f;
	fread(&num, sizeof(float), 1, data);
	floats_push(all, num);
    }

    for (int i = 0; i < samples; i++){
	int num = (rand() % (count));
	floats_push(sample_vec, all->data[num]);
    }

    fclose(data);

    qsort(sample_vec->data, samples, sizeof(float), cmp);
    floats* medians = make_floats(0);
    floats_push(medians, 0);
    for (int i = 1; i < samples; i = i + 3){
        floats_push(medians, sample_vec->data[i]);
    }

    floats_push(medians, FLT_MAX);

    long index = 0;
    floats* final = make_floats(count);

    for(int i = 0; i < p; i++){
	floats* part = make_floats(0);
	for(int j = 0; j < count; j++){
	    if(all->data[j] >= medians->data[i] && all->data[j] < medians->data[i+1]){
	        floats_push(part, all->data[j]);
	    }
        }
        printf("%d: start %.*f, count %ld\n", i, 4, medians->data[i], part->size);

	float* sorted = floats_sort(part->data, part->size);
        for(long i = 0; i < part->size; i++){
	    final->data[index + i] = sorted[i];
        }	    

        index += part->size;
	free_floats(part);
    }

    FILE* new = fopen(filename, "w");
    fwrite(&count, sizeof(long), 1, new);
    for(long i = 0; i < count; i++){
	fwrite(&(final->data[i]), sizeof(float), 1, new);
    }
    fclose(new);

    free_floats(final);
    free_floats(sample_vec);
    free_floats(all);
    free_floats(medians);
    return 0;
}
