#ifndef __libcr_test_bench_event_hpp_defined
#define __libcr_test_bench_event_hpp_defined


#include "../Benchmark.hpp"

#include <libcr/libcr.hpp>

namespace cr::test::bench
{
	template<class EventT>
	class Event : public Benchmark
	{
		COROUTINE(Waiter, void)
		CR_STATE
			std::size_t i;
			EventT * events;
			std::size_t * finished;

			inline void cr_prepare(
				std::size_t iterations,
				EventT * events,
				std::size_t * finished);
			inline void cr_destroy();
		CR_EXTERNAL

		std::vector<Waiter> m_coroutines;
		std::size_t m_iterations;
		EventT m_events[2];
		std::size_t m_finished;

	public:
		Event(
			std::size_t coroutines,
			std::size_t iterations);

		void prepare() override;
		void execute() override;
		void destroy() override;

		static constexpr char const * name() { return "EventBench"; }
	};
}

#include "Event.inl"

#endif