/*
 * Copyright (c) 2021, Krisna Pranav, johnsmith5c12
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Memory.h>
#include <libabi/Result.h>
#include <pranaos/Time.h>
#include "system/memory/MemoryRange.h"

struct Task;

namespace Arch
{

void disable_interrupts();

void enable_interrupts();

void halt();

void yield();

void save_context(Task *task);

void load_context(Task *task);

void task_go(Task *task);

size_t debug_write(const void *buffer, size_t size);

TimeStamp get_time();

NO_RETURN void reboot();

NO_RETURN void shutdown();

void dump_stack_frame(void *stackframe);

void backtrace();

void virtual_initialize();

void virtual_memory_enable();

struct AddressSpace
{
};

AddressSpace *kernel_address_space();

AddressSpace *address_space_create();

void address_space_destroy(AddressSpace *address_space);

void address_space_switch(AddressSpace *address_space);

bool virtual_present(AddressSpace *address_space, uintptr_t virtual_address);

uintptr_t virtual_to_physical(AddressSpace *address_space, uintptr_t virtual_address);

JResult virtual_map(AddressSpace *address_space, MemoryRange physical_range, uintptr_t virtual_address, MemoryFlags flags);

MemoryRange virtual_alloc(AddressSpace *address_space, MemoryRange physical_range, MemoryFlags flags);

void virtual_free(AddressSpace *address_space, MemoryRange virtual_range);

}