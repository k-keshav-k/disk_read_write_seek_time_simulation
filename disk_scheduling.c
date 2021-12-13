#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

struct request_data{    //structure of requests
    int platter;
    int cylinder;
    int sector;
    int number;
};

struct rwarm{           //structure of rwhead
    int cylinder;
    int sector;
};

int absolute(int a){    //function to calculate absolute value 
    if (a < 0) return (-1)*a;
    return a;
}

int comparator(const void *p, const void *q){   //comparator of qsort to sort according to track number
    struct request_data l = *(struct request_data*)p;
    struct request_data r = *(struct request_data*)q;
    /*if (l.platter != r.platter) {
        return l.platter - r.platter;
    }*/
    return l.cylinder - r.cylinder;
}

int rand_comparator(const void *p, const void *q){  //comparator for random shuffling
    return rand()%2 ? 1 : -1;
}

struct request_data* random_order(struct request_data* arr, int rs){    //this function randomly shuffles the request
    struct request_data* res = malloc(sizeof(struct request_data)*rs);
    for(int i=0;i<rs;i++) res[i] = arr[i];
    qsort((void*)res, rs, sizeof(res[0]), rand_comparator);
    for(int i=0;i<rs;i++){
        printf("(%d %d %d %d)\t", res[i].platter, res[i].cylinder, res[i].sector, res[i].number);
    }
    printf("\n");
    return res;
}

struct request_data* scan(struct request_data* arr, int rs){    
    struct request_data* res = malloc(sizeof(struct request_data)*rs);
    qsort((void*)arr, rs, sizeof(arr[0]), comparator);
    int li = -1;
    int index = 0;
    while(index<rs && arr[index].cylinder<12) index++;
    li = index-1;
    int index2 = 0;
    while(index< rs){
        res[index2] = arr[index];
        index++;
        index2++;
    }
    while(li >= 0){
        res[index2] = arr[li];
        index2++;
        li--;
    }
    for(int i=0;i<rs;i++){
        printf("(%d %d %d %d)\t", res[i].platter, res[i].cylinder, res[i].sector, res[i].number);
    }
    printf("\n");
    return res;
}

struct request_data* cscan(struct request_data* arr, int rs){
    struct request_data* res = malloc(sizeof(struct request_data)*rs);
    qsort((void*)arr, rs, sizeof(arr[0]), comparator);
    int li = -1;
    int index = 0;
    while(index<rs && arr[index].cylinder<12) index++;
    li = index-1;
    int index2 = 0;
    while(index< rs){
        res[index2] = arr[index];
        index++;
        index2++;
    }
    for(int i=0;i<=li;i++){
        res[index2] = arr[i];
        index2++;
    }
    for(int i=0;i<rs;i++){
        printf("(%d %d %d %d)\t", res[i].platter, res[i].cylinder, res[i].sector, res[i].number);
    }
    printf("\n");
    return res;
}

struct request_data* sstf(struct request_data* arr, int rs){
    struct request_data* res = malloc(sizeof(struct request_data)*rs);
    bool ch[rs];
    for (int i=0;i<rs;i++) ch[i] = false;
    int rwhead = 12;
    for(int i=0;i<rs;i++){
        int min = __INT_MAX__;
        int min_index = 0;
        for(int j=0;j<rs;j++){
            if (absolute(arr[j].cylinder-rwhead) < min && ch[j] == false){
                min = absolute(arr[j].cylinder-rwhead);
                min_index = j;
            }
        }
        rwhead = arr[min_index].cylinder;
        res[i] = arr[min_index];
        ch[min_index] = true;
    }
    for(int i=0;i<rs;i++){
        printf("(%d %d %d %d)\t", res[i].platter, res[i].cylinder, res[i].sector, res[i].number);
    }
    printf("\n");
    return res;
}

int minimum(int a, int b){
    return (a < b) ? a : b;
}

