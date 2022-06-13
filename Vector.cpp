#include <iostream>
#include <cmath>
#include "Vector.h"

using Value = double;

Vector::Vector(const Value* rawArray, const size_t size, float coef)
:_multiplicativeCoef(coef), _size(size), _capacity(size)
{
	_data = new Value[_capacity];
	for (size_t i = 0; i < size; i++)
	{
		_data[i] = rawArray[i];
	}
}

Vector::Vector(const Vector& other)
{
	_size = other._size;
	_capacity = other._size;
	_multiplicativeCoef = other._multiplicativeCoef;
	_data = new Value[_capacity];
	for (size_t i = 0; i < _size; i++)
	{
		_data[i] = other._data[i]; 
	}
}

Vector& Vector::operator=(const Vector& other)
{
	if (this != &other)
	{
		delete[] _data;
		Vector secondVector(other);
		*this = std::move(secondVector);
	}
	return *this;
}

Vector::Vector(Vector&& other) noexcept
{
	*this = std::move(other);
}

Vector& Vector::operator=(Vector&& other) noexcept
{
	if(this != &other)
	{
		delete[] _data;
		_size = other._size;
		_capacity = other._capacity;
		_data = other._data;
		_multiplicativeCoef = other._multiplicativeCoef;
		other._data = nullptr;
		other._size = 0;
		other._capacity = 0;
		other._multiplicativeCoef = 0;
	}
	return *this;
}

Vector::~Vector()
{
	delete[] _data;
}

void Vector::pushBack(const Value& value)
{
	insert(value, _size);
}

void Vector::pushFront(const Value& value)
{
	insert(value, 0);
}

void Vector::insert(const Value& value, size_t pos)
{
	if (_capacity == 0)
	{
		reserve(size_t(_multiplicativeCoef));
	}
	if (loadFactor() == 1)
	{
		reserve(_capacity * size_t(_multiplicativeCoef));
	}
	for (size_t i = _size; i > pos; --i) 
	{
		_data[i] = _data[i - 1];
	}
	_data[pos] = value;
	_size++;
}

void Vector::insert(const Value* values, size_t size, size_t pos)
{
	if (_size + size > _capacity)
	{
		reserve(size + _size);
	}
	for (size_t i = 0; i < size; ++i)
	{
		insert(values[i], pos + i);
	}
}

void Vector::insert(const Vector& vector, size_t pos)
{
	insert(vector._data, vector._size, pos);
}

void Vector::popBack()
{
	erase(_size - 1, 1);
}

void Vector::popFront()
{
	erase(0, 1);
}

void Vector::erase(size_t pos, size_t count)
{
	if (_size == 0) 
	{
		throw std::out_of_range("Removing elements from a vector with 0");
	}
	if (pos + count > _size)
	{
		_size = pos;
	}
	else
	{
		for (size_t i = pos; i < _size - count; i++)
		{
			_data[i] = _data[i + count];
		}
		_size -= count;
	}
}

void Vector::eraseBetween(size_t beginPos, size_t endPos)
{
	if (endPos <= beginPos)
	{
		throw std::out_of_range("endPos <= beginPos");
	}
	else
	{
		erase(beginPos, endPos - beginPos);
	}
}

size_t Vector::size() const
{
	return _size;
}

size_t Vector::capacity() const
{
	return _capacity;
}

double Vector::loadFactor() const
{
	return double(_size) / double(_capacity);
}

Value& Vector::operator[](size_t idx)
{
	return _data[idx];
}

const Value& Vector::operator[](size_t idx) const
{
	return _data[idx];
}

long long Vector::find(const Value& value) const
{
	for (size_t idx = 0; idx < _size; idx++)
	{
		if (_data[idx] == value)
		{
			return idx; 
		}
	}
	return -1;
}

void Vector::reserve(size_t capacity)
{
	if (capacity > _capacity)
	{	
		Value* ptr = _data;
		_data = new Value[capacity];
		if (ptr != nullptr)
		{
			for (size_t i = 0; i < _size; i++) 
			{
				_data[i] = ptr[i];
			}
			delete[] ptr;
		}
		_capacity = capacity;
	}
}

void Vector::shrinkToFit()
{
	Value* ptr = new Value[_size];
	if (_data != nullptr) 
	{
		for (size_t i = 0; i < _size; ++i) 
		{
			ptr[i] = _data[i];
		}
		delete[] _data;
	}
	_data = ptr;
	_capacity = _size;
}

Vector::Iterator Vector::begin()
{
	return Vector::Iterator(&_data[0]);
}

Vector::Iterator Vector::end()
{
	return Vector::Iterator(&_data[_size]);
}

Vector::Iterator::Iterator(Value* ptr):_ptr(ptr)
{}

Value& Vector::Iterator:: operator*()
{
	return *_ptr;
}

const Value& Vector::Iterator:: operator*() const
{
	return *_ptr;
}

Value* Vector::Iterator:: operator->()
{
	return _ptr;
}

const Value* Vector::Iterator:: operator->() const
{
	return _ptr;
}

Vector::Iterator Vector::Iterator:: operator++()
{
	++_ptr;
	return *this;
}

Vector::Iterator Vector::Iterator:: operator++(int)
{
	Vector::Iterator bufPtr = *this;
	++*this;
	return bufPtr;
}

bool Vector::Iterator:: operator==(const Vector::Iterator& other) const
{
	return _ptr == other._ptr;
}

bool Vector::Iterator:: operator!=(const Vector::Iterator& other) const
{
	return !(*this == other);
}

