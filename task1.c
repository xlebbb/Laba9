#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int value = 0;

void * incr(void * arg) { 
     int val = *(int*) arg; //количество операций
     for (int i = 0; i < val; i++)
     	value++;	
     printf("Value:%d\n", value);
     sleep(1);
     pthread_exit(NULL);
}

void * decr(void * arg) { 
     int val = *(int*) arg; //количество операций
     for (int i = 0; i < val; i++)
     	value--;	
     printf("Value:%d\n", value);
     sleep(1);
     pthread_exit(NULL);
}

int main(int argc, char **argv){
	pthread_t *pt1, *pt2;
	int th = 6; 			//кол-во потоков по умолчанию
	int val = 6;			//количество операций по умолчанию
	char *opts = "t:v:"; 		//количество потоков, количество операций
	int opt; 
	printf("Enter even number of threads!\n");
	while((opt = getopt(argc, argv, opts)) != -1) {
		switch(opt) {
			case 't': 
				th = atoi(optarg);
				break;
			case 'v': 
				val = atoi(optarg);
				break;
			case '?': default: {
				printf("Unknown command!\n");
				break;
			};		
		}
	}
	printf("Threads: %d , operations: %d .\n", th, val);
	pt1 = malloc (sizeof(pthread_t) * th/2); 	//три потока добавления
	pt2 = malloc (sizeof(pthread_t) * th/2); 	//три потока вычитания


	for (int i = 0; i < th/2; ++i){
		if (pthread_create(&pt1[i], NULL, &incr, &val) != 0) {	
			fprintf(stderr, "Error thread\n");
			return 1;
		}
		if (pthread_create(&pt2[i], NULL, &decr, &val) != 0) {
			fprintf(stderr, "Error thread\n");
			return 1;
		}		
	}
		
	for(int i = 0; i < th/2; ++i){				
		if (pthread_join(pt1[i], NULL) != 0) {	//ожидание завершения потока
			printf("Error.\n");		
		}
		if (pthread_join(pt2[i], NULL) != 0) {
			printf("Error.\n");		
		}
	}
	printf("Final value: %d\n", value);
	free(pt1);
	free(pt2);
	return 0;
}
