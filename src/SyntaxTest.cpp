#include <libcr/libcr.hpp>

namespace cr::test::syntax
{
	COROUTINE(InlineCoroutine, void)

	CR_STATE()
	CR_INLINE
	CR_FINALLY
	CR_INLINE_END

	template<class Scheduler>
	TEMPLATE_COROUTINE(YieldTester, (Scheduler), Scheduler)
	CR_STATE((int) x)
	CR_INLINE
		CR_YIELD;
	CR_FINALLY
	CR_INLINE_END

	template class YieldTester<cr::mt::Scheduler>;
	template class YieldTester<cr::mt::FIFOScheduler>;
	template class YieldTester<cr::sync::Scheduler>;
	template class YieldTester<cr::sync::FIFOScheduler>;

	COROUTINE(CallTest, void)
	CR_STATE()
		InlineCoroutine ilc;
		union {
			YieldTester<cr::mt::Scheduler> mt;
			YieldTester<cr::mt::FIFOScheduler> mtFifo;
			YieldTester<cr::sync::Scheduler> sync;
			YieldTester<cr::sync::FIFOScheduler> syncFifo;
		} yield;
	CR_EXTERNAL


	CR_IMPL(CallTest)
		CR_CALL(ilc, ());
		// Test all schedulers.
		CR_CALL(yield.mt, (1));
		CR_CALL(yield.mtFifo, (1));
		CR_CALL(yield.sync, (1));
		CR_CALL(yield.syncFifo, (1));
	CR_FINALLY
	CR_IMPL_END

	PROTOTHREAD(Proto)

	PT_STATE((int) x)
	PT_INLINE
		PT_YIELD;
		PT_RETURN;
	PT_INLINE_END

	template<class T>
	TEMPLATE_PROTOTHREAD(Proto2, (T))
	PT_STATE((int &) y)
		Proto p;

	PT_EXTERNAL

	template<class T>
	PT_IMPL(Proto2<T>)
		PT_CALL(p, (1));
	PT_IMPL_END

	template class Proto2<int>;
}