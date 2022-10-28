/* auto-generated by gen_syscalls.py, don't edit */

#include <syscalls/pwm.h>

extern int z_vrfy_pwm_capture_cycles(const struct device * dev, uint32_t channel, pwm_flags_t flags, uint32_t * period, uint32_t * pulse, k_timeout_t timeout);
uintptr_t z_mrsh_pwm_capture_cycles(uintptr_t arg0, uintptr_t arg1, uintptr_t arg2,
		uintptr_t arg3, uintptr_t arg4, void *more, void *ssf)
{
	_current->syscall_frame = ssf;
	Z_OOPS(Z_SYSCALL_MEMORY_READ(more, 2 * sizeof(uintptr_t)));
	union { uintptr_t x; const struct device * val; } parm0;
	parm0.x = arg0;
	union { uintptr_t x; uint32_t val; } parm1;
	parm1.x = arg1;
	union { uintptr_t x; pwm_flags_t val; } parm2;
	parm2.x = arg2;
	union { uintptr_t x; uint32_t * val; } parm3;
	parm3.x = arg3;
	union { uintptr_t x; uint32_t * val; } parm4;
	parm4.x = arg4;
	union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm5;
	parm5.split.lo = (((uintptr_t *)more)[0]);
	parm5.split.hi = (((uintptr_t *)more)[1]);
	int ret = z_vrfy_pwm_capture_cycles(parm0.val, parm1.val, parm2.val, parm3.val, parm4.val, parm5.val);
	_current->syscall_frame = NULL;
	return (uintptr_t) ret;
}

