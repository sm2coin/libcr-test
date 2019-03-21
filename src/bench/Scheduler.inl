namespace cr::test::bench
{
	template<class SchedulerT>
	void Scheduler<SchedulerT>::Yielder::cr_destroy()
	{
	}

	template<class SchedulerT>
	CR_IMPL(Scheduler<SchedulerT>::Yielder)
		while(iterations--)
			CR_YIELD;
		++testbench->m_finished;
	CR_IMPL_END

	template<class SchedulerT>
	Scheduler<SchedulerT>::Scheduler(
		std::size_t coroutines,
		std::size_t iterations):
		m_iterations(iterations),
		m_coroutine_count(coroutines)
	{
	}

	template<class SchedulerT>
	void Scheduler<SchedulerT>::prepare()
	{
		m_coroutines = new Yielder[m_coroutine_count];
		m_finished = 0;
	}

	template<class SchedulerT>
	void Scheduler<SchedulerT>::execute()
	{
		for(std::size_t i = 0; i < m_coroutine_count; i++)
			m_coroutines[i].start(nullptr, m_iterations, this);
		while(SchedulerT::instance().schedule());
	}

	template<class SchedulerT>
	void Scheduler<SchedulerT>::destroy()
	{
		assert(m_finished == m_coroutine_count);
		delete [] m_coroutines;
	}
}