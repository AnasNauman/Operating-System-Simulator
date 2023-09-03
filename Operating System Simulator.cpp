#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <iomanip>
#include <cmath>
using namespace std; 
int getInput(string str)
{
	int val;
	do {
		cout << "Enter Your " << str << ": ";
		cin >> val;
		if (val >= 0)
		{
			break;
		}
		else
		{
			cout << "\t INVALID INPUT" << endl;
		}
	} while (1);
	return val;
}
float getTime (string str)
{
	float val;
	do {
		cout << "Enter " << str << ": ";
		cin >> val;
		if (val >= 0)
		{
			break;
		}
		else
		{
			cout << "\t INVALID INPUT" << endl;
		}
	} while (1);
	return val;
}
int getRandomDigit(int digits)
{
	int var = 0;
	for (int j = 0; j < digits; j++)
	{
		int randDigit = rand() % 9 + 1;
		var = (var * 10) + randDigit;
	}
	return var;
}
void entryOrExit(int val)
{
	if (val == 1)
	{
		cout << endl << endl << "\t\t\tPress Any Key To Start...";
		char ch = _getch();
		system("CLS");
		cout << endl << endl << endl << endl;
	}
	else if (val == 2)
	{
		cout << endl << endl << "\t\t\tPress Any Key To Exit...";
		char ch = _getch();
		system("CLS");
	}
}
int getDigitsCount(int value)
{
	int count = 0;
	while (value > 0)
	{
		count++;
		value /= 10;
	}
	return count;
}
/////////////
class Process
{
private:
	int   process_ID;
	float arrival_Time; // TIME IN SECONDS
	float burst_Time;   // TIME IN SECONDS
public:

	Process();
	void setValues(int,float,float);
	int getProcessID();
	float getBurstTime();
	float getArrivalTime();
};
Process::Process()
{
	process_ID = 0;
	arrival_Time = 0;
	burst_Time = 0;
}
void Process::setValues(int pid,float arrivalTime,float burstTime)
{
	process_ID = pid;
	arrival_Time = arrivalTime;
	burst_Time = burstTime;
}
int Process::getProcessID()
{
	return process_ID;
}
float Process::getBurstTime()
{
	return burst_Time;
}
float Process::getArrivalTime()
{
	return arrival_Time;
}

struct Node_X
{
	Process process;
	int priority;  // LOWER THE NUMBER; HIGHER THE PRIORITY
	Node_X();
};
Node_X::Node_X()
{
	priority = INT_MAX;
}

struct Node_Y
{
	Process process;
	int remainingBurstTime;
	Node_Y();
};
Node_Y::Node_Y()
{
	remainingBurstTime = INT_MAX;
}

struct Node_Z
{
	Process process;
	int remainingBurstTime;
	Node_Z();
};
Node_Z::Node_Z()
{
	remainingBurstTime = INT_MAX;
}

