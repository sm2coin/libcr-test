namespace cr::test::bench
{
	template<class EventT>
	CR_IMPL(Event<EventT>::Waiter)
		while(iterations--)
		{
			CR_AWAIT(events[iterations&1].wait());
		}
	CR_FINALLY
		++*finished;
	CR_IMPL_END

	template<class EventT>
	Event<EventT>::Event(
		std::size_t coroutines,
		std::size_t iterations):
		m_coroutines(coroutines),
		m_iterations(iterations)
	{
	}

	template<class EventT>
	void Event<EventT>::prepare()
	{
		for(auto &coroutine : m_coroutines)
			coroutine.prepare(
				nullptr,
				m_iterations,
				m_events,
				&m_finished);

		m_finished = 0;
	}

	template<class EventT>
	void Event<EventT>::execute()
	{
		for(auto &coroutine : m_coroutines)
			coroutine.start_prepared();

		for(std::size_t i = m_iterations; i--;)
		{
			m_events[i&1].fire();
			m_events[i&1].clear();
		}
	}

	template<class EventT>
	void Event<EventT>::destroy()
	{
		assert(m_finished == m_coroutines.size());
	}
}