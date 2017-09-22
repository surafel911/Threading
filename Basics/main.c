#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

int _thread_count;

void*
thread_fn(void* thread_id)
{
	printf("Thread ID: %d\n", (*(int*)thread_id) + 1);

	pthread_exit(NULL);
}

int
main(int argc, char** argv)
{
	_thread_count = argv[1][0] - '0';
	if (_thread_count == 1) {
		printf("Thread count command line argument needed.\n");
		return -1;
	}

	if (argc > 3) {
		printf("Too many arguments or too many threads.\n");
		return -1;
	}

	int* thread_ids = malloc(sizeof(int) * _thread_count);
	pthread_t* threads = malloc(sizeof(pthread_t) * _thread_count);

	int i;
	for (i = 0; i < _thread_count; i++) {
		thread_ids[i] = i;
		pthread_create(&threads[i], NULL, thread_fn, &thread_ids[i]);
	}

	for (i = 0; i < _thread_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(thread_ids);
	free(threads);

	return 0;
}
