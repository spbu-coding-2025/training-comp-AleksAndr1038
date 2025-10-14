#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_RESULT_STRING_LENGTH 32

#define array_length(x) (sizeof(x) / sizeof(x[0]))
#define ERROR_PRINT(...) (fprintf(stderr, __VA_ARGS__))

struct interval_t {
	double left_border;
	double right_border;
};

void free_results(char **results, unsigned int count);

double rectangle_rule(double left_border, double right_border) {
	return (right_border - left_border) * sin((left_border + right_border) / 2);
}

double simpsons_rule(double left_border, double right_border) {
	return (right_border - left_border) / 6 * (sin(left_border) + 4 * sin((left_border + right_border) / 2) + sin(right_border));
}

double integrate(struct interval_t interval, unsigned int partition_size, double (*method)(double, double)) {
	double integral_value = 0.0;
	double part_length = (interval.right_border - interval.left_border) / partition_size;
	double part_left_border = interval.left_border;
	for (unsigned int i = 0; i < partition_size; ++i) {
		double part_right_border = part_left_border + part_length;
		integral_value += method(part_left_border, part_right_border);
		part_left_border = part_right_border;
	}
	return integral_value;
}

char **calculate_integrals(struct interval_t interval, unsigned int count, const unsigned int *partition_sizes) {
	char **results = (char **)malloc(sizeof(char *) * count);
	if (!results) {
		return NULL;
	}

	for (unsigned int i = 0; i < count; ++i) {
		double integral_rectangle = integrate(interval, partition_sizes[i], rectangle_rule);
		double integral_simpson = integrate(interval, partition_sizes[i], simpsons_rule);

		results[i] = (char *)malloc(sizeof(char) * MAX_RESULT_STRING_LENGTH);
		if (!results[i]) {
			free_results(results, i);
			ERROR_PRINT("Cannot allocate memory for result string in %d experiment\n", i);
			return NULL;
		}

		if (!sprintf(results[i], "%d %.5f %.5f", partition_sizes[i], integral_rectangle, integral_simpson)) {
			free_results(results, i + 1);
			ERROR_PRINT("Cannot write results to string in %d experiment\n", i);
			return NULL;
		}
	}

	return results;
}

void free_results(char **results, unsigned int count) {
	for (unsigned int i = 0; i < count; ++i) {
		free(results[i]);
	}
	free(results);
}

int main(int argc, char **argv) {
	struct interval_t interval = {0.0, M_PI};
	unsigned int partition_sizes[] = {5, 10, 20, 100, 500, 1000};
	size_t experiments_count = array_length(partition_sizes);
	char **results = calculate_integrals(interval, experiments_count, partition_sizes);
	if (!results) {
		return 1;
	}
	for (unsigned int i = 0; i < experiments_count; ++i) {
		if (printf("%s\n", results[i]) < 0) {
			ERROR_PRINT("Cannot wtite %d result to stdout", i);
			break;
		}
	}
	free_results(results, experiments_count);
	return 0;
}
