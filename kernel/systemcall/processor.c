/*
 * Copyright (c) 2010, Stefan Lankes, RWTH Aachen University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the University nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "processor.h"
#include "errno.h"

/*
 * Note that linker symbols are not variables, they have no memory allocated for
 * maintaining a value, rather their address is their value.
 */

static void default_writefs(size_t fs)
{
	wrmsr(MSR_FS_BASE, fs);
}

static size_t default_readfs(void)
{
	return rdmsr(MSR_FS_BASE);
}

static void default_writegs(size_t gs)
{
	wrmsr(MSR_GS_BASE, gs);
}

static size_t default_readgs(void)
{
	return rdmsr(MSR_GS_BASE);
}

#if 0
static void wrfsbase(size_t fs)
{
	asm volatile ("wrfsbase %0" :: "r"(fs));
}

static size_t rdfsbase(void)
{
	size_t ret = 0;

	asm volatile ("rdfsbase %0" : "=r"(ret) :: "memory");

	return ret;
}

static void wrgsbase(size_t gs)
{
	asm volatile ("wrgsbase %0" :: "r"(gs));
}

static size_t rdgsbase(void)
{
	size_t ret = 0;

	asm volatile ("rdgsbase %0" : "=r"(ret) :: "memory");

	return ret;
}
#endif

func_read_fsgs readfs = default_readfs;
func_read_fsgs readgs = default_readgs;
func_write_fsgs writefs = default_writefs;
func_write_fsgs writegs = default_writegs;

