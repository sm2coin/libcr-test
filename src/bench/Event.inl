namespace cr::test::bench
{
	template<class EventT>
	void Event<EventT>::Waiter::cr_prepare(
		std::size_t iterations,
		EventT * events,
		std::size_t * finished)
	{
		this->i = iterations;
		this->events = events;
		this->finished = finished;
	}

	template<class EventT>
	void Event<EventT>::Waiter::cr_destroy()
	{
		++*finished;
	}

	template<class EventT>
	CR_IMPL(Event<EventT>::Waiter)
		while(i--)
		{
			CR_AWAIT(events[i&1].wait());
		}
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