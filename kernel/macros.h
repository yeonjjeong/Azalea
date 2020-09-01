#ifndef __MACROS_H__
#define __MACROS_H__

#if (__GNUC__ >= 3)
#define unlikely(x)		__builtin_expect((x), FALSE)
#define likely(x)		__builtin_expect((x), TRUE)
#define likelyval(x,val)	__builtin_expect((x), (val))
#else /* __GNUC__ < 3 */
#define likely(x)		(x)
#define unlikely(x)		(x)
#define likelyval(x,val)	(x)
#endif /* __GNUC__ < 3 */

#define BUILTIN_EXPECT(exp, b)  __builtin_expect((exp), (b))

#define DECLARE_PERCPU(type, name)	\
type name[MAX_PROCESSOR_COUNT] __attribute__ ((aligned (64)))

#define per_cpu(val)	(val)[get_apic_id()]
#define per_cpu_byid(val, cpu)	(val)[(cpu)]

#endif // __MACROS_H__
