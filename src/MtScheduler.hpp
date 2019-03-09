#ifndef __libcr_test_mtscheduler_hpp_defined
#define __libcr_test_mtscheduler_hpp_defined

#include <thread>
#include <vector>
#include <atomic>

template<class ConditionVariable>
/** Simple scheduler using multiple condition variables.
@tparam PODConditionVariable:
	The POD condition variable type. */
class MtSchedulerBase
{
	std::vector<ConditionVariable> m_cvs;
	std::atomic_size_t m_notify_counter;
	std::atomic_size_t m_wait_counter;
public:
	MtSchedulerBase():
		m_cvs(std::thread::hardware_concurrency()),
		m_notify_counter(0),
		m_wait_counter(0)
	{
	}

	/** Returns a singleton instance. */
	static inline MtSchedulerBase<ConditionVariable> &instance()
	{
		static MtSchedulerBase<ConditionVariable> instance;
		return instance;
	}

	/** Progresses all currently waiting coroutines.
	@return
		Whether any coroutines were waiting. */
	bool schedule()
	{
		return m_cvs[m_notify_counter.fetch_add(1,std::memory_order_relaxed)
			% m_cvs.size()].notify_one();
	}

	/** Enqueues a coroutine to wait */
	inline typename ConditionVariable::WaitCall enqueue()
	{
		return m_cvs[m_wait_counter.fetch_add(1, std::memory_order_relaxed) % m_cvs.size()].wait();
	}
};

typedef MtSchedulerBase<cr::mt::ConditionVariable> MtScheduler;
typedef MtSchedulerBase<cr::mt::FIFOConditionVariable> FIFOMtScheduler;

#endif