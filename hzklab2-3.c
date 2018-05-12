/*
 * hzklab2-3.c
 *
 *  Created on: Feb 9, 2016
 *      Author: zhqk6
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <rtai.h>
#include <rtai_lxrt.h>
#include <string.h>

void T1(void *ptr1);
void T2(void *ptr2);
void T3(void *ptr3);

FILE *fp1;
FILE *fp2;                               // define files 1 and 2
char buffer[100];                        //common buffer
char stringarray[20][sizeof(buffer)];    //string array in which we will store our common buffer
RTIME BaseP;
int i;


int main(){
	fp1 =fopen("first.txt","r");
	if(fp1==NULL){
		return-1;
	}
	fp2 =fopen("second.txt","r");
	if(fp2==NULL){
		return-1;
	}                                    // open file first.txt and second.txt
	BaseP = start_rt_timer(nano2count(1000000));
	pthread_t thread1,thread2,thread3;
	pthread_create(&thread1,NULL,(void *)&T1,NULL);
	pthread_create(&thread2,NULL,(void *)&T2,NULL);
	pthread_create(&thread3,NULL,(void *)&T3,NULL);
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread3,NULL);           //create 3 threads to implement

    printf("\n");
    printf("this is read from string array in which we store common buffer:");
    printf("\n");
    for(i=0;i<16;i++){
    printf("%s",stringarray[i]);
    }                                     //printf the string array to make sure if it is
                                          //exactly the same as common buffers

    fclose(fp1);
    fclose(fp2);
    return 0;
}

//function 1, read lines in first.txt
void T1(void *ptr1){
	RT_TASK* rttask1 = rt_task_init(nam2num("thrd1"),0,512,256);
    rt_task_make_periodic(rttask1,rt_get_time()+10*BaseP,20*BaseP);   // period is 20, start at 10
	while(1){
	fgets(buffer,sizeof(buffer),fp1);                                 //read 1 line
	rt_task_wait_period();
	if(feof(fp1))
	break;                                                            //if end of file, break the loop
	}
}
//function 2, read lines in second.txt
void T2(void *ptr2){
	RT_TASK* rttask2 = rt_task_init(nam2num("thrd2"),0,512,256);
	rt_task_make_periodic(rttask2,rt_get_time()+20*BaseP,20*BaseP);   // period is 20, start at 20
	while(1){
	fgets(buffer,sizeof(buffer),fp2);                                 //read 1 line
	rt_task_wait_period();
	if(feof(fp2))
	break;                                                            //if end of file, break the loop
	}
}
//function 3, store common buffers in a string array
void T3(void *ptr3){
	RT_TASK* rttask3 = rt_task_init(nam2num("thrd3"),0,512,256);
	rt_task_make_periodic(rttask3,rt_get_time()+15*BaseP,10*BaseP);   // period is 10, start at 15
	for(i=0;i<16;i++){                                                //we have 16 times in total
    printf("%s",buffer);                                              //printf common buffers we get from files
    strcpy((char*)stringarray[i],buffer);                             //store the common buffers                                                                //in string array
	rt_task_wait_period();                                            //in string array
	}
}

