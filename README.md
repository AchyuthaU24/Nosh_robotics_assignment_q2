# Producer-Consumer Sensor Data Buffer Simulation

## Overview
A C program simulating a periodic sensor data producer and a periodic
consumer reading from a shared buffer, using POSIX threads. Written to
run on online C compilers (tested target: Programiz).

## Behavior
- **Producer** runs every simulated 1 second, appends a random number
  (0–5) of random bytes to a shared, globally accessible buffer.
- **Consumer** runs every simulated 10 seconds. If the buffer holds
  ≥50 bytes, it prints the **latest 50 bytes** (in hex) and deletes
  them from the buffer. If fewer than 50 bytes are present, it does
  nothing that cycle.

## Assumptions
1. **Time compression**: Online compilers enforce short execution time
   limits, so real-time waiting (1s/10s) isn't feasible. 1 simulated
   second is scaled to 200ms real time (`TICK_MS`). The full demo
   (30 simulated seconds) completes in ~6 real seconds.
2. **"Latest 50 bytes"**: Interpreted as the 50 most recently added
   bytes. If the buffer has more than 50 bytes when checked (e.g. 65),
   only the newest 50 are printed and the entire buffer is cleared —
   older excess bytes are discarded, not carried to the next cycle.
   This follows the literal wording: "prints... latest 50 bytes...
   and deletes the printed bytes from the data structure."
3. **Buffer capacity**: Fixed-size array (`BUF_SIZE = 500`) as a
   practical cap; producer stops adding bytes if full. Not expected
   to be hit during the short demo window.
4. **Thread safety**: Producer and consumer run as independent
   `pthread` threads; a mutex (`pthread_mutex_t lock`) guards all
   reads/writes to the shared buffer and count to prevent race
   conditions.

## How to Run

**On Programiz (as specified in the assignment):**
1. Go to https://www.programiz.com/c-programming/online-compiler/
2. Paste the contents of `producer_consumer.c`
3. Click Run — completes in ~6 seconds

**Locally:**
```bash
gcc -pthread producer_consumer.c -o sim
./sim
```

## Sample Output
