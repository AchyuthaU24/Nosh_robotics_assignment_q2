/*
 * Producer-Consumer Sensor Buffer Simulation
 * 
 * Producer: every 1 sim-second, adds 0-5 random bytes to shared buffer
 * Consumer: every 10 sim-seconds, if buffer has >=50 bytes,
 *           prints latest 50 (hex) and clears them
 *
 * ASSUMPTION: 1 real second = 200ms, so the whole demo finishes in a
 * few seconds instead of actually waiting 30+ real seconds.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUF_SIZE 500
#define THRESHOLD 50
#define TICK_MS 200          // 1 sim-second = 200ms real time
#define RUN_SECONDS 30

unsigned char buffer[BUF_SIZE];
int count = 0;                        // how many bytes currently in buffer
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Runs every 1 sim-second: adds random bytes
void* producer(void* arg) {
    for (int t = 1; t <= RUN_SECONDS; t++) {
        usleep(TICK_MS * 1000);

        int bytes_to_add = rand() % 6;   // random 0-5

        pthread_mutex_lock(&lock);
        for (int i = 0; i < bytes_to_add && count < BUF_SIZE; i++) {
            buffer[count] = rand() % 256;  // random byte value
            count++;
        }
        printf("t=%2ds | Producer added %d bytes | buffer=%d\n", t, bytes_to_add, count);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

// Runs every 10 sim-seconds: prints latest 50 bytes if available
void* consumer(void* arg) {
    for (int t = 10; t <= RUN_SECONDS; t += 10) {
        usleep(TICK_MS * 1000 * 10);

        pthread_mutex_lock(&lock);
        if (count >= THRESHOLD) {
            printf("\nt=%2ds | Buffer has %d bytes -> printing latest %d:\n", t, count, THRESHOLD);

            int start = count - THRESHOLD;   // index where the "latest 50" begins
            for (int i = start; i < count; i++) {
                printf("%02X ", buffer[i]);
            }
            printf("\n\n");

            count = 0;   // delete printed bytes (clear buffer)
        } else {
            printf("t=%2ds | Buffer has %d bytes -> not enough to print\n", t, count);
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t p1, p2;

    pthread_create(&p1, NULL, producer, NULL);
    pthread_create(&p2, NULL, consumer, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    return 0;
}
