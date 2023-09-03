# **Operating System Simulator**
C++ Implementation of Operating System Simulator.  

Utilizes concepts of CPU Scheduling, Semaphores, Deadlocks, Memory Management and Page Replacement.

 ##  **1. CPU Scheduling**
Implements the following CPU Scheduling algorithms
### - Multilevel Queue Scheduling
Implements 3 queues; each queue will utilize a different scheduling
algorithm.  

1^st^ queue will implement priority scheduling, 2^nd^ will implement Round Robin and 3^rd^ queue will implement Shortest Remaining Job First.  
  

Starvation and aging cases are handled.

### - Multilevel Feedback Queue Scheduling 
Implements 3 queues; each queue will utilize a different scheduling
algorithm.  

1^st^ queue will implement First Come First Serve, 2^nd^ will implement Shortest Job First and 3^rd^ queue will implement Round Robin with Shortest Remaining Job First.  
  

Starvation and aging cases are handled.

### - Longest Job First 
Process with longest burst time to be serviced first by the CPU. This is a non-preemptive algorithm.
### - Lowest Feedback Ratio Next Scheduling
This is a non-preemptive algorithm in which the scheduling is done on the basis of an extra parameter called Feedback Ratio.  
A Feedback Ratio is calculated for each of the available jobs and the Job with the lowest feedback ratio is given priority over the others.
Feedback Ratio is calculated by the given formula:  
 **Feedback Ratio = (W+S)/S**.  
Where: W is Waiting Time, S is Service Time or Burst Time.

 ##  **2. Semaphores**
Implements The Dining Philosopher Problem.  

In this problem, a group of philosophers sit at a round table and alternate between thinking and eating. However, there are only a limited number of chopsticks available for the philosophers to use as utensils. To eat, a philosopher must pick up both chopsticks adjacent to them. If a philosopher cannot pick up both chopsticks, they must wait until both are available. 
 
Semaphores are used to ensure that each philosopher is able to eat without creating a deadlock or starvation scenario.


 ##  **3. Deadlock**
Implements The Banker Algorithm.  

Banker’s algorithm used to avoid deadlock and allocate resources safely to each process in the computer system. The 'S-State' examines all possible tests or activities before deciding whether the allocation should be allowed to each process. It also helps the operating system to successfully share the resources between all the processes. The banker's algorithm is
named because it checks whether a person should be sanctioned a loan amount or not to help the bank system safely simulate allocation resources. 
 

Suppose the number of account holders in a particular bank is 'n', and the total money in a bank is ‘T’. If an account holder applies for a loan; first, the bank subtracts the loan amount from full cash and then estimates the cash difference is greater than T to approve the loan
amount. These steps are taken because if another person applies for a loan or withdraws some amount from the bank, it helps the bank manage and operate all things without any restriction in the functionality of the banking system.  


Similarly, it works in an operating system, when a new process is created in a computer system, the process must provide all types of information to the operating system like upcoming processes, requests for their resources, counting them, and delays. Based on these, the operating system decides which process sequence should be executed or waited so that no deadlock occurs in a system. Therefore, it is also known as deadlock avoidance algorithm or deadlock detection in the operating system.  


When working with a banker's algorithm, it requests to know about three things:  
- How much each process can request for each resource in the system.  
- How much each process is currently holding each resource in a system.  
- How much of each resource is currently available in the system.  


Following are the important data structures terms applied in the banker's algorithm as follows:  
- **Safety Algorithm**  
It is a safety algorithm used to check whether or not a system is in a safe state or follows the safe sequence in a banker's algorithm.  
- **Resource Request Algorithm**  
A resource request algorithm checks how a system will behave when a process makes each type of resource request in a system as a request matrix.

##  **4. Memory Management**
Implements following two concepts:  
- Two-Level Page-Table Scheme  
- Hashed Page Table with TLB

 ##  **5. Page Replacement**
Implements Second Chance Algorithm using circular linked list.  

Also calculates Number of page faults, page fault probability and page fault percentage. 
