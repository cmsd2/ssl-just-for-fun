#pragma once

class Buffer {
public:
	Buffer();
	~Buffer();

	int recv(int fd);

	void flip();

	void compact();

	void clear();

	int getc();

	int remaining() const;
protected:
	int capacity;
	int len;
	int pos;
	char * buffer;
};