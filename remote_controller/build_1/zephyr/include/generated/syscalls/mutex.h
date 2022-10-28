/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_MUTEX_H
#define Z_INCLUDE_SYSCALLS_MUTEX_H


#include <zephyr/tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <syscall_list.h>
#include <zephyr/syscall.h>

#include <zephyr/linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_z_sys_mutex_kernel_lock(struct sys_mutex * mutex, k_timeout_t timeout);

__pinned_func
static inline int z_sys_mutex_kernel_lock(struct sys_mutex * mutex, k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; struct sys_mutex * val; } parm0 = { .val = mutex };
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm1 = { .val = timeout };
		return (int) arch_syscall_invoke3(parm0.x, parm1.split.lo, parm1.split.hi, K_SYSCALL_Z_SYS_MUTEX_KERNEL_LOCK);
	}
#endif
	compiler_barrier();
	return z_impl_z_sys_mutex_kernel_lock(mutex, timeout);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define z_sys_mutex_kernel_lock(mutex, timeout) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_Z_SYS_MUTEX_KERNEL_LOCK, z_sys_mutex_kernel_lock, mutex, timeout); 	retval = z_sys_mutex_kernel_lock(mutex, timeout); 	sys_port_trace_syscall_exit(K_SYSCALL_Z_SYS_MUTEX_KERNEL_LOCK, z_sys_mutex_kernel_lock, mutex, timeout, retval); 	retval; })
#endif
#endif


extern int z_impl_z_sys_mutex_kernel_unlock(struct sys_mutex * mutex);

__pinned_func
static inline int z_sys_mutex_kernel_unlock(struct sys_mutex * mutex)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; struct sys_mutex * val; } parm0 = { .val = mutex };
		return (int) arch_syscall_invoke1(parm0.x, K_SYSCALL_Z_SYS_MUTEX_KERNEL_UNLOCK);
	}
#endif
	compiler_barrier();
	return z_impl_z_sys_mutex_kernel_unlock(mutex);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define z_sys_mutex_kernel_unlock(mutex) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_Z_SYS_MUTEX_KERNEL_UNLOCK, z_sys_mutex_kernel_unlock, mutex); 	retval = z_sys_mutex_kernel_unlock(mutex); 	sys_port_trace_syscall_exit(K_SYSCALL_Z_SYS_MUTEX_KERNEL_UNLOCK, z_sys_mutex_kernel_unlock, mutex, retval); 	retval; })
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */
