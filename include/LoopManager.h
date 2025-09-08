// // LoopManager.h

// #ifndef LOOPMANAGER_H
// #define LOOPMANAGER_H

// typedef void (*TaskFunc)();

// #define MAX_TASKS 4
// static TaskFunc loopTasks[MAX_TASKS];
// static int loopTaskCount = 0;

// inline void registerLoopTask(TaskFunc f) {
//   if (loopTaskCount < MAX_TASKS) loopTasks[loopTaskCount++] = f;
// }

// inline void runLoopTasks() {
//   for (int i = 0; i < loopTaskCount; i++) loopTasks[i]();
// }

// #endif
