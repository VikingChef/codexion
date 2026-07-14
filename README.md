*This project has been created as part of the 42 curriculum by rrasmuss.*

# Codexion

## Description

Codexion is a multithreaded C simulation in which coders compete for shared USB dongles.

Each coder is represented by its own POSIX thread. A coder repeatedly performs three phases:

1. compile;
2. debug;
3. refactor.

To begin compiling, a coder must be granted both adjacent dongles. A coder cannot hold only one dongle while waiting for the other.

The simulation ends when either:

* every coder has completed the required number of compiles; or
* one coder reaches its burnout deadline without beginning another compile.

The project focuses on:

* thread creation and lifecycle management;
* synchronization of shared resources;
* deadlock prevention;
* prevention of duplicated dongle ownership;
* cooldown enforcement;
* serialized logging;
* burnout monitoring;
* heap-based FIFO and EDF scheduling;
* complete cleanup of allocated memory and synchronization objects.

The two supported scheduling policies are:

* `fifo`: the request that arrived first has priority;
* `edf`: the request with the earliest burnout deadline has priority.

No mutable global variables are used to manage dongles, scheduling, logging, coder state, or simulation state. Shared data is stored in project structures and passed explicitly to threads and functions.

## Instructions

### Compilation

Compile the project with:

```bash
make
```

This creates the executable:

```bash
codexion
```

The project is compiled with:

```bash
-Wall -Wextra -Werror -pthread
```

### Cleaning

Remove object files:

```bash
make clean
```

Remove object files and the executable:

```bash
make fclean
```

Rebuild the project from scratch:

```bash
make re
```

### Execution

The program requires exactly eight arguments:

```bash
./codexion number_of_coders time_to_burnout time_to_compile \
time_to_debug time_to_refactor number_of_compiles_required \
dongle_cooldown scheduler
```

Example using FIFO:

```bash
./codexion 5 2000 200 200 200 10 0 fifo
```

Example using EDF:

```bash
./codexion 5 2000 200 200 200 7 0 edf
```

### Arguments

* `number_of_coders`: number of coder threads and dongles.
* `time_to_burnout`: maximum time in milliseconds between compile starts.
* `time_to_compile`: duration of the compile phase in milliseconds.
* `time_to_debug`: duration of the debug phase in milliseconds.
* `time_to_refactor`: duration of the refactor phase in milliseconds.
* `number_of_compiles_required`: successful compiles required from every coder.
* `dongle_cooldown`: time in milliseconds before a released dongle may be granted again.
* `scheduler`: scheduling policy, either `fifo` or `edf`.

The first seven arguments must contain digits only.

`number_of_coders` must be greater than zero.

The scheduler must be exactly:

```text
fifo
```

or:

```text
edf
```

### Output

Every log entry uses this format:

```text
timestamp coder_id status
```

A compile grant is printed as one serialized three-line event:

```text
0 1 has taken a dongle 1
0 1 has taken a dongle 2
0 1 is compiling
```

The two dongle lines and the compile line:

* use the same timestamp;
* belong to the same coder;
* are printed while holding one logging mutex;
* cannot be interrupted by another coder's output.

Coder and dongle IDs are displayed from `1`, even though internal array indexes begin at `0`.

A burnout entry has this format:

```text
800 1 burned out
```

Once a burnout is printed, no later status messages are printed.

A successful simulation exits silently after every coder completes the required number of compiles.

## Resources

Resources used during the project include:

* the official Codexion subject;
* the Codexion evaluation scale;
* the 42 Norm;
* POSIX thread documentation;
* C standard-library documentation;
* manual pages for:

  * `pthread_create`;
  * `pthread_join`;
  * `pthread_mutex_init`;
  * `pthread_mutex_lock`;
  * `pthread_mutex_unlock`;
  * `pthread_mutex_destroy`;
  * `pthread_cond_init`;
  * `pthread_cond_broadcast`;
  * `pthread_cond_destroy`;
  * `gettimeofday`;
  * `usleep`;
  * `malloc`;
  * `calloc`;
  * `free`;
  * `printf`;
  * `strcmp`;
  * `atoi`;
* *The C Programming Language* by Brian W. Kernighan and Dennis M. Ritchie;
* peer discussion and review;
* runtime tools including ThreadSanitizer and the macOS `leaks` tool.

### AI usage

AI was used as a support and development tool during the project.

It was used for:

* explaining pthreads, mutexes, priority queues, FIFO, and EDF;
* discussing scheduler architecture;
* reviewing lock ordering and shared-state ownership;
* identifying race, deadlock, logging, and cleanup risks;
* reviewing and generating replacement implementations during debugging;
* checking function and file responsibilities against the 42 Norm;
* constructing benchmark and regression tests;
* interpreting evaluation requirements;
* drafting and reviewing this README.

AI suggestions were not treated as automatically correct. Proposed changes were compiled, checked with Norminette, tested against the evaluation benchmarks, checked with ThreadSanitizer, and checked for leaks.

