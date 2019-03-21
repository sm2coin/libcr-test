#ifndef __libcr_test_bench_scheduler_hpp_defined
#define __libcr_test_bench_scheduler_hpp_defined

#include "../Benchmark.hpp"

#include <libcr/libcr.hpp>

namespace cr::test::bench
{
	template<class SchedulerT>
	class Scheduler : public Benchmark
	{
		COROUTINE(Yielder, SchedulerT)
		CR_STATE(
			(std::size_t) iterations,
			(Scheduler<SchedulerT> *) testbench)
			inline void cr_destroy();
		CR_EXTERNAL

		Yielder * m_coroutines;
		std::size_t m_iterations;
		std::size_t m_coroutine_count;
		std::size_t m_finished;
	public:
		Scheduler(
			std::size_t coroutines,
			std::size_t iterations);

		void prepare() override;
		void execute() override;
		void destroy() override;

		static constexpr char const * name() { return "SchedulerBench"; }
	};
}

#include "Scheduler.inl"

#endif