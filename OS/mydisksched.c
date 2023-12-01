#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define DISK_SIZE   170

void fcfs(int request[], int n, int head);
void sstf(int request[], int n, int head);
void scan(int request[], int n, int head, int direction);
void cscan(int request[], int n, int head);
void look(int request[], int n, int head, int direction);
void clook(int request[], int n, int head);

int higherSeek(int request[], int n, int* pHead);
int lowerSeek(int request[], int n, int* pHead);

int main(void){
    int n, head, direction;

    printf("Enter number of requests:");
    scanf("%d", &n);

    int* request = NULL;
    request = (int*)malloc(n * sizeof(int));
    assert(request != NULL);
    
    printf("Enter requests:");
    for(int i = 0; i < n; ++i){
        scanf("%d", &request[i]);
    }

    printf("Enter initial head position:");
    scanf("%d", &head);
    if(head < 0 || head >= DISK_SIZE){
        puts("Invalid head position!");
        exit(EXIT_FAILURE);
    }

    printf("Enter the direction (0 for higher values and 1 for lower values):");
    scanf("%d", &direction);

    if(direction != 0 && direction != 1){
        puts("Invalid direction!");
        exit(EXIT_FAILURE);
    }

    fcfs(request, n, head);
    sstf(request, n, head);
    scan(request, n, head, direction);
    look(request, n, head, direction);
    cscan(request, n, head);
    clook(request, n, head);

    exit(0);
}

void fcfs(int request[], int n, int head){
    int totalSeekTime = 0;

    puts("\n---FCFS Algorithm---");
    printf("Head position:\t%d\n", head);
    for(int i = 0; i < n; ++i){
        totalSeekTime += abs(head - request[i]);
        head = request[i];
        printf("Request processed:\t%d\n", head);
    }

    printf("\nTotal Seek Time = %d\n", totalSeekTime);
    puts("---End of FCFS---");
}

void sstf(int request[], int n, int head){
    int totalSeekTime = 0;
    puts("\n---SSTF Algorithm---");
    printf("Head position:\t%d\n", head);

    int* visited = NULL;
    visited = (int*)malloc(n * sizeof(int));
    assert(visited != NULL);

    for(int i = 0; i < n; ++i)
        visited[i] = 0;

    for(int i = 0; i < n; ++i){
        int minDistance = DISK_SIZE;    // asssuming maximum possible distance
                                        // from head to the request
        int index = -1;

        // finding the request with the shortest seek time
        for(int j = 0; j < n; ++j){
            if(!visited[j] && abs(head - request[j]) < minDistance){
                index = j;
                minDistance = abs(head - request[j]);
            }
        }

        visited[index] = 1;
        totalSeekTime += minDistance;
        head = request[index];
        printf("Request processed:\t%d\n", head);
    }

    printf("\nTotal Seek Time = %d\n", totalSeekTime);
    puts("---End of SSTF---");
}

void scan(int request[], int n, int head, int direction){
    puts("\n---SCAN Algorithm---");
    printf("Head Position:\t%d\n", head);

    int currentHead;  // dummy head especially used for skipping preprocessed requests
    int totalSeekTime = 0;

    if(direction == 0){
        currentHead = head;
        // Moving towards higher values
        totalSeekTime += higherSeek(request, n, &currentHead);

        // from head to end of disk and turnaround
        totalSeekTime += 2 * (DISK_SIZE - currentHead);

        // skipping the already processed higher requests and adding its equivalent
        totalSeekTime += abs(head - currentHead);

        currentHead = head;
        // Moving towards lower valuesS
        totalSeekTime += lowerSeek(request, n, &currentHead);
    } else {
        currentHead = head;
        // Moving towards lower values
        totalSeekTime += lowerSeek(request, n, &currentHead);

        // from head to start of disk and turnaround
        totalSeekTime += 2 * (currentHead);

        // skipping the already processed lower requests and adding its equivalent
        totalSeekTime += abs(head - currentHead);

        currentHead = head + 1;
        // Moving towards higher valuesS
        totalSeekTime += higherSeek(request, n, &currentHead);
    }

    printf("\nTotal Seek Time = \t%d\n", totalSeekTime);
    puts("---End of SCAN---");
}

