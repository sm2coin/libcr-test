#ifndef __libcr_test_benchmark_hpp_defined
#define __libcr_test_benchmark_hpp_defined

#include <timer/Timer.hpp>

namespace cr::test
{
	/** The benchmark interface. */
	class Benchmark
	{
	public:
		/** Executes the benchmark, and returns its execution time. */
		timer::Timer<>::duration_t operator()();

		struct BatchTimes
		{
			timer::Timer<>::duration_t min_duration;
			timer::Timer<>::duration_t avg_duration;
			timer::Timer<>::duration_t med_duration;
			timer::Timer<>::duration_t max_duration;
		};

		BatchTimes batch(
			std::size_t iterations);
	private:
		/** Prepares the execution environment. */
		virtual void prepare() = 0;
		/** Executes the code to be measured. */
		virtual void execute() = 0;
		/** Destroys the execution environment. */
		virtual void destroy() = 0;
	};
}

#endif