#pragma once
#include <stdio.h>
#include <iostream>

enum DirectVersion
{
	Direct9,
	Direct10,
	Direct11,
	Direct12,
};


class DirectError :public std::exception
{
private:
	std::string _error_str;

public:
	DirectError(std::string error);
	~DirectError();

public:
	virtual char const* what() const throw();

};


class Direct9
{
private:

public:
	Direct9();
	~Direct9();

public:

};


class Direct10
{
private:

public:
	Direct10();
	~Direct10();

public:

};


class Direct11
{
private:

public:
	Direct11();
	~Direct11();

public:

};


class Direct12
{
private:

public:
	Direct12();
	~Direct12();

public:

};


class Direct
{
private:
	DirectVersion _current_version = DirectVersion::Direct9;

public:
	Direct();
	~Direct();

public:

};