float* disk(struct request_data* res, int rs, float seekt, int rpm, int disk_scheduler){
    struct request_data* arr;
    if (disk_scheduler == 1) {
        arr = random_order(res, rs);
        printf("RANDOM is used\n");
    }
    else if (disk_scheduler == 2) {
        arr = res;
        printf("FIFO is used\n");
    }
    else if (disk_scheduler == 3) {
        arr = sstf(res, rs);
        printf("SSTF is used\n");
    }
    else if (disk_scheduler == 4) {
        arr = scan(res, rs);
        printf("SCAN is used\n");
    }
    else if (disk_scheduler == 5) {
        arr = cscan(res, rs);
        printf("C-SCAN is used\n");
    }
    else {printf("Wrong value"); exit(1);}
    float* response_time = malloc(sizeof(float)*rs);
    struct rwarm rwa;
    rwa.cylinder = 12;
    rwa.sector = 0;
    float rd = (60.0/rpm)*(1000/20.0); //rd is time to cross 1 sector
    for(int i=0;i<rs;i++){
        if (i!=0) response_time[i] = response_time[i-1];
        else response_time[i] = 0;
        response_time[i] += absolute(rwa.cylinder - arr[i].cylinder)*seekt;
        response_time[i] += minimum(absolute(rwa.sector - arr[i].sector), absolute(20 - absolute(rwa.sector - arr[i].sector))) * rd;
        response_time[i] += (arr[i].number-1) * rd;
        //response time due to number of sectors
        rwa.cylinder = arr[i].cylinder;
        rwa.sector = (arr[i].sector + arr[i].number - 1)%20;
    }
    return response_time;
}

float min_element(float *arr, int rs){
    float min = arr[0];
    for(int i=0;i<rs;i++){
        if (arr[i] < min) min = arr[i];
    }
    return min;
}

float max_element(float *arr, int rs){
    float max = arr[0];
    for(int i=0;i<rs;i++){
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

float average(float* arr, int rs){
    float a = 0.0;
    for(int i=0;i<rs;i++){
        a += arr[i];
    }
    return a/rs;
}

float stdDev(float* arr, int rs){
    float avg = average(arr, rs);
    float sd = 0.0;
    for(int i=0;i<rs;i++){
        sd += pow(arr[i]-avg, 2.0);
    }
    return sqrt(sd/rs);
}

float throughput(float *arr, int rs){
    float sum = arr[rs-1];
    return rs/sum;
}

int main(){
    int rpm;            //rotational speed of disk
    int seekt;          //seek time
    int sector_size;    //size of each sector
    printf("Rotational speed(in revolutions/min):  ");
    scanf("%d", &rpm);
    printf("Average seek time(in ms):  ");
    scanf("%d", &seekt);
    printf("Sector size:(in bytes):  ");
    scanf("%d", &sector_size);
    int num_requests;
    printf("Number of requests:  ");
    scanf("%d", &num_requests);
    printf("Algorithm number:  ");
    int disk_scheduler;
    scanf("%d", &disk_scheduler);
    struct request_data *request_arr = malloc(sizeof(struct request_data)*num_requests);    //array of requests
    for(int i=0;i<num_requests;i++){    //defining the requests
        request_arr[i].platter = rand()%4 + 1;
        request_arr[i].cylinder = rand()%25 + 1;
        request_arr[i].sector = rand()%20;
        request_arr[i].number = rand()%20 + 1;
        printf("(%d %d %d %d)\t", request_arr[i].platter, request_arr[i].cylinder, request_arr[i].sector, request_arr[i].number);
    }
    printf("\n");
    float* response_time;
    response_time = disk(request_arr, num_requests, 3.0*seekt/25.0, rpm, disk_scheduler);
    for(int i=0;i<num_requests;i++){
        printf("%f ", response_time[i]);
    }
    printf("\n");
    printf("Throughput:\t%f responses/sec\n", throughput(response_time, num_requests)*1000);
    printf("Avg response:\t%f ms\nmin response:\t%f ms\nmax response:\t%f ms\nStdDev time:\t%f ms\n", average(response_time, num_requests), min_element(response_time, num_requests), max_element(response_time, num_requests), stdDev(response_time, num_requests));
}