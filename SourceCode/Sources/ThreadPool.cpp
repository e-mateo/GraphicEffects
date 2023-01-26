#include <ThreadPool.h>


ThreadPool::ThreadPool()
{
	stopProg = false;
	maxThread = std::thread::hardware_concurrency() / 2;
}

ThreadPool::~ThreadPool()
{
	stopProg = true;
	for (int i = 0; i < workers.size(); i++)
		workers[i].join();
}

void ThreadPool::StartThread()
{
	for (int i = 0; i < maxThread; i++)
		workers.push_back(std::thread(&ThreadPool::ThreadFunction, this));
}

void ThreadPool::ThreadFunction()
{
	while (!stopProg)
	{
		while (signal.test_and_set() && !stopProg)
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (jobs.size() != 0)
		{
			Task task = jobs.front();
			jobs.pop();
			signal.clear();
			task.func(task.filePath);
		}
		else
		{
			signal.clear();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

void ThreadPool::AddQueue(Task task)
{
	while (signal.test_and_set())
	{
		//Wait
	}

	jobs.push(task);

	signal.clear();
}

void ThreadPool::DeleteJobs()
{
	while (signal.test_and_set())
	{
		//Wait
	}

	while (!jobs.empty())
		jobs.pop();

	signal.clear();
}