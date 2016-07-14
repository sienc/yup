#pragma once

#include <atomic>
#include "yup.h"
#include "Thread.h"

BEGIN_YUP_NAMESPACE

class LoopThread :
	public Thread
{
private:
	std::atomic_bool mStop = false;

public:
	LoopThread() {}
	virtual ~LoopThread() { stop(); }

	void stop() {
		mStop = true;
		join();
	}

protected:
	virtual bool init() = 0;
	virtual bool loop() = 0;
	virtual void shutdown() = 0;

	virtual void threadFunc() {
		mStop = false;

		if (init())
			while (!mStop && loop());

		shutdown();
	}
};

END_YUP_NAMESPACE