#ifndef __libcr_test_bench_scheduler_hpp_defined
#define __libcr_test_bench_scheduler_hpp_defined

#include "../Benchmark.hpp"

#include <vector>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace cr::test::bench
{
	template<class Scheduler>
	/** Benchmarks a multithreaded scheduler.
	@tparam Scheduler:
		The scheduler to benchmark. Has to support multi-threading.*/
	class MtScheduler : public Benchmark
	{
		/** Simple coroutine that yields `iterations` times. */
		COROUTINE(Yielder, Scheduler)
		CR_STATE
			std::size_t i;
			MtScheduler<Scheduler> * testbench;

			inline void cr_prepare(
				std::size_t iterations,
				MtScheduler<Scheduler> * testbench);
			inline void cr_destroy();
		CR_EXTERNAL

		/** The coroutines used to benchmark the scheduler. */
		Yielder * m_coroutines;
		/** The coroutine count. */
		std::size_t m_coroutine_count;
		/** The yields per coroutine. */
		std::size_t m_iterations;
		/** The number of coroutines still running. */
		std::atomic_size_t m_running;

		/** The condition variable signalling the main thread when the benchmark is over. */
		std::condition_variable m_done;
		/** The mutex protecting the condition variable. */
		std::mutex m_done_mutex;
		/** The lock instance that belongs to the mutex. */
		std::unique_lock<std::mutex> m_done_mutex_lock;
		/** The scheduling threads. */
		std::vector<std::thread> m_threads;

		/** */
		static void scheduling_thread(
			MtScheduler<Scheduler> * scheduler);
		/** Notifies the testbench that a coroutine finished. */
		void notify();
	public:
		/** Makes `coroutines` coroutines yield `iterations` times each to the scheduler.
		@param[in] coroutines:
			The number of coroutines to run.
		@param[in] iterations:
			The number of iterations per coroutine. */
		MtScheduler(
			std::size_t coroutines,
			std::size_t iterations);

		~MtScheduler();

		void prepare() override;
		void execute() override;
		void destroy() override;

		static constexpr char const * name() { return "MtSchedulerBench"; }
	};
}

#include "MtScheduler.inl"

#endif