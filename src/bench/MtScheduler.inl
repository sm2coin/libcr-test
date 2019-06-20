#include <iostream>
namespace cr::test::bench
{
	template<class Scheduler>
	CR_IMPL(MtScheduler<Scheduler>::Yielder)
		while(iterations-->0)
			CR_YIELD;
		testbench->notify();
	CR_FINALLY
	CR_IMPL_END


	template<class Scheduler>
	void MtScheduler<Scheduler>::scheduling_thread(
		MtScheduler<Scheduler> * scheduler,
		std::size_t index)
	{
		std::size_t hits = 0;
		std::size_t misses = 0;
		while(scheduler->m_running.load(std::memory_order_relaxed))
		{
			if(Scheduler::instance().schedule(index))
				++hits;
			else
				++misses;
		}
	}

	template<class Scheduler>
	void MtScheduler<Scheduler>::notify()
	{
		// Notify the main thread if this is the last coroutine.
		if(1 == m_running.fetch_sub(1,std::memory_order_relaxed))
			m_done.notify_one();
	}

	template<class Scheduler>
	MtScheduler<Scheduler>::MtScheduler(
		std::size_t coroutines,
		std::size_t iterations):
		m_coroutine_count(coroutines),
		m_iterations(iterations),
		m_done_mutex_lock(m_done_mutex, std::defer_lock)
	{
		m_coroutines = new Yielder[m_coroutine_count];
	}

	template<class Scheduler>
	MtScheduler<Scheduler>::~MtScheduler()
	{
		delete[] m_coroutines;
		m_coroutines = nullptr;
		m_coroutine_count = 0;
		m_iterations = 0;
	}

	template<class Scheduler>
	void MtScheduler<Scheduler>::prepare()
	{
		for(std::size_t i = 0; i < m_coroutine_count; i++)
			m_coroutines[i].prepare(
				nullptr,
				m_iterations,
				this);

		m_done_mutex_lock.lock();
		m_running.store(m_coroutine_count, std::memory_order_relaxed);

		m_threads.reserve(std::thread::hardware_concurrency());
		for(std::size_t i = 0; i < std::thread::hardware_concurrency(); i++)
			m_threads.emplace_back(scheduling_thread, this, i);
	}

	template<class Scheduler>
	void MtScheduler<Scheduler>::execute()
	{
		for(std::size_t i = 0; i < m_coroutine_count; i++)
			m_coroutines[i].start_prepared();
		m_done.wait(m_done_mutex_lock);
	}

	template<class Scheduler>
	void MtScheduler<Scheduler>::destroy()
	{
		m_done_mutex_lock.unlock();

		for(std::thread & thread : m_threads)
			if(thread.joinable())
				thread.join();

		m_threads.clear();
	}
}