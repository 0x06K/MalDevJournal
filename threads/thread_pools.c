#include <windows.h>
#include <stdio.h>

#define MAX_THREADS 4
#define WORK_QUEUE_SIZE 10

// Work item structure
typedef struct {
    int taskId;
    int data;
} WorkItem;

// Thread pool context
typedef struct {
    PTP_POOL threadPool;
    TP_CALLBACK_ENVIRON callbackEnvironment;
    CRITICAL_SECTION criticalSection;
    WorkItem workQueue[WORK_QUEUE_SIZE];
    int queueHead;
    int queueTail;
    int itemCount;
} ThreadPoolContext;

// Worker function for thread pool
VOID CALLBACK WorkerCallback(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID Context,
    PTP_WORK Work
) {
    WorkItem* item = (WorkItem*)Context;
    
    // Simulate work
    printf("Processing Task ID: %d, Data: %d\n", item->taskId, item->data);
    Sleep(500); // Simulated processing time
}

// Initialize thread pool
BOOL InitializeThreadPool(ThreadPoolContext* context) {
    // Create thread pool
    context->threadPool = CreateThreadpool(NULL);
    if (context->threadPool == NULL) {
        printf("CreateThreadpool failed\n");
        return FALSE;
    }

    // Set max thread count
    SetThreadpoolThreadMaximum(context->threadPool, MAX_THREADS);
    SetThreadpoolThreadMinimum(context->threadPool, 1);

    // Initialize callback environment
    InitializeThreadpoolEnvironment(&context->callbackEnvironment);
    SetThreadpoolCallbackPool(&context->callbackEnvironment, context->threadPool);

    // Initialize critical section for thread-safe queue
    InitializeCriticalSection(&context->criticalSection);

    // Initialize queue
    context->queueHead = 0;
    context->queueTail = 0;
    context->itemCount = 0;

    return TRUE;
}

// Enqueue work item
BOOL EnqueueWorkItem(ThreadPoolContext* context, WorkItem item) {
    EnterCriticalSection(&context->criticalSection);

    if (context->itemCount >= WORK_QUEUE_SIZE) {
        LeaveCriticalSection(&context->criticalSection);
        return FALSE; // Queue full
    }

    context->workQueue[context->queueTail] = item;
    context->queueTail = (context->queueTail + 1) % WORK_QUEUE_SIZE;
    context->itemCount++;

    LeaveCriticalSection(&context->criticalSection);
    return TRUE;
}

// Submit work to thread pool
void SubmitWork(ThreadPoolContext* context, WorkItem* item) {
    PTP_WORK work = CreateThreadpoolWork(
        WorkerCallback, 
        item, 
        &context->callbackEnvironment
    );

    if (work != NULL) {
        SubmitThreadpoolWork(work);
        CloseThreadpoolWork(work);
    }
}

int main() {
    ThreadPoolContext context;

    // Initialize thread pool
    if (!InitializeThreadPool(&context)) {
        return 1;
    }

    // Create and submit work items
    for (int i = 0; i < 20; i++) {
        WorkItem item = { i, rand() % 100 };
        
        if (EnqueueWorkItem(&context, item)) {
            SubmitWork(&context, &item);
        }
    }

    // Wait for all work to complete
    WaitForThreadpoolWorkCallbacks(NULL, FALSE);

    // Cleanup
    DestroyThreadpoolEnvironment(&context.callbackEnvironment);
    CloseThreadpool(context.threadPool);
    DeleteCriticalSection(&context.criticalSection);

    return 0;
}