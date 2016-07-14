#include <Log.h>
#include <LoopThread.h>

class DebugLooper :
	public LoopThread
{
	int i = 0;

	virtual bool init() {
		LogD("init!");
		return true;
	}
	
	virtual bool loop() {
	
		LogI("loop %d", ++i);

		return i < 10;
	}
	
	virtual void shutdown() {
		LogD("shutdown!");
	}
};


void main()
{
	Log::SetLogOutput(LO_FILE | LO_STDOUT | LO_DEBUGWINDOW);

	DebugLooper looper;
	looper.run();

	looper.join();
}