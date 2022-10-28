/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_LOG_MSG_H
#define Z_INCLUDE_SYSCALLS_LOG_MSG_H


#include <zephyr/tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <syscall_list.h>
#include <zephyr/syscall.h>

#include <zephyr/linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern void z_impl_z_log_msg_static_create(const void * source, const struct log_msg_desc desc, uint8_t * package, const void * data);

__pinned_func
static inline void z_log_msg_static_create(const void * source, const struct log_msg_desc desc, uint8_t * package, const void * data)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const void * val; } parm0 = { .val = source };
		union { uintptr_t x; const struct log_msg_desc val; } parm1 = { .val = desc };
		union { uintptr_t x; uint8_t * val; } parm2 = { .val = package };
		union { uintptr_t x; const void * val; } parm3 = { .val = data };
		(void) arch_syscall_invoke4(parm0.x, parm1.x, parm2.x, parm3.x, K_SYSCALL_Z_LOG_MSG_STATIC_CREATE);
		return;
	}
#endif
	compiler_barrier();
	z_impl_z_log_msg_static_create(source, desc, package, data);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define z_log_msg_static_create(source, desc, package, data) do { 	sys_port_trace_syscall_enter(K_SYSCALL_Z_LOG_MSG_STATIC_CREATE, z_log_msg_static_create, source, desc, package, data); 	z_log_msg_static_create(source, desc, package, data); 	sys_port_trace_syscall_exit(K_SYSCALL_Z_LOG_MSG_STATIC_CREATE, z_log_msg_static_create, source, desc, package, data); } while(false)
#endif
#endif


extern void z_impl_z_log_msg_runtime_vcreate(uint8_t domain_id, const void * source, uint8_t level, const void * data, size_t dlen, uint32_t package_flags, const char * fmt, va_list ap);

__pinned_func
static inline void z_log_msg_runtime_vcreate(uint8_t domain_id, const void * source, uint8_t level, const void * data, size_t dlen, uint32_t package_flags, const char * fmt, va_list ap)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; uint8_t val; } parm0 = { .val = domain_id };
		union { uintptr_t x; const void * val; } parm1 = { .val = source };
		union { uintptr_t x; uint8_t val; } parm2 = { .val = level };
		union { uintptr_t x; const void * val; } parm3 = { .val = data };
		union { uintptr_t x; size_t val; } parm4 = { .val = dlen };
		union { uintptr_t x; uint32_t val; } parm5 = { .val = package_flags };
		union { uintptr_t x; const char * val; } parm6 = { .val = fmt };
		union { uintptr_t x; va_list val; } parm7;
		va_copy(parm7.val, ap);
		uintptr_t more[] = {
			parm5.x,
			parm6.x,
			parm7.x
		};
		(void) arch_syscall_invoke6(parm0.x, parm1.x, parm2.x, parm3.x, parm4.x, (uintptr_t) &more, K_SYSCALL_Z_LOG_MSG_RUNTIME_VCREATE);
		va_end(parm7.val);
		return;
	}
#endif
	compiler_barrier();
	z_impl_z_log_msg_runtime_vcreate(domain_id, source, level, data, dlen, package_flags, fmt, ap);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define z_log_msg_runtime_vcreate(domain_id, source, level, data, dlen, package_flags, fmt, ap) do { 	sys_port_trace_syscall_enter(K_SYSCALL_Z_LOG_MSG_RUNTIME_VCREATE, z_log_msg_runtime_vcreate, domain_id, source, level, data, dlen, package_flags, fmt, ap); 	z_log_msg_runtime_vcreate(domain_id, source, level, data, dlen, package_flags, fmt, ap); 	sys_port_trace_syscall_exit(K_SYSCALL_Z_LOG_MSG_RUNTIME_VCREATE, z_log_msg_runtime_vcreate, domain_id, source, level, data, dlen, package_flags, fmt, ap); } while(false)
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */
