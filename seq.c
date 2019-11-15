#include<time.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>

#define BUFFER 410000

void _assign(FILE *file, double *values) {
	int index = 0;
	for(index = 0; fscanf(file, "%lf", &*(values + index)) != EOF; index++) {}
	fclose(file);
}

void _standardDeviation(double *values) {
	int index = 0;
	double sum = 0.0;
	double mean = 0.0;
	double standardDeviation = 0.0;

	for(index = 0; index < BUFFER; index += 1) {
		sum += *(values + index);
	}
	mean = sum / BUFFER;
	printf("\nThe avarage is %.5lf", mean);
	
	for(index = 0; index < BUFFER; index += 1) {
		standardDeviation += pow(*(values + index) - mean, 2);
	}
	printf("\nThe standard deviation is %.5lf\n", sqrt(standardDeviation / BUFFER));
}

int main(int argc, char *argv[]) {
	if(argc == 2) {
		printf("\nSequential program launched");
		FILE *file = fopen(argv[1], "r");

		if(file != NULL) {
			double *values = (double *)malloc(BUFFER * sizeof(double));

			printf("\nReading %s", argv[1]);
			_assign(file, values);

			printf("\nThe timer started.");
            clock_t start = clock();
            
			_standardDeviation(values);
			
			clock_t difference = clock() - start;
            printf("Calculation completed in %ld milliseconds.\n\n", (difference * 1000 / CLOCKS_PER_SEC % 1000));

			free(values);
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
