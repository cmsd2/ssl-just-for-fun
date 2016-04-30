#include "stdafx.h"
#include "Buffer.h"

#define DEFAULT_LINE_LEN 255

Buffer::Buffer()
	: capacity(DEFAULT_LINE_LEN),
	len(capacity),
	pos(0),
	buffer(new char[capacity])
{
}

Buffer::~Buffer()
{
	delete[] buffer;
}

int Buffer::recv(int fd)
{
	int result = ::recv(fd, buffer + pos, remaining(), 0);

	if (result > 0)
	{
		pos += result;
	}

	return result;
}

int Buffer::remaining() const
{
	return len - pos;
}

void Buffer::flip()
{
	len = pos;
	pos = 0;
}

void Buffer::compact()
{
	::memmove(buffer, buffer + pos, remaining());

	pos = len;
}

void Buffer::clear()
{
	pos = 0;
	len = capacity;
}

int Buffer::getc()
{
	if (remaining() > 0)
	{
		int c = buffer[pos];
		pos++;
		return c;
	}
	else
	{
		return -1;
	}
}