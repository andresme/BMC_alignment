#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "main.h"

#define NUM_THREADS 10
#define TEST_SIZE 100

typedef struct workNode {
	int i;
	int j;
	struct workNode *next;
} workNode;

typedef struct workList {
	workNode *head;
} workList;

pthread_mutex_t mutexWork;
pthread_mutex_t mutexDone;
workList *workQueue;
workList *doneQueue;
int stop = 0;

workNode* getFirstNode() {
	if (workQueue->head != NULL) {
		workNode *node = workQueue->head;
		workQueue->head = workQueue->head->next;
		return node;
	}
	return NULL;
}

int isInQueue(workNode *node) {
	if (doneQueue->head == NULL && workQueue->head == NULL) {
		return 0;
	} else {
		workNode *curr = workQueue->head;
		while (curr != NULL) {
			if (curr->i == node->i && curr->j == node->j) {
				return 1;
			}
			curr = curr->next;
		}
		curr = doneQueue->head;
		while (curr != NULL) {
			if (curr->i == node->i && curr->j == node->j) {
				return 1;
			}
			curr = curr->next;
		}
	}
	return 0;
}

void addWorkNode(workNode *node) {
	node->next = NULL;
	if (workQueue->head == NULL) {
		workQueue->head = node;
	} else {
		workNode *curr = workQueue->head;
		while (curr->next != NULL) {
			curr = curr->next;
		}
		curr->next = node;

	}
}

void addDoneNode(workNode *node) {
	node->next = NULL;
	if (doneQueue->head == NULL) {
		doneQueue->head = node;
	} else {
		workNode *curr = doneQueue->head;
		while (curr->next != NULL) {
			curr = curr->next;
		}
		curr->next = node;
		node->next = NULL;
	}
}

void *workThread(void *id) {
	while (stop != 1) {
		pthread_mutex_lock(&mutexWork);
		workNode *work = getFirstNode();
		pthread_mutex_unlock(&mutexWork);
		if (work != NULL) {
			pthread_mutex_lock(&mutexDone);
			addDoneNode(work);
			pthread_mutex_unlock(&mutexDone);

			//TODO: replace sleep for actual work
			sleep(0.250);

			if (work->i == TEST_SIZE - 1 && work->j == TEST_SIZE - 1) {
				stop = 1;
				printf("must stop working\n");
				break;
			}

			workNode *newNode1 = (workNode *) malloc(sizeof(workNode));
			newNode1->i = work->i + 1;
			newNode1->j = work->j;
			workNode *newNode2 = (workNode *) malloc(sizeof(workNode));
			newNode2->i = work->i;
			newNode2->j = work->j + 1;
			pthread_mutex_lock(&mutexDone);
			pthread_mutex_lock(&mutexWork);
			if (newNode1->i < TEST_SIZE && newNode1->j < TEST_SIZE
					&& isInQueue(newNode1) == 0) {
				addWorkNode(newNode1);
			} else {
				free(newNode1);
			}
			if (newNode2->i < TEST_SIZE && newNode2->j < TEST_SIZE
					&& isInQueue(newNode2) == 0) {
				addWorkNode(newNode2);
			} else {
				free(newNode2);
			}
			pthread_mutex_unlock(&mutexDone);
			pthread_mutex_unlock(&mutexWork);
		} else {
			//LOG("thread: %d, no work to be done\n", (int ) id);
		}
	}
	pthread_exit((void*) 0);
}

void destroyList(workList *list) {
	workNode * current = list->head;
	workNode * next = current;
	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
	free(list);
}

int main(int argc, char *argv[]) {
	void *status;

	for (int t = 1; t <= 100; t++) {
		workQueue = (workList *) malloc(sizeof(workList));
		workNode *head = (workNode *) malloc(sizeof(workNode));
		head->i = 1;
		head->j = 1;
		head->next = NULL;
		workQueue->head = head;
		doneQueue = (workList *) malloc(sizeof(workList));
		doneQueue->head = NULL;
		stop = 0;
		pthread_t callThd[t];
		pthread_attr_t attr;

		pthread_mutex_init(&mutexWork, NULL);
		pthread_mutex_init(&mutexDone, NULL);
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		clock_t t1, t2;
		t1 = clock();
		for (int i = 0; i < t; i++) {
			pthread_create(&callThd[i], &attr, workThread, (void *) i);
		}

		for (int i = 0; i < t; i++) {
			pthread_join(callThd[i], &status);
			if (status != 0) {
				printf("status: %d", status);
			}
		}
		t2 = clock();

		float diff = (((float) t2 - (float) t1) / 1000000.0F) * 1000;
		printf("it took: %f ms for %d threads\n", diff, t);
		destroyList(workQueue);
		destroyList(doneQueue);
		pthread_mutex_destroy(&mutexWork);
		pthread_mutex_destroy(&mutexDone);
	}
	pthread_exit(NULL);

}
