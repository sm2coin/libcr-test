#include <libcr/libcr.hpp>
#include "bench/MtScheduler.hpp"
#include "bench/Scheduler.hpp"
#include "bench/Event.hpp"
#include "bench/ConsumableEvent.hpp"
#include "bench/Queue.hpp"
#include "MtScheduler.hpp"
#include <iostream>
#include <climits>

using namespace cr::test;

template<class Benchmark>
void benchmark(
	char const * name,
	std::size_t coroutines,
	std::size_t iterations,
	std::size_t batch)
{
	std::size_t const k_mega = 1000000;
	std::size_t switches = coroutines * iterations;
	Benchmark bench {coroutines, iterations};

	std::cout << "\n+===============================================================================\n";
	std::cout << "| " << Benchmark::name() << "<" << name << ">: " << batch << " batches (" << coroutines << ", " << iterations << ")\n";
	std::cout << "+-------------------------------------------------------------------------------\n";
	cr::test::Benchmark::BatchTimes dur = bench.batch(batch);
	std::cout << "| Sum: " << dur.avg_duration * batch << "s\n";
	std::cout << "| Min: " << dur.min_duration << "s (" << switches / dur.min_duration / k_mega << "MHz)\n";
	std::cout << "| Avg: " << dur.avg_duration << "s (" << switches / dur.avg_duration / k_mega << "MHz)\n";
	std::cout << "| Med: " << dur.med_duration << "s (" << switches / dur.med_duration / k_mega << "MHz)\n";
	std::cout << "| Max: " << dur.max_duration << "s (" << switches / dur.max_duration / k_mega << "MHz)\n";
	std::cout << "+===============================================================================\n";

}

void test_mt_schedulers(
	std::size_t coroutines,
	std::size_t iterations,
	std::size_t batches)
{
	std::cout << "\n==== Multi-threaded tests ====\n";

	benchmark<bench::MtScheduler<cr::mt::Scheduler>>(
		"cr::mt::Scheduler",
		coroutines,
		iterations,
		batches);

	benchmark<bench::MtScheduler<cr::mt::FIFOScheduler>>(
		"cr::mt::FIFOScheduler",
		coroutines,
		iterations,
		batches);

	benchmark<bench::MtScheduler<MtScheduler>>(
		"MtScheduler",
		coroutines,
		iterations,
		batches);

	benchmark<bench::MtScheduler<FIFOMtScheduler>>(
		"FIFOMtScheduler",
		coroutines,
		iterations,
		batches);

	benchmark<bench::MtScheduler<cr::HybridScheduler<cr::mt::FIFOConditionVariable, cr::sync::ConditionVariable>>>(
		"HybridScheduler<mt::FIFO,sync::CV>",
		coroutines,
		iterations * 2,
		batches);
}

void test_sync_schedulers(
	std::size_t coroutines,
	std::size_t iterations,
	std::size_t batches)
{
	std::cout << "\n==== Single-threaded tests ====\n";

	benchmark<bench::Scheduler<cr::sync::Scheduler>>(
		"cr::sync::Scheduler",
		coroutines,
		iterations,
		batches);

	benchmark<bench::Scheduler<cr::sync::FIFOScheduler>>(
		"cr::sync::FIFOScheduler",
		coroutines,
		iterations,
		batches);

	benchmark<bench::Scheduler<cr::mt::Scheduler>>(
		"cr::mt::Scheduler",
		coroutines,
		iterations,
		batches);

	benchmark<bench::Scheduler<cr::mt::FIFOScheduler>>(
		"cr::mt::FIFOScheduler",
		coroutines,
		iterations,
		batches);

	benchmark<bench::Scheduler<cr::HybridScheduler<cr::mt::FIFOConditionVariable, cr::sync::ConditionVariable>>>(
		"HybridScheduler<mt::FIFO,sync::CV>",
		coroutines,
		iterations,
		batches);
}

void test_sync_events(
	std::size_t coroutines,
	std::size_t iterations,
	std::size_t batches)
{
	std::cout << "\n==== Single-threaded events ====\n";

	benchmark<bench::Event<cr::sync::Event>>(
		"cr::sync::Event",
		coroutines,
		iterations,
		batches);

	benchmark<bench::Event<cr::sync::FIFOEvent>>(
		"cr::sync::FIFOEvent",
		coroutines,
		iterations,
		batches);

	benchmark<bench::Event<cr::mt::Event>>(
		"cr::mt::Event",
		coroutines,
		iterations,
		batches);

	benchmark<bench::Event<cr::mt::FIFOEvent>>(
		"cr::mt::FIFOEvent",
		coroutines,
		iterations,
		batches);
}

void test_sync_consumable_events(
	std::size_t coroutines,
	std::size_t iterations,
	std::size_t batches)
{
	std::cout << "\n==== Single-threaded consumable events ====\n";

	benchmark<bench::ConsumableEvent<cr::sync::ConsumableEvent>>(
		"cr::sync::ConsumableEvent",
		coroutines,
		iterations,
		batches);

	benchmark<bench::ConsumableEvent<cr::sync::FIFOConsumableEvent>>(
		"cr::sync::FIFOConsumableEvent",
		coroutines,
		iterations,
		batches);

	benchmark<bench::ConsumableEvent<cr::mt::ConsumableEvent>>(
		"cr::mt::ConsumableEvent",
		coroutines,
		iterations,
		batches);

	benchmark<bench::ConsumableEvent<cr::mt::FIFOConsumableEvent>>(
		"cr::mt::FIFOConsumableEvent",
		coroutines,
		iterations,
		batches);
}

void intro()
{
	std::cout << "libcr benchmark tool (" << sizeof(int*)*CHAR_BIT << " bit ";
#ifdef LIBCR_RELEASE
	std::cout << "RELEASE";
#else
	std::cout << "DEBUG";
#endif
	std::cout << " build, " << sizeof(cr::Coroutine) << " bytes/coroutine) [" << std::thread::hardware_concurrency() << " threads]\n";
}

int main(int, char **)
{
	intro();
	std::size_t coroutines = 600;
	std::size_t iterations = 10000;
	std::size_t batches = 20;

	benchmark<bench::Queue<cr::sync::FIFOFixedQueue<int, 10>>>(
		"cr::sync::FIFOFixedQueue<int, 10>",
		coroutines,
		iterations,
		batches);
	benchmark<bench::Queue<cr::mt::FIFOFixedQueue<int, 10>>>(
		"cr::mt::FIFOFixedQueue<int, 10>",
		coroutines,
		iterations,
		batches);

	test_mt_schedulers(
		coroutines,
		iterations,
		batches);

	test_sync_schedulers(
		coroutines,
		iterations,
		batches);

	test_sync_events(
		coroutines,
		iterations,
		batches);

	test_sync_consumable_events(
		coroutines,
		iterations,
		batches);
}