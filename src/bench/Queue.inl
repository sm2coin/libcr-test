template<class T>
inline T &id(T & x) { return x; }
template<class T>
inline T &&id(T &&x) { return std::move(x); }

namespace cr::test::bench
{
	template<class QueueT>
	CR_IMPL(Queue<QueueT>::Consumer)
		while(iterations--)
			CR_CALL(pop, (id(*queue), dummy));
		++*finished;
	CR_IMPL_END

	template<class QueueT>
	CR_IMPL(Queue<QueueT>::Producer)
		while(iterations--)
			CR_CALL(push.copy, (id(*queue), 1));
		++*finished;
	CR_IMPL_END

	template<class QueueT>
	Queue<QueueT>::Queue(
		std::size_t coroutines,
		std::size_t iterations):
		m_consumers(coroutines),
		m_producers(coroutines),
		m_iterations(iterations),
		m_finished(0),
		m_coroutines(coroutines)
	{
	}

	template<class QueueT>
	void Queue<QueueT>::prepare()
	{
		m_finished = 0;
		for(Consumer & consumer: m_consumers)
			consumer.prepare(
				nullptr,
				m_iterations,
				m_queue,
				m_finished);
		for(Producer & producer: m_producers)
			producer.prepare(
				nullptr,
				m_iterations,
				m_queue,
				m_finished);
	}

	template<class QueueT>
	void Queue<QueueT>::execute()
	{
		for(Consumer & consumer: m_consumers)
			consumer.start_prepared();

		for(Producer & producer: m_producers)
			producer.start_prepared();
	}

	template<class QueueT>
	void Queue<QueueT>::destroy()
	{
		if(m_finished != 2 * m_coroutines)
			std::abort();
	}
}