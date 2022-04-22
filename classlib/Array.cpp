/* ***********************************************

Copyright, 1997 AT&T Corporation.  All rights
reserved.  Permission is granted for individuals
to use this material for educational purposes
in connection with the book Ruminations on C++,
Addison-Wesley, 1997, ISBN 0-201-42339-1.

************************************************** */

#include "array.h"

template <class T>
void Array_data<T>::resize(unsigned n) 
{
	if (n == sz) return;

	T* odata = data;
	data = new T[n];
	copy(odata, sz > n ? n: sz);
	delete [] odata;
	sz = n;
}

template <class T>
void Array_data<T>::copy(T* arr, unsigned n) 
{
	for (int i = 0; i < n; i++)
		data[i] = arr[i];
}

template <class T>
void Array_data<T>::grow(unsigned new_sz)
{
	unsigned nsz = sz;
	
	if (nsz == 0) nsz = 1;

	while (nsz <= new_sz) nsz *= 2;

	resize(nsz);
}

template <class T>
void Array_data<T>::clone(const Array_data& a, unsigned n)
{
	delete [] data;
	data = new T[sz = a.sz];
	copy(a.data, sz);
}

template <class T>
int operator-(const Ptr_to_const<T>& op1, 
		 const Ptr_to_const<T>& op2) 
{
	return (int)op1.sub - (int)op2.sub;
}

template <class T>
int operator==(const Ptr_to_const<T>& op1, 
	       const Ptr_to_const<T>& op2) 
{

	if (op1.ap != op2.ap) 
		return 0;
	return (op1.sub == op2.sub);
}

template <class T>
int operator!=(const Ptr_to_const<T>& op1, 
	       const Ptr_to_const<T>& op2) 
{
	return !(op1 == op2);
}

template <class T>
int operator<(const Ptr_to_const<T>& op1, 
	      const Ptr_to_const<T>& op2) 
{

	if (op1.ap != op2.ap) 
		throw "== on different Arrays";
	return op1.sub < op2.sub;
}

template <class T>
Ptr_to_const<T> operator-(const Ptr_to_const<T>& p, int n) 
{
	Ptr_to_const<T> ret = p;
	return ret -= n;
}

template <class T>
Ptr_to_const<T> operator-(int n, const Ptr_to_const<T>& p) 
{
	Ptr_to_const<T> ret = p;
	return ret -= n;
}

template <class T>
Ptr_to_const<T> operator+ (const Ptr_to_const<T>& p, int n) 
{
	Ptr_to_const<T> ret = p;
	return ret += n;
}

template <class T>
Ptr_to_const<T> operator+(int n, const Ptr_to_const<T>& p) 
{
	Ptr_to_const<T> ret = p;
	return ret += n;
}

template <class T>
Pointer<T> operator-(const Pointer<T>& p, int n) 
{
	Pointer<T> ret = p;
	return ret -= n;
}

template <class T>
Pointer<T> operator-(int n, const Pointer<T>& p) 
{
	Pointer<T> ret = p;
	return ret -= n;
}

template <class T>
Pointer<T> operator+ (const Pointer<T>& p, int n) 
{
	Pointer<T> ret = p;
	return ret += n;
}

template <class T>
Pointer<T> operator+(int n, const Pointer<T>& p) 
{
	Pointer<T> ret = p;
	return ret += n;
}

template <class T>
Ptr_to_const<T> operator-(const Array<T>& p, int n) 
{
	Ptr_to_const<T> ret = p;
	return ret -= n;
}

template <class T>
Ptr_to_const<T> operator-(int n, const Array<T>& p) 
{
	Ptr_to_const<T> ret = p;
	return ret -= n;
}

template <class T>
Ptr_to_const<T> operator+ (const Array<T>& p, int n) 
{
	Ptr_to_const<T> ret = p;
	return ret += n;
}

template <class T>
Ptr_to_const<T> operator+(int n, const Array<T>& p) 
{
	Ptr_to_const<T> ret = p;
	return ret += n;
}

template <class T>
Pointer<T> operator-(Array<T>& p, int n) 
{
	Pointer<T> ret = p;
	return ret -= n;
}

template <class T>
Pointer<T> operator-(int n, Array<T>& p) 
{
	Pointer<T> ret = p;
	return ret -= n;
}

template <class T>
Pointer<T> operator+ (Array<T>& p, int n) 
{
	Pointer<T> ret = p;
	return ret += n;
}

template <class T>
Pointer<T> operator+(int n, Array<T>& p) 
{
	Pointer<T> ret = p;
	return ret += n;
}
