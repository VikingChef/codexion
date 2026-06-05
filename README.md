*This project has been created as part of the 42 curriculum by rrasmuss.*

# Codexion

## Description

Codexion is a C concurrency project about coders competing for shared USB dongles before they burn out.

Each coder is represented by a thread. To compile, a coder must acquire two dongles, one on the left and one on the right. After compiling, the coder debugs, then refactors, and repeats this cycle until either:

* every coder has completed the required number of compiles, or
* one coder burns out.

The project focuses on thread synchronization, fair access to shared resources, burnout detection, serialized logging, cooldown handling, and scheduler-based arbitration.

The scheduler can run in two modes:

* `fifo`: requests are served by arrival order.
* `edf`: requests are served by earliest burnout deadline.

In other words: a small group of coders, not enough dongles, hard deadlines, and exactly the kind of situation where C quietly asks, “Are you sure you know what you are doing?”

## Instructions

### Compilation

```bash
make
```

This creates the executable:

```bash
./codexion
```

The project compiles with:

```bash
-Wall -Wextra -Werror -pthread
```

### Cleaning

```bash
make clean
```

Removes object files.

```bash
make fclean
```

Removes object files and the executable.

```bash
make re
```

Rebuilds the project from scratch.

### Execution

The program expects exactly 8 arguments:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Example:

```bash
./codexion 3 800 100 100 100 3 10 fifo
```

Another example using EDF:

```bash
./codexion 5 620 150 100 100 2 10 edf
```

### Arguments

* `number_of_coders`: number of coder threads.
* `time_to_burnout`: time in milliseconds before a coder burns out without compiling.
* `time_to_compile`: time in milliseconds spent compiling.
* `time_to_debug`: time in milliseconds spent debugging.
* `time_to_refactor`: time in milliseconds spent refactoring.
* `number_of_compiles_required`: number of successful compiles required for each coder.
* `dongle_cooldown`: cooldown in milliseconds before a released dongle can be taken again.
* `scheduler`: must be either `fifo` or `edf`.

### Output format

The output format is:

```text
timestamp coder_id status
```

Example:

```text
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
100 1 is debugging
200 1 is refactoring
```

Coder IDs are printed from `1` to `number_of_coders`.

## Resources

The main references and resources used during this project were:

* *The C Programming Language* by Brian W. Kernighan and Dennis M. Ritchie. Small book. Large consequences.
* The official Codexion subject.
* The 42 Norm.
* Manual pages and documentation for:

  * `pthread_create`
  * `pthread_join`
  * `pthread_mutex_init`
  * `pthread_mutex_lock`
  * `pthread_mutex_unlock`
  * `pthread_mutex_destroy`
  * `pthread_cond_init`
  * `pthread_cond_broadcast`
  * `pthread_cond_destroy`
  * `gettimeofday`
  * `usleep`
  * `malloc`
  * `free`
  * `printf`
  * `strcmp`
  * `atoi`
* Peer discussion and review.
* AI assistance.

### AI usage

### AI usage

AI was used as a support tool during the project, mainly for tutoring, review, and debugging discussion.

It was used for:

* explaining concurrency concepts;
* discussing architecture and file responsibilities;
* reviewing edge cases;
* suggesting tests;
* helping identify unclear or risky logic;
* checking whether implementation choices matched the project requirements.

The code was written and tested by the author. AI assistance was used mostly through guided hints, questions, and corrections rather than ready-made solutions.

When suggestions were unclear, too broad, incorrect, or not aligned with the project constraints, they were challenged, revised, or rejected.

In short: AI was used as a stubborn rubber duck with occasional useful ideas, not as the project’s driver.


## Blocking cases handled

### Deadlock prevention

The program avoids the classic problem where a coder takes one dongle and waits forever for the second.

Dongles are acquired as a pair. A coder must be granted access to both required dongles before compiling. This prevents partial ownership of one dongle while waiting for another.

The helper functions in `dongles_lock.c` lock two dongle mutexes in a deterministic order. This avoids circular waits between threads.

### Dongle duplication prevention

Each dongle has its own protected state.

A dongle cannot be marked as taken by two coders at the same time because dongle access is guarded by mutexes. Availability is checked while the relevant dongle mutexes are locked.

### Fair arbitration

Each dongle owns a request queue.

Requests are ordered according to the selected scheduler:

* FIFO uses `arrival_time`.
* EDF uses `deadline`.

The project implements a custom heap-based priority queue for request ordering.

A coder request is only granted when it has the correct queue position for the required dongles and both dongles are available.

### Cooldown handling

After a coder releases dongles, both dongles receive a `cooldown_until` timestamp.

A dongle cannot be taken again until the current time is greater than or equal to its cooldown timestamp.

This prevents immediate reuse and enforces the required cooldown behavior.

### Burnout detection

A separate monitor thread checks coder state and detects burnout.

The monitor observes the time since each coder’s last compile start and stops the simulation if a coder exceeds `time_to_burnout`.

### Log serialization

All log output is protected by a log mutex.

This prevents interleaved log lines such as two threads writing half a message each into the same line.

The result is readable output and evaluator-friendly logs.

### One-coder edge case

When there is only one coder, left and right dongle references can point to the same dongle.

Queue logic handles this case without pushing or popping the same dongle queue twice.

## Thread synchronization mechanisms

### `pthread_mutex_t`

Mutexes are used to protect shared state.

They are used for:

* dongle state;
* coder state;
* simulation end state;
* logging.

Each dongle has a mutex protecting its availability, cooldown, and queue state.

Coder state is protected when reading or updating values such as compile progress and last compile timing.

The log mutex ensures only one thread prints at a time.

### `pthread_cond_t`

Each dongle contains a condition variable.

Condition broadcasts are used when dongles are released, so waiting coders can be notified that shared dongle state has changed.

### Shared simulation state

The simulation end flag is protected by a mutex.

This allows coder threads and the monitor thread to safely check whether the simulation should continue.

### Queue synchronization

Dongle request queues are accessed while the relevant dongle mutexes are locked.

This prevents two coders from modifying the same queue at the same time.

The scheduler heap is used to keep queue ordering deterministic under FIFO or EDF.

## Scheduler behavior

### FIFO

FIFO prioritizes the request with the earliest arrival time.

If two requests have the same arrival time, the implementation uses coder ID as a deterministic tie-breaker.

### EDF

EDF prioritizes the request with the earliest burnout deadline.

The deadline is based on the coder’s last compile start and `time_to_burnout`.

If two deadlines are equal, coder ID is used as a deterministic tie-breaker.

## Notes for evaluation

Useful smoke tests:

```bash
./codexion 3 800 100 100 100 3 10 fifo
```

```bash
./codexion 3 800 100 100 100 3 10 edf
```

Pressure-style comparison:

```bash
./codexion 5 620 150 100 100 2 10 fifo
```

```bash
./codexion 5 620 150 100 100 2 10 edf
```

Recommended final checks before submission:

```bash
norminette
make re
make fclean
```

Memory checking should also be performed using an available tool such as `valgrind` or `leaks`, depending on the evaluation machine.

## Project status

The project is implemented in C using pthreads and custom synchronization logic.

No global mutable state is used to manage dongles, scheduling, or logging.

All shared state is stored in project structures and protected through mutexes.

