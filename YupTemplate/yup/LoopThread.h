// ========================================================================== //
//
//  LoopThread.h
//  ---
//  The native app class
//
//  Created: 2016-08-24
//  Updated: 2016-09-09
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>
#include "yup.h"
#include "Thread.h"

BEGIN_NAMESPACE_YUP

// -------------------------------------------------------------------------- //
//  - Call run() to spawn the thread
//  - Call stop() to join the thread
// -------------------------------------------------------------------------- //
class LoopThread :
	public Thread
{
private:
	std::atomic_bool mStop = false;
	bool mInitialized = false;
	std::atomic_bool mInitResult;

	std::mutex mInitMutex;
	std::condition_variable mInitCond;

public:
	LoopThread() {}
	virtual ~LoopThread() { stop(); }

	void stop() {
		mStop = true;
		onStop();
		join();
	}

	bool stopping() { return mStop; }

protected:
	virtual bool init() = 0;
	virtual bool loop() = 0;
	virtual void shutdown() = 0;

	virtual void onStop() {}

	virtual void threadFunc() {
		mStop = false;

		{
			std::lock_guard<std::mutex> lk(mInitMutex);
			mInitResult = init();
			mInitialized = true;
		}

		mInitCond.notify_all();

		if (mInitResult)
			while (!mStop && loop());

		shutdown();
	}

public:
	bool waitForInitResult()
	{
		std::unique_lock<std::mutex> lk(mInitMutex);
		mInitCond.wait(lk, [&] { return mInitialized; });

		return mInitResult;
	}
};

END_NAMESPACE_YUP