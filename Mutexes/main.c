#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <pthread.h>

#define ARRAY_SIZE 100000

struct _thread_data
{
	int sum;
	int data[ARRAY_SIZE];
};
int _thread_count;
pthread_mutex_t _mutex;
struct _thread_data _data;

char* get_argument(int argc, const char** argv, const char* prefix)
{
	for (int i = 1; i < argc; i++) {
		char* arg = (char*)(*(argv + i));
		char* prefix_ptr = (char*)prefix;

		while (true) {
			char prefix_ptr_c = (*prefix_ptr);
			char arg_c = (*(arg + (prefix_ptr - prefix)));
			if (arg_c == prefix_ptr_c) {
				prefix_ptr++;
			} else if (prefix_ptr_c == '\0') {
				return (arg + (prefix_ptr - prefix));
			} else {
				break;
			}
		}
	}

	return NULL;
}

void* thread_fn(void* arg)
{
	int i;
	int sum = 0;
	int thread_id = (*(int*)arg);

	for (int i = thread_id * (ARRAY_SIZE / _thread_count); i < (thread_id + 1) * (ARRAY_SIZE / _thread_count); i++) {
		sum += _data.data[i];
	}

	if (thread_id == _thread_count - 1 && ARRAY_SIZE % _thread_count != 0) {
		for (i = 0; i < ARRAY_SIZE % _thread_count; i++) {
			sum += _data.data[i];
		}
	}

	pthread_mutex_lock(&_mutex);

	_data.sum += sum;

	pthread_mutex_unlock(&_mutex);
}

int main(int argc, const char** argv)
{
	_thread_count = strtol(get_argument(argc, argv, "-threads="), NULL, 10);

	int i;
	_data.sum = 0;
	for (i = 0; i < ARRAY_SIZE; i++) {
		_data.data[i] = 1;
	}

	int* thread_ids = malloc(sizeof(int) * _thread_count);
	pthread_t* threads = malloc(sizeof(pthread_t) * _thread_count);

	pthread_attr_t thread_attr;
	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

	pthread_mutex_init(&_mutex, NULL);

	for (i = 0; i < _thread_count; i++) {
		(*(thread_ids + i)) = i;
		pthread_create((threads + i), &thread_attr, thread_fn, (thread_ids + i));
	}

	for (i = 0; i < _thread_count; i++) {
		pthread_join((*(threads + i)), NULL);
	}

	pthread_attr_destroy(&thread_attr);
	pthread_mutex_destroy(&_mutex);

	free(thread_ids);
	free(threads);

	printf("Sum: %d\n", _data.sum);

	return 0;
}
