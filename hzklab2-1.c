/*
 * hzklab2-1.c
 *
 *  Created on: Feb 7, 2016
 *      Author: zhqk6
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int a[20][20];     /*the first matrix a */
int b[20][20];     /*the second matrix b */
int a_a[400];
int b_b[400];
int add1[20][20];
int add2[20][20];/*3 resulting matrices in 3 functions*/
int add_3[400];
int add3[20][20];

int row;
int col;

void addition1(void *ptr1);    /*used by 1-thread case*/
void addition2(void *ptr2);    /*used by 20-thread case*/
void addition3(void* ptr3);    /*used by 400-thread case*/


int main(){
	int i,j;
	struct timeval start1,start2,start3;
	struct timeval end1,end2,end3;
	int timer1,timer2,timer3;      /*define timers to calculate the time it takes*/
	FILE *fp;
	fp = fopen("addition.txt","r");
	if(fp==NULL){
		return(-1);
	}
	fscanf(fp,"%d %d",&row,&col);    /*skip the first 2 number 20 20 of matrix a*/
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			fscanf(fp,"%d",&a[i][j]);    /*read matrix a from addition.txt*/
		}
	}
	fscanf(fp,"%d %d",&row,&col);    /*skip the first 2 number 20 20 of matrix b*/
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			fscanf(fp,"%d",&b[i][j]);    /*read matrix b from addition.txt*/
		}
	}

	for(i=0;i<20;i++){
		for(j=0;j<20;j++){
			a_a[i*20+j]=a[i][j];
		}
	}
	for(i=0;i<20;i++){
		for(j=0;j<20;j++){
			b_b[i*20+j]=b[i][j];
		}
	}                                        /* create 2 array(1 dimension)*/

	pthread_t thread1,thread2[20],thread3[400];   /*named 3 cases of threads*/
	int pass1=0, pass2[20];
	int pass3[400];               /*int arguments for each threads*/

	/*         case1*/

	gettimeofday(&start1,NULL);                   /*timer1 for case 1 begins */
	pthread_create(&thread1,NULL,(void *)&addition1,(void *)&pass1);
	pthread_join(thread1,NULL);        /* case 1: using 1 thread for 400 times addition */
	gettimeofday(&end1,NULL);          /*timer1 for case 1 ends */
	printf("resulting matrix from case 1, using 1 threads: \n");
	for(i=0;i<20;i++){
		for(j=0;j<20;j++){
			printf("%d ",add1[i][j]);
		}
		printf("\n");                  /* printf the result in case 1 */
	}
	timer1 = 1000000*(end1.tv_sec-start1.tv_sec)+end1.tv_usec-start1.tv_usec;
	printf("\n");
	printf("time we use in case 1 = %d us\n",timer1);     /*times used in case 1 */
	printf("\n");

	/*        case2*/

	gettimeofday(&start2,NULL);          /*timer2 for case 2 begins */
	for(j=0;j<20;j++){
		pass2[j]=j;
		pthread_create(&thread2[j],NULL,(void *)&addition2,(void *)&pass2[j]);
	}
	for(j=0;j<20;j++){
		pthread_join(thread2[j],NULL);
	}
	/* case 2: using 20 threads, each threads is used in one row(20 rows in total)*/
	gettimeofday(&end2,NULL);    /*timer2 for case 2 ends */
	printf("\n");
	printf("resulting matrix from case 2, using 20 threads: \n");
	printf("\n");
	for(i=0;i<20;i++){
		for(j=0;j<20;j++){
			printf("%d ",add2[i][j]);
		}
		printf("\n");                  /* printf the result in case 2 */
	}
	timer2 = 1000000*(end2.tv_sec-start2.tv_sec)+end2.tv_usec-start2.tv_usec;
	printf("time we use in case 2 = %d us\n",timer2);    /*times used in case 2 */
	/*         case3*/
	gettimeofday(&start3,NULL);          /*timer3 for case 3 begins */

	/* case 3: using 400 threads, each threads is used in addition between two elements(400 in total)*/

	int k;
	for(k=0;k<400;k++){
		pass3[k]=k;
		pthread_create(&thread3[k],NULL,(void *)&addition3,(void*)&pass3[k]);
	}

	int l;
	for(l=0;l<400;l++){
		pthread_join(thread3[l],NULL);

	}

	gettimeofday(&end3,NULL);            /*timer3 for case 3 ends */

	printf("\n");
	printf("resulting matrix from case 3, using 400 threads:\n");
	printf("\n");

	for(i=0;i<20;i++){
		for(j=0;j<20;j++){
			printf("%d ",add_3[i*20+j]);
		}
		printf("\n");                   /* printf the result in case 3 */
	}
	timer3 = 1000000*(end3.tv_sec-start3.tv_sec)+end3.tv_usec-start3.tv_usec;
	printf("\n");
	printf("time we use in case 3 = %d us\n",timer3);     /*times used in case 3 */
	fclose(fp);
	return 0;
	}

	void addition1(void *ptr1){      /* function 1 */
		int var1 = *(int*)ptr1;
		int i,j;
		for(i=var1;i<20;i++){        /*this for loop will be executed 1 time in thread1*/
			for(j=0;j<20;j++){
				add1[i][j]=a[i][j]+b[i][j];
			}
		}
		pthread_exit(NULL);
	}

	void addition2(void *ptr2){      /* function 2 */
		int var2 = *(int*)ptr2;
		int i,j;
		for(i=var2;i<(var2+1);i++){      /*this for loop will be executed 20 times in thread2 */
			for(j=0;j<20;j++){
				add2[i][j]=a[i][j]+b[i][j];
			}
		}
		pthread_exit(NULL);
	}

	void addition3(void *ptr3){      /* function 3 */
		int *var3 = (int*)ptr3;
		add_3[*var3] = a_a[*var3]+b_b[*var3];

	}