The final implementation remains the responsibility of the author, including understanding, testing, defending, and maintaining the submitted code.

## Blocking cases handled

### Partial dongle ownership

A coder is never allowed to take one dongle and then wait while holding it for the second.

A request is granted only when both required dongles can be assigned as a pair.

This prevents the classic circular-wait situation in which every coder holds one resource and waits forever for another.

### Deadlock prevention

When two dongle mutexes must be locked, `lock_two_dongles()` always locks them in deterministic numeric order.

The lower-indexed dongle is locked first and the higher-indexed dongle second.

Every thread follows the same ordering rule, which prevents opposing lock orders from creating a circular wait.

### One-coder case

With one coder, the left and right dongle indexes refer to the same physical dongle.

One dongle cannot satisfy a request for two dongles, so the coder cannot compile and eventually burns out.

The implementation rejects a same-index pair request before entering normal pair scheduling.

### Dongle duplication

Each dongle has:

* an availability field;
* a cooldown deadline;
* its own mutex.

A scheduler grant checks and changes both dongles while their mutexes are held.

Once granted, both dongles are marked unavailable before their mutexes are released.

This prevents two coders from being granted the same dongle simultaneously.

### Conflicting scheduler grants

The scheduler uses a shared priority queue protected by `scheduler_mutex`.

During a scheduler pass, dongles associated with an earlier eligible request are reserved in temporary scheduler state.

A later request in the same pass cannot be granted if it overlaps with an already reserved pair.

Non-conflicting pairs may still be granted during the same scheduler pass.

### Queue consistency

Scheduler requests are stored in a bounded binary heap.

All insertion, ordering, grant, and removal operations on the shared scheduler queue occur while `scheduler_mutex` is held.

A request is removed after its coder has been granted both required dongles.

If the simulation ends while a coder is waiting, its remaining request is removed before the thread exits.

### Cooldown enforcement

When a coder finishes compiling, both dongles receive a `cooldown_until` timestamp.

The dongles are marked available, but the scheduler does not grant them until:

```text
current_time >= cooldown_until
```

Availability and cooldown are checked while both dongle mutexes are locked.

This separates physical release from eligibility for immediate reuse.

### Burnout during queueing

A coder's deadline is based on:

* the simulation start time, before its first compile; or
* its most recent compile-start time.

The scheduler records `last_compile_start` when the coder is granted its dongle pair.

This prevents a coder that has already received a valid compile grant from being incorrectly declared burned out merely because its thread has not yet resumed execution.

A dedicated monitor thread continually checks every coder's deadline.

### Output after burnout

Burnout logging acquires both the logging mutex and simulation-end mutex.

The burnout function sets `simulation_end` before releasing the logging mutex.

Ordinary logging functions check `simulation_end` while holding the logging mutex.

This guarantees that the burnout line is the final line printed.

### Interleaved compile logging

The two dongle-acquisition messages and the compile message are printed by one function while holding `log_mutex` once.

Another thread cannot print between those three lines.

This makes the compile state transition directly visible in the log.

### Simulation completion

Every coder protects its compile counter with its own state mutex.

The monitor checks whether all coders have reached `number_of_compiles_required`.

When all coders are finished, it sets the protected simulation-end flag so waiting or sleeping threads can exit cleanly.

### Cleanup after normal completion or burnout

The program joins all created coder threads and the monitor thread before destroying shared resources.

Cleanup includes:

* coder mutexes;
* dongle mutexes;
* dongle condition variables;
* per-dongle allocations;
* the shared scheduler queue;
* the scheduler mutex;
* the logging mutex;
* the simulation-end mutex;
* coder and dongle arrays.

The normal completion, burnout, and one-coder paths have been checked with the macOS `leaks` tool.

## Thread synchronization mechanisms

### Coder threads

Each coder is represented by one `pthread_t`.

Coder threads run `coder_routine()` and repeatedly execute the compile, debug, and refactor cycle until:

* their required compile count is reached; or
* the shared simulation-end flag is set.

### Monitor thread

A separate monitor thread checks:

* whether every coder has completed;
* whether any coder has reached its burnout deadline.

The monitor does not replace coder threads. It only observes protected coder state and controls simulation termination.

### `pthread_mutex_t`

The project uses mutexes for separate classes of shared state.

#### Dongle mutexes

Each dongle has its own mutex.

It protects:

* `available`;
* `cooldown_until`;
* dongle ownership transitions.

Two dongles are locked in deterministic index order.

#### Scheduler mutex

`scheduler_mutex` protects the shared scheduler heap.

It serializes:

* request insertion;
* request ordering;
* scheduler passes;
* request removal;
* collection of scheduler grants.

#### Coder state mutexes

Each coder has a `state_mutex`.

It protects:

* `compiles_done`;
* `last_compile_start`;
* `granted`.

This allows the coder thread, scheduler, and monitor thread to safely access coder state.

#### Logging mutex

