#include <libcr/libcr.hpp>
#include "bench/MtScheduler.hpp"
#include "bench/Scheduler.hpp"
#include "MtScheduler.hpp"
#include <iostream>
#include <climits>

using namespace cr::test;

template<class SchedulerBench>
void test_scheduler(
	char const * name,
	std::size_t coroutines,
	std::size_t iterations,
	std::size_t batch)
{
	std::size_t const k_mega = 1000000;
	std::size_t switches = coroutines * iterations;
	SchedulerBench scheduler {coroutines, iterations};

	std::cout << "\n+===============================================================================\n";
	std::cout << "| " << SchedulerBench::name() << "<" << name << ">: " << batch << " batches (" << coroutines << ", " << iterations << ")\n";
	std::cout << "+-------------------------------------------------------------------------------\n";
	Benchmark::BatchTimes dur = scheduler.batch(batch);
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

	test_scheduler<bench::MtScheduler<cr::mt::Scheduler>>(
		"cr::mt::Scheduler",
		coroutines,
		iterations,
		batches);

	test_scheduler<bench::MtScheduler<cr::mt::FIFOScheduler>>(
		"cr::mt::FIFOScheduler",
		coroutines,
		iterations,
		batches);

	test_scheduler<bench::MtScheduler<MtScheduler>>(
		"MtScheduler",
		coroutines,
		iterations,
		batches);

	test_scheduler<bench::MtScheduler<FIFOMtScheduler>>(
		"FIFOMtScheduler",
		coroutines,
		iterations,
		batches);
}

void test_sync_schedulers(
	std::size_t coroutines,
	std::size_t iterations,
	std::size_t batches)
{
	std::cout << "\n==== Single-threaded tests ====\n";

	test_scheduler<bench::Scheduler<cr::sync::Scheduler>>(
		"cr::sync::Scheduler",
		coroutines,
		iterations,
		batches);

	test_scheduler<bench::Scheduler<cr::sync::FIFOScheduler>>(
		"cr::sync::FIFOScheduler",
		coroutines,
		iterations,
		batches);

	test_scheduler<bench::Scheduler<cr::mt::Scheduler>>(
		"cr::mt::Scheduler",
		coroutines,
		iterations,
		batches);

	test_scheduler<bench::Scheduler<cr::mt::FIFOScheduler>>(
		"cr::mt::FIFOScheduler",
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
	std::cout << " build, " << sizeof(cr::Coroutine) << " bytes/coroutine)\n";
}

int main(int, char **)
{
	intro();
	std::size_t mt_coroutines = 600;
	std::size_t sync_coroutines = 600;
	std::size_t iterations = 10000;
	std::size_t batches = 20;

	test_mt_schedulers(
		mt_coroutines,
		iterations,
		batches);

	test_sync_schedulers(
		sync_coroutines,
		iterations,
		batches);
}