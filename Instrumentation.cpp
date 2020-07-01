#include "Instrumentation.h"

namespace Instrumentation
{
	// Countdown constructor
	Countdown::Countdown(int inSeconds) : start_time_(_CLOCK::now()) { span_ = inSeconds * 1000; }


	// Has the countdown expired?
	bool Countdown::isExpired() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(_CLOCK::now() - start_time_).count() > span_;
	}


	// Start the average timer
	void AvgTimer::start()
	{
		start_time_ = _CLOCK::now();
	}


	// stoip the average timer
	void AvgTimer::stop()
	{
		// push the elapsed time in milliseconds into the sample buffer
		sample_buffer[idx_] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(_CLOCK::now() - start_time_).count()) / 1000000.0f;
		// increment the index
		idx_++;
		idx_ = idx_ % _SAMPLE_BUFFER_SIZE;
		// increment the sample count
		sample_count_++;
		if (sample_count_ > _SAMPLE_BUFFER_SIZE) sample_count_ = _SAMPLE_BUFFER_SIZE;
	}


	// Get average time. Only do the math on request
	float AvgTimer::getAverageMs() const
	{
		// How many samples do we have? (is the ring full)
		int end = _SAMPLE_BUFFER_SIZE;
		if (sample_count_ < _SAMPLE_BUFFER_SIZE)
			end = sample_count_;

		// Determine the average by adding fractions to keep accuracy and not run out of bits
		float result = 0;
		for (int i = 0; i < end; i++)
		{
			result += sample_buffer[i] / (float)end;
		}
		return result;
	}


	// Increment a counter
	void Counter::tick()
	{
		//  Treat this as an average timer stop & start.
		if (started_)
		{
			stop();
		}
		started_ = true;
		start();
	}


	// Get rate. (rate = 1/time)
	float Counter::rate() const
	{
		return 1.0f / getAverageSeconds();
	}


	// Push a value to the moving average
	void MovingAverage::push(float inValue)
	{
		// remember what time this was measured
		_TIMEPOINT this_measure_time = _CLOCK::now();
		
		if (!has_first_value_)
		{
			// If this is the first value, it's the average
			moving_average_ = inValue;
			has_first_value_ = true;
		}
		else 
		{
			// EMA
			long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(this_measure_time - last_measure_time_).count();
			double long alpha = -elapsed  / (double long)span_;
			alpha = exp2l(alpha);
			alpha = 1.0L - alpha;
			moving_average_ = alpha * inValue + (1.0f - alpha) * moving_average_;

		}

		// update measure times
		last_measure_time_ = this_measure_time;
	}
}

