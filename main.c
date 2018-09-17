#define HAVE_STRUCT_TIMESPEC

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<Windows.h>

int philosopherNum = 0;

pthread_t philosopher[100];
pthread_mutex_t chopstick[100];
int eatingTime = 30;

void *func(int *n) {

	int randomNum;
	int num;
	
	for (int i = 0; i < eatingTime; i++) {
		
		randomNum = (rand() % 3) + 3; // 3~5사이의 난수를 생성
		num = (int)n;

		pthread_mutex_lock(&chopstick[num]);
		pthread_mutex_lock(&chopstick[(num + 1) % 3]);
		printf("P%d (%d, %d) \n", num+1, i+1, eatingTime);  // Philosopher is eating

		pthread_mutex_unlock(&chopstick[num]);
		pthread_mutex_unlock(&chopstick[(num + 1) % 3]);
		Sleep(randomNum); //3-5ms thinking
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	int i, k;
	int join_status;

	if (argc > 1) {
		// Enter # of Philosophers
		philosopherNum = atoi(argv[1]);

		for (i = 0; i < philosopherNum; i++) {
			k = pthread_mutex_init(&chopstick[i], NULL);   // thread 초기화
			if (k == -1) {
				printf("\n Mutex initialization failed");
				exit(1);
			}
		}

		for (i = 0; i < philosopherNum; i++) {
			k = pthread_create(&philosopher[i], NULL, (void*)func, (int*)i);  // thread 생성
			if (k != 0) {
				printf("\n Thread creation error \n");
				exit(1);
			}
		}

		for (i = 0; i < philosopherNum; i++) {
			k = pthread_join(philosopher[i], (void **)&join_status); // 종료대기
			if (k != 0) {
				printf("\n Thread join failed \n");
				exit(1);
			}
		}
		for (i = 0; i < philosopherNum; i++) {
			k = pthread_mutex_destroy(&chopstick[i]);
			if (k != 0) {
				printf(" \n Mutex Destroyed \n");
				exit(1);
			}
		}
	}
	else {
		printf("You entered incorrectly");
	}

	system("pause");
	return 0;
}