class MultilevelQueue
{
private:
	int roundRobinTimeQuantum;
	queue<Node_X> priorityScheduling;
	queue<Node_Y> roundRobin;
	queue<Node_Z> shortestRemainingTimeFirst;
	bool checkWaitingQueue_PS(float, Node_X* , int&);
	bool checkWaitingQueue_RR(float, queue<Node_Y>&);
	bool checkWaitingQueue_SRTF(float, Node_Z*, int&);
	void stableSortOnPriority(Node_X*, int,int);
	void stableSortOnShortestRemainingTime(Node_Z*, int, int);
	void stableSortOnArrivalTime_PS();
	void stableSortOnArrivalTime_RR();
	void stableSortOnArrivalTime_SRTF();
public:
	void setRoundRobinTimeQuantum(int);
	void takeInput(int,int,int);
	void run();
};
bool MultilevelQueue::checkWaitingQueue_PS(float currTime, Node_X * ReadyQueue, int& endIndex)
{
	bool updated = false;
	while (!priorityScheduling.empty())
	{
		Node_X temp = priorityScheduling.front();
		if (temp.process.getArrivalTime() <= currTime)
		{
			updated = true;
			ReadyQueue[endIndex++] = temp;
			priorityScheduling.pop();
		}
		else
		{
			break;
		}
	}
	return updated;
}
bool MultilevelQueue::checkWaitingQueue_RR(float currTime, queue<Node_Y>& ReadyQueue)
{
	bool updated = false;
	while (!roundRobin.empty())
	{
		Node_Y temp = roundRobin.front();
		if (temp.process.getArrivalTime() <= currTime)
		{
			updated = true;
			ReadyQueue.push(temp);
			roundRobin.pop();
		}
		else
		{
			break;
		}
	}
	return updated;
}
bool MultilevelQueue::checkWaitingQueue_SRTF(float currTime, Node_Z * ReadyQueue, int& endIndex)
{
	bool updated = false;
	while (!shortestRemainingTimeFirst.empty())
	{
		Node_Z temp = shortestRemainingTimeFirst.front();
		if (temp.process.getArrivalTime() <= currTime)
		{
			updated = true;
			ReadyQueue[endIndex++] = temp;
			shortestRemainingTimeFirst.pop();
		}
		else
		{
			break;
		}
	}
	return updated;
}
void MultilevelQueue::stableSortOnPriority(Node_X* ReadyQueue, int startIndex, int endIndex)
{
	for (int i = startIndex; i < endIndex; i++)
	{
		for (int j = i + 1; j < endIndex; j++)
		{
			if (ReadyQueue[i].priority > ReadyQueue[j].priority)
			{
				Node_X temp = ReadyQueue[i];
				ReadyQueue[i] = ReadyQueue[j];
				ReadyQueue[j] = temp;
			}
			else if (ReadyQueue[i].priority == ReadyQueue[j].priority && ReadyQueue[i].process.getProcessID()> ReadyQueue[j].process.getProcessID())
			{
				Node_X temp = ReadyQueue[i];
				ReadyQueue[i] = ReadyQueue[j];
				ReadyQueue[j] = temp;
			}
		}
	}
}
void MultilevelQueue::stableSortOnShortestRemainingTime(Node_Z* ReadyQueue, int startIndex, int endIndex)
{
	for (int i = startIndex; i < endIndex; i++)
	{
		for (int j = i + 1; j < endIndex; j++)
		{
			if (ReadyQueue[i].remainingBurstTime > ReadyQueue[j].remainingBurstTime)
			{
				Node_Z temp = ReadyQueue[i];
				ReadyQueue[i] = ReadyQueue[j];
				ReadyQueue[j] = temp;
			}
		}
	}
}
void MultilevelQueue::stableSortOnArrivalTime_PS()
{
	vector<Node_X> vector;
	while (!priorityScheduling.empty())
	{
		vector.push_back(priorityScheduling.front());
		priorityScheduling.pop();
	}

	for (int i = 0; i < vector.size(); i++)
	{
		for (int j = i + 1; j < vector.size(); j++)
		{
			if (vector[i].process.getArrivalTime() > vector[j].process.getArrivalTime())
			{
				Node_X temp = vector[i];
				vector[i] = vector[j];
				vector[j] = temp;
			}
		}
	}

	for (int i = 0; i < vector.size(); i++)
	{
		priorityScheduling.push(vector[i]);
	}

	/*cout << "\n\n\tprioritysched:   ";
	while (!priorityScheduling.empty())
	{
		Node_X t=priorityScheduling.front();
		cout << t.process.getArrivalTime() << " ";
		priorityScheduling.pop();
	}*/
}
void MultilevelQueue::stableSortOnArrivalTime_RR()
{
	vector<Node_Y> vector;
	while (!roundRobin.empty())
	{
		vector.push_back(roundRobin.front());
		roundRobin.pop();
	}

	for (int i = 0; i < vector.size(); i++)
	{
		for (int j = i + 1; j < vector.size(); j++)
		{
			if (vector[i].process.getArrivalTime() > vector[j].process.getArrivalTime())
			{
				Node_Y temp = vector[i];
				vector[i] = vector[j];
				vector[j] = temp;
			}
		}
	}

	for (int i = 0; i < vector.size(); i++)
	{
		roundRobin.push(vector[i]);
	}

	/*cout << "\n\n\t round robin:   ";
	while (!roundRobin.empty())
	{
		Node_Y t = roundRobin.front();
		cout << t.process.getArrivalTime() << " ";
		roundRobin.pop();
	}*/
}
void MultilevelQueue::stableSortOnArrivalTime_SRTF()
{
	vector<Node_Z> vector;
	while (!shortestRemainingTimeFirst.empty())
	{
		vector.push_back(shortestRemainingTimeFirst.front());
		shortestRemainingTimeFirst.pop();
	}

	for (int i = 0; i < vector.size(); i++)
	{
		for (int j = i + 1; j < vector.size(); j++)
		{
			if (vector[i].process.getArrivalTime() > vector[j].process.getArrivalTime())
			{
				Node_Z temp = vector[i];
				vector[i] = vector[j];
				vector[j] = temp;
			}
		}
	}

	for (int i = 0; i < vector.size(); i++)
	{
		shortestRemainingTimeFirst.push(vector[i]);
	}


	//cout << "\n\n\t SRTF:   ";
	//while (!shortestRemainingTimeFirst.empty())
	//{
	//	Node_Z t = shortestRemainingTimeFirst.front();
	//	cout << t.process.getArrivalTime() << " ";
	//	shortestRemainingTimeFirst.pop();
	//}
}
void MultilevelQueue::setRoundRobinTimeQuantum(int val)
{
	if (val == 0)
	{
		val = 2;
	}
	roundRobinTimeQuantum = val;
}
void MultilevelQueue::takeInput(int processNum1, int processNum2, int processNum3)
{
	cout << endl << endl<<"\t\t\tPRIORITY SCHEDULING INPUT"<<endl<<endl;
	for (int i = 0; i < processNum1; i++)
	{
		Node_X node;
		int process_ID = getRandomDigit(3);
		float arrival_time = getTime("Arrival Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		float burst_time = getTime("  Burst Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		int priority = getInput(" Priority For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		cout << endl << endl;
		node.process.setValues(process_ID, arrival_time, burst_time);
		node.priority = priority;
		priorityScheduling.push(node);
	}

	cout << endl << endl << "\t\t\tROUND ROBIN INPUT" << endl << endl;
	for (int i = 0; i < processNum2; i++)
	{
		Node_Y node;
		int process_ID = getRandomDigit(3);
		float arrival_time = getTime("Arrival Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		float burst_time = getTime("  Burst Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		cout << endl << endl;
		node.process.setValues(process_ID, arrival_time, burst_time);
		node.remainingBurstTime = burst_time;
		roundRobin.push(node);
	}

	cout << endl << endl << "\t\t\tSHORTEST REMAINING TIME FIRST INPUT" << endl << endl;
	for (int i = 0; i < processNum3; i++)
	{
		Node_Z node;
		int process_ID = getRandomDigit(3);
		float arrival_time = getTime("Arrival Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		float burst_time = getTime("  Burst Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		cout << endl << endl;
		node.process.setValues(process_ID, arrival_time, burst_time);
		node.remainingBurstTime = burst_time;
		shortestRemainingTimeFirst.push(node);
	}
}
void MultilevelQueue::run()
{
	entryOrExit(1);
	stableSortOnArrivalTime_PS();
	stableSortOnArrivalTime_RR();
	stableSortOnArrivalTime_SRTF();
	float currTime = 0;
	int processIncomplete = priorityScheduling.size() + roundRobin.size() + shortestRemainingTimeFirst.size();
	Node_X* ReadyQueue_PS = new Node_X[priorityScheduling.size()];
	queue<Node_Y> ReadyQueue_RR;
	Node_Z* ReadyQueue_SRTF = new Node_Z[shortestRemainingTimeFirst.size()];
	int startIndex_PS   = 0, endIndex_PS   = 0;
	int startIndex_SRTF = 0, endIndex_SRTF = 0;
	while (processIncomplete > 0)
	{
		checkWaitingQueue_PS(currTime, ReadyQueue_PS, endIndex_PS);
		checkWaitingQueue_RR(currTime, ReadyQueue_RR);
		checkWaitingQueue_SRTF(currTime, ReadyQueue_SRTF, endIndex_SRTF);

		stableSortOnPriority(ReadyQueue_PS, startIndex_PS,endIndex_PS);
		stableSortOnShortestRemainingTime(ReadyQueue_SRTF, startIndex_SRTF,endIndex_SRTF);
		
		if (startIndex_PS!= endIndex_PS)
		{
			cout << "\n\tStarting Priority Scheduling ...";
			Node_X temp = ReadyQueue_PS[startIndex_PS++];
			cout << "\n\t\tCPU Currently Executing Process PID:" << temp.process.getProcessID() << " With Burst Time: " << temp.process.getBurstTime() << endl << endl;
			Sleep(temp.process.getBurstTime() * 1000);
			currTime += temp.process.getBurstTime();
			processIncomplete--;
			cout << "\n\t\tProcess PID:" << temp.process.getProcessID() << " Has Finished Execution! " << endl << endl;
		}
		else if (!ReadyQueue_RR.empty())
		{
			cout << "\n\tStarting Round Robin ...";
			bool isQuantaComplete = true, isProcessComplete = false;
			Node_Y temp = ReadyQueue_RR.front();
			ReadyQueue_RR.pop();
			for (int i = 1; i <= roundRobinTimeQuantum; i++)
			{
				if (temp.remainingBurstTime > 1)
				{
					cout << "\n\t\tCPU Currently Executing Process PID:" << temp.process.getProcessID() << " With Remaining Burst Time: " << temp.remainingBurstTime  << endl;
					Sleep(1 * 1000);
					currTime++;
					checkWaitingQueue_RR(currTime, ReadyQueue_RR);
					temp.remainingBurstTime--;
				}
				else
				{
					cout << "\n\t\tCPU Currently Executing Process PID:" << temp.process.getProcessID() << " With Remaining Burst Time: " << temp.remainingBurstTime << endl;
					Sleep(temp.remainingBurstTime * 1000);
					currTime+= temp.remainingBurstTime;
					checkWaitingQueue_RR(currTime, ReadyQueue_RR);
					temp.remainingBurstTime = 0;
					isProcessComplete = true;
					processIncomplete--;
					cout << "\n\t\tProcess PID:" << temp.process.getProcessID() << " Has Finished Execution! " << endl << endl;
					break;
				}
				if (checkWaitingQueue_PS(currTime, ReadyQueue_PS, endIndex_PS))
				{
					cout << "\n\tPre-empting Round Robin. Priority Process Arrived In Priority Scheduling Queue" << endl;
					cout << "\t\t\tContext Switching ..." << endl << endl;
					ReadyQueue_RR.push(temp);
					isQuantaComplete = false;  // SET FALSE AS ONE QUANTA SLICE NOT FULFILLED
					break;
				}
			}
			if (isQuantaComplete)
			{
				if (!isProcessComplete)
				{
					cout << "\n\t\tProcess PID:" << temp.process.getProcessID() << " Has Completed Its Time Quantum " << endl << endl;
					ReadyQueue_RR.push(temp);
				}
			}
		}
		else if (startIndex_SRTF != endIndex_SRTF)
		{
			cout << "\n\tStarting Shortest Remaining Time First ...";
			bool isProcessComplete = false;
			cout << "\n\t\tCPU Currently Executing Process PID:" << ReadyQueue_SRTF[startIndex_SRTF].process.getProcessID() << " With Remaining Burst Time: " << ReadyQueue_SRTF[startIndex_SRTF].remainingBurstTime << endl;
			while( true)
			{
				if (ReadyQueue_SRTF[startIndex_SRTF].remainingBurstTime>1)
				{
					Sleep(1 * 1000);
					currTime++;
					ReadyQueue_SRTF[startIndex_SRTF].remainingBurstTime--;
				}
				else
				{
					Sleep(ReadyQueue_SRTF[startIndex_SRTF].remainingBurstTime * 1000);
					currTime += ReadyQueue_SRTF[startIndex_SRTF].remainingBurstTime;
					ReadyQueue_SRTF[startIndex_SRTF].remainingBurstTime = 0;
					processIncomplete--;
					isProcessComplete = true;
					cout << "\n\t\tProcess PID:" << ReadyQueue_SRTF[startIndex_SRTF].process.getProcessID() << " Has Finished Execution! " << endl << endl;
					break;
				}
				if (checkWaitingQueue_PS(currTime, ReadyQueue_PS, endIndex_PS))
				{
					cout << "\n\tPre-empting SRTF. Priority Process Arrived In Priority Scheduling Queue" << endl;
					cout << "\t\t\tContext Switching ..." << endl << endl;
					break;
				}
				if (checkWaitingQueue_RR(currTime, ReadyQueue_RR))
				{
					cout << "\n\tPre-empting SRTF. Priority Process Arrived In Round Robin Queue" << endl;
					cout << "\t\t\tContext Switching ..." << endl << endl;
					break;
				}
				if (checkWaitingQueue_SRTF(currTime, ReadyQueue_SRTF, endIndex_SRTF)&& ReadyQueue_SRTF[endIndex_SRTF].process.getBurstTime()< ReadyQueue_SRTF[startIndex_SRTF].remainingBurstTime)
				{
					cout << "\n\tPre-empting Current Execution. Shorter Timed Process Arrived In SRTF Queue" << endl;
					cout << "\t\t\tContext Switching ..." << endl << endl;
					break;
				}
			}

			if(isProcessComplete)
			{
				startIndex_SRTF++;
			}
		}
		else
		{
			cout << "\t\tReady Queues Empty. No Currently Executing Process In CPU ..." << endl;
			Sleep(1 * 1000);
			currTime++;
		}		
	}
	entryOrExit(2);
	delete [] ReadyQueue_PS;
	delete [] ReadyQueue_SRTF;
}

struct Node_L
{
	Process process;
	int remainingBurstTime;
	Node_L();
};
Node_L::Node_L()
{
	remainingBurstTime = INT_MAX;
}

class MultilevelFeedbackQueue
{
private:
	int roundRobinTimeQuantum;
	int shortestJobFirstQueueTimeQuantum;
	queue<Node_L> firstComeFirstServe ;
	queue<Node_L> shortestJobFirst;
	queue<Node_L> RoundRobin_SRTF;
	bool checkWaitingQueue_FCFS(float, queue<Node_L>&);
	bool checkWaitingQueue_SJF(float, queue<Node_L>&);
	bool checkWaitingQueue_RR_SRTF(float, queue<Node_L>&);
	void stableSortOnRemainingBurstTime(queue<Node_L>&);
	void stableSortOnArrivalTime(queue<Node_L>&);
	void checkAging_SJF(float,queue<Node_L>&, queue<Node_L>&);
	void checkAging_RR_SRTF(float,queue<Node_L>&, queue<Node_L>&);
public:
	void setRoundRobinTimeQuantum(int);
	void setShortestJobFirstQueueTimeQuantum(int);
	void takeInput(int, int, int);
	void run();
};
bool MultilevelFeedbackQueue::checkWaitingQueue_FCFS(float currTime, queue<Node_L>& ReadyQueue)
{
	bool updated = false;
	while (!firstComeFirstServe.empty())
	{
		Node_L temp = firstComeFirstServe.front();
		if (temp.process.getArrivalTime() <= currTime)
		{
			updated = true;
			ReadyQueue.push(temp);
			firstComeFirstServe.pop();
		}
		else
		{
			break;
		}
	}
	return updated;
}
bool MultilevelFeedbackQueue::checkWaitingQueue_SJF(float currTime, queue<Node_L>& ReadyQueue)
{
	bool updated = false;
	while (!shortestJobFirst.empty())
	{
		Node_L temp = shortestJobFirst.front();
		if (temp.process.getArrivalTime() <= currTime)
		{
			updated = true;
			ReadyQueue.push(temp);
			shortestJobFirst.pop();
		}
		else
		{
			break;
		}
	}
	return updated;
}
bool MultilevelFeedbackQueue::checkWaitingQueue_RR_SRTF(float currTime, queue<Node_L>& ReadyQueue)
{
	bool updated = false;
	while (!RoundRobin_SRTF.empty())
	{
		Node_L temp = RoundRobin_SRTF.front();
		if (temp.process.getArrivalTime() <= currTime)
		{
			updated = true;
			ReadyQueue.push(temp);
			RoundRobin_SRTF.pop();
		}
		else
		{
			break;
		}
	}
	return updated;
}
void MultilevelFeedbackQueue::stableSortOnRemainingBurstTime(queue<Node_L>& queue)
{
	vector<Node_L> vector;
	while (!queue.empty())
	{
		vector.push_back(queue.front());
		queue.pop();
	}

	for (int i = 0; i < vector.size(); i++)
	{
		for (int j = i + 1; j < vector.size(); j++)
		{
			if (vector[i].remainingBurstTime > vector[j].remainingBurstTime)
			{
				Node_L temp = vector[i];
				vector[i] = vector[j];
				vector[j] = temp;
			}
		}
	}

	for (int i = 0; i < vector.size(); i++)
	{
		queue.push(vector[i]);
	}
}
void MultilevelFeedbackQueue::stableSortOnArrivalTime(queue<Node_L>& queue)
{
	vector<Node_L> vector;
	while (!queue.empty())
	{
		vector.push_back(queue.front());
		queue.pop();
	}

	for (int i = 0; i < vector.size(); i++)
	{
		for (int j = i + 1; j < vector.size(); j++)
		{
			if (vector[i].process.getArrivalTime() > vector[j].process.getArrivalTime())
			{
				Node_L temp = vector[i];
				vector[i] = vector[j];
				vector[j] = temp;
			}
		}
	}

	for (int i = 0; i < vector.size(); i++)
	{
		queue.push(vector[i]);
	}

	/*cout << "\n\n\t queue:   ";
	while (!queue.empty())
	{
		Node_L t = queue.front();
		cout << t.process.getArrivalTime() << " ";
		queue.pop();
	}*/
}
void MultilevelFeedbackQueue::checkAging_SJF(float currTime,queue<Node_L>& SJFqueue, queue<Node_L>& FCFSqueue)
{
	vector<Node_L> vector;
	while (!SJFqueue.empty())
	{
		vector.push_back(SJFqueue.front());
		SJFqueue.pop();
	}

	for (int i = 0; i < vector.size(); i++)
	{
		if ((currTime-vector[i].process.getArrivalTime()) >= 11)
		{
			cout << "\n\t\tAging Detected: Process PID:" << vector[i].process.getProcessID() << " Has Waited Too Long In SJF Queue" << endl;
			cout << "\t\t\tPromoting To FCFS ..." << endl << endl;
			FCFSqueue.push(vector[i]);
		}
		else
		{
			SJFqueue.push(vector[i]);
		}
	}
}
void MultilevelFeedbackQueue::checkAging_RR_SRTF(float currTime, queue<Node_L>& RRqueue, queue<Node_L>& SJFqueue)
{
	vector<Node_L> vector;
	while (!RRqueue.empty())
	{
		vector.push_back(RRqueue.front());
		RRqueue.pop();
	}

	for (int i = 0; i < vector.size(); i++)
	{
		if ((currTime - vector[i].process.getArrivalTime()) >= 8)
		{
			cout << "\n\t\tAging Detected: Process PID:" << vector[i].process.getProcessID() << " Has Waited Too Long In RR (SRTF) Queue" << endl;
			cout << "\t\t\tPromoting To SJF ..." << endl << endl;
			SJFqueue.push(vector[i]);
		}
		else
		{
			RRqueue.push(vector[i]);
		}
	}
}
void MultilevelFeedbackQueue::setRoundRobinTimeQuantum(int val)
{
	if (val == 0)
	{
		val = 2;
	}
	roundRobinTimeQuantum = val;
}
void MultilevelFeedbackQueue::setShortestJobFirstQueueTimeQuantum(int val)
{
	if (val == 0)
	{
		val = 2;
	}
	shortestJobFirstQueueTimeQuantum = val;
}
void MultilevelFeedbackQueue::takeInput(int processNum1, int processNum2, int processNum3)
{
	cout << endl << endl << "\t\t\tFIRST COME FIRST SEREVE INPUT" << endl << endl;
	for (int i = 0; i < processNum1; i++)
	{
		Node_L node;
		int process_ID = getRandomDigit(3);
		float arrival_time = getTime("Arrival Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		float burst_time = getTime("  Burst Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		cout << endl << endl;
		node.process.setValues(process_ID, arrival_time, burst_time);
		node.remainingBurstTime = burst_time;
		firstComeFirstServe.push(node);
	}

	cout << endl << endl << "\t\t\tSHORTEST JOB FIRST INPUT" << endl << endl;
	for (int i = 0; i < processNum2; i++)
	{
		Node_L node;
		int process_ID = getRandomDigit(3);
		float arrival_time = getTime("Arrival Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		float burst_time = getTime("  Burst Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		cout << endl << endl;
		node.process.setValues(process_ID, arrival_time, burst_time);
		node.remainingBurstTime = burst_time;
		shortestJobFirst.push(node);
	}

	cout << endl << endl << "\t\t\tROUND ROBIN (SRTF) INPUT" << endl << endl;
	for (int i = 0; i < processNum3; i++)
	{
		Node_L node;
		int process_ID = getRandomDigit(3);
		float arrival_time = getTime("Arrival Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		float burst_time = getTime("  Burst Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		cout << endl << endl;
		node.process.setValues(process_ID, arrival_time, burst_time);
		node.remainingBurstTime = burst_time;
		RoundRobin_SRTF.push(node);
	}
}
void MultilevelFeedbackQueue::run()
{
	entryOrExit(1);
	stableSortOnArrivalTime(firstComeFirstServe);
	stableSortOnArrivalTime(shortestJobFirst);
	stableSortOnArrivalTime(RoundRobin_SRTF);
	float currTime = 0;
	int processIncomplete = firstComeFirstServe.size() + shortestJobFirst.size() + RoundRobin_SRTF.size();
	queue<Node_L> ReadyQueue_FCFS;
	queue<Node_L> ReadyQueue_SJF;
	queue<Node_L> ReadyQueue_RR_SRTF;
	while (processIncomplete > 0)
	{
		checkWaitingQueue_FCFS(currTime, ReadyQueue_FCFS);
		checkWaitingQueue_SJF(currTime, ReadyQueue_SJF);
		if (checkWaitingQueue_RR_SRTF(currTime, ReadyQueue_RR_SRTF))
		{
			stableSortOnRemainingBurstTime(ReadyQueue_RR_SRTF);
		}
		stableSortOnRemainingBurstTime(ReadyQueue_SJF);

		if (!ReadyQueue_FCFS.empty())
		{
			cout << "\n\tStarting First Come First Serve ...";
			Node_L temp = ReadyQueue_FCFS.front();
			ReadyQueue_FCFS.pop();
			cout << "\n\t\tCPU Currently Executing Process PID:" << temp.process.getProcessID() << " With Remaining Burst Time: " << temp.remainingBurstTime  << endl;
			if (temp.remainingBurstTime > 20)
			{
				cout << "\n\tProcess PID:" << temp.process.getProcessID() << " Will Consume Too Much CPU Time" << endl;
				cout << "\t\t\tDemoting To SJF ..." << endl << endl;
				ReadyQueue_SJF.push(temp);
			}
			else
			{
				Sleep(temp.process.getBurstTime() * 1000);
			    currTime += temp.process.getBurstTime();
		        processIncomplete--; 
				cout << "\n\t\tProcess PID:" << temp.process.getProcessID() << " Has Finished Execution! " << endl << endl;
			}
		}
		else if (!ReadyQueue_SJF.empty())
		{
			cout << "\n\tStarting Shortest Job First ...";
			bool isQuantaComplete = true, isProcessComplete = false;
			Node_L temp = ReadyQueue_SJF.front();
			ReadyQueue_SJF.pop();
			if (temp.remainingBurstTime > 15)
			{
				cout << "\n\tProcess PID:" << temp.process.getProcessID() << " Will Consume Too Much CPU Time" << endl;
				cout << "\t\t\tDemoting To RR (SRTF) ..." << endl << endl;
				ReadyQueue_RR_SRTF.push(temp);
			}
			else
			{
				for (int i = 1; i <= shortestJobFirstQueueTimeQuantum; i++)
				{
					if (temp.remainingBurstTime > 1)
					{
						cout << "\n\t\tCPU Currently Executing Process PID:" << temp.process.getProcessID() << " With Remaining Burst Time: " << temp.remainingBurstTime << endl;
						Sleep(1 * 1000);
						currTime++;
						temp.remainingBurstTime--;
					}
					else
					{
						cout << "\n\t\tCPU Currently Executing Process PID:" << temp.process.getProcessID() << " With Remaining Burst Time: " << temp.remainingBurstTime << endl;
						Sleep(temp.remainingBurstTime * 1000);
						currTime += temp.remainingBurstTime;
						temp.remainingBurstTime = 0;
						processIncomplete--;
						isProcessComplete = true;
						cout << "\n\t\tProcess PID:" << temp.process.getProcessID() << " Has Finished Execution! " << endl << endl;
						break;
					}
					if (checkWaitingQueue_FCFS(currTime, ReadyQueue_FCFS))
					{
						cout << "\n\tPre-empting Shortest Job First. Priority Process Arrived In First Come First Serve Queue" << endl;
						cout << "\t\t\tContext Switching ..." << endl << endl;
						ReadyQueue_SJF.push(temp);
						isQuantaComplete = false;
						break;
					}
					if (isQuantaComplete)
					{
						if (!isProcessComplete)
						{
							cout << "\n\t\tProcess PID:" << temp.process.getProcessID() << " Has Completed Its Time Quantum " << endl;
							cout << "\t\t\tPromoting To First Come First Serve ..." << endl << endl;
							ReadyQueue_FCFS.push(temp);
						}
					}
				}
			}
		}
		else if (!ReadyQueue_RR_SRTF.empty())
		{
			cout << "\n\tStarting Round Robin (SRTF) ...";
			bool isQuantaComplete = true, isProcessComplete = false;
			Node_L temp = ReadyQueue_RR_SRTF.front();
			ReadyQueue_RR_SRTF.pop();
			cout << "\n\t\tCPU Currently Executing Process PID:" << temp.process.getProcessID() << " With Remaining Burst Time: " << temp.remainingBurstTime << endl;

			for (int i = 1; i <= roundRobinTimeQuantum; i++)
			{
				if (temp.remainingBurstTime > 1)
				{
					cout << "\n\t\tCPU Currently Executing Process PID:" << temp.process.getProcessID() << " With Remaining Burst Time: " << temp.remainingBurstTime << endl;
					Sleep(1 * 1000);
					currTime++;
					checkWaitingQueue_RR_SRTF(currTime, ReadyQueue_RR_SRTF);
					checkAging_RR_SRTF(currTime, ReadyQueue_RR_SRTF, ReadyQueue_SJF);
					checkAging_SJF(currTime, ReadyQueue_SJF, ReadyQueue_FCFS);
					temp.remainingBurstTime--;
				}
				else
				{
					cout << "\n\t\tCPU Currently Executing Process PID:" << temp.process.getProcessID() << " With Remaining Burst Time: " << temp.remainingBurstTime << endl;
					Sleep(temp.remainingBurstTime * 1000);
					currTime += temp.remainingBurstTime;
					temp.remainingBurstTime = 0;
					checkWaitingQueue_RR_SRTF(currTime, ReadyQueue_RR_SRTF);
					processIncomplete--;
					isProcessComplete = true;
					cout << "\n\t\tProcess PID:" << temp.process.getProcessID() << " Has Finished Execution! " << endl << endl;
					break;
				}
				if (checkWaitingQueue_FCFS(currTime, ReadyQueue_FCFS))
				{
					cout << "\n\tPre-empting Round Robin (SRTF). Priority Process Arrived In First Come First Serve Queue" << endl;
					cout << "\t\t\tContext Switching ..." << endl << endl;
					ReadyQueue_RR_SRTF.push(temp);
					isQuantaComplete = false;
					break;
				}
				if (checkWaitingQueue_SJF(currTime, ReadyQueue_SJF))
				{
					cout << "\n\tPre-empting Round Robin (SRTF). Priority Process Arrived In Shortest Job First" << endl;
					cout << "\t\t\tContext Switching ..." << endl << endl;
					ReadyQueue_RR_SRTF.push(temp);
					isQuantaComplete = false;
					break;
				}
				if (isQuantaComplete)
				{
					if (!isProcessComplete)
					{
						cout << "\n\t\tProcess PID:" << temp.process.getProcessID() << " Has Completed Its Time Quantum " << endl;
						cout << "\t\t\tPromoting To Shortest Job First ..." << endl << endl;
						ReadyQueue_SJF.push(temp);
					}
				}
			}

		}
		else
		{
			cout << "\t\tReady Queues Empty. No Currently Executing Process In CPU ..." << endl;
			Sleep(1 * 1000);
			currTime++;
		}

		checkAging_RR_SRTF(currTime,ReadyQueue_RR_SRTF,ReadyQueue_SJF);
		checkAging_SJF(currTime,ReadyQueue_SJF, ReadyQueue_FCFS);
	}
	entryOrExit(2);
}

struct Node_A
{
	Process process;
	Node_A* next;
	Node_A();
};
Node_A::Node_A()
{
	next = NULL;
}

class LongestJobFirst
{
private:
	int numOfProcess;
	Node_A* head,* tail;
	void checkWaitingQueue(float,Process*, int&);
	void stableSortOnBurstTime(Process*, int, int);
	void stableSortOnArrivalTime();
public:
	LongestJobFirst();
	void add(Node_A);
	Node_A* remove();
	bool isEmpty() const;
	void takeInput(int);
	void run();
	~LongestJobFirst();
};
void LongestJobFirst::checkWaitingQueue(float currTime, Process *ReadyQueue,int &endIndex)
{
	while (head != NULL)
	{
		if (head->process.getArrivalTime() <= currTime)
		{
			Node_A* ptr=remove();
			ReadyQueue[endIndex++] = ptr->process;
			delete ptr;
		}
		else
		{
			break;
		}
	}
}
void LongestJobFirst::stableSortOnBurstTime(Process* ReadyQueue, int startIndex, int endIndex)
{
	for (int i = startIndex; i < endIndex; i++)
	{
		for (int j = i+1; j < endIndex; j++)
		{
			if (ReadyQueue[i].getBurstTime()< ReadyQueue[j].getBurstTime())
			{
				Process temp = ReadyQueue[i];
				ReadyQueue[i] = ReadyQueue[j];
				ReadyQueue[j] = temp;
			}
		}
	}
}
void LongestJobFirst::stableSortOnArrivalTime()
{
	Node_A** h;
	for (int i = 0; i <= numOfProcess; i++)
	{
		h = &head;       // h NOW POINTS TO head POINTER VARIBALE
		int swapped = 0;
		for (int j = 0; j < numOfProcess - i - 1; j++)
		{
			Node_A* p1 = *h;        // p1 OCCUPIES THE MEMORY ADDRESS THAT head POINTS TO
			Node_A* p2 = p1->next;  // p2 OCCUPIES THE MEMORY ADDRESS THAT head POINTS TO NEXT
			bool updateTail = false;
			if (p2 == tail)             // WE WILL BE CHANGING TAIL POINTER
			{
				updateTail = true;   // SET FLAG TO TRUE
			}

			if (p1->process.getArrivalTime() > p2->process.getArrivalTime())
			{
				Node_A* tmp = p2->next;
				p2->next = p1;
				p1->next = tmp;

				*h = p2;             // UPDATE h ACCORDINGLY AS SWAPPING OF NODES
				if (updateTail)      // WE CHANGED TAIL POINTER VALUE SO UPDATE IT
				{
					tail = p1;       // UPDATE THE TAIL POINTER ACCORDINGLY
				}
				swapped = 1;
			}
			h = &(*h)->next;
		}
		if (swapped == 0)
		{
			break;
		}
	}
	//cout <<"PRINTING BURST TIMES OF LIST: \n ";
	//Node_A* ptr=head;
	//while (ptr != NULL)
	//{
	//	cout << ptr->process.getBurstTime()<<" ";
	//	ptr = ptr->next;
	//}
}
LongestJobFirst::LongestJobFirst()
{
	numOfProcess = 0;
	head = NULL;
	tail = NULL;
}
void LongestJobFirst::add(Node_A node)
{
	Node_A* current = new Node_A;
	*current = node;
	if (head == NULL)
	{
		head = current;
		tail = current;
	}
	else
	{
		tail->next = current;
		tail = current;
	}
	numOfProcess++;
}
Node_A* LongestJobFirst::remove()
{
	Node_A* ptr = NULL;
	if (head != NULL)
	{
		ptr = head;
		head = head->next;
		if (head == NULL)
		{
			tail = NULL;
		}
		numOfProcess--;
	}
	return ptr;
}
bool LongestJobFirst::isEmpty() const
{
	return (head == NULL);
}
void LongestJobFirst::takeInput(int processNum)
{
	cout << endl<<endl;
	for (int i = 0; i < processNum; i++)
	{
		Node_A node;
		int process_ID = getRandomDigit(3);
		float arrival_time = getTime("Arrival Time For Process " + to_string(i + 1)+" pid("+ to_string(process_ID)+")");
		float burst_time=getTime("  Burst Time For Process "+ to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		cout << endl << endl;
		node.process.setValues(process_ID, arrival_time,burst_time);
		add(node);     // ADD THIS NODE TO OUR LONGEST JOB FIRST QUEUE
	}
}
void LongestJobFirst::run()
{
	entryOrExit(1);
	stableSortOnArrivalTime();   // SORT THIS QUEUE IN INCREASING ORDER IN STABLE SORT BECAUSE IF ARRIVAL TIMES ARE EQUAL THEN WE USE FCFS!
	float currTime = 0;
	int processIncomplete = numOfProcess,startIndex=0,endIndex=0;
	Process* ReadyQueue=new Process[numOfProcess];
	while (processIncomplete >0)
	{
		checkWaitingQueue(currTime,ReadyQueue, endIndex);       // UPDATE READYQUEUE ON BASIS OF FINISHED ARRIVAL TIME
		stableSortOnBurstTime(ReadyQueue, startIndex, endIndex);// UPDATE READYQUEUE ON BASIS OF LONGEST BURST TIME
		if (startIndex==endIndex)
		{
			cout << "\t\tReady Queue Empty. No Currently Executing Process In CPU ..." << endl;
			Sleep(1 * 1000);
			currTime ++;
		}
		else
		{
			Process temp = ReadyQueue[startIndex++];
			cout << "\n\t\tCPU Currently Executing Process PID:"<<temp.getProcessID()<<" With Burst Time: "<<temp.getBurstTime() << endl << endl;
			Sleep(temp.getBurstTime() * 1000);
			currTime += temp.getBurstTime();
			processIncomplete--;
		}
	}
	entryOrExit(2);
	delete[] ReadyQueue;
}
LongestJobFirst::~LongestJobFirst()
{
	if (head != NULL)
	{
		while (head != NULL)
		{
			Node_A* ptr= head;
			head = head->next;
			delete ptr;
		}
		tail = NULL;
	}
}

struct Node_B
{
	Process process;
	float feedbackRatio;
	Node_B* next;
	Node_B();
};
Node_B::Node_B()
{
	feedbackRatio = INT_MAX;
	next = NULL;
}

class LowestFeedbackRatioNext
{
private:
	int numOfProcess;
	Node_B* head, * tail;
	void checkWaitingQueue(float, Node_B*, int&);
	void calculateFeedBackRatio(float,Node_B*, int, int);
	void stableSortOnFeedbackRatio(Node_B*, int, int);
	void stableSortOnArrivalTime();
public:
	LowestFeedbackRatioNext();
	void add(Node_B);
	Node_B* remove();
	bool isEmpty() const;
	void takeInput(int);
	void run();
	~LowestFeedbackRatioNext();
};
void LowestFeedbackRatioNext::checkWaitingQueue(float currTime, Node_B* ReadyQueue, int& endIndex)
{
	while (head != NULL)
	{
		if (head->process.getArrivalTime() <= currTime)
		{
			Node_B* ptr = remove();
			ptr->next = NULL;
			ReadyQueue[endIndex++] = *ptr;
			delete ptr;
		}
		else
		{
			break;
		}
	}
}
void LowestFeedbackRatioNext::calculateFeedBackRatio(float currTime, Node_B* ReadyQueue, int startIndex, int endIndex)
{
	while (startIndex < endIndex)
	{
		float W = currTime - ReadyQueue[startIndex].process.getArrivalTime();
		float S = ReadyQueue[startIndex].process.getBurstTime();
		ReadyQueue[startIndex++].feedbackRatio=(W+S)/S;
	}
}
void LowestFeedbackRatioNext::stableSortOnFeedbackRatio(Node_B* ReadyQueue, int startIndex, int endIndex)
{
	for (int i = startIndex; i < endIndex; i++)
	{
		for (int j = i + 1; j < endIndex; j++)
		{
			if (ReadyQueue[i].feedbackRatio > ReadyQueue[j].feedbackRatio)
			{
				Node_B temp = ReadyQueue[i];
				ReadyQueue[i] = ReadyQueue[j];
				ReadyQueue[j] = temp;
			}
		}
	}
}
void LowestFeedbackRatioNext::stableSortOnArrivalTime()
{
	Node_B ** h;
	for (int i = 0; i <= numOfProcess; i++)
	{
		h = &head;       // h NOW POINTS TO head POINTER VARIBALE
		int swapped = 0;
		for (int j = 0; j < numOfProcess - i - 1; j++)
		{
			Node_B * p1 = *h;        // p1 OCCUPIES THE MEMORY ADDRESS THAT head POINTS TO
			Node_B * p2 = p1->next;  // p2 OCCUPIES THE MEMORY ADDRESS THAT head POINTS TO NEXT
			bool updateTail = false; 
			if(p2==tail)             // WE WILL BE CHANGING TAIL POINTER
			{
				updateTail = true;   // SET FLAG TO TRUE
			}

			if (p1->process.getArrivalTime() > p2->process.getArrivalTime())
			{
				Node_B * tmp = p2->next;
				p2->next = p1;
				p1->next = tmp;
				
				*h = p2;             // UPDATE h ACCORDINGLY AS SWAPPING OF NODES
				if (updateTail)      // WE CHANGED TAIL POINTER VALUE SO UPDATE IT
				{
					tail = p1;       // UPDATE THE TAIL POINTER ACCORDINGLY
				}
				swapped = 1;
			}
			h = &(*h)->next;
		}
		if (swapped == 0)
		{
			break;
		}
	}

	//cout << "PRINTING Arrival TIMES OF LIST: \n ";
	//Node_B* ptr=head;
	//while (ptr != tail)
	//{
	//	cout << ptr->process.getArrivalTime()<<" ";
	//	ptr = ptr->next;
	//}
	//cout << tail->process.getArrivalTime() << " ";
}
LowestFeedbackRatioNext::LowestFeedbackRatioNext()
{
	numOfProcess = 0;
	head = NULL;
	tail = NULL;
}
void LowestFeedbackRatioNext::add(Node_B node)
{
	Node_B* current = new Node_B;
	*current = node;
	if (head == NULL)
	{
		head = current;
		tail = current;
	}
	else
	{
		tail->next = current;
		tail = current;
	}
	numOfProcess++;
}
Node_B* LowestFeedbackRatioNext::remove()
{
	Node_B* ptr = NULL;
	if (head != NULL)
	{
		ptr = head;
		head = head->next;
		if (head == NULL)
		{
			tail = NULL;
		}
		numOfProcess--;
	}
	return ptr;
}
bool LowestFeedbackRatioNext::isEmpty() const
{
	return (head == NULL);
}
void LowestFeedbackRatioNext::takeInput(int processNum)
{
	cout << endl << endl;
	for (int i = 0; i < processNum; i++)
	{
		Node_B node;
		int process_ID = getRandomDigit(3);
		float arrival_time = getTime("Arrival Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		float burst_time = getTime("  Burst Time For Process " + to_string(i + 1) + " pid(" + to_string(process_ID) + ")");
		cout << endl << endl;
		node.process.setValues(process_ID, arrival_time, burst_time);
		add(node);          // ADD THIS NODE TO OUR LOWEST FEEDBACK RATIO NEXT QUEUE
	}
}
void LowestFeedbackRatioNext::run()
{
	entryOrExit(1);
	stableSortOnArrivalTime();   // SORT THIS QUEUE IN INCREASING ORDER IN STABLE SORT BECAUSE IF ARRIVAL TIMES ARE EQUAL THEN WE USE FCFS!
	float currTime = 0;
	int processIncomplete = numOfProcess, startIndex = 0, endIndex = 0;
	Node_B* ReadyQueue = new Node_B[numOfProcess];
	while (processIncomplete > 0)
	{
		checkWaitingQueue(currTime, ReadyQueue, endIndex);           // UPDATE READYQUEUE ON BASIS OF FINISHED ARRIVAL TIME
		calculateFeedBackRatio(currTime, ReadyQueue, startIndex, endIndex);    // CALCULATE FEEDBACK RATIO
		stableSortOnFeedbackRatio(ReadyQueue, startIndex, endIndex); // UPDATE READYQUEUE ON BASIS OF LOWEST FEEDBACK RATIO
		if (startIndex == endIndex)
		{
			cout << "\t\tReady Queue Empty. No Currently Executing Process In CPU ..." << endl;
			Sleep(1 * 1000);
			currTime++;
		}
		else
		{
			Node_B temp = ReadyQueue[startIndex++];
			cout << "\n\t\tCPU Currently Executing Process PID:" << temp.process.getProcessID() << " With Burst Time: " << temp.process.getBurstTime() << endl << endl;
			Sleep(temp.process.getBurstTime() * 1000);
			currTime += temp.process.getBurstTime();
			processIncomplete--;
		}
	}
	entryOrExit(2);
	delete [] ReadyQueue;
}
LowestFeedbackRatioNext::~LowestFeedbackRatioNext()
{
	if (head != NULL)
	{
		while (head != NULL)
		{
			Node_B* ptr = head;
			head = head->next;
			delete ptr;
		}
		tail = NULL;
	}
}

void Multilevel_Queue()
{
	int n1 = getInput("Number Of Processes For Priority Scheduling");
	int n2 = getInput("Number Of Processes For Round Robin");
	int n3 = getInput("Number Of Processes For Shortest Remaining Time First");
	cout << endl << endl;
	int n4 = getInput("Time Quantum For Round Robin");
	MultilevelQueue multilevelQueue;
	multilevelQueue.setRoundRobinTimeQuantum(n4);
	multilevelQueue.takeInput(n1,n2,n3);
	multilevelQueue.run();
}
void Multilevel_Feedback_Queue()
{
	int n1 = getInput("Number Of Processes For First Come First Serve");
	int n2 = getInput("Number Of Processes For Shortest Job First");
	int n3 = getInput("Number Of Processes For Round Robin (SRTF)");
	cout << endl << endl;
	int n4 = getInput("Time Quantum For SJF Queue");
	int n5 = getInput("Time Quantum For Round Robin");
	MultilevelFeedbackQueue multilevelFeedbackQueue;
	multilevelFeedbackQueue.setShortestJobFirstQueueTimeQuantum(n4);
	multilevelFeedbackQueue.setRoundRobinTimeQuantum(n5);
	multilevelFeedbackQueue.takeInput(n1, n2, n3);
	multilevelFeedbackQueue.run();
}
void Longest_Job_First()
{
	int n = getInput("Number Of Processes");
	LongestJobFirst longestJob;
	longestJob.takeInput(n);
	longestJob.run();
}
void Lowest_Feedback_Ratio_Next()
{
	int n = getInput("Number Of Processes");
	LowestFeedbackRatioNext lowestFeedbackRatio;
	lowestFeedbackRatio.takeInput(n);
	lowestFeedbackRatio.run();
}
void CPUSchedulingMenuDisplay()
{
	cout << endl;
	cout << "\t\t\t**** CPU SCHEDULING ****" << endl;
	cout << "\t\t\t       0.Back" << endl;
	cout << "\t\t\t       1.Multilevel Queue Scheduling" << endl;
	cout << "\t\t\t       2.Multilevel Feedback Queue Scheduling" << endl;
	cout << "\t\t\t       3.Longest Job First" << endl;
	cout << "\t\t\t       4.Lowest Feedback Ratio Next Scheduling" << endl;
	cout << endl;
}
void CPU_Scheduling()
{
	system("CLS");
	int option;
	do
	{
		CPUSchedulingMenuDisplay();
		cout << "Enter Option: ";
		cin >> option;
		if (option == 0)
		{
			break;
		}
		else if (option == 1)
		{
			Multilevel_Queue();
		}
		else if (option == 2)
		{
			Multilevel_Feedback_Queue();
		}
		else if (option == 3)
		{
			Longest_Job_First();
		}
		else if (option == 4)
		{
			Lowest_Feedback_Ratio_Next();
		}
	} while (1);
	system("CLS");
}
/////////////
int * chopsticks,numOfChopsticks,maxAllowed,printMutex=1;
void wait(int& index)
{
	while (index <= 0);
	index--;
}
void signal(int& index)
{
	index++;
}
void think(int philospherID)
{
	wait(printMutex);
	cout << "\n\t\tPhilospher " << philospherID << " Is Currently Thinking" << endl;
	signal(printMutex);
	Sleep(((rand()%5)+1)*1000);
	wait(printMutex);
	cout << "\n\t\tPhilospher " << philospherID << " Has Finished Thinking" << endl;
	signal(printMutex);
}
void eat(int philospherID)
{
	wait(printMutex);
	cout << "\n\t\tPhilospher " << philospherID << " Is Currently Eating" << endl;
	signal(printMutex);
	Sleep(((rand() % 5) + 1) * 1000);
	wait(printMutex);
	cout << "\n\t\t\tPhilospher " << philospherID << " Has Finished Eating" << endl;
	signal(printMutex);
}
void diningTable(int philospherID)
{
	Sleep(((rand() % 6) + 2) * 100);
	int numSpoonfuls = 3,i=0;
	do
	{
		think(philospherID+1);
		   wait(maxAllowed);
		        wait(chopsticks[philospherID]);
		             wait(chopsticks[(philospherID+1)% numOfChopsticks]);
		                     eat(philospherID+1);
		             signal(chopsticks[(philospherID + 1) % numOfChopsticks]);
		        signal(chopsticks[philospherID]);
		   signal(maxAllowed);
		i++;
	} while (i < numSpoonfuls);
}

void semaphore()
{
	system("CLS");
	cout << endl << endl;
	int numPhilosopher = getInput("Number Of Philosophers");
	if (numPhilosopher < 2)
	{
		cout << "\n\t\tInvalid Input. Setting Number Of Philosophers to 2 "<< endl;
		numPhilosopher = 2;
	}
	numOfChopsticks = numPhilosopher;
	chopsticks = new int[numOfChopsticks];
	for (int i = 0; i < numOfChopsticks; i++)
	{
		chopsticks[i] = 1;
	}
	maxAllowed = numOfChopsticks - 1;
	thread * threadArray= new thread[numOfChopsticks];
	for (int i = 0; i < numPhilosopher; i++)
	{
		threadArray[i] = thread(diningTable,i);
		Sleep(500);
	}
	for (int i = 0; i < numPhilosopher; i++)
	{
		threadArray[i].join();
	}
	delete[] chopsticks;
	entryOrExit(2);
}
/////////////
class Bank
{
private:
	int numOfProcesses;
	int numOfResources;
	int** max;
	int** allocation;
	int** need;
	int* available;
	void bankMenuDisplay();
	bool safeState(bool);
	bool request();
public:
	Bank();
	void setNumOfProcesses(int);
	void setNumOfResources(int);
	void allocateMatrixes();
	void initializeAvailablity();
	void initializeMatrixes();
	void print();
	void  run();
	~Bank();
};
void Bank::bankMenuDisplay()
{
	cout << endl;
	cout << "\t\t\t**** Deadlock Avoidance Algorithm ****" << endl;
	cout << "\t\t\t             0.Exit" << endl;
	cout << "\t\t\t             1.Evaluate Safe State" << endl;
	cout << "\t\t\t             2.Make A Request" << endl;
	cout << "\t\t\t             3.Print Matrixes" << endl;
	cout << endl;
}
bool Bank::safeState(bool print)
{
	bool isSafe = true;
	bool* processExecuted = new bool[numOfProcesses];
	vector<int> safeState, tempAvailability;
	for (int i = 0; i < numOfProcesses; i++)
	{
		processExecuted[i] = false;

	}
	for (int i = 0; i < numOfResources; i++)
	{
		tempAvailability.push_back(available[i]);
	}

	for (int i = 0, count = 0; safeState.size() < numOfProcesses && isSafe; i = (i + 1) % numOfProcesses, count++)
	{
		if (!processExecuted[i])
		{
			bool isExecutable = true;
			for (int j = 0; j < numOfResources && isExecutable; j++)
			{
				if (need[i][j] > tempAvailability[j])
				{
					isExecutable = false;
				}
			}
			if (isExecutable)
			{
				for (int j = 0; j < numOfResources; j++)
				{
					tempAvailability[j] += allocation[i][j];
				}
				processExecuted[i] = true;
				safeState.push_back(i);
				count = 0;
			}
		}

		if (count == numOfProcesses)
		{
			isSafe = false;
		}
	}
	if (print)
	{
		if (isSafe)
		{
			cout << endl << endl << "\t\t\Safe State Exists ";
			cout << endl << "\t\t\t< ";
			for (int i = 0; i < safeState.size(); i++)
			{
				cout << "P" << safeState[i] + 1 << " ";
			}
			cout << " >";
			cout << endl;
		}
		else
		{
			cout <<endl<<endl <<"\t\t\tDeadlock Exists ";
			cout << endl << "\t\t\t< ";
			for (int i = 0; i < safeState.size(); i++)
			{
				cout << "P" << safeState[i] + 1 << " ";
			}
			cout << " >";
			cout << endl;
		}
     }
	delete[] processExecuted;
	return isSafe;
}
bool Bank::request()
{
	bool isGranted = true,step1=true,step2=true;
	int processNum, val;
	string letter = "A";
	vector<int> request,tempAvailable,tempAllocation,tempNeed;
		do 
		{
			cout << "Enter Process Number That Wishes To Make Request: ";
			cin >> processNum;
			if (processNum > 0&& processNum<=numOfProcesses)
			{
				break;
			}
			else
			{
				cout << "\t INVALID INPUT" << endl;
			}
		} while (1);

		for (int i = 0; i < numOfResources; i++)
		{
			do
			{
				cout << "Enter Instances For Resource " + letter+":";
				cin >> val;
				if (val >= 0 && val <= max[processNum-1][i])
				{
					break;
				}
				else
				{
					cout << "\t INVALID INPUT" << endl;
				}
			} while (1);
			request.push_back(val);
            letter[0] = char(int(letter[0]) + 1);

			tempAvailable.push_back(available[i]);
			tempAllocation.push_back(allocation[processNum - 1][i]);
			tempNeed.push_back(need[processNum - 1][i]);
		}

		entryOrExit(1);

		for (int i = 0; i < numOfResources && step1; i++)
		{
			if (request[i] > need[processNum - 1][i])
			{
				step1 = false;
			}
		}

		for (int i = 0; i < numOfResources && step2; i++)
		{
			if (request[i] > available[i])
			{
				step2 = false;
			}
		}

		if (step1)//(Request <= Need)
		{
			if (step2)//(Request <= Available)
			{
				for (int i = 0; i < numOfResources; i++)
				{
					available[i] -= request[i];                       //Available = Available – Request
					allocation[processNum - 1][i] += request[i];   //Allocation = Allocation + Request
					need[processNum - 1][i] -= request[i];	       //Need = Need – Request
				}

				cout << endl << endl << "\t\t\tRunning Safety Algorithm ..." << endl;
				Sleep(600);
				cout << "\t\t\t      Please Wait ..." << endl;
				Sleep(1500);
				if (safeState(0))
				{
					cout << endl << endl << "\t\tGranting Request Will NOT Create Deadlock ..." << endl;
					Sleep(900);
					cout << "\t\t\t      Granting Request ..." << endl;
					Sleep(1000);
					cout << "\t\t    Matrixes Have Been Updated Accordingly ..." << endl;
				}
				else
				{
				
					cout << endl << endl << "\t\tGranting Request Will Create Deadlock In Future! ..." << endl;
					Sleep(800);
					cout << "\t\t\t      Discarding Request ..." << endl << endl;
					for (int i = 0; i < numOfResources; i++)
					{
						available[i] = tempAvailable[i];
						allocation[processNum - 1][i] = tempAllocation[i];
						need[processNum - 1][i] = tempNeed[i];
					}
					isGranted = false;
				}
			}
			else
			{
				cout << endl << endl << "\t\tRequest Greater Than Availability Detected ..." << endl;
				Sleep(500);
				cout << "\t\t\t          Try Later ..." << endl;
				Sleep(500);
				cout << "\t\t\t      Discarding Request ..." << endl << endl;
				isGranted = false;
			}
		}
		else
		{
			cout << endl << endl << "\t\tRequest Greater Than Need Detected ..." << endl;
			Sleep(500);
			cout << "\t\t\t      Discarding Request ..." << endl << endl;
			isGranted = false;
		}
	return isGranted;
}
Bank::Bank()
{
	numOfProcesses = 0;
    numOfResources = 0;
	max = NULL;
	allocation = NULL;
	need = NULL;
	available = NULL;
}
void Bank::setNumOfProcesses(int val)
{
	numOfProcesses = val;
}
void Bank::setNumOfResources(int val)
{
	numOfResources = val;
}
void Bank::allocateMatrixes()
{
	max = new int* [numOfProcesses];
	allocation = new int* [numOfProcesses];
	need = new int* [numOfProcesses];
	for (int i = 0; i < numOfProcesses; i++)
	{
		max[i] = new int[numOfResources];
		allocation[i] = new int [numOfResources];
		need[i] = new int [numOfResources];
	}
	available = new int [numOfResources];
}
void  Bank::initializeAvailablity()
{
	if (available != NULL)
	{
		cout << endl << "\t\t\tResource Instances Input" << endl << endl;
		string letter = "A";
		for (int i = 0; i < numOfResources; i++)
		{
			available[i] = getInput("Instances For Resource "+ letter);
			letter[0] = char(int(letter[0]) + 1);
		}
		cout << endl << endl;
	}
}
void  Bank::initializeMatrixes()
{
	if (max != NULL && allocation != NULL && available != NULL)
	{
		vector<int> availabilityCount;
		for (int i = 0; i < numOfResources; i++)
		{
			availabilityCount.push_back(available[i]);
		}

		for (int i = 0; i < numOfProcesses; i++)
		{
			string letter = "A";
			for (int j= 0; j < numOfResources; j++)
			{
				int val = 0;
				do
				{
					cout << "Enter    Max     Resource " + letter + " For Process " + to_string(i + 1) << ": ";
					cin >> val;
					if (val >= 0 &&val<= availabilityCount[j])
					{
						break;
					}
					else
					{
						cout << "\t INVALID INPUT" << endl;
					}
				} while (1);

				max[i][j]= val;
				
				do
				{
					cout << "Enter Allocated  Resource " + letter + " For Process " + to_string(i + 1) << ": ";
					cin >> val;
					if (val >= 0 && val <= available[j] && val<=max[i][j])
					{
						break;
					}
					else
					{
						cout << "\t INVALID INPUT" << endl;
					}
				} while (1);
				available[j] -= val;
				allocation[i][j] = val;
				need[i][j] = max[i][j] - allocation[i][j];
				letter[0] = char(int(letter[0]) + 1);
			}
			cout << endl;
		}
	}
}
void  Bank::print()
{
	entryOrExit(1);
	cout << endl << endl << "\t\t\t\t Matrixes View";
	cout << endl << endl << endl;

	cout << "    Process#     " << "\t    MAX  " << "  \t         ALLOCATED "<<"            \t    NEED" << endl;
	Sleep(600);
	for (int i = 0; i < numOfProcesses; i++)
	{
		Sleep(300);
		cout << "       P" << setw(2)<<left << i + 1 << "        \t";
		Sleep(300);
		for (int j = 0; j < numOfResources; j++)
		{
			cout << setw(4)<<left<< max[i][j] << " ";
			Sleep(300);
		}
		cout << "   \t"; 
		for (int j = 0; j < numOfResources; j++)
		{
			cout << setw(4)<< left << allocation[i][j] << " ";
			Sleep(300);
		}
		cout << "     \t";
		for (int j = 0; j < numOfResources; j++)
		{
			cout << setw(4) << left << need[i][j] << " ";
			Sleep(300);
		}

		cout << endl;
		Sleep(600);
	}
	cout << endl<<endl<<"\t\t\t\t AVAILABLE "<<endl<<"\t\t\t\t";
	for (int i = 0; i < numOfResources; i++)
	{
		cout << setw(4) << left << available[i] << " ";
		Sleep(300);
	}
	entryOrExit(1);
}
void  Bank::run()
{
	system("CLS");
	int option;
	do
	{
		bankMenuDisplay();
		cout << "Enter Option: ";
		cin >> option;
		if (option == 0)
		{
			break;
		}
		else if (option == 1)
		{
			entryOrExit(1);
			cout << endl << endl << "\t\t\tRunning Safety Algorithm ..." << endl;
			Sleep(600);
			cout << "\t\t\t      Please Wait ..." << endl;
			Sleep(1500);
			safeState(1);
			entryOrExit(2);
		}
		else if (option == 2)
		{
			request();
			entryOrExit(2);
		}
		else if (option == 3)
		{
			print();
		}

	} while (1);
	system("CLS");
}
Bank::~Bank()
{
	for (int i = 0; i < numOfProcesses; i++)
	{
		delete[] max[i];
		delete[] allocation[i];
		delete[] need[i];
	}
	delete[] max;
	delete[] allocation;
	delete[] need;
	delete[] available;
	numOfProcesses = 0;
	numOfResources = 0;
}

void deadlock()
{
	int n1 = getInput("Number Of Processes");
	int n2 = getInput("Number Of Resources");
	cout << endl << endl;
	Bank bank;
	bank.setNumOfProcesses(n1);
	bank.setNumOfResources(n2);
	bank.allocateMatrixes();
	bank.initializeAvailablity();
	bank.initializeMatrixes();
	bank.run();
}
/////////////
int fixedFrameSize = 16;

bool isLog2(int value)
{
	bool isLog = true;
	double result = log2(value);
	if (result - (int)result > 0)
	{
		isLog = false;
	}
	return isLog;
}

int getMemorySize(string str)
{
	cout << "\t\t\t\t**** RAM SIZE INPUT ****" << endl;
	int val;
	do {
		cout << "\t\tEnter Your " << str << ": ";
		cin >> val;
		if (val >= 0 && isLog2(val)&&val>=fixedFrameSize)
		{
			break;
		}
		else if (!isLog2(val))
		{
			cout << "\t INVALID INPUT: Enter Size According To Powers Of 2" << endl;
		}
		else if (val < fixedFrameSize)
		{
			cout << "\t INVALID INPUT: RAM Size Should Be Greater Than Fixed Frame Size "<<fixedFrameSize << endl;
		}
		else
		{
			cout << "\t INVALID INPUT" << endl;
		}
	} while (1);
	return val;
}
int getProcessSize(string str)
{
	int val;
	do {
		cout << "Enter Your " << str << ": ";
		cin >> val;
		if (val >= 0 /*&& isLog2(val)*/)
		{
			break;
		}
		/*else if (!isLog2(val))
		{
			cout << "\t INVALID INPUT: Enter Size According To Powers Of 2" << endl;
		}*/
		else
		{
			cout << "\t INVALID INPUT" << endl;
		}
	} while (1);
	return val;
}
int binaryToInt(string str)
{
	int  i = 0, rem, number = 0;
	long long bin = stoi(str);
	while (bin != 0)
	{
		rem = bin % 10;
		bin /= 10;
		number += rem * pow(2, i);
		++i;
	}
	return number;
}
string intToBinary(int num)
{
	string str;
	for (int i = 0; num > 0; i++)
	{
		str.append(to_string(num % 2));
		num = num / 2;
		
	}
	for (int si = 0, ei = str.length() - 1; si < ei; si++, ei--)
	{
		char temp = str[si];
		str[si] = str[ei];
		str[ei] = temp;
	}
	return str;
}

class RandomAccessMemory
{
private:
	int* RAM;
	int sizeRAM;
	int numOfFrames;
	int freeFrames;
public:
	RandomAccessMemory();
	RandomAccessMemory(int);
	void initializeSize(int);
	void initializeMemory();
	bool isFreeFrameAvailable();
	int getFreeFrameIndex();
	int getNumOfFrames();
	void allocateFrame(int,int);
	void deallocateFrame(int);
	void clear();
	void print();
	~RandomAccessMemory();
} RAM;
RandomAccessMemory::RandomAccessMemory()
{
	RAM = NULL;
	sizeRAM = 0;
	numOfFrames = 0;
	freeFrames = 0;
}
RandomAccessMemory::RandomAccessMemory(int size)
{
	if (size < 0 || !isLog2(size))
	{
		size = 512;
	}
	sizeRAM = size;
	numOfFrames = sizeRAM / fixedFrameSize;
	freeFrames = numOfFrames;
	RAM = new int[sizeRAM];
	for (int i = 0; i < sizeRAM; i++)
	{
		RAM[i] = 0;
	}
}
void RandomAccessMemory::initializeSize(int size)
{
	if (size < 0 || !isLog2(size))
	{
		size = 512;
	}
	sizeRAM = size;	
	numOfFrames = sizeRAM / fixedFrameSize;
	freeFrames = numOfFrames;
	RAM = new int[sizeRAM];
	for (int i = 0; i < sizeRAM; i++)
	{
		RAM[i] = 0;
	}
}
void RandomAccessMemory::initializeMemory()
{
	int occupiedFrames = (((rand() % 25 + 15)) *fixedFrameSize)/100;
	while (occupiedFrames > 0)
	{
		int frameNo = (rand() % numOfFrames) * fixedFrameSize;
		if (RAM[frameNo] == 0)  // FIND  UNOCCUPIED RAM SEGMENT/BYTE
		{
			for (int i = frameNo, ct = 0; ct < fixedFrameSize; ct++, i++)
			{
				RAM[i] = 1;       // SET TO OCCUPIED RAM SEGMENT/BYTE
			}
			freeFrames--;
	        occupiedFrames--;
		}
	}
}
bool RandomAccessMemory::isFreeFrameAvailable()
{
	return freeFrames > 0;
}
int RandomAccessMemory::getFreeFrameIndex()
{
	int frameNo = -1;
	if (isFreeFrameAvailable())
	{
		while (1)
		{
			frameNo = (rand() % numOfFrames)*fixedFrameSize;
			if(RAM[frameNo]==0)
			{
				break;
			}
		}
	}
	return frameNo;
}
int RandomAccessMemory::getNumOfFrames()
{
	return numOfFrames;
}
void RandomAccessMemory::allocateFrame(int frameNo,int bytes)
{
	if (RAM[frameNo] == 0)
	{
		for (int i = frameNo, ct = 0; ct < fixedFrameSize; ct++, i++)
		{
			if (ct < bytes)
			{
				RAM[i] = 1;
			}
			else
			{
				RAM[i] = 2;  // INTERNAL FRAGMENTATION
			}
		}
		freeFrames--;
	}
	else
	{
		cout << endl << "\t\t CANT OVER-WRITE EXISTING SEGMENT IN RAM" << endl << endl;
	}
}
void RandomAccessMemory::deallocateFrame(int frameNo)
{
	for (int i = frameNo, ct = 0; ct < fixedFrameSize; ct++, i++)
	{
			RAM[i] = 0;
	}
	freeFrames++;
}
void RandomAccessMemory::print()
{
	cout << endl << endl << "\tRAM: " << endl << endl;
	cout << endl << "\t\t";
	for (int i = 0, ct = 0; i < sizeRAM; ct++)
	{
		if (ct == log2(fixedFrameSize))
		{
			cout << endl<<"\t\t";
			ct = 0;
		}
		for (int j = 0; j < fixedFrameSize; i++,j++)
		{
			cout << RAM[i];
		}
		cout << "  ";
	}
	cout << endl<<endl;
	cout << "\n\tKEY:" << endl;
	cout << "\t      0:   Free   Byte" << endl;
	cout << "\t      1: Occupied Byte" << endl;
	cout << "\t      2: Internal Fragmentation" << endl;
	entryOrExit(2);
}
void RandomAccessMemory::clear()
{
	sizeRAM = 0;
	if (RAM != NULL)
	{
		delete[] RAM;
	}
	RAM = NULL;
}
RandomAccessMemory::~RandomAccessMemory()
{
	clear();
}

class TranslationLookAsideBuffers
{
private:
	int** TLB;
	int sizeTLB;
	int firstInFirstOut;
	int infoPresent;
public:
	TranslationLookAsideBuffers();
	bool searchTLB(int,int);
	int getFrameNum(int, int);
	void updateTLB(int,int,int);
	void print();
	~TranslationLookAsideBuffers();
}TLB;
     TranslationLookAsideBuffers::TranslationLookAsideBuffers()
{
	sizeTLB = 10;
	TLB = new int* [sizeTLB];
	for (int i = 0; i < sizeTLB; i++)
	{
		TLB[i] = new int[3];
	}
	for (int i = 0; i < sizeTLB; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			TLB[i][j] = INT_MIN;
		}
	}
	firstInFirstOut = 0;
	infoPresent = 0;
}
bool TranslationLookAsideBuffers::searchTLB(int pid, int pageNo)
{
	bool found = false;
	for (int i = 0; i < sizeTLB&&!found; i++)
	{
		if (TLB[i][0] == pid && TLB[i][1] == pageNo)
		{
			found = true;
		}
	}
	return found;
}
int  TranslationLookAsideBuffers::getFrameNum(int pid, int pageNo)
{
	bool found = false;
	int frame = -1;
	for (int i = 0; i < sizeTLB && !found; i++)
	{
		if (TLB[i][0] == pid && TLB[i][1] == pageNo)
		{
			found = true;
			frame = TLB[i][2];
		}
	}
	return frame;
}
void TranslationLookAsideBuffers::updateTLB(int pid, int pageNo,int frameNo)
{
	if (infoPresent < sizeTLB)
	{
		TLB[infoPresent][0] = pid;
		TLB[infoPresent][1] = pageNo;
		TLB[infoPresent][2] = frameNo;
		infoPresent++;
	}
	else
	{
		TLB[firstInFirstOut][0] = pid;
		TLB[firstInFirstOut][1] = pageNo;
		TLB[firstInFirstOut][2] = frameNo;
		firstInFirstOut=(firstInFirstOut+1)% sizeTLB;
	}
}
void TranslationLookAsideBuffers::print()
{
	cout << endl << endl << "\tTLB Table: " << endl << endl;
	cout << endl << "\t\t\t ProcessID        Page#       Frame#";
	for (int i = 0; i < sizeTLB;i++)
	{
		if (TLB[i][0] > 0)
		{
			cout << endl << "\t\t\t    " << TLB[i][0] << "             " << TLB[i][1] << "           " << TLB[i][2];
		}
		else
		{
			cout << endl << "\t\t\t    NULL           NULL        NULL";
		}
	}
	cout << endl << endl;
	entryOrExit(2);
}
TranslationLookAsideBuffers::~TranslationLookAsideBuffers()
{
	sizeTLB = 0;
	firstInFirstOut = 0;
	for (int i = 0; i < sizeTLB; i++)
	{
		delete [] TLB[i];
	}
	delete[]TLB;
	TLB = NULL;
}

class PagedProcess_TwoLevel
{
private:
	int process_ID;
	int process_Size;
	int page_Size;
	int numOfPages;
	int** outerPageTable;
	int fixedOuterPageTableSize;
	int fixedInnerPageTableSize;
	void copyTwoLevel(const PagedProcess_TwoLevel&);
	void destroyTwoLevel();
	void updatePageTableSize();
	void printRandomFrame(int, int);
public:
	PagedProcess_TwoLevel();
	PagedProcess_TwoLevel(const PagedProcess_TwoLevel&);
	int getProcessID();
	int getNumOfPages();
	void initialize();
	void getLogicalAddress();
	int getFrameNumber(int,int);
	void terminate();
	void printTable();
	const PagedProcess_TwoLevel& operator = (const PagedProcess_TwoLevel&);
	~PagedProcess_TwoLevel();
};
void PagedProcess_TwoLevel::copyTwoLevel(const PagedProcess_TwoLevel& obj)
{
	if (outerPageTable != NULL)
	{
		destroyTwoLevel();
	}
	process_ID=obj.process_ID;
	process_Size=obj.process_Size;
	page_Size=obj.page_Size;
	numOfPages = obj.numOfPages;
	fixedOuterPageTableSize = obj.fixedOuterPageTableSize;
	fixedInnerPageTableSize = obj.fixedInnerPageTableSize;
	outerPageTable = new int* [fixedOuterPageTableSize];
	for (int i = 0; i < fixedOuterPageTableSize; i++)
	{
		outerPageTable[i] = NULL;
		if (obj.outerPageTable[i]!=NULL)
		{
			outerPageTable[i] = new int[fixedInnerPageTableSize];
			for (int j = 0; j < fixedInnerPageTableSize; j++)
			{
				outerPageTable[i][j] = obj.outerPageTable[i][j];
			}
		}
	}
}
void PagedProcess_TwoLevel::destroyTwoLevel()
{
	process_ID = 0;
	process_Size = 0;
	page_Size = 0;
	numOfPages = 0;
	for (int i = 0; i < fixedOuterPageTableSize; i++)
	{
		if (outerPageTable[i] != NULL)
		{
			delete[] outerPageTable[i];
		}
	}
	delete[] outerPageTable;
}
void PagedProcess_TwoLevel::updatePageTableSize()
{
	fixedOuterPageTableSize = 4;// NOT 2^4=16 RATHER ITS 2^2=4
	fixedInnerPageTableSize = 4;// NOT 2^4=16 RATHER ITS 2^2=4

	string pageSizeBinary = intToBinary(page_Size-1);// PAGE_SIZE-1 BECAUSE INDEXING STARTS FROM 0
	int bitsPageSize = pageSizeBinary.length();

	string processSizeBinary = intToBinary(process_Size-1);// PROCESS_SIZE-1 BECAUSE INDEXING STARTS FROM 0
	int bitsProcessSize = processSizeBinary.length();

	int y= ((bitsProcessSize)-(bitsPageSize)) / 2;
	int x= ((bitsProcessSize)-(bitsPageSize)) - y;
	if (bitsProcessSize> bitsPageSize && pow(2, y) >0 && pow(2, x) >0)
	{
		fixedInnerPageTableSize = pow(2,y);
		fixedOuterPageTableSize = pow(2,x);
	}
	cout << "\t\t Setting Outer Page Table Size "<< fixedOuterPageTableSize <<" ..." << endl;
	Sleep(200);
	cout << "\t\t Setting Inner Page Table Size " << fixedInnerPageTableSize << " ..." << endl;
	Sleep(200);
	/*CURRENT LOGIC CAN MAP ONLY UPTO 256 BYTE SIZED PROCESS AND EXTRA BYTES ARE UN-MAPPED
	TO MAKE IT WORK ACCORDING TO NUMBER OF BITS AND ITS DIVISION ETC JUST UPDATE THE VALUE
	OF THESE TWO VARIABLE ACCORDINGLY. FUNCTIONS FOR CONVERSION TO BINARY AND INTEGER AND VICE VERSA ARE PROVIDED
	MAKE SURE THESE VARIABLE CONTAINS THE VALUE OF THE 2 power NOT THE NUMBER OF BITS etc*/
}
void PagedProcess_TwoLevel::printRandomFrame(int frameNo, int offset)
{
	cout << endl << endl << "\tFrame Fetched: " << frameNo << endl << endl;
	cout << "\t\t       Memory#     Offset#          Data" << endl;
	for (int i = 0, j = frameNo * fixedFrameSize; i < fixedFrameSize; i++, j++)
	{
		int t = rand() % 2;
		cout << "\t\t\t";
		if (t == 0)
		{
			cout << " " << setw(4) << left << j << "         " << setw(4) << left << i << "           " << getRandomDigit(rand() % 6 + 2);
		}
		else
		{
			char ch = rand() % 26;
			int x = rand() % 2;
			if (x == 0)
			{
				ch += 'A';
			}
			else
			{
				ch += 'a';
			}
			cout << " " << setw(4) << left << j << "         " << setw(4) << left << i << "           " << ch;
		}
		cout << endl;
	}
	cout << endl << "\tYour Desired Value According To Logical Address Is Stored At Offset Number: " << offset << " Of Frame: " << frameNo << " In RAM" << endl << endl;
	cout << endl << "  Working:" << endl;
	cout << "           Physical Address = [(Frame Number * Frame Size) + Offset]" << endl;
	cout << "                            = [(" << frameNo << " * " << fixedFrameSize << ") + " << offset << "]" << endl;
	cout << "                            = [(" << frameNo * fixedFrameSize << ") + " << offset << "]" << endl;
	cout << "                            = [" << frameNo * fixedFrameSize << " + " << offset << "]" << endl;
	cout << "                            = [" << (frameNo * fixedFrameSize) + offset << "]" << endl;
}
PagedProcess_TwoLevel::PagedProcess_TwoLevel()
{
	process_ID = 0;
	process_Size = 0;
	page_Size = 0;
	numOfPages = 0;
	fixedOuterPageTableSize = 4;
	fixedInnerPageTableSize = 4;
	outerPageTable = NULL;
}
PagedProcess_TwoLevel::PagedProcess_TwoLevel(const PagedProcess_TwoLevel& obj)
	 {
		 outerPageTable = NULL;
		 copyTwoLevel(obj);
	 }
int  PagedProcess_TwoLevel::getProcessID()
{
	return process_ID;
}
int  PagedProcess_TwoLevel::getNumOfPages()
{
	return numOfPages;
}
void PagedProcess_TwoLevel::initialize()
{
	process_ID = getRandomDigit(3);
	process_Size = getProcessSize("Process Size In Bytes");
	page_Size = fixedFrameSize;
	numOfPages = (process_Size / page_Size);
	if (process_Size % page_Size != 0)
	{
		numOfPages++; // 1 extra page to handle the REMAINING BITS OF PAGE
	}
	cout << "\t\t Creating Two Level Page Table ..." << endl;
	Sleep(700);
	updatePageTableSize();
	outerPageTable = new int* [fixedOuterPageTableSize];
	for (int i = 0; i < fixedOuterPageTableSize; i++)
	{
		outerPageTable[i] = NULL;
	}
	cout << "\t\t Mapping Pages Into Two Level Page Table ..." << endl;
	Sleep(800);
	bool isAllocated = true;
	int remainingPages = numOfPages,i=0,j=0,k=0, bytes = process_Size;
	while (remainingPages > 0 && isAllocated)
	{
		if (outerPageTable[i] == NULL)
		{
			outerPageTable[i] = new int[fixedInnerPageTableSize];
			for (int x = 0; x < fixedInnerPageTableSize && remainingPages > 0; x++)
			{
				outerPageTable[i][x] = INT_MIN;
				if (RAM.isFreeFrameAvailable())
				{
					int frameNo = (RAM.getFreeFrameIndex() / fixedFrameSize);
					outerPageTable[i][x] = frameNo;
					RAM.allocateFrame(frameNo * fixedFrameSize, bytes);
					remainingPages--;
					if (bytes < fixedFrameSize)
					{
						bytes = 0;
					}
					else
					{
						bytes -= fixedFrameSize;
					}
				}
				else
				{
					cout << endl << "\t\tSOME PAGES WERE NOT SUCCESFULLY MAPPED INTO RAM DUE TO INSUFFICIENT SPACE IN RAM" << endl;
					isAllocated = false;
					break;
				}
			}
			i++;
		}
		if (i >= fixedOuterPageTableSize && remainingPages > 0)
		{
			cout << endl << "\t\tSOME PAGES WERE NOT SUCCESFULLY MAPPED INTO RAM DUE TO INSUFFICEINT SIZES OF PAGE TABLES" << endl;
			isAllocated = false;
			break;
		}
	}
}
void PagedProcess_TwoLevel::getLogicalAddress()
{
	int logicalAddress, pageNo, offset, frameNo, physicalAddress,outerPageIndex,innerPageIndex;
	do
	{
		cout << "Enter Logical Address You Wish To FETCH [indexing starts from 0]: ";
		cin >> logicalAddress;
		if (logicalAddress >= 0 && logicalAddress < process_Size)
		{
			break;
		}
		else
		{
			cout << "\t INVALID INPUT: INCORRECT LOGICAL ADDRESS REQUESTED" << endl;
		}
	} while (1);

	pageNo = logicalAddress / fixedFrameSize;
	offset = logicalAddress % fixedFrameSize;
	outerPageIndex = pageNo / fixedInnerPageTableSize;
	innerPageIndex = pageNo-(outerPageIndex* fixedInnerPageTableSize);
	cout << "\t\t Checking Two Level Page Table ..." << endl;
	Sleep(900);
	if ((frameNo= getFrameNumber(outerPageIndex, innerPageIndex)) < 0)
	{
		cout << "\t\t FAILURE: Required Page Number Isnt Mapped Into RAM ..." << endl;
		cout << "\t\t                      EXITING  ..." << endl;
		Sleep(600);
		return;
	}
	physicalAddress = (frameNo * fixedFrameSize) + offset;

	cout << endl << "\t\t Your  Logical Address : " << logicalAddress;
	cout << endl << "\t\t Your     Page Number  : " << pageNo;
	cout << endl << "\t\t Your       Offset     : " << offset;
	cout << endl << "\t\t Your    Frame Number  : " << frameNo;
	cout << endl << "\t\t Your  Physical Address: " << physicalAddress;
	cout << endl << "\t\t Your  Outer Page Index: " << outerPageIndex;
	cout << endl << "\t\t Your  Inner Page Index: " << innerPageIndex;
	cout << endl << endl;
	cout << "\t\t Fetching Your Frame From RAM ..." << endl;
	Sleep(1500);
	printRandomFrame(frameNo, offset);
	entryOrExit(2);
}
int  PagedProcess_TwoLevel::getFrameNumber(int outerPageIndex,int innerPageIndex)
{
	int frameNo = -1;
	if (outerPageTable[outerPageIndex] != NULL&& outerPageIndex<fixedOuterPageTableSize)
	{
		if (innerPageIndex < fixedInnerPageTableSize)
		{
			frameNo = outerPageTable[outerPageIndex][innerPageIndex];
		}
	}
	return frameNo;
}
void PagedProcess_TwoLevel::terminate()
{
	cout << "\t\t Terminating Process " << process_ID << " ..." << endl;
	Sleep(1200);
	cout << "\t\t RAM Freed Up Succesfully ..." << endl;
	Sleep(500);
	for (int i = 0; i < fixedOuterPageTableSize; i++)
	{
		if (outerPageTable[i] != NULL)
		{
			for (int j = 0; j < fixedInnerPageTableSize; j++)
			{
				if (outerPageTable[i][j]>0)
				{
					RAM.deallocateFrame(outerPageTable[i][j] * fixedFrameSize);
				}
			}
		}
	}
}
void PagedProcess_TwoLevel::printTable()
{
	int w= getDigitsCount(numOfPages),x = getDigitsCount(RAM.getNumOfFrames()),y= getDigitsCount(fixedOuterPageTableSize);
	cout << endl << endl << "\tTwo Level Page Table: " << endl << endl;
	cout << endl << "\t\t  Index#               <Page#,Frame#> pairs";
	for (int i = 0, pg = 0; i < fixedOuterPageTableSize; i++)
	{
		cout << endl << "\t\t Index[" << setw(y) << right << i << "]: ";
		if (outerPageTable[i] == NULL)
		{
			cout << "     NULL";
			pg += fixedInnerPageTableSize;
		}
		else
		{
			cout << "     ";
			for (int j = 0; j < fixedInnerPageTableSize; j++, pg++)
			{
				if (outerPageTable[i][j] > 0)
				{
					cout << "<" << setw(w) << left << pg << "," << setw(x) << left << outerPageTable[i][j] << ">   ";
				}
			}
		}
	}
	cout << endl << endl;
	cout << "\n\tValues:" << endl;
	cout << "\t              Process ID         : " << process_ID<<endl;
	cout << "\t         Outer Page Table Size   : " << fixedOuterPageTableSize<<endl;
	cout << "\t         Inner Page Table Size   : " << fixedInnerPageTableSize<<endl;
	cout << "\t             Process Size (bytes): " << process_Size << endl;
    cout << "\t               Page Size  (bytes): " << page_Size<< endl;
    cout << "\t            Number Of Pages      : " << numOfPages<< endl;
	entryOrExit(2);
}
const PagedProcess_TwoLevel& PagedProcess_TwoLevel::operator = (const PagedProcess_TwoLevel& obj)
{
	if (this != &obj)
	{
		copyTwoLevel(obj);
	}
	return *this;
}
PagedProcess_TwoLevel::~PagedProcess_TwoLevel()
{
	destroyTwoLevel();
}

struct Node_Hash
{
	int pageNo;
	int frameNo;
	Node_Hash* next;
	Node_Hash();
	Node_Hash(int,int);
};
Node_Hash::Node_Hash()
{
	pageNo=INT_MIN;
	frameNo=INT_MIN;
	next = NULL;
}
Node_Hash::Node_Hash(int page,int frame)
{
	pageNo = page;
	frameNo = frame;
	next = NULL;
}

class HashTable
{
private:
	Node_Hash** list;
	int hashTableSize;	
	void copyHashTable(const HashTable&);
	void destroyHashTable();
	int hashFunction(int);
public:
	HashTable();
	HashTable(const HashTable&);
	int getHashTableSize();
	void createTable(int);
	void insert(Node_Hash*);
	Node_Hash* getHashNode(int);
	const HashTable& operator = (const HashTable&);
	void print();
	~HashTable();
	friend class PagedProcess_Hash;
};
void HashTable::copyHashTable(const HashTable& obj)
{
	if (list != NULL)
	{
		destroyHashTable();
	}
	hashTableSize = obj.hashTableSize;
	list = new Node_Hash * [hashTableSize];
	for (int i = 0; i < hashTableSize; i++)
	{
		list[i] = NULL;
		if (obj.list[i] != NULL)
		{
			Node_Hash* it = obj.list[i], * it2;
			list[i] = new Node_Hash(it->pageNo, it->frameNo);
			it = it->next;
			it2 = list[i];
			while (it != NULL)
			{
				it2->next = new Node_Hash(it->pageNo, it->frameNo);
				it = it->next;
				it2 = it2->next;
			}
		}
	}
}
void HashTable::destroyHashTable()
{
	for (int i = 0; i < hashTableSize; i++)
	{
		if (list[i] != NULL)
		{
			Node_Hash* it = list[i];
			while (it != NULL)
			{
				Node_Hash* del = it;
				it = it->next;
				delete del;
			}
		}
	}
	delete[] list;
	hashTableSize = 0;
}
int HashTable::hashFunction(int value)
{
	return (value)% hashTableSize;
}
HashTable::HashTable()
{
	list = NULL;
	hashTableSize = 0;
}
HashTable::HashTable(const HashTable& obj)
{
	list = NULL;
	copyHashTable(obj);
}
int HashTable::getHashTableSize()
{
	return hashTableSize;
}
void HashTable::createTable(int size)
{
	if (size <= 0)
	{
		size = 3;
	}
	hashTableSize = size;
	cout << endl << "\t\tCreating Hash Table Of Size " << hashTableSize << " ..." << endl << endl;
	Sleep(600);
	list = new Node_Hash * [hashTableSize];
	for (int i = 0; i < hashTableSize; i++)
	{
		list[i] = NULL;
	}
}
void HashTable::insert(Node_Hash* node)
{
	int index = hashFunction(node->pageNo);

	if (list[index] == NULL)
	{
		list[index] = node;
	}
	else
	{
		Node_Hash* it = list[index];
		while (it->next != NULL)
		{
			it = it->next;
		}
		it->next = node;
	}
}
Node_Hash* HashTable::getHashNode(int pageNo)
{
	Node_Hash* ptr = NULL;
	int index = hashFunction(pageNo);
	if (list[index] != NULL)
	{
		bool found = false;
		Node_Hash* it = list[index];
		while (it != NULL && !found)
		{
			if (it->pageNo == pageNo)
			{
				ptr = it;
				found = true;
			}
			it = it->next;
		}
	}
	return ptr;
}
void HashTable::print()
{
	int x = getDigitsCount(RAM.getNumOfFrames()), y = getDigitsCount(hashTableSize);
	for (int i = 0; i < hashTableSize; i++)
	{
		cout << endl << "\t\t Index[" << setw(y) << right <<i<<"]: ";
		if (list[i] == NULL)
		{
			cout << "     NULL";
		}
		else
		{ 
			cout << "     ";
			Node_Hash* it = list[i];
			while (it != NULL)
			{
				cout << "<" << setw(x) << left << it->pageNo << "," << setw(x) << left << it->frameNo << ">   ";
				it = it->next;
			}
		}
	}
}
const HashTable& HashTable::operator = (const HashTable& obj)
{
	if (this != &obj)    //  CHECK IF OBJ DOESNT POINTS TO ITSELF 
	{
		copyHashTable(obj);  //  CALL COPY TO MAKE DEEP COPY
	}
	return *this;
}
HashTable::~HashTable()
{
	destroyHashTable();
}

class PagedProcess_Hash
{
private:
	int process_ID;
	int process_Size;
	int page_Size;
	int numOfPages;
	HashTable pageTable;
	void printRandomFrame(int,int);
public:
	PagedProcess_Hash();
	PagedProcess_Hash(const PagedProcess_Hash&);
	int getProcessID();
	int getNumOfPages();
	void initialize();
	void getLogicalAddress();
	int getFrameNumber(int);
	void terminate(); 
	void printTable();
	~PagedProcess_Hash();
};
void PagedProcess_Hash::printRandomFrame(int frameNo,int offset)
{
	cout << endl << endl << "\tFrame Fetched: " << frameNo<<endl<<endl;
	cout << "\t\t       Memory#     Offset#          Data" << endl;
	for (int i = 0,j= frameNo*fixedFrameSize; i < fixedFrameSize; i++,j++)
	{
		int t = rand() % 2;
		cout << "\t\t\t";
		if (t == 0)
		{
			cout <<" " << setw(4) << left << j << "         " << setw(4) << left << i << "           " << getRandomDigit(rand() % 6 + 2);
		}
		else
		{
			char ch = rand() % 26;
			int x = rand() % 2;
			if (x == 0)
			{
				ch += 'A';
			}
			else
			{
				ch += 'a';
			}
			cout << " " << setw(4) << left<< j << "         " << setw(4) << left << i << "           " << ch;
		}
		cout << endl;
	}
	cout <<endl<< "\tYour Desired Value According To Logical Address Is Stored At Offset Number: " << offset<<" Of Frame: " <<frameNo<<" In RAM"<< endl << endl;
	cout << endl << "  Working:" << endl;
	cout << "           Physical Address = [(Frame Number * Frame Size) + Offset]" << endl;
	cout << "                            = [("<<frameNo<<" * "<<fixedFrameSize<<") + "<<offset<<"]" << endl;
	cout << "                            = [("<<frameNo* fixedFrameSize<<") + "<<offset<<"]" << endl;
	cout << "                            = ["<<frameNo* fixedFrameSize<<" + "<<offset<<"]" << endl;
	cout << "                            = ["<<(frameNo* fixedFrameSize) + offset<<"]" << endl;
}
PagedProcess_Hash::PagedProcess_Hash()
{
	process_ID=0;
	process_Size=0;
	page_Size=0;
	numOfPages=0;
}
PagedProcess_Hash::PagedProcess_Hash(const PagedProcess_Hash& obj)
{
	process_ID=obj.process_ID;
	process_Size=obj.process_Size;
	page_Size=obj.page_Size;
	numOfPages=obj.numOfPages;
	pageTable = obj.pageTable;
}
int PagedProcess_Hash::getProcessID()
{
	return process_ID;
}
int PagedProcess_Hash::getNumOfPages()
{
	return numOfPages;
}
void PagedProcess_Hash::initialize()
{
	process_ID= getRandomDigit(3);
	process_Size = getProcessSize("Process Size In Bytes");
	page_Size = fixedFrameSize;
	numOfPages = (process_Size / page_Size);
	if (process_Size % page_Size != 0)
	{
		numOfPages++; // 1 extra page to handle the REMAINING BITS OF PAGE
	}
	pageTable.createTable((((rand() % 50 + 20))*numOfPages)/100);
	cout << "\t\t Mapping Pages Into Hash Table ..." << endl;
	Sleep(700);
	for (int i = 0,bytes= process_Size; i < numOfPages; i++)
	{
		if (RAM.isFreeFrameAvailable())
		{
			Node_Hash *node=new Node_Hash;
			node->frameNo = (RAM.getFreeFrameIndex()/fixedFrameSize);
			node->pageNo = i;
			RAM.allocateFrame(node->frameNo* fixedFrameSize, bytes);
			pageTable.insert(node);
			if(bytes < fixedFrameSize)
			{ 
				bytes = 0;
			}
			else {
				bytes -= fixedFrameSize;
			}
		}
		else
		{ 
			cout << endl << "\t\tSOME PAGES WERE NOT SUCCESFULLY MAPPED INTO RAM DUE TO INSUFFICIENT SPACE IN RAM" << endl;
			break;
		}
	}
}
void PagedProcess_Hash::getLogicalAddress()
{
	int logicalAddress,pageNo,offset,frameNo, physicalAddress;
	do
	{
		cout << "Enter Logical Address You Wish To FETCH [indexing starts from 0]: ";
		cin >> logicalAddress;
		if (logicalAddress >= 0 && logicalAddress < process_Size)
		{
			break;
		}
		else
		{
			cout << "\t INVALID INPUT: INCORRECT LOGICAL ADDRESS REQUESTED" << endl;
		}
	} while (1);


	pageNo =logicalAddress / fixedFrameSize;
	offset =logicalAddress % fixedFrameSize;
	cout << "\t\t Checking TLB Table ..." << endl;
	Sleep(900);
	if (TLB.searchTLB(process_ID, pageNo))
	{
		cout << "\t\t SUCCESS: TLB HIT ..." << endl;
		Sleep(500);
		frameNo = TLB.getFrameNum(process_ID, pageNo);

	}
	else
	{
		cout << "\t\t FAILURE: TLB MISS ..." << endl;
		Sleep(500);
		if ((frameNo = getFrameNumber(pageNo))< 0)
		{
			cout << "\t\t FAILURE: Required Page Number Isnt Mapped Into RAM ..." << endl;
			cout << "\t\t                      EXITING  ..." << endl;
			Sleep(600);
			return;
		}
		TLB.updateTLB(process_ID, pageNo, frameNo);
		cout << "\t\t Checking Hash Table ..." << endl;
		Sleep(1300);
	}
	physicalAddress = (frameNo * fixedFrameSize) + offset;

	cout << endl << "\t\t Your  Logical Address : " << logicalAddress;
	cout << endl << "\t\t Your     Page Number  : " << pageNo;
	cout << endl << "\t\t Your       Offset     : " << offset;
	cout << endl << "\t\t Your    Frame Number  : " << frameNo;
	cout << endl << "\t\t Your  Physical Address: " << physicalAddress;
	cout << endl << endl;
	cout << "\t\t Fetching Your Frame From RAM ..." <<endl;
	Sleep(1500);
	printRandomFrame(frameNo, offset);
	entryOrExit(2);
}
int PagedProcess_Hash::getFrameNumber(int pageNo)
{
	Node_Hash *node = pageTable.getHashNode(pageNo);
	if (node != NULL)
	{
		return node->frameNo;
	}
	return -1;
}
void PagedProcess_Hash::terminate()
{
	cout << "\t\t Terminating Process "<<process_ID << " ..." << endl;
	Sleep(1200);
	cout << "\t\t RAM Freed Up Succesfully ..." << endl;
	Sleep(500);
	for (int i = 0; i < pageTable.hashTableSize; i++)
	{
		if (pageTable.list[i] != NULL)
		{
			Node_Hash* it = pageTable.list[i];
			while (it != NULL)
			{
				RAM.deallocateFrame(it->frameNo*fixedFrameSize);
				it = it->next;
			}
		}
	}

}
void PagedProcess_Hash::printTable()
{
	cout << endl << endl << "\tHash Table: " << endl << endl;
	cout << endl  << "\t\t  Index#               <Page#,Frame#> pairs";
	pageTable.print();
	cout << endl << endl;
	cout << "\n\tValues:" << endl;
	cout << "\t              Process ID         : " << process_ID << endl;
	cout << "\t             Process Size (bytes): " << process_Size << endl;
	cout << "\t               Page Size  (bytes): " << page_Size << endl;
	cout << "\t            Number Of Pages      : " << numOfPages << endl;
	entryOrExit(2);
}
PagedProcess_Hash::~PagedProcess_Hash()
{
	process_ID = 0;
	process_Size = 0;
	page_Size = 0;
	numOfPages = 0;
}

void printProcessesList(vector< PagedProcess_TwoLevel>& Processes)
{
	cout << endl << endl << "\t\tYour Processes Are: " << endl << "\t\t\t";
	for (int i = 0; i < Processes.size(); i++)
	{
		cout << "P" << i + 1 << "(" << Processes[i].getProcessID() << ") ";
	}
	cout << endl << endl << endl;
}
void printProcessesList(vector< PagedProcess_Hash>& Processes)
{
	cout << endl << endl << "\t\tYour Processes Are: " << endl << "\t\t\t";
	for (int i = 0; i < Processes.size(); i++)
	{
		cout << "P" << i + 1 << "(" << Processes[i].getProcessID() << ") ";
	}
	cout << endl << endl << endl;
}
void TwoLevelPageTableMenuDisplay()
{
	cout << endl;
	cout << "\t\t\t**** TWO LEVEL PAGE TABLE ****" << endl;
	cout << "\t\t\t       0.Back" << endl;
	cout << "\t\t\t       1.Add A Process" << endl;
	cout << "\t\t\t       2.Fetch A Process's Logical Address" << endl;
	cout << "\t\t\t       3.Terminate A Process" << endl;
	cout << "\t\t\t       4.Print A Process's Two Level Page Table" << endl;
	cout << "\t\t\t       5.Print Processes List" << endl;
	cout << "\t\t\t       6.View RAM" << endl;
	cout << endl;
}
void Two_Level_Scheme_Paging()
{
	system("CLS");
	cout << endl << endl << endl;
	int n1 = getMemorySize("RAM Size In Bytes");
	RAM.initializeSize(n1);
	RAM.initializeMemory();
	system("CLS");
	vector <PagedProcess_TwoLevel> Processes;
	int option;
	do
	{
		TwoLevelPageTableMenuDisplay();
		cout << "Enter Option: ";
		cin >> option;
		if (option == 0)
		{
			break;
		}
		else if (option == 1)
		{
			PagedProcess_TwoLevel process;
			process.initialize();
			Processes.push_back(process);
		}
		else if (option == 2)
		{
			if (Processes.size() > 0)
			{
				printProcessesList(Processes);
				int processNo;
				do
				{
					cout << "Enter Process Number You Wish To Get: ";
					cin >> processNo;
					if (processNo - 1 >= 0 && processNo - 1 < Processes.size())
					{
						break;
					}
					else
					{
						cout << "\t INVALID INPUT" << endl;
					}
				} while (1);
				cout << endl << endl;
				Processes[processNo - 1].getLogicalAddress();
			}
			else
			{
				cout << endl << endl << "\t\t NO Processes In The Processes List" << endl << endl;
			}
		}
		else if (option == 3)
		{
			if (Processes.size() > 0)
			{
				printProcessesList(Processes);
				int processNo;
				do
				{
					cout << "Enter Process Number You Wish To Terminate: ";
					cin >> processNo;
					if (processNo - 1 >= 0 && processNo - 1 < Processes.size())
					{
						break;
					}
					else
					{
						cout << "\t INVALID INPUT" << endl;
					}
				} while (1);
				cout << endl << endl;

				Processes[processNo - 1].terminate();
				for (int i = processNo - 1; i + 1 < Processes.size(); i++)
				{
					Processes[i] = Processes[i + 1];
				}
				Processes.pop_back();
			}
			else
			{
				cout << endl << endl << "\t\t NO Processes In The Processes List" << endl << endl;
			}
		}
		else if (option == 4)
		{
			if (Processes.size() > 0)
			{
				printProcessesList(Processes);
				int processNo;
				do
				{
					cout << "Enter Process Number You Wish To Get: ";
					cin >> processNo;
					if (processNo - 1 >= 0 && processNo - 1 < Processes.size())
					{
						break;
					}
					else
					{
						cout << "\t INVALID INPUT" << endl;
					}
				} while (1);
				cout << endl << endl;
				Processes[processNo - 1].printTable();
			}
			else
			{
				cout << endl << endl << "\t\t NO Processes In The Processes List" << endl << endl;
			}
		}
		else if (option == 5)
		{
			if (Processes.size() > 0)
			{
				printProcessesList(Processes);
			}
			else
			{
				cout << endl << endl << "\t\t NO Processes In The Processes List" << endl << endl;
			}
		}
		else if (option == 6)
		{
			RAM.print();
		}
	} while (1);
	RAM.clear();
	system("CLS");
}
void HashedTableWithTLBMenuDisplay()
{
	cout << endl;
	cout << "\t\t\t**** HASH TABLE WITH TLB ****" << endl;
	cout << "\t\t\t       0.Back" << endl;
	cout << "\t\t\t       1.Add A Process" << endl;
	cout << "\t\t\t       2.Fetch A Process's Logical Address" << endl;
	cout << "\t\t\t       3.Terminate A Process" << endl;
	cout << "\t\t\t       4.Print A Process's Hash Table" << endl;
	cout << "\t\t\t       5.Print Processes List" << endl;
	cout << "\t\t\t       6.View RAM" << endl;
	cout << "\t\t\t       7.View TLB Table" << endl;
	cout << endl;
}
void Hashed_Table_With_TLB()
{
	system("CLS");
	cout << endl<<endl<<endl;
	int n1 = getMemorySize("RAM Size In Bytes");
	RAM.initializeSize(n1);
	RAM.initializeMemory();
	system("CLS");
	vector <PagedProcess_Hash> Processes;
	int option;
	do
	{
		HashedTableWithTLBMenuDisplay();
		cout << "Enter Option: ";
		cin >> option;
		if (option == 0)
		{
			break;
		}
		else if (option == 1)
		{
			PagedProcess_Hash process;
			process.initialize();
			Processes.push_back(process);
		}
		else if (option == 2)
		{
			if (Processes.size() > 0)
			{
				printProcessesList(Processes);
				int processNo;
				do
				{
					cout << "Enter Process Number You Wish To Get: ";
					cin >> processNo;
					if (processNo-1 >= 0 && processNo-1 < Processes.size())
					{
						break;
					}
					else
					{
						cout << "\t INVALID INPUT" << endl;
					}
				} while (1);
				cout << endl << endl;
				Processes[processNo - 1].getLogicalAddress();
			}
			else
			{
				cout << endl << endl<<"\t\t NO Processes In The Processes List" << endl<<endl;
			}
		}
		else if (option == 3)
		{
			if (Processes.size() > 0)
			{
				printProcessesList(Processes);
				int processNo;
				do
				{
					cout << "Enter Process Number You Wish To Terminate: ";
					cin >> processNo;
					if (processNo - 1 >= 0 && processNo - 1 < Processes.size())
					{
						break;
					}
					else
					{
						cout << "\t INVALID INPUT" << endl;
					}
				} while (1);
				cout << endl << endl;

				Processes[processNo - 1].terminate();
				for (int i = processNo - 1; i+1 < Processes.size(); i++)
				{
					Processes[i] = Processes[i+1];
				}
				Processes.pop_back();
			}
			else
			{
				cout << endl << endl << "\t\t NO Processes In The Processes List" << endl << endl;
			}
		}
		else if (option == 4)
		{
			if (Processes.size() > 0)
			{
				printProcessesList(Processes);
				int processNo;
				do
				{
					cout << "Enter Process Number You Wish To Get: ";
					cin >> processNo;
					if (processNo - 1 >= 0 && processNo - 1 < Processes.size())
					{
						break;
					}
					else
					{
						cout << "\t INVALID INPUT" << endl;
					}
				} while (1);
				cout << endl << endl;
				Processes[processNo - 1].printTable();
			}
			else
			{
				cout << endl << endl << "\t\t NO Processes In The Processes List" << endl << endl;
			}
		}
		else if (option == 5)
		{
			if (Processes.size() > 0)
			{
				printProcessesList(Processes);
			}
			else
			{
				cout << endl << endl << "\t\t NO Processes In The Processes List" << endl << endl;
			}
		}
		else if (option == 6)
		{
			RAM.print();
		}
		else if (option == 7)
		{
			TLB.print();
		}
	} while (1);
	RAM.clear();
	system("CLS");
}
void MemoryManagementMenuDisplay()
{
	cout << endl;
	cout << "\t\t\t**** MEMORY MANAGEMENT ****" << endl;
	cout << "\t\t\t       0.Back" << endl;
	cout << "\t\t\t       1.Two Level Scheme Paging" << endl;
	cout << "\t\t\t       2.Hashed Table With TLB" << endl;
	cout << endl;
}
void memoryManagement()
{
	if (!isLog2(fixedFrameSize))
	{
		fixedFrameSize = 16;
	}
	system("CLS");
	int option;
	do
	{
		MemoryManagementMenuDisplay();
		cout << "Enter Option: ";
		cin >> option;
		if (option == 0)
		{
			break;
		}
		else if (option == 1)
		{
			Two_Level_Scheme_Paging();
		}
		else if (option == 2)
		{
			Hashed_Table_With_TLB();
		}
	} while (1);
	system("CLS");
}
/////////////
double pageFault = 0;
struct Node_PageFault
{
	int data;
	int bit;
	Node_PageFault* next;
	Node_PageFault();
	Node_PageFault(int,int);
};
Node_PageFault::Node_PageFault()
{
	data= INT_MIN;
    bit=INT_MIN;
	next=NULL;
}
Node_PageFault::Node_PageFault(int Data, int Bit)
{
	data = Data;
	bit = Bit;
	next = NULL;
}

void insert(Node_PageFault** head, Node_PageFault** tail, Node_PageFault** node)
{
	if (*head == NULL)
	{
		*head = *node;
		*tail = *node;
		(*tail)->next = *head;
	}
	else
	{
		(*tail)->next = *node;
		(*tail) = *node;
		(*tail)->next = *head;
	}
}
void destroyCircularBuffer(Node_PageFault* head)
{
	if (head != NULL)
	{
		Node_PageFault* ptr = head->next;
		head->next = NULL;
		while (ptr != NULL)
		{
			Node_PageFault* del = ptr;
			ptr = ptr->next;
			delete del;
		}
	}
}
Node_PageFault* createCricularBuffer(vector<int>& refrencedPagesString, int circularBufferSize)
{
	cout << endl << "\t\t\t Creating Circular Buffer Linked List ..." << endl << endl;
	Sleep(1100);
	int check2 = 1, start_condition = 0;
	Node_PageFault* head = NULL,* tail = NULL;
	if (refrencedPagesString.size() < circularBufferSize)
	{
		for (int i = 0; i < refrencedPagesString.size(); i++)
		{
			if (start_condition > 0)
			{
				Node_PageFault* ptr3 = head;
				do
				{
					if (ptr3->data == refrencedPagesString[i])
					{
						i++;
						check2 = 0;
						break;
					}
					else
					{
						check2 = 1;
					}
					ptr3 = ptr3->next;
				} while (ptr3 != head);
			}
			if (check2 == 1)
			{
				start_condition++;
				Node_PageFault* newNode = new Node_PageFault(refrencedPagesString[i],1);
				cout << "\t\tALERT: Page Fault For Page Number " << refrencedPagesString[i] << " ..." << endl;
				pageFault++;
				insert(&head,&tail,&newNode);
			}
		}
	}
	else 
	{
		for (int i = 0; i < circularBufferSize; i++)
		{
			if (start_condition > 0)
			{
				Node_PageFault* ptr4 = head;
				do
				{
					if (ptr4->data == refrencedPagesString[i])
					{
						i++;
						check2 = 0;
						break;
					}
					else
					{
						check2 = 1;
					}
					ptr4 = ptr4->next;
				} while (ptr4 != head);
			}
			start_condition++;
			if (check2 == 1)
			{
				Node_PageFault* newNode = new Node_PageFault(refrencedPagesString[i], 1);
				cout << "\t\tALERT: Page Fault For Page Number " << refrencedPagesString[i] << " ..." << endl;
				pageFault++;
				insert(&head, &tail, &newNode);
			}
		}
	}
	cout << endl << "\t\t\t Circular Buffer Created Succesfully ..." << endl << endl << endl;
	Sleep(800);
	return head;
}
void secondChanceAlgorithm(vector<int>& refrencedPagesString, int circularBufferSize, Node_PageFault* head)
{
	cout << endl << "\t\t\t Running Second Chance Algorithm ..." << endl << endl;
	Sleep(600);
	if (head != NULL)
	{
		Node_PageFault* ptr = head, * ptr2 = head;
		while (circularBufferSize < refrencedPagesString.size())
		{
			bool found = false;
			do
			{
				if (ptr->data == refrencedPagesString[circularBufferSize])
				{
					cout << "\t\t Page Number " << ptr->data << " Is Already In Memory ..." << endl;
					cout << "\t\t     No Need To Change Bit To 1 ..." << endl << endl;
					Sleep(300);
					ptr->bit = 1;
					ptr = head;
					circularBufferSize++;
					found = true;
				}
				ptr = ptr->next;
			} while (ptr != head && !found);
			if (!found)
			{
				cout << "\t\tALERT: Page Fault For Page Number " << refrencedPagesString[circularBufferSize] << " ..." << endl;
				Sleep(650);
				cout << "\t\tReplacing With Circular Buffer Value " << ptr2->data << " ..." << endl << endl;
				Sleep(650);
				while (ptr2->bit)
				{
					ptr2->bit = 0;
					ptr2 = ptr2->next;
				}
				ptr2->data = refrencedPagesString[circularBufferSize++];
				ptr2->bit = 1;
				ptr2 = ptr2->next;
				pageFault++;
			}
		}
	}
}
void pageReplacement()
{
	int referenceStringSize, circularBufferSize; 
	vector<int>referencedPagesString;
	referenceStringSize= getInput("Size Of Referenced String");
	for (int i = 0; i < referenceStringSize; i++)
	{
		referencedPagesString.push_back(getInput("Page Number"));
	}
	circularBufferSize = getInput("Size Of Circular Buffer");
	entryOrExit(1);
	Node_PageFault* buffer = createCricularBuffer(referencedPagesString, circularBufferSize);
	secondChanceAlgorithm(referencedPagesString, circularBufferSize, buffer);
	double  pagefaultprob = pageFault / referencedPagesString.size(),pagefaultperc = pagefaultprob * 100;
	cout << endl << endl;
	cout << "\t\t Number Of Page Faults: " << pageFault << endl;
	cout << "\t\tPage Fault Probability: " << pagefaultprob << endl;
	cout << "\t\t Page Fault Percentage: " << pagefaultperc << "%" << endl;
	entryOrExit(2);
	destroyCircularBuffer(buffer);
	pageFault = 0;
}
/////////////
void MainMenuDisplay()
{
	cout << endl;
	cout << "\t\t\t**** OPERATING SYSTEM SIMULATOR ****" << endl;
	cout << "\t\t\t             0.Exit" << endl;
	cout << "\t\t\t             1.CPU Scheduling" << endl;
	cout << "\t\t\t             2.Semaphore" << endl;
	cout << "\t\t\t             3.Deadlock" << endl;
	cout << "\t\t\t             4.Memory Management" << endl;
	cout << "\t\t\t             5.Page Replacement" << endl;
	cout << endl;
}
int main()
{
	srand(time(NULL));
	int option;
	do
	{
		MainMenuDisplay();
		cout << "Enter Option: ";
		cin >> option;
		if (option == 0)
		{
			break;
		}
		else if (option == 1)
		{
			CPU_Scheduling();
		}
		else if (option == 2)
		{
			semaphore();
		}
		else if (option == 3)
		{
			deadlock();
		}
		else if (option == 4)
		{
			memoryManagement();
		}
		else if (option == 5)
		{
			pageReplacement();
		}
	} while (1);
	return 0;
}