`log_mutex` protects all simulation output.

It ensures:

* complete lines are not mixed together;
* a compile grant is printed as one uninterrupted event;
* no status line is printed after burnout.

#### Simulation-end mutex

`end_mutex` protects `simulation_end`.

Coder threads, the scheduler, the monitor, sleeping phases, and logging functions use this mutex when reading or changing the end state.

### `pthread_cond_t`

Each dongle contains a condition variable.

Condition variables are initialized and destroyed with the dongle.

A broadcast occurs when dongles are released.

The current scheduler still rechecks eligibility through mutex-protected scheduler passes rather than relying exclusively on condition-variable waiting. The broadcast indicates that dongle state has changed, while the mutex-protected checks remain the authority for granting resources.

### Binary heap

The scheduler queue is implemented as a binary heap.

Heap operations include:

* insertion;
* peek;
* pop;
* removal at an arbitrary index;
* restoration upward or downward after removal.

The same heap implementation supports both scheduling modes. The selected policy changes the comparison function, not the queue structure.

## Scheduler behavior

### FIFO

FIFO gives priority to the earliest request arrival time.

The comparison order is:

1. lower `arrival_time`;
2. lower internal `coder_id` as a deterministic tie-breaker.

In simplified form:

```text
earlier arrival wins
```

### EDF

EDF gives priority to the earliest burnout deadline.

The comparison order is:

1. lower `deadline`;
2. lower `arrival_time`;
3. lower internal `coder_id` as a deterministic final tie-breaker.

In simplified form:

```text
earlier burnout deadline wins
```

A coder closer to burnout may therefore be granted a dongle pair before a coder whose request arrived earlier.

### Shared scheduling pass

The scheduler copies the current heap into an ordered temporary representation.

It then examines requests in scheduler priority order.

For each request:

* overlapping dongles already reserved in this pass cause the request to wait;
* unavailable dongles cause the request to wait;
* available but cooling dongles reserve their position but are not granted;
* available dongles whose cooldown has expired may be granted;
* non-overlapping ready requests can be granted during the same pass.

This permits safe concurrent compiling while retaining policy-based arbitration for conflicting resources.

## Testing

### Norm and compilation

```bash
norminette
make fclean
make
```

### Required evaluation benchmarks

Single-coder burnout:

```bash
./codexion 1 800 200 200 200 10 0 fifo
```

Expected: coder 1 burns out around `800 ms`.

Feasible FIFO:

```bash
./codexion 5 2000 200 200 200 10 0 fifo
```

Expected: all coders finish without burnout.

Feasible EDF:

```bash
./codexion 5 2000 200 200 200 7 0 edf
```

Expected: all coders finish without burnout.

Infeasible timing:

```bash
./codexion 5 500 200 200 200 10 0 fifo
```

Expected: one coder burns out around `500 ms`, and the burnout line is last.

Cooldown:

```bash
./codexion 5 3000 200 200 200 10 400 fifo
```

Expected: all coders finish without burnout and each dongle respects its cooldown.

FIFO contention:

```bash
./codexion 5 3000 200 200 200 10 800 fifo
```

EDF contention:

```bash
./codexion 5 3000 200 200 200 10 800 edf
```

The two contention runs are used to compare grant ordering under FIFO and EDF.

### Race checking

Example ThreadSanitizer build:

```bash
make fclean
make CFLAGS="-Wall -Wextra -Werror -pthread -fsanitize=thread -g"
./codexion 5 2000 200 200 200 3 0 fifo
```

Restore the normal build afterward:

```bash
make fclean
make
```

On Linux, appropriate alternatives include:

```bash
valgrind --tool=helgrind ./codexion 5 2000 200 200 200 3 0 fifo
```

or:

```bash
valgrind --tool=drd ./codexion 5 2000 200 200 200 3 0 fifo
```

### Leak checking

On macOS:

```bash
leaks --atExit -- ./codexion 5 2000 200 200 200 3 0 fifo
```

```bash
leaks --atExit -- ./codexion 5 500 200 200 200 10 0 fifo
```

```bash
leaks --atExit -- ./codexion 1 800 200 200 200 10 0 fifo
```

Expected:

```text
0 leaks for 0 total leaked bytes
```

On Linux:

```bash
valgrind --leak-check=full --show-leak-kinds=all \
./codexion 5 2000 200 200 200 3 0 fifo
```

## Project status

The project currently passes:

* Norminette;
* compilation with the required flags;
* the required single-coder burnout benchmark;
* the required feasible FIFO benchmark;
* the required feasible EDF benchmark;
* the required infeasible timing benchmark;
* the required cooldown benchmark;
* FIFO versus EDF contention comparison;
* ThreadSanitizer testing without a reported race;
* leak testing on normal completion;
* leak testing after burnout;
* leak testing for the one-coder case.

Before submission, the repository should be cleaned with:

```bash
make fclean
```

Only the clean, committed repository should be submitted and evaluated.