// cscan always scans forward
void cscan(int request[], int n, int head){
    puts("\n---CSCAN Algorithm---");
    printf("Head Position:\t%d\n", head);

    int currentHead = head;
    int totalSeekTime = 0;

    // Moving towards higher values
    totalSeekTime += higherSeek(request, n, &currentHead);

    totalSeekTime += abs(DISK_SIZE - currentHead);  // going till the end
    
    currentHead = head;
    // Moving towards lower track numbers (wrap-around)
    for (int currentIndex = 0; currentIndex <= currentHead; ++currentIndex) {
        for (int i = 0; i < n; i++) {
            if (currentIndex == request[i]) {
                totalSeekTime += abs(head - request[i]);
                head = request[i];
                printf("Request processed:\t%d\n", head);
            }
        }
    }

    printf("Total Seek Time = %d\n", totalSeekTime);
    puts("---End of CSCAN---");
}

void look(int request[], int n, int head, int direction){
    puts("\n---LOOK Algorithm---");
    printf("Head Position\t%d\n", head);

    int currentHead;
    int totalSeekTime = 0;

    if(direction == 0){
        currentHead = head;
        // Moving towards higher values
        totalSeekTime += higherSeek(request, n, &currentHead);

        // skipping the already processed values and adding their equivalent
        totalSeekTime += abs(currentHead - head);

        currentHead = head;
        // Moving towards the lower values
        totalSeekTime += lowerSeek(request, n, &currentHead);
    } else {
        currentHead = head;
        // Moving towards lower values
        totalSeekTime += lowerSeek(request, n, &currentHead);

        // skipping the already processed values and adding their equivalent
        totalSeekTime += abs(currentHead - head);

        currentHead = head + 1;
        // Moving towards the higher values
        totalSeekTime += higherSeek(request, n, &currentHead);
    }

    printf("\nTotal Seek Time = %d\n", totalSeekTime);
    puts("---End of LOOK---");
}

void clook(int request[], int n, int head){
    puts("\n---CLOOK Algorithm---");
    printf("Head Position:\t%d\n", head);

    int currentHead = head;
    int totalSeekTime = 0;

    // Moving towards higher values
    totalSeekTime += higherSeek(request, n, &currentHead);

    totalSeekTime += abs(DISK_SIZE - currentHead);
    
    currentHead = head;
    // Moving towards lower track numbers (wrap-around)
    for (int currentIndex = 0; currentIndex <= currentHead; ++currentIndex) {
        for (int i = 0; i < n; i++) {
            if (currentIndex == request[i]) {
                totalSeekTime += abs(head - request[i]);
                head = request[i];
                printf("Request processed:\t%d\n", head);
            }
        }
    }

    printf("Total Seek Time = %d\n", totalSeekTime);
    puts("---End of CLOOK---");
}

int higherSeek(int request[], int n, int* pHead){
    // seeking requests above the head
    int totalSeekTime = 0;
    int currentIndex;
    int head = *pHead;

    for(currentIndex = head; currentIndex < DISK_SIZE; ++currentIndex){
        for(int i = 0; i < n; ++i){
            if(currentIndex == request[i] && request[i] > *pHead){
                totalSeekTime += abs(request[i] - head);
                head = request[i];
                printf("Request processed:\t%d\n", head);
            }
        }
    }

    *pHead = head;
    return totalSeekTime;
}

int lowerSeek(int request[], int n, int* pHead){
    // seeking requests below the head
    int totalSeekTime = 0;
    int currentIndex;
    int head = *pHead;

    for(currentIndex = head; currentIndex >= 0; --currentIndex){
        for(int i = 0; i < n; ++i){
            if(currentIndex == request[i] && request[i] < *pHead){
                totalSeekTime += abs(request[i] - head);
                head = request[i];
                printf("Request processed:\t%d\n", head);
            }
        }
    }

    *pHead = head;
    return totalSeekTime;
}
