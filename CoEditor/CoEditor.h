#ifndef OS_EX3_COEDITOR_H
#define OS_EX3_COEDITOR_H

#include "../UnboundedQueue/UnBoundedQueue.h"
#include "../ControlAndData/ControlAndData.h"
#include "../BoundedQueue/BoundedQueue.h"

typedef struct {
    UnBoundedQueue *unBoundedQueue;
    int unboundedQueueSize;
    int articleSerial;
    BoundedQueue *SMBoundedQueue;
} CoEditor;

void *coEditorJob(void *coEditorArg);

CoEditor *createCoEditor(int serial, UnBoundedQueue *unBoundedQueue);

void destroyCoEditor(CoEditor *coEditor);

#endif //OS_EX3_COEDITOR_H
