#ifndef __libcr_test_bench_consumableevent_hpp_defined
#define __libcr_test_bench_consumableevent_hpp_defined

#include "../Benchmark.hpp"

#include <libcr/libcr.hpp>

namespace cr::test::bench
{
	template<class EventT>
	class ConsumableEvent : public Benchmark
	{
		COROUTINE(Waiter, void)
		CR_STATE
			std::size_t i;
			EventT * event;
			std::size_t * finished;

			inline void cr_prepare(
				std::size_t iterations,
				EventT * event,
				std::size_t * finished);
			inline void cr_destroy();
		CR_EXTERNAL

		std::vector<Waiter> m_coroutines;
		std::size_t m_iterations;
		EventT m_event;
		std::size_t m_finished;

	public:
		ConsumableEvent(
			std::size_t coroutines,
			std::size_t iterations);

		void prepare() override;
		void execute() override;
		void destroy() override;

		static constexpr char const * name() { return "EventBench"; }
	};
}

#include "ConsumableEvent.inl"

#endif