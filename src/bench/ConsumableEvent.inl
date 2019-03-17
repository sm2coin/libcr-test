namespace cr::test::bench
{
	template<class EventT>
	void ConsumableEvent<EventT>::Waiter::cr_prepare(
		std::size_t iterations,
		EventT * event,
		std::size_t * finished)
	{
		this->i = iterations;
		this->event = event;
		this->finished = finished;
	}

	template<class EventT>
	void ConsumableEvent<EventT>::Waiter::cr_destroy()
	{
		++*finished;
	}

	template<class EventT>
	CR_IMPL(ConsumableEvent<EventT>::Waiter)
		while(i--)
			CR_AWAIT(event->consume());
	CR_IMPL_END

	template<class EventT>
	ConsumableEvent<EventT>::ConsumableEvent(
		std::size_t coroutines,
		std::size_t iterations):
		m_coroutines(coroutines),
		m_iterations(iterations)
	{
	}

	template<class EventT>
	void ConsumableEvent<EventT>::prepare()
	{
		for(auto &coroutine : m_coroutines)
			coroutine.prepare(
				nullptr,
				m_iterations,
				&m_event,
				&m_finished);

		m_finished = 0;
	}

	template<class EventT>
	void ConsumableEvent<EventT>::execute()
	{
		for(auto &coroutine : m_coroutines)
			coroutine.start_prepared();

		for(std::size_t i = m_coroutines.size() * m_iterations; i--;)
		{
			m_event.fire();
		}
	}

	template<class EventT>
	void ConsumableEvent<EventT>::destroy()
	{
		assert(m_finished == m_coroutines.size());
	}
}