# Performance Aware Programming

## The 5 multipliers of slow software

- Waste
    - The biggest multiple
    - Influenced greatly by programming language
        - A language like C has little to no waste
        - A language like Python has a large amount of waste, 100-200x
    - Performance is not so much about hardcore optimization, but more like coming to our senses and eliminating unnecessary waste
- Instructions per Clock (IPC)
    - The average number of instructions the CPU executes on every clock cycle
    - Based on the fact that most CPUs can execute more than one instruction a
    - Based on the fact that most CPUs can execute more than one instruction at a time
    - One way is to "unroll" a loop and ratio between the work (e.g. add) and overhead (e.g. loop cond, counter inc)
    - In the case of the rolling summation loop, unrolling the loop resulted in a serial dependency chain, so the CPU must do them in order
    - By breaking the one long serial dependency chain, we unlock the ILP capabilities of the chip and get a significant perf boost
- Single Instruction, Multiple Data (SIMD)
    - CPU design based on the observation that if you are doing the same operation on many pieces of data, 
      do them as one instruction to avoid the need to decode multiple instructions
    - It doesn't save any work in terms of the operation we are doing (ADD, for example), but it reduces the amount of strain on the CPU 
      to decode instructions and analyze the instruction stream for dependencies (which takes a lot of work!)
    - Multiple families of SIMD instruction sets:
        - SSE (128 bit registers), supported basically everywhere
        - AVX (256 bit registers), decent support
        - AVX512 (512 bit registers), poor support
    - Can be combined with IPC to stack data + instruction parallelism together
    - Performance multipler is not as powerful when caching is bad
- Caching
    - The idea of creating smaller but faster storage as part of the CPU core and thus closer to the ALU, to avoid always paying RAM access cost
    - The register file is the circuitry which feeds data into the ALU as fast as the ALU can take them
    - When a load occurs, the CPU must get the data into its register file, which means reaching out to L1 -> L2 -> L3 -> RAM until the data is found
    - Some caches are per core, some are shared across cores
    - Caching can make or break an algorithm - memory access is crucial to get right
- Multithreading
    - Workloads that are memory bound can be saved by multithreading because the division of work can result in data fitting into the smaller (and faster) caches
    - If a workload is not memory bound, the maximum speedup we can expect if Nx where N is the number of threads, 
      if a workload IS memory bound, we can expect much more than NX
    - By increasing the number of cores for an algorithm, we not only increase the number of instruction streams, 
      but also the amount of cache we can work with since the fast caches are per core
    - Some chips are designed so that a single core gets most of the potential memory bandwidth, so adding cores gives you little extra bandwidth,
      other chips are designed so that a single core gets a certain amount of memory bandwidth and adding cores linearly increases your memory bandwidth,
      so the amount of extra memory bandwidth acquired per core is heavily dependent on the chip

