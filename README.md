# CPU-Cache-Simulator

# Cache Simulator

A configurable CPU cache simulator written in modern C++17. Simulates cache reads and writes against a trace file, supporting pluggable write policies, N-way set associativity, and LRU eviction — mirroring how real CPU caches behave.

---

## Features

- **N-way set associative cache** — configurable associativity (1 = direct mapped, 4 = 4-way, etc.)
- **Pluggable write policies** — swap between Write-Through and Write-Back at compile time
- **LRU eviction** — per-set Least Recently Used eviction policy
- **Dirty bit tracking** — Write-Back policy flushes dirty lines on eviction
- **Address decoding** — stateless tag / index / offset bit extraction from 32-bit addresses
- **Trace-driven simulation** — reads a memory access trace file and replays it against the cache
- **Hit/miss statistics** — per-operation and overall hit rates printed at the end

---

## Architecture

```mermaid
┌─────────────────────────────────────────────┐
│                   Cache                     │
│  - sets: vector<CacheSet>                   │
│  - WritePolicy*                             │
│  - AddressDecoder*                          │
│  - Loader*                                  │
└──────┬──────────┬──────────┬────────────────┘
       │          │          │
       ▼          ▼          ▼
 AddressDecoder  Loader  WritePolicy (interface)
 (stateless)      │       ├── WriteThroughPolicy
                  │       └── WriteBackPolicy
                  ▼
               Memory
             (uint8_t[])

CacheSet
  └── lines: vector<CacheLine>    ← N-way associativity
        └── EvictionPolicy (interface)
              └── LRUPolicy

classDiagram
    class Cache {
        +int32_t readHit
        +int32_t readMiss
        +int32_t writeHit
        +int32_t writeMiss
        +int32_t offsetBits
        +int32_t blockSize
        +int32_t indexBits
        +int32_t tagBits
        +AddressDecoder* decoder
        +Loader* loader
        +read() int8_t
        +write() void
        +printStats() void
    }

    class Decoder {
        +DecodeAddress(int32_t address, int32_t block_size, int32_t index_bit, int32_t offset_bits) AddressParts
    }

    class Memory {
        -vector~int32_t~ memory
        +read() int32_t
        +write() void
    }

    class Loader {
        +Memory& memory
        +LoadFromMemory(int32_t address, CacheLine& cacheline, int32_t blocksize, int32_t index_bit, int32_t offset_bit) void
    }

    Cache --> Decoder : uses
    Cache --> Loader  : uses
    Loader --> Memory : reads from
```

### Components

| Class | Responsibility |
|---|---|
| `Cache` | Top-level orchestrator — routes reads/writes, tracks stats |
| `CacheSet` | Holds N cache lines per set, manages hit detection and victim selection |
| `CacheLine` | Single cache line: valid bit, dirty bit, tag, and data bytes |
| `AddressDecoder` | Stateless: splits a 32-bit address into tag / index / offset |
| `Loader` | Loads a block from `Memory` into a `CacheLine` on a miss |
| `Memory` | Flat byte-addressable RAM, initialized with `i % 256` pattern |
| `WritePolicy` | Interface for write behaviour — `WriteThroughPolicy` or `WriteBackPolicy` |
| `EvictionPolicy` | Interface for eviction — currently `LRUPolicy` |

---

## Project Structure

\```
Cache_Simulator/
├── include/
│   ├── AddressParts.h
│   ├── cache.h
│   ├── CacheLine.h
│   ├── CacheSet.h
│   ├── Decoder.h
│   ├── EvictionPolicy.h
│   ├── Loader.h
│   ├── LRUPolicy.h
│   ├── memory.h
│   ├── WriteBackPolicy.h
│   ├── WritePolicy.h
│   └── WriteThroughPolicy.h
├── src/
│   ├── cache.cpp
│   ├── CacheSet.cpp
│   ├── Decoder.cpp
│   ├── Loader.cpp
│   ├── LRUPolicy.cpp
│   ├── main.cpp
│   ├── memory.cpp
│   └── policy/
│       ├── WriteBackPolicy.cpp
│       └── WriteThroughPolicy.cpp
├── tools/
│   └── generate_trace.cpp
├── traces/
│   └── trace.txt
└── CMakeLists.txt
\```

---

## Build Instructions

### Prerequisites

- CMake ≥ 3.15
- C++17 compiler (clang++ on macOS, g++ on Linux)

### Build

\```bash
mkdir build && cd build
cmake ..
cmake --build .
\```

> **Note:** After adding or removing source files, always re-run `cmake ..` before building.

### Debug build

\```bash
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
\```

---

## Running

### Step 1 — Generate a trace file

\```bash
./build/generate_trace
\```

This writes `traces/trace.txt` with 10,000 memory operations — 80% accesses from a hot region (`0x1000–0x10FF`), 20% from a cold region (`0x0000–0xFFFF`), and 30% writes.

### Step 2 — Run the simulator

\```bash
./build/cache_simulator
\```

Run from the project root so `traces/trace.txt` is found correctly.

### Sample output

\```
READ  0x10a3 → 163
READ  0x1042 → 66
WRITE 0x10f1 ← 200
...
=== Cache Stats ===
Read  Hits: 6821  Misses: 179  Hit Rate: 97.44%
Write Hits: 2871  Misses: 129  Hit Rate: 95.70%
Total Accesses: 10000
Overall Hit Rate: 96.92%
\```

---

## Configuration

All parameters are set in `src/main.cpp`:

\```cpp
Memory memory{1024 * 1024};       // 1MB backing memory

Cache cache(
    1024 * 4,    // cache size  — 4KB total
    64,          // block size  — 64 bytes per line
    4,           // associativity — 4-way set associative
    memory,
    &decoder,
    &loader,
    &writePolicy // swap WriteThroughPolicy ↔ WriteBackPolicy here
);
\```

### Address bit layout (with above settings)

\```
[ 31 ............. 12 | 11 ...... 6 | 5 .... 0 ]
        Tag (20 bits)   Index (6 bits) Offset (6 bits)

NumSets    = 4096 / (64 × 4) = 16
IndexBits  = log2(16) = 4
OffsetBits = log2(64) = 6
\```

---

## Write Policies

### Write-Through (default)
Every write updates both the cache line and backing memory immediately. Simple, consistent, higher memory bus traffic.

\```cpp
WriteThroughPolicy writePolicy;
\```

### Write-Back
Writes go only to the cache line, marked dirty. The dirty line is flushed to memory only when evicted. Lower memory traffic, more complex.

\```cpp
WriteBackPolicy writePolicy;
\```

---

## Trace File Format

\```
R 0x10a3        ← Read from address 0x10a3
W 0x10f1 200    ← Write value 200 to address 0x10f1
\```

Traces are plain text and can be written by hand or generated by `tools/generate_trace.cpp`.

---

## Extending

| What to add | Where |
|---|---|
| New eviction policy (LFU, Random) | Implement `EvictionPolicy` interface, swap in `CacheSet.cpp` |
| New write policy | Implement `WritePolicy` interface, pass to `Cache` constructor |
| Multi-level cache (L1/L2) | Wrap `Cache` as the `Memory` seen by an outer `Cache` |
| Prefetching | Extend `Loader::LoadFromMemory` to fetch adjacent blocks |

---

## Requirements

- C++17
- CMake ≥ 3.15
- Standard library only — no external dependencies
