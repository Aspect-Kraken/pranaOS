/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Syscalls.h>
#include <libio/Seek.h>
#include <libsystem/process/Process.h>
#include <libutils/String.h>

namespace IO
{

struct Handle :
    public RefCounted<Handle>
{
private:
    int _handle = HANDLE_INVALID_ID;
    JResult _result = ERR_BAD_HANDLE;

    NONCOPYABLE(Handle);

public:
    int id() const { return _handle; }

    Handle(int handle) : _handle(handle), _result(handle != HANDLE_INVALID_ID ? SUCCESS : ERR_BAD_HANDLE)
    {
    }

    Handle(const String path, JOpenFlag flags)
    {
        auto resolved_path = process_resolve(path);
        _result = J_handle_open(&_handle, resolved_path.cstring(), resolved_path.length(), flags);
    }

    Handle(Handle &&other)
    {
        _handle = std::exchange(other._handle, HANDLE_INVALID_ID);
        _result = std::exchange(other._result, ERR_BAD_HANDLE);
    }

    Handle &operator=(Handle &&other)
    {
        std::swap(_handle, other._handle);
        std::swap(_result, other._result);

        return *this;
    }

    ~Handle()
    {
        if (_handle != HANDLE_INVALID_ID)
        {
            J_handle_close(_handle);
            _handle = -1;
        }
    }

    ResultOr<size_t> read(void *buffer, size_t size)
    {
        size_t data_read = 0;
        _result = TRY(J_handle_read(_handle, buffer, size, &data_read));
        return data_read;
    }

    ResultOr<size_t> write(const void *buffer, size_t size)
    {
        size_t data_written = 0;
        _result = TRY(J_handle_write(_handle, buffer, size, &data_written));
        return data_written;
    }

    JResult call(IOCall request, void *args)
    {
        _result = J_handle_call(_handle, request, args);
        return _result;
    }

    ResultOr<ssize64_t> seek(IO::SeekFrom from)
    {
        ssize64_t result_offset = 0;
        _result = TRY(J_handle_seek(_handle, &from.position, (JWhence)from.whence, &result_offset));
        return result_offset;
    }

    ResultOr<ssize64_t> tell()
    {
        ssize64_t position = 0;
        ssize64_t result_offset = 0;
        _result = TRY(J_handle_seek(_handle, &position, J_WHENCE_CURRENT, &result_offset));
        return result_offset;
    }

    ResultOr<JStat> stat()
    {
        JStat stat{};
        _result = TRY(J_handle_stat(_handle, &stat));
        return stat;
    }

    ResultOr<RefPtr<Handle>> accept()
    {
        int connection_handle;
        _result = TRY(J_handle_accept(_handle, &connection_handle));
        return make<Handle>(connection_handle);
    }

    bool valid()
    {
        return _handle != HANDLE_INVALID_ID;
    }

    JResult result()
    {
        return _result;
    }
};

struct RawHandle
{
    virtual ~RawHandle() {}

    virtual RefPtr<Handle> handle() = 0;
};

} // namespace IO