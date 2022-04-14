#include <iostream>
#include "Vector.h"

using Value = double;

Vector::Vector(const Value* rawArray, const size_t size, float coef)
:_multiplicativeCoef(coef)
{
	reserve(size);
	for (size_t i = 0; i < size; i++)
	{
		_data[i] = rawArray[i];
	}
	_size = size;
}

Vector::Vector(const Vector& other)
{
	_size = other._size;
	_capacity = other._capacity;
	_data = new Value[_capacity];
	for(size_t i = 0; i < _size; i++)
	{
		_data[i] = other._data[i]; 
	}
}

Vector& Vector::operator=(const Vector& other)
{
	if(this == &other)
	{

	}
	else
	{
		delete[] _data;
		_size = other._size;
		_capacity = other._capacity;
		_data = new Value[_capacity];
		for(size_t i = 0; i < _size; i++)
		{
			_data[i] = other._data[i];
		}
	}

	return *this;
}

Vector::Vector(Vector&& other) noexcept
{
	_size = other._size;
	_capacity = other._capacity;
	_data = other._data;
	other._data = nullptr;
	other._size = 0;
	other._capacity = 1;
}

Vector& Vector::operator=(Vector&& other) noexcept
{
	if(this == &other)
	{

	}
	else
	{
		delete[] _data;
		_size = other._size;
		_capacity = other._capacity;
		_data = other._data;
		other._data = nullptr;
		other._size = 0;
		other._capacity = 1;
	}

	return *this;
}

Vector::~Vector()
{
	delete[] _data;
}

void Vector::pushBack(const Value& value)
{
	reserve(_size + 1);

	_data[_size] = value;
	_size++;
}

void Vector::pushFront(const Value& value)
{
	reserve(_size + 1);

	for(int i = _size - 1; i >= 0; i--)
	{
		_data[i + 1] = _data[i];
	}

	_data[0] = value;
	_size++;
}

void Vector::insert(const Value& value, size_t pos)
{
	reserve(_size + 1);
	
	for (size_t i = _size - 1; i >= pos; --i)
	{
		_data[i + 1] = _data[i]; 
	}
	
	_data[pos] = value;
	_size++;
}

void Vector::insert(const Value* values, size_t size, size_t pos)
{
	reserve(_size + size);

	for (size_t i = _size - 1; i >= pos; --i)
	{
		_data[i + size] = _data[i]; 
	}

	for (size_t i = pos; i < pos + size; i++)
	{
		_data[i] = values[i - pos];
	}

	_size += size;
}

void Vector::insert(const Vector& vector, size_t pos)
{
	reserve(_size + vector._size);

	for (size_t i = _size - 1; i >= pos; --i)
	{
		_data[i + vector._size] = _data[i]; 
	}

	for (size_t i = pos; i < pos + vector._size; i++)
	{
		_data[i] = vector._data[i - pos];
	}

	_size += vector._size;
}

void Vector::popBack()
{
	if (_size > 0)
	{
		_size--;
	}
}

void Vector::popFront()
{
	for (size_t i = 0; i < _size - 1; i++)
	{
		_data[i] = _data[i + 1];
	}

	_size--;
}

void Vector::erase(size_t pos, size_t count)
{
	if (pos + count > _size)
	{
		_size -= (_size - pos);
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

	}
	else if (beginPos >= _size)
	{

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
	return static_cast<double>(_size/_capacity);
}

Value& Vector::operator[](size_t idx)
{
	/*
		if (idx >= _size)
		{
			return
		}
	*/	
	return _data[idx];
}

const Value& Vector::operator[](size_t idx) const
{
	/*
		if (idx >= _size)
		{
			return
		}
	*/	
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
	if ( _data == nullptr)
	{
		_data = new Value[1];
	}
	if (capacity <= _capacity)
	{
		
	}
	else
	{
		while(_capacity < capacity)
		{
			_capacity = static_cast<size_t>(_capacity * _multiplicativeCoef);
		}
		
		Value* ptr = _data;
		_data = new Value[_capacity];
		
		for (size_t i = 0; i < _size; ++i)
		{
			_data[i] = ptr[i];
		}
		
		delete[] ptr;
	}
}

void Vector::shrinkToFit()
{
	if (loadFactor() <= (1 / _multiplicativeCoef))
	{
		while (loadFactor() <= (1 / _multiplicativeCoef))
		{
			_capacity /= _multiplicativeCoef;
		}
	}

	Value* ptr = _data;
	_data = new Value[_capacity];
		
	for (size_t i = 0; i < _size; ++i)
	{
		_data[i] = ptr[i];
	}
		
	delete[] ptr;
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
{

}

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

Vector::Iterator Vector::Iterator::operator--()
{
	--_ptr;
	return *this;
}

Vector::Iterator Vector::Iterator::operator--(int)
{
	Vector::Iterator bufIt = *this;
	--*this;
	return bufIt;
}

bool Vector::Iterator:: operator==(const Vector::Iterator& other) const
{
    return _ptr == other._ptr;
}

bool Vector::Iterator:: operator!=(const Vector::Iterator& other) const
{
    return !(*this == other);
}

