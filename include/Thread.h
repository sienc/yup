#pragma once

#include <thread>
#include "yup.h"

BEGIN_YUP_NAMESPACE
	
// Thread class
class Thread
{
	friend void ThreadFunc(Thread *thread);

private:
	std::thread * mThread = nullptr;

public:
	Thread() {}
	virtual ~Thread() { join(); }

	void run() {
		if (!mThread)
			mThread = new std::thread(ThreadFunc, this);
	}

	void join() {
		if (mThread)
		{
			if (mThread->joinable())
				mThread->join();

			delete mThread;
			mThread = nullptr;
		}
	}

protected:
	virtual void threadFunc() = 0;
};


// Thread function
static void ThreadFunc(yup::Thread *thread) {
	thread->threadFunc();
}


END_YUP_NAMESPACE
