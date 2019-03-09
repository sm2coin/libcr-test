#include "Benchmark.hpp"
#include <vector>

namespace cr::test
{
	timer::Timer<>::duration_t Benchmark::operator()()
	{
		prepare();

		timer::Timer timer;
		timer.start();
		execute();
		timer.stop();

		destroy();

		return timer.elapsed();
	}

	Benchmark::BatchTimes Benchmark::batch(
		std::size_t iterations)
	{
		BatchTimes result;

		std::vector<timer::Timer<>::duration_t> times;
		times.resize(iterations);

		result.avg_duration = 0;
		for(std::size_t i = 0; i < iterations; i++)
		{
			timer::Timer<>::duration_t time = (*this)();
			result.avg_duration += time;

			// Find the location where the entry belongs using binary search.
			std::size_t left = 0;
			std::size_t right = i;
			std::size_t mid = (left+right) >> 1;
			while((left < right)
				&&(times[mid] != time))
			{
				if(times[mid] > time)
					right = mid;
				else
					left = mid+1;

				mid = (left + right) >> 1;
			}

			// Shift all entries to the right.
			for(std::size_t j = i; j-->mid;)
				times[j+1] = times[j];
			// Insert entry.
			times[mid] = time;
		}

		result.min_duration = times.front();
		result.avg_duration /= iterations;
		result.med_duration = times[times.size()>>1];
		result.max_duration = times.back();

		return result;
	}
}
