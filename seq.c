#include<stdio.h>
#include<stdlib.h>

#define BUFFER 410000

void _assign(FILE *file, double *values) {
	int index = 0;
	for(index = 0; fscanf(file, "%lf", &*(values + index)) != EOF; index++) {}
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
	printf("The mean is %.5lf\n", mean);

	for(index = 0; index < BUFFER; index += 1) {
		standardDeviation += pow(*(values + index) - mean, 2);
	}
	printf("The standard deviation is %.5lf\n", sqrt(standardDeviation / BUFFER));
}

int main(int argc, char *argv[]) {
	if(argc == 2) {
		FILE *file = fopen(argv[1], "r");

		if(file != NULL) {
			double *values = (double *)malloc(BUFFER * sizeof(double));

			_assign(file, values);
			_standardDeviation(values);

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