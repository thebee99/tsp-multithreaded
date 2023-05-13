#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int m[17][17];

int path[17];
int used[17];
int length = 0;
int min = -1;

void _travel(int idx) {
    int i;

    if (idx == 17) {
        length += m[path[16]][path[0]];
        if (min == -1 || min > length) {
            min = length;

            printf("%d (", length);
            for (i = 0; i < 17; i++)
                printf("%d ", path[i]);
            printf("%d)\n", path[0]);
        }
        length -= m[path[16]][path[0]];
    } else {
        for (i = 0; i < 17; i++) {
            if (used[i] == 0) {
                path[idx] = i;
                used[i] = 1;
                length += m[path[idx - 1]][i];
                _travel(idx + 1);
                length -= m[path[idx - 1]][i];
                used[i] = 0;
            }
        }
    }
}

void *thread_travel(void *arg) {
    int start = *(int *)arg;
    travel(start);
    return NULL;
}

void travel(int start) {
    path[0] = start;
    used[start] = 1;
    _travel(1);
    used[start] = 0;
}

int main() {
    int i, j, t;
    pthread_t thread_id[17];
    int start_args[17];

    FILE *fp = fopen("gr17.tsp", "r");

    for (i = 0; i < 17; i++) {
        for (j = 0; j < 17; j++) {
            fscanf(fp, "%d", &t);
            m[i][j] = t;
        }
    }
    fclose(fp);

    for (i = 0; i < 17; i++) {
        start_args[i] = i;
        pthread_create(&thread_id[i], NULL, thread_travel, &start_args[i]);
    }

    for (i = 0; i < 17; i++) {
        pthread_join(thread_id[i], NULL);
    }

    return 0;
}
