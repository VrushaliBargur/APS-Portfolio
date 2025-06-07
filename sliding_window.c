
// C implementation of a Sliding Window algorithm that calculates the moving average over the last k data points in a stream
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *window;
    int size;
    int count;
    int start;
    int sum;
} SlidingWindow;

SlidingWindow* createWindow(int size) {
    SlidingWindow *sw = (SlidingWindow *)malloc(sizeof(SlidingWindow));
    sw->window = (int *)malloc(size * sizeof(int));
    sw->size = size;
    sw->count = 0;
    sw->start = 0;
    sw->sum = 0;
    return sw;
}

double process(SlidingWindow *sw, int new_value) {
    if (sw->count < sw->size) {
        sw->window[(sw->start + sw->count) % sw->size] = new_value;
        sw->sum += new_value;
        sw->count++;
    } else {
        // Remove oldest value and add new value
        int oldest = sw->window[sw->start];
        sw->sum = sw->sum - oldest + new_value;
        sw->window[sw->start] = new_value;
        sw->start = (sw->start + 1) % sw->size;
    }
    return (double)sw->sum / sw->count;
}

void freeWindow(SlidingWindow *sw) {
    free(sw->window);
    free(sw);
}

int main() {
    int stream_data[] = {10, 20, 30, 40, 50, 60, 70};
    int n = sizeof(stream_data) / sizeof(stream_data[0]);
    int window_size = 3;

    SlidingWindow *sw = createWindow(window_size);

    for (int i = 0; i < n; i++) {
        double avg = process(sw, stream_data[i]);
        printf("New value: %d, Current %d-point average: %.2f\n", stream_data[i], window_size, avg);
    }

    freeWindow(sw);
    return 0;
}
