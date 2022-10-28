/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_NET_IP_H
#define Z_INCLUDE_SYSCALLS_NET_IP_H


#include <zephyr/tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <syscall_list.h>
#include <zephyr/syscall.h>

#include <zephyr/linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_net_addr_pton(sa_family_t family, const char * src, void * dst);

__pinned_func
static inline int net_addr_pton(sa_family_t family, const char * src, void * dst)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; sa_family_t val; } parm0 = { .val = family };
		union { uintptr_t x; const char * val; } parm1 = { .val = src };
		union { uintptr_t x; void * val; } parm2 = { .val = dst };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_NET_ADDR_PTON);
	}
#endif
	compiler_barrier();
	return z_impl_net_addr_pton(family, src, dst);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define net_addr_pton(family, src, dst) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_NET_ADDR_PTON, net_addr_pton, family, src, dst); 	retval = net_addr_pton(family, src, dst); 	sys_port_trace_syscall_exit(K_SYSCALL_NET_ADDR_PTON, net_addr_pton, family, src, dst, retval); 	retval; })
#endif
#endif


extern char * z_impl_net_addr_ntop(sa_family_t family, const void * src, char * dst, size_t size);

__pinned_func
static inline char * net_addr_ntop(sa_family_t family, const void * src, char * dst, size_t size)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; sa_family_t val; } parm0 = { .val = family };
		union { uintptr_t x; const void * val; } parm1 = { .val = src };
		union { uintptr_t x; char * val; } parm2 = { .val = dst };
		union { uintptr_t x; size_t val; } parm3 = { .val = size };
		return (char *) arch_syscall_invoke4(parm0.x, parm1.x, parm2.x, parm3.x, K_SYSCALL_NET_ADDR_NTOP);
	}
#endif
	compiler_barrier();
	return z_impl_net_addr_ntop(family, src, dst, size);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define net_addr_ntop(family, src, dst, size) ({ 	char * retval; 	sys_port_trace_syscall_enter(K_SYSCALL_NET_ADDR_NTOP, net_addr_ntop, family, src, dst, size); 	retval = net_addr_ntop(family, src, dst, size); 	sys_port_trace_syscall_exit(K_SYSCALL_NET_ADDR_NTOP, net_addr_ntop, family, src, dst, size, retval); 	retval; })
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */
