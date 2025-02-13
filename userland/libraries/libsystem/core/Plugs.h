/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libabi/Filesystem.h>
#include <libabi/Handle.h>
#include <libabi/IOCall.h>
#include <libabi/Launchpad.h>
#include <libabi/System.h>
#include <libabi/Time.h>
#include <libio/Seek.h>
#include <libutils/String.h>


TimeStamp __plug_system_get_time();

Tick __plug_system_get_ticks();


int __plug_process_this();

const char *__plug_process_name();

JResult __plug_process_launch(Launchpad *launchpad, int *pid);

void NO_RETURN __plug_process_exit(int code);

JResult __plug_process_cancel(int pid);

String __plug_process_resolve(String raw_path);

JResult __plug_process_get_directory(char *buffer, size_t size);

JResult __plug_process_set_directory(const char *directory);

JResult __plug_process_sleep(int time);

JResult __plug_process_wait(int pid, int *exit_value);


JResult __plug_handle_open(Handle *handle, const char *path, JOpenFlag flags);

void __plug_handle_close(Handle *handle);

size_t __plug_handle_read(Handle *handle, void *buffer, size_t size);

size_t __plug_handle_write(Handle *handle, const void *buffer, size_t size);

JResult __plug_handle_call(Handle *handle, IOCall request, void *args);

int __plug_handle_seek(Handle *handle, IO::SeekFrom from);

int __plug_handle_tell(Handle *handle);

int __plug_handle_stat(Handle *handle, JStat *stat);