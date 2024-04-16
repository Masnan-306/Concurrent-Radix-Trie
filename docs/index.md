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

- Lock-Free Mechanism Evaluation: How effective are lock-free mechanisms in reducing the overhead associated with synchronization in distributed systems, specifically in the context of trie operations? We will compare the performance of lock-free tries against traditional lock-based approaches to understand the trade-offs in complexity and performance.

- CUDA Acceleration Analysis: To what extent does CUDA acceleration improve the performance of bulk operations in the trie data structure? We will benchmark the CUDA-implemented trie against its CPU counterparts to quantify the performance gains in terms of throughput and latency.

- Load Balancing Effectiveness: How do different strategies for load balancing affect the performance and scalability of distributed tries? We will explore various techniques for partitioning the trie among different nodes and measure their impact on the system's overall efficiency.

- Memory Access Patterns and Optimization: How do trie data structures' memory access patterns influence performance, and what optimizations can mitigate any negative impacts? This analysis will include looking at spatial and temporal locality, as well as the effects of hardware cache utilization.

## PLATFORM CHOICE

We will implement the trie data structure in C++ and develop the benchmark scripts in Python. Experiments will be conducted on GHC machines. If time permnits, we will also experiment on PSC machines because it can easily scale from 1 core to 256 cores allowing for more comprehensive study into the performance of the data structure when scaling to larger systems. 

## PAST SCHEDULE

1. **Week of April 1**: Project Setup, Initial Research, and Design Planning
- Deeped dive into algroithm for Trie, Radix Trie, and PATRICIA(Practical Algorithm to Retrieve Information Coded in Alphanumeric) Trie.
- Discussed on parallel implementations of the Patricia trie: distributed version with locks and lock-free version.

2. **Week of April 8**: Sequential Implementation of Radix Trie and Patricia Trie
- Implemented the basic structure of sequential version of Radix trie.
- Developed test cases to evaluate the correctness of the trie.
- Further studied on Patricia Trie data structure and algorithm, and implemented the basic structure of sequential version of patricia trie.


## FUTURE SCHEDULE
1. **April 17 - April 21**: Patricia Trie and Time Test Cases
- Jake Wen: Determine suitable test cases for timing all versions of the trie, for later use on graph and report. The test cases should vary on word length, total word count, and word characteristic.
- Jake Wen: Run benchmark on the implemented sequetial version of the trie.
- Sam Wang: Continue working on sequential implementation of Patricia Trie.

2. **April 22 - April 25**: Coarse-grained and Fine-grained Lock Version of Distributed Radix Trie
- Jake Wen: Implement the fine-grained lock verion of distributed trie and benchmark
- Sam Wang: Implement the coarse-grained lock verion of distributed trie and benchmark

3. **April 26 - April 28**: Final Exam for Other Courses, No Schedule

4. **April 29 - May 1**: Lock-free version of Radix Trie
- Sam Wang: Implement the lock-free version of Radix Trie.
- Jake Wen: Benchmark and create speedup and other graphs.

5. **May 2 - May 5**: Documentation, Report Writing, and Poster Creation
- Jake Wen: Ensure all code is well-documented, with clear explanations of the parallel algorithms and optimizations used.
- Sam Wang and Jake Wen: Compile comprehensive analysis of the project’s development process, with a focus on the implementation and optimization of parallel algorithms. Include detailed benchmarking results and insights gained from the comparison of different trie implementations.
- Sam Wang and Jake Wen: Design a poster that highlights the project’s achievements in parallel computing, showcasing the benefits of the parallel trie structures, lock-free synchronization techniques, and CUDA acceleration.

## Milestone
Over the past two weeks, our team has concentrated on developing sequential and course-grained lock algorithms for the Radix Trie and Patricia Trie. While a plain parallel prefix trie can easily be implemented, a Paricia trie incorporates more involved mechanisms that convert string into bits and have circular references to save sapce and handle all unicode characters. Due to this complication, we struggled with implementing even the correct sequential Patricia trie. After several failed attempts to implement the patricia trie, we turned to first implement a Radix Trie which is a simplified version of Patricia trie. To make things easier, we decided to constrain our problem to solve within ASCII characters, which free ourselves from the bit conversions. While the Radix Trie met all correctness tests, we keep encountering issues with the Patricia Trie and are currently debugging these errors. This setback slows down our progress so that we did not finish implementing the fine-grained and lock-free variants in our original schedule.

As outlined in our future proposal, we aim to complete the Patricia Trie implementation within this week. Success in this endeavor will allow us to develop parallel algorithms for both trie types. Conversely, should complications arise, our efforts will pivot towards refining the Radix Trie, especially considering its structural complexity compared to the binary node limit in the Patricia Trie, which could pose challenges in parallel processing when using a large number of threads. One of our biggest concerns is that we couldn't find or generate a suitable problem size that helps us understand how much speedup we get from the distribute Radix tries.

Before our final presentation, our objectives include completing the coarse-grained lock, fine-grained lock, and lock-free versions of the Radix Trie. While parallel implementations of the Patricia Trie remain a secondary goal, our final report and presentation will feature speed-up, cache-miss, and storage efficiency graphs to illustrate our findings and optimizations.
