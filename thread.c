#include<math.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define BUFFER 410000

int work_pieces = 0;
int global_part = 0;
int thread_nums = 0;
double global_sum = 0.0;
double global_avarage = 0.0;

double *sum;
double *sigma;
double *values;

void _assign(FILE *file, double *values) {
	int index = 0;
	for(index = 0; fscanf(file, "%lf", &*(values + index)) != EOF; index++) {}
	fclose(file);
}

void *_findAvg(void *args) {
	int part = global_part++;
	int index = 0;
	if(part == thread_nums - 1 && BUFFER != work_pieces) {
		for(index = part * (work_pieces / thread_nums); index < BUFFER; index += 1) {
			*(sum + part) += *(values + index);
		}
	} else {
		for(index = part * (work_pieces / thread_nums); index < (part + 1) * (work_pieces / thread_nums); index += 1) {
			*(sum + part) += *(values + index);
		}
	}
}

void *_findSigma(void *args) {
	int part = global_part++;
	int index = 0;
	if(part == thread_nums - 1 && BUFFER != work_pieces) {
		for(index = part * (work_pieces / thread_nums); index < BUFFER; index += 1) {
			*(sigma + part) += pow((*(values + index) - global_avarage), 2);
		}	
	} else {
		for(index = part * (work_pieces / thread_nums); index < (part + 1) * (work_pieces / thread_nums); index += 1) {
			*(sigma + part) += pow((*(values + index) - global_avarage), 2);
		}
	}
}

int main(int argc, char *argv[]) {
	if(argc == 3) {
		printf("\nMulti-threaded program launched");
		FILE *file = fopen(argv[1], "r");

		if(file != NULL) {
			values = (double *)malloc(BUFFER * sizeof(double));

			printf("\nReading %s", argv[1]);
			_assign(file, values);
			
			thread_nums = atoi(argv[2]);				
			pthread_t *threads = (pthread_t *)malloc(thread_nums * sizeof(pthread_t));
			printf("\n%s threads are created.", argv[2]);

			sum = (double *)calloc(thread_nums, sizeof(double));
			sigma = (double *)calloc(thread_nums, sizeof(double));

			work_pieces = BUFFER;
			while(work_pieces % atoi(argv[2]) != 0) {
				work_pieces -= 1;
			}

			printf("\nThe timer started.");
            clock_t start = clock();

			int index = 0;
			for(index = 0; index < thread_nums; index++) {
				pthread_create(&*(threads + index), NULL, _findAvg, NULL);
			}

			for(index = 0; index < thread_nums; index++) {
				pthread_join(*(threads + index), NULL);
			}

			for(index = 0; index < thread_nums; index++) {
				global_sum += *(sum + index);
			}
			global_avarage = global_sum / BUFFER;
			printf("\nThe global average is %.5lf", global_avarage);
			global_part = 0;

			for(index = 0; index < thread_nums; index++) {
				pthread_create(&*(threads + index), NULL, _findSigma, NULL);
			}

			for(index = 0; index < thread_nums; index++) {
				pthread_join(*(threads + index), NULL);
			}

			double sum = 0.0;
			for(index = 0; index < thread_nums; index++) {
				sum += *(sigma + index);
			}
			printf("\nThe standard deviation is %.5lf", sqrt(sum / BUFFER));
			clock_t difference = clock() - start;
            printf("\nCalculation completed in %ld milliseconds.\n", (difference * 1000 / CLOCKS_PER_SEC % 1000));
			free(values);
		} else {
			perror("\nError on source file ");
			exit(EXIT_FAILURE);
		}
	} else {
		printf("\nYou have to enter file name!");
		exit(EXIT_FAILURE);
	}
	return 0;
}
