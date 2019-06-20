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

void display_line(
	char const * name,
	double duration,
	std::size_t switches)
{
	double mhz = switches / (duration * 1000000);
	double ns = (1000000000 * duration) / switches;
	std::printf("| %s: %12.7fs (%12.8fMHz | %12.8fns)                        |\n", name, duration, mhz, ns);
}

template<class Benchmark>
void benchmark(
	char const * name,
	std::size_t coroutines,
	std::size_t iterations,
	std::size_t batch)
{
	std::size_t switches = coroutines * iterations;
	Benchmark bench {coroutines, iterations};

	std::cout << "\n+==============================================================================+\n";
	std::cout << "| " << Benchmark::name() << "<" << name << ">: " << batch << " batches (" << coroutines << ", " << iterations << ")\n";
	std::cout << "+------------------------------------------------------------------------------+\n";
	cr::test::Benchmark::BatchTimes dur = bench.batch(batch);
	display_line("Sum", dur.avg_duration * batch, switches * batch);
	display_line("Min", dur.min_duration, switches);
	display_line("Avg", dur.avg_duration, switches);
	display_line("Med", dur.med_duration, switches);
	display_line("Max", dur.max_duration, switches);
	std::cout << "+==============================================================================+\n";

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