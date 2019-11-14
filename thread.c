#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define BUFFER 410000

int cilgin_part = 0;
int number_of_threads = 0;

double *values;
double bigMean = 0.0;
double *sum;
double *mean;
double *standardDeviation;

void _assign(FILE *file, double *values) {
	int index = 0;
	for(index = 0; fscanf(file, "%lf", &*(values + index)) != EOF; index++) {}
}

void *_findAvg(void *args) {
	int part = cilgin_part++;
	int index = 0;
	for(index = part * (BUFFER / number_of_threads); index < (part + 1) * (BUFFER / number_of_threads); index += 1) {
		sum[part] += *(values + index);
	}
	mean[part] = sum[part] / (BUFFER / number_of_threads);
}

void *_findStandardDeviation(void *args) {
	int part = cilgin_part++;
	int index = 0;
	for(index = part * (BUFFER / number_of_threads); index < (part + 1) * (BUFFER / number_of_threads); index += 1) {
		standardDeviation[part] += pow((*(values + index) - bigMean), 2);
	}
}
int main(int argc, char *argv[]) {
	if(argc == 3) {
		FILE *file = fopen(argv[1], "r");

		int work_piece = BUFFER;
		while(work_piece % atoi(argv[2]) != 0){
			work_piece -= 1;
		}

		if(file != NULL) {
			values = (double *)malloc(BUFFER * sizeof(double));

			_assign(file, values);

			int numberOfThreads = atoi(argv[2]);
			number_of_threads = numberOfThreads;
			pthread_t *threads = (pthread_t*)malloc(numberOfThreads*sizeof(pthread_t));
			
			sum = (double*)calloc(numberOfThreads, sizeof(double));
			mean = (double*)calloc(numberOfThreads, sizeof(double));
			standardDeviation = (double*)calloc(numberOfThreads, sizeof(double));

			int index = 0;
			for(index = 0; index < numberOfThreads; index++) {
				pthread_create(&*(threads + index), NULL, _findAvg, (void *)NULL);
			}

			for(index = 0; index < numberOfThreads; index++) {
				pthread_join(*(threads + index), NULL);
			}

			for(index = 0; index < numberOfThreads; index++) {
				bigMean += mean[index];
			}
			bigMean /= numberOfThreads;
			cilgin_part = 0;

			for(index = 0; index < numberOfThreads; index++) {
				pthread_create(&*(threads + index), NULL, _findStandardDeviation, (void *)NULL);
			}

			for(index = 0; index < numberOfThreads; index++) {
				pthread_join(*(threads + index), NULL);
			}

			double sum = 0.0;
			for(index = 0; index < numberOfThreads; index++) {
				sum += standardDeviation[index];
			}
			printf("The standard deviation is %.5lf\n", sqrt(sum / BUFFER));

			free(values);
			fclose(file);
		} else {
			perror("Error on source file ");
			exit(EXIT_FAILURE);
		}
	} else {
		printf("You have to enter file name!");
		exit(EXIT_FAILURE);
	}
	return 0;
}
