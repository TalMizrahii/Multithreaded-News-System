#ifndef OS_EX3_COEDITOR_H
#define OS_EX3_COEDITOR_H

#include "UnBoundedQueue.h"
#include "ControlAndData.h"
#include "BoundedQueue.h"


typedef struct {
    UnBoundedQueue *unBoundedQueue;
    int unboundedQueueSize;
    int articleSerial;
    BoundedQueue *SMBoundedQueue;
} CoEditor;


void *coEditorJob(void *coEditorArg);

CoEditor *createCoEditor(int serial, UnBoundedQueue *unBoundedQueue);

#endif //OS_EX3_COEDITOR_H
