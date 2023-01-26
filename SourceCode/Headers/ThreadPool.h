#pragma once
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <vector>
#include <iostream>
#include <functional>

#include <ResourceManager.h>
#include <Task.h>


class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	void StartThread();
	void ThreadFunction();
	void AddQueue(Task task);
	void DeleteJobs();

private:
	bool stopProg;
	int maxThread;
	std::queue<Task> jobs;
	std::vector<std::thread> workers;
	std::atomic_flag signal;
};