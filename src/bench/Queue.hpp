#ifndef __libcr_test_bench_queue_hpp_defined
#define __libcr_test_bench_queue_hpp_defined

#include "../Benchmark.hpp"
#include <libcr/libcr.hpp>

namespace cr::test::bench
{
	template<class QueueT>
	class Queue : public Benchmark
	{
		COROUTINE(Consumer, void)
		CR_STATE(
			(std::size_t) iterations,
			(QueueT &) queue,
			(std::size_t &) finished);
			int dummy;

			typename QueueT::Pop pop;

			void cr_destroy() {}
		CR_EXTERNAL

		COROUTINE(Producer, void)
		CR_STATE(
			(std::size_t) iterations,
			(QueueT &) queue,
			(std::size_t &) finished);

		typename QueueT::Push push;
			void cr_destroy() {}
		CR_EXTERNAL

		std::vector<Consumer> m_consumers;
		std::vector<Producer> m_producers;
		std::size_t m_iterations;
		QueueT m_queue;
		std::size_t m_finished;
		std::size_t m_coroutines;
	public:
		Queue(
			std::size_t coroutines,
			std::size_t iterations);

		void prepare() override;
		void execute() override;
		void destroy() override;

		static constexpr char const * name() { return "QueueBench"; }
	};
}

#include "Queue.inl"

#endif