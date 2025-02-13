/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Handle.h>
#include <libio/Seek.h>
#include "system/node/Node.h"

struct FsHandle : public RefCounted<FsHandle>
{
private:
    Lock _lock{"fshandle"};
    RefPtr<FsNode> _node = nullptr;
    JOpenFlag _flags = 0;
    size_t _offset = 0;

public:
    void *attached;
    size_t attached_size;

    auto node() { return _node; }
    auto offset() { return _offset; }
    auto flags() { return _flags; }

    bool has_flag(JOpenFlag flag) { return (_flags & flag) == flag; }

    FsHandle(RefPtr<FsNode> node, JOpenFlag flags);

    FsHandle(FsHandle &other);

    ~FsHandle();

    bool locked();

    void acquire(int who_acquire);
    void release(int who_release);

    PollEvent poll(PollEvent events);

    ResultOr<size_t> read(void *buffer, size_t size);

    ResultOr<size_t> write(const void *buffer, size_t size);

    ResultOr<ssize64_t> seek(IO::SeekFrom from);

    JResult call(IOCall request, void *args);

    JResult stat(JStat *stat);

    ResultOr<RefPtr<FsHandle>> accept();
};