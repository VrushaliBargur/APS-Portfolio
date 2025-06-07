#include <stdio.h>

#define STREAM_SIZE 100

// Simulated stream data
int stream[STREAM_SIZE];

// Checkpoint to store last processed index
int checkpoint = -1;

// Process data from checkpoint + 1
void processStream() {
    for (int i = checkpoint + 1; i < STREAM_SIZE; i++) {
        // Process current record (example: just print)
        printf("Processing record %d: %d\n", i, stream[i]);

        // Update checkpoint after processing
        checkpoint = i;

        // Simulate saving checkpoint (in real use, persist to durable storage)
    }
}

int main() {
    // Initialize stream with sample data
    for (int i = 0; i < STREAM_SIZE; i++) {
        stream[i] = i * 2;
    }

    // First run: process entire stream
    processStream();

    // Simulate restart: processStream() resumes from last checkpoint
    printf("\nResuming processing after failure...\n");
    processStream();

    return 0;
}
