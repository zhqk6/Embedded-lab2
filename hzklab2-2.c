/*
 * hzklab2-2.c
 *
 *  Created on: Feb 7, 2016
 *      Author: zhqk6
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int a[2][4];
int b[4][5];
int mul1[2][5],mul2[2][5],mul3[2][5];
pthread_t thread1[2][5],thread2[2];
pthread_t thread3;
int pass1[2][5][2];
int pass2[2];
int pass3=0;

void Multiply1(void* ptr1);
void Multiply2(void* ptr2);
void Multiply3(void* ptr3);

int main(){
	int i,j;
	int row1=2, col1=4, row2=4,col2=5;
	struct timeval start1,start2,start3;
	struct timeval end1,end2,end3;
    long timer1,timer2,timer3;      /*define timers to calculate the time it takes*/
	FILE *fp;
	fp = fopen("multiplication.txt","r");
    if(fp==NULL){
    	return(-1);
    }
      fscanf(fp,"%d %d",&row1,&col1);
       for(i=0;i<2;i++){
    	   for(j=0;j<4;j++){
    		   fscanf(fp,"%d",&a[i][j]);
    	   }
       }                     /*read matrix a from multiplication.txt*/
       fscanf(fp,"%d %d",&row2,&col2);
       for(i=0;i<4;i++){
    	   for(j=0;j<5;j++){
    		   fscanf(fp,"%d",&b[i][j]);
    	   }
       }                      /*read matrix b from multiplication.txt*/

       /*case 1*/

       gettimeofday(&start1,NULL);
       for(i=0;i<2;i++){
    	   for(j=0;j<5;j++){
    		   pass1[i][j][0]=i;
    		   pass1[i][j][1]=j;
    		   pthread_create(&thread1[i][j],NULL,(void *)Multiply1,(void *)&pass1[i][j]);
    	   }
       }
       for(i=0;i<2;i++){
           	   for(j=0;j<5;j++){
               pthread_join(thread1[i][j],NULL);
           	   }
       }
       gettimeofday(&end1,NULL);
       printf("\n");
       printf("resulting matrix from case 1, using 10 threads: \n");
       printf("\n");
       for(i=0;i<2;i++){
           	   for(j=0;j<5;j++){
           		  printf("%d ",mul1[i][j]);
           	   }
           	  printf("\n");
       }
       timer1 = 1000000*(end1.tv_sec-start1.tv_sec)+end1.tv_usec-start1.tv_usec;
       printf("time we use in case 1 = %ld us\n",timer1);

       /*case 2*/

       gettimeofday(&start2,NULL);
        for(i=0;i<2;i++){
             pass2[i]=i;
             pthread_create(&thread2[i],NULL,(void *)Multiply2,(void *)&pass2[i]);
         }
         for(i=0;i<2;i++){
             pthread_join(thread2[i],NULL);
         }
         gettimeofday(&end2,NULL);
         printf("\n");
         printf("resulting matrix from case 2 using 2 threads: \n");
         printf("\n");
         for(i=0;i<2;i++){
                  for(j=0;j<5;j++){
                       printf("%d ",mul2[i][j]);
                    }
                   printf("\n");
         }
         timer2 = 1000000*(end2.tv_sec-start2.tv_sec)+end2.tv_usec-start2.tv_usec;
         printf("time we use in case 2 = %ld us\n",timer2);

         /*case 3*/

        gettimeofday(&start3,NULL);
        pthread_create(&thread3,NULL,(void *)Multiply3,(void *)&pass3);
        pthread_join(thread3,NULL);
        gettimeofday(&end3,NULL);
        printf("\n");
        printf("resulting matrix from case 3 using 1 threads: \n");
        printf("\n");
        for(i=0;i<2;i++){
              for(j=0;j<5;j++){
                 printf("%d ",mul3[i][j]);
                  }
                 printf("\n");
             }
       timer3 = 1000000*(end3.tv_sec-start3.tv_sec)+end3.tv_usec-start3.tv_usec;
       printf("time we use in case 3 = %ld us\n",timer3);

         return 0;
}

void Multiply1(void* ptr1){        /* function 1  */
	int *var;
	var = (int *)ptr1;
	int row = var[0];
	int col = var[1];
	int rs = 0;
	int i;
	for(i=0;i<4;i++){
		rs+=a[row][i]*b[i][col];
		mul1[row][col]=rs;
	}
	 pthread_exit(NULL);
}

void Multiply2(void* ptr2){      /* function 2 */
	int row=*(int *)ptr2;
    int i,j;
    int rs=0;
    for(j=0;j<5;j++){
        for(i=0;i<4;i++){
        rs=a[row][i]*b[i][j];
        mul2[row][j]+=rs;
        }
  }
    pthread_exit(NULL);
}

void Multiply3(void* ptr3){             /* function 3 */
        int i,j,k;
        int id = *(int *)ptr3;
        int rs = 0;
        for(i=id;i<2;i++){
            for(k=0;k<5;k++){
                for(j=0;j<4;j++){
                    rs=a[i][j]*b[j][k];
                    mul3[i][k]+=rs;
                }
            }
        }
        pthread_exit(NULL);
    }
