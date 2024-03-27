# Distributed Trie Project Proposal 

*Implementations of Parallel Trie and Measurement of their Performance*  

**Team Members**: Sam Wang, Jake Wen

## URL

[Project Web Page URL](https://masnan-306.github.io/masnan.github.io)

[Source Code](https://github.com/Masnan-306/masnan.github.io)

## SUMMARY

In this project, we plan to implement versions of parallel trie data structure with different programming models including Cuda and OpenMPI as well as with the lock-free variant. Also, we plan to bechmark their performances by measuring throughput and scalability.

## BACKGROUND

The evolution of large-scale, data-intensive applications in domains such as distributed databases, real-time content retrieval, and networking has magnified the need for efficient data storage and retrieval systems. Tries, especially Patricia tries, have emerged as a favorable data structure for their ability to facilitate rapid searches, insertions, and deletions.

However, a significant challenge lies in the synchronization overhead when tries are adapted to distributed systems. Traditional lock-based synchronization mechanisms introduce bottlenecks that impair system scalability and degrade performance. This project proposes the research and implementation of a lock-free distributed trie (alternatively, a distributed Patricia trie), leveraging non-blocking synchronization methods to mitigate the issues associated with locks when exploiting distributed computing.

The key component of the application is the concurrent operations on the trie data structure, which includes node insertion, deletion, and search operations that correspond to database update, remove, and query functions. The structure of the Patricia trie itself inherently benefits from parallelism—updates to different parts of the trie can occur independently without interference if no common prefixes are involved. This will be exploited in our distributing approach that partitions the trie based on distinct key values and communicates them among different work node with OpenMPI.
`
The proposed lock-free approach will likely benefit from parallelism in several aspects:
1. Node-level Operations: Individual nodes within the trie can be modified concurrently without the need for locking the entire structure, thus enabling high parallelism for write-heavy workloads.
2. Read-Write Separation: Read operations, which are typically more frequent, can be performed concurrently with write operations.
3. Horizontal Scalability: By distributing the trie across multiple nodes, we can horizontally scale the system. Each node operates independently, which makes adding more nodes to the system a way to linearly increase the capacity and throughput.

Lastly, we plan to explore CUDA to accelerates bulk lookups and inserts conducted on a trie data structure. leveraging CUDA to implement trie data structures presents a transformative opportunity for accelerating bulk operations. This parallelism is particularly beneficial for applications involving large-scale data, like big data analytics, where millions of keys may need to be inserted or looked up in the trie concurrently.

## THE CHALLENGE

The main challenges in this project includes:

1) We need to correctly implement the locks on the data structure such that the critical sections are  protected. Otherwise, concurrent lookups or insertions may lead to a corrupted data structure.
2) Lock-free trie is challenging to implement because we would need to understand the underlying machine instructions and apply complex techniques to ensure atomicity.
3) Achieving a linear speedup is hard because the trie does not provide good spacial locality for memory accessing. We would also need to do research to form a good strategy of diving the workload among processes. A trie can usually be skewed in a application setting, demanding careful load balance techniques to scale.
4) Since the data structure can evolve dynamically over updates including insertions and deletions, we need to implement rebalancing mechanisms to distribute work among processes.

## RESOURCES

1. [Lock-free Radix Trie](https://github.com/purehyperbole/rad)
2. [Lock-free Insertions](https://stackoverflow.com/questions/68420953/trie-structure-lock-free-inserting)
3. [Distrbuted Trie](https://arxiv.org/abs/1809.04923)
4. [Cuda Acceleration](https://developer.nvidia.com/blog/cuda-turing-new-gpu-compute-possibilities/)

## GOALS AND DELIVERABLES

**Plan to Achieve**:

We expect to successfully implement the following:
  1. sequential patricia trie & large workload test cases
  2. distributed patricia trie with locks
  3. lock-free patricia trie
  4. lock-free and distributed trie with partitions among nodes

**Hope to Achieve**:

We also hope to achieve:
  1. linear speedup on look-up intensive workloads
  2. efficient implementations of cuda functions that accelerate bulk insertions

**Demo**:

We plan to demonstrate our outcomes in speedup graphs and also graphs on other metrics that shed light into the improvement of scalability and capacity of data the system can hold.

**Analysis Projects**:
- Performance Benchmarking: How does each implementation of the trie (sequential, lock-based distributed, lock-free, and lock-free distributed) perform under various workloads? This includes measuring throughput, latency, and scalability under high contention scenarios and heavy read or write workloads.

- Lock-Free Mechanism Evaluation: How effective are lock-free mechanisms in reducing the overhead associated with synchronization in distributed systems, specifically in the context of trie operations? We will compare the performance of lock-free tries against traditional lock-based approaches to understand the trade-offs in complexity and performance.

- CUDA Acceleration Analysis: To what extent does CUDA acceleration improve the performance of bulk operations in the trie data structure? We will benchmark the CUDA-implemented trie against its CPU counterparts to quantify the performance gains in terms of throughput and latency.

- Load Balancing Effectiveness: How do different strategies for load balancing affect the performance and scalability of distributed tries? We will explore various techniques for partitioning the trie among different nodes and measure their impact on the system's overall efficiency.

- Memory Access Patterns and Optimization: How do trie data structures' memory access patterns influence performance, and what optimizations can mitigate any negative impacts? This analysis will include looking at spatial and temporal locality, as well as the effects of hardware cache utilization.

**System Projects**:

- Sequential Patricia Trie Implementation: Develop a baseline sequential version of the Patricia trie for benchmark comparison. Performance targets include optimizing for speed and memory efficiency in a single-threaded context.
- Distributed Patricia Trie with Locks: Implement a distributed version of the Patricia trie using traditional lock-based synchronization methods. The performance targets here are to achieve scalable throughput with minimal contention overhead.
- Lock-Free Patricia Trie: Design and implement a lock-free version of the Patricia trie, focusing on reducing synchronization overhead and avoiding contention hotspots. The performance goal is to surpass the lock-based version in terms of throughput and scalability under contention.
- Lock-Free and Distributed Trie with Partitions Among Nodes: Create a distributed, lock-free Patricia trie that partitions the trie structure among multiple nodes to exploit parallelism. The primary performance targets are to achieve near-linear scalability with increasing nodes and to maintain high throughput under mixed workloads.

## PLATFORM CHOICE

We will implement the trie data structure in C++ and develop the benchmark scripts in Python. Experiments will be conducted on GHC machines. If time permnits, we will also experiment on PSC machines because it can easily scale from 1 core to 256 cores allowing for more comprehensive study into the performance of the data structure when scaling to larger systems. 

## SCHEDULE

1. **Week of April 1**: Project Setup, Initial Research, and Design Planning
- Deep dive into parallel algorithms literature, with a focus on parallel trie structures, lock-free synchronization, and CUDA acceleration techniques.
- Draft comprehensive design plans for parallel implementations of the Patricia trie: distributed version with locks, lock-free version, and an outline for CUDA-accelerated operations.

2. **Week of April 8**: Parallel Implementation of Distributed Trie with Locks
- Implement the basic structure of the distributed Patricia trie with a focus on parallel operations using lock-based synchronization. Utilize OpenMPI for managing communication between distributed nodes.
- Develop parallel test cases to evaluate the functionality and preliminary performance of the distributed trie.
- Begin benchmarking to understand scalability and identify optimization points.

3. **Week of April 15**: Development of Lock-Free Trie for Parallel Environments
- Start the implementation of the lock-free version of the Patricia trie, emphasizing non-blocking synchronization mechanisms suitable for parallel computing environments.
- Continue the development of parallel test and benchmark suites specifically designed to measure the performance impact of eliminating locks.
- Assess initial performance data from the distributed trie implementation to refine and adjust development strategies.

4. **Week of April 22**: Introduction of CUDA Acceleration and Advanced Parallel Techniques
- Explore CUDA acceleration for key trie operations, especially focusing on bulk insertions and lookups that can benefit from GPU parallelism.
- Finalize and benchmark the lock-free implementation, including any necessary refinements based on performance analysis.
- Start integrating CUDA-accelerated operations into the trie implementations where applicable.

5. **Week of April 29**: Documentation, Report Writing, and Poster Creation
- Documentation: Ensure all code is well-documented, with clear explanations of the parallel algorithms and optimizations used.
- Report Writing: Compile comprehensive analysis of the project’s development process, with a focus on the implementation and optimization of parallel algorithms. Include detailed benchmarking results and insights gained from the comparison of different trie implementations.
- Poster Creation: Design a poster that highlights the project’s achievements in parallel computing, showcasing the benefits of the parallel trie structures, lock-free synchronization techniques, and CUDA acceleration.

