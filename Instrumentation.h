// Instrumentation.h - Useful time based instrumentaiton for C++ Neil Chatterjee ©2020 Random Studio
#include <chrono>


namespace Instrumentation
{
#define _SAMPLE_BUFFER_SIZE 64 // how many samples will be taken when averaging things

#ifdef HI_RES_TIME_ // Will hurt performance. Only use if you need nanosecond accuracy.
	typedef std::chrono::high_resolution_clock _CLOCK;
#else
	typedef std::chrono::system_clock _CLOCK;
#endif
	typedef std::chrono::time_point<_CLOCK > _TIMEPOINT;
	typedef std::chrono::milliseconds _MILLIS;

	/*
	@brief: elapsed timer - get the time elapsed since an event in seconds
	*/
	class ElapsedTimer
	{
		public:
					ElapsedTimer() { Reset(); }
		float		GetElapsed();
		void		Reset();
	protected:
		_TIMEPOINT  event_time_;
	};

	/*
	@brief: A countdown of a given number of seconds
	*/
	class Countdown
	{
	public:
					Countdown(int inSeconds);					// Construct and start the clock
		bool		isExpired() const;							// Has the clock expired?
		void		reset() { start_time_ = _CLOCK::now(); }	// Restart the clock
	protected:
		_TIMEPOINT start_time_;									// When was the clock reset
		long span_;												// How long is the timer set for
	};


	/*
	@brief: Averages how many MS this timer runs between start and stop.
	Sample size of 64, using ring buffer averaging.
	*/
	class AvgTimer
	{
	public:
		void		start();									// start the timer
		void		stop();										// stop the timer
		float		getAverageMs() const;						// Avg. ms the timers' running for
		float		getAverageSeconds() const { return getAverageMs() / 1000.f; } // Avg seconds the timers' running for
		void		clear() { idx_ = 0; sample_count_ = 0; }	// Clear all samples
	protected:
		_TIMEPOINT start_time_;									// When timer was started
		float sample_buffer[_SAMPLE_BUFFER_SIZE];				// Ring buffer for averages
		int sample_count_ = 0;									// How many samples have we taken
		int idx_ = 0;											// current idx in ring buffer
	};

	/*
	@brief: Counts how often "tick" is called per second. Should be used for frequent things.
	*/
	class Counter : private AvgTimer
	{
	public:
		void		tick();					// Call when the thing you want to measure happens
		float		rate() const;			// rate in ticks/second
		void		reset() { clear(); }	// Clear averaging

	protected:
		bool started_ = false;
	};

	/*
	@brief: Moving float average over a number of second. TODO - make this a template class so we can handle nonfloats
	*/
	class MovingAverage
	{
	public:
		MovingAverage(int inSeconds) : span_(inSeconds * 1000) {}// Construct. How long will we average for?

		void push(float inValue);								// Register a value
		float average() const { return moving_average_; }		// What's the current moving average?

	private:
		long span_;												// How long are we taking moving average for
		_TIMEPOINT last_measure_time_;							// When did we last push
		bool has_first_value_ = false;							// Have we received any pushes yet?
		float moving_average_ = FLT_MAX;						// moving average
	};
}

