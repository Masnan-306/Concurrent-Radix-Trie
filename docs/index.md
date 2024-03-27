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
- (State the questions you aim to answer through your analysis.)

**System Projects**:
- (Describe the capabilities and performance targets of the system you're developing.)

## PLATFORM CHOICE

We chose (describe the platform, such as "NVIDIA GPUs and CUDA programming") because (explain why this platform is suitable for your project, considering the specific requirements and characteristics of your workload).

## SCHEDULE

(Provide a week-by-week schedule of your project tasks leading up to the parallelism competition. Include at least one task per week and note how you will adapt your schedule based on progress. Highlight key milestones.)

- **Week 1**: Task 1
- **Week 2**: Task 2
- ...
- **Intermediate Milestone (April 16th)**: (Describe the milestone.)

