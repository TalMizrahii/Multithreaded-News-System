<h1 align="center">
<br>
  <a href="https://github.com/TalMizrahii/OS-Automated-Grading-System"><img src="https://logodownload.org/wp-content/uploads/2022/05/linux-logo-1.png" alt="HTML" width="200"></a>
  <br>
  Operating Systems - Multithreaded-News-System
  <br>
</h1>
<h4 align="center">The third assignment for Operating Systems course, Bar Ilan university.


## General Description

The purpose of this assignment is to gain experience with concurrent programming and synchronization mechanisms. The scenario simulates a news broadcasting system, where different types of stories are produced, sorted, and displayed to the public.

The assignment involves four types of active actors: Producers, Dispatcher, Co-Editors, and Screen Manager. The Producers generate strings representing news stories and send them to the Dispatcher. The Dispatcher sorts the stories based on their type and sends them to the corresponding Co-Editors. The Co-Editors edit the stories and pass them to the Screen Manager. Finally, the Screen Manager displays the stories on the screen. Each of them works in a different thread, allowing the system to work simultaneously.

The system includes a bounded buffer, which is used as a queue between the Producers and the Dispatcher, as well as between the Co-Editors and the Screen Manager. The Dispatcher's queues are unbounded buffers.

## System Design

The system can be visualized as follows:

Three Producers -> Dispatcher -> Three Co-Editors -> Screen Manager

 <img width="350" alt="2" src="https://github.com/TalMizrahii/Multithreaded-News-System/assets/103560553/8d67b13c-42d2-401a-b556-c9db0c08ad60">


The Producers communicate with the Dispatcher through their private queues. The Dispatcher scans the Producers' queues using a [round-robin](https://en.wikipedia.org/wiki/Round-robin_scheduling) algorithm and sorts the messages based on their type. The sorted messages are then inserted into the Dispatcher's queues. The Co-Editors receive the messages from the Dispatcher's queues, perform editing, and pass the edited messages to the Screen Manager through a shared queue. The Screen Manager displays the messages received from the Co-Editors on the screen.

## Bounded Buffer

The Producer queues and the Co-Editors' shared queue are implemented as a bounded buffer. The bounded buffer supports two main operations: `Push` and `Pop`. The `Push` operation is used to insert an object into the buffer, while the `Pop` operation removes and returns the first object from the buffer.

A thread-safe bounded and unbounded queues implemented using synchronization mechanisms such as [mutexes](https://en.wikipedia.org/wiki/Mutual_exclusion) and counting [semaphores](https://en.wikipedia.org/wiki/Semaphore_(programming)).
  
 <img width="300" alt="2" src="https://github.com/TalMizrahii/Multithreaded-News-System/assets/103560553/9a0a549b-a6bc-4221-b4e3-de300186d574">

## Configuration File

The system reads a configuration file to determine the number of Producers, the number of strings each Producer should create, and the size of the queues. The format of the configuration file is as follows:

PRODUCER 1
[number of products]
queue size = [size]

PRODUCER 2
[number of products]
queue size = [size]

...

PRODUCER n
[number of products]
queue size = [size]

Co-Editor queue size = [size]

  For example:
  
<img width="150" alt="2" src="https://github.com/TalMizrahii/Multithreaded-News-System/assets/103560553/9e9ebf29-b448-42ee-82d0-96f282a918a6">

  I also added a test configuration file called conf.txt to this repository.
  
## Implementation
  
### Directory Structure

The system directory contains the following folders:

- BoundedQueue: Contains the implementation files BoundedQueue.c and BoundedQueue.h for a bounded queue data structure.
- CoEditor: Contains the implementation files CoEditor.c and CoEditor.h for the Co-Editor actor.
- ControlAndData: Contains the implementation files ControlAndData.c and ControlAndData.h for managing control and data structures.
- Dispatcher: Contains the implementation files Dispatcher.c and Dispatcher.h for the Dispatcher actor.
- Producer: Contains the implementation files Producer.c and Producer.h for the Producer actor.
- ScreenManager: Contains the implementation files ScreenManager.c and ScreenManager.h for the Screen Manager actor.
- The main.c file is in the system's directory.
  
  
## Getting Started

To run the program, follow these steps:

1. Clone this repository.

