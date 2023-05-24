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

The system includes a bounded buffer, which is used as a queue between the Producers and the Dispatcher, as well as between the Co-Editors and the Screen Manager. This desgind needs to face the  [Producer–consumer problem](https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem). The Dispatcher's queues are unbounded buffers.
  
  
<img width="300" alt="4" src="https://github.com/TalMizrahii/Multithreaded-News-System/assets/103560553/95d8602d-24a0-41dc-8c8c-00e35bca733d">

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
  
The implementation of the Multithreaded News System consists of several components that work together to process and display news stories. Each component performs specific tasks and operates in its own thread, allowing for concurrent execution. Here are the main components of the system:
  
  <img width="250" alt="5" src="https://github.com/TalMizrahii/Multithreaded-News-System/assets/103560553/c4e3c836-9dfe-4a2b-be59-0627cbf308a2">


Producers:

- Generate strings representing news stories.
- Send the generated stories to the Dispatcher for further processing.
- Operate in their own threads.

Dispatcher:

- Receives news stories from Producers.
- Sorts the stories based on their type.
- Forwards each story to the corresponding Co-Editor for editing.
- Operates in its own thread.

Co-Editors:

- Receive news stories from the Dispatcher.
- Edit the stories according to their assigned type.
- Pass the edited stories to the Screen Manager for display.
- Each Co-Editor operates in its own thread.

Screen Manager:

- Receives edited news stories from the Co-Editors.
- Displays the stories on the screen.
- Operates in its own thread.

  
The system utilizes multithreading to enable concurrent execution of these components. Each component is implemented as a separate module, with its own set of functions and data structures to fulfill its specific responsibilities.

The main.c file serves as the entry point of the program and coordinates the initialization and execution of the system. It performs tasks such as reading the configuration file, creating the necessary threads for the components, and waiting for all threads to finish execution.

The system directory contains folders for each component, with corresponding .c and .h files that provide the implementation and necessary functions for each component.

 I have thoroughly tested the Multi-Threaded News System for memory leaks and errors using [Valgrind](https://en.wikipedia.org/wiki/Valgrind), a powerful tool for detecting memory management issues. The extensive testing process involved running the system with different scenarios and input data to ensure its stability and reliability.
  
  
Overall, the implementation follows a modular and concurrent design, allowing the system to efficiently process news stories and display them on the screen.
  
### Execution
To clone and run this program, you'll need [Git](https://git-scm.com) installed on your computer. From your command line:


```bash
// Clone this repository:
git clone https://github.com/TalMizrahii/Multithreaded-News-System

// Go into the repository:
cd Multithreaded-News-System

// Compile using Makefile:
 make

// An ex3.ou file will be created. You can run:
 make run

// Or directly give the path to the configuration file.
 ./ex3.out conf.txt

// To clean the executabke file:
 make clean
```

## Author
* [@Tal Mizrahi](https://github.com/TalMizrahii)
