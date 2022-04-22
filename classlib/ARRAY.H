/* ***********************************************

Copyright, 1997 AT&T Corporation.  All rights
reserved.  Permission is granted for individuals
to use this material for educational purposes
in connection with the book Ruminations on C++,
Addison-Wesley, 1997, ISBN 0-201-42339-1.

************************************************** */

#ifndef   __ARRAY_H
#define   __ARRAY_H

template <class T> class Array;
template <class T> class Array_data;

template<class T> class Pointer {
	Array_data<T>* ap;
	unsigned sub;

public:
	friend int operator-(const Pointer<T>&, const Pointer<T>&);
	//friend int operator==(const Pointer<T>&, const Pointer<T>&);
friend int operator==(const Pointer<T>& op1, const Pointer<T>& op2) 
{

	if (op1.ap != op2.ap) 
		return 0;
	return (op1.sub == op2.sub);
}

	//friend int operator!=(const Pointer<T>&, const Pointer<T>&);
friend int operator!=(const Pointer<T>& op1, 
	       const Pointer<T>& op2) 
{
	return !(op1 == op2);
}

	friend int operator<(const Pointer<T>&, const Pointer<T>&);

	friend Pointer<T> operator-(const Array<T>&, int);
	friend Pointer<T> operator-(int, const Array<T>&);
	friend Pointer<T> operator+(const Array<T>&, int);
	friend Pointer<T> operator+(int, const Array<T>&);

	friend Pointer<T> operator-(const Pointer<T>&, int);
	friend Pointer<T> operator-(int, const Pointer<T>&);
	friend Pointer<T> operator+(const Pointer<T>&, int);
	friend Pointer<T> operator+(int, const Pointer<T>&);
public:
	Pointer(Array<T>& a, unsigned n = 0):
		ap(a.data), 	
		sub(n) { ++ap->use; }

	Pointer(): ap(0), sub(0) { }

	Pointer(const Pointer<T>& p): 
		ap(p.ap), sub(p.sub) 
	{
		if (ap)
			++ap->use;
	}

	~Pointer() 
	{
		if (ap && --ap->use == 0)
			delete ap;
	}

	Pointer& operator=(const Pointer<T>& p) 
	{
		if (p.ap)
			++p.ap->use;
		if (ap && --ap->use == 0)
			delete ap;
		ap = p.ap;
		sub = p.sub;
		return *this;
	}

	T& operator*() const 
	{
		if (ap == 0) 
			throw "* of unbound Ptr_to_const";
		return (*ap)[sub];
	}


	Pointer& operator++() 
	{
		++sub;
		return *this;
	}
	Pointer& operator--() 
	{
		--sub;
		return *this;
	}

	Pointer operator++(int) 
	{
		Pointer ret = *this;
		++sub;
		return ret;
	}
	Pointer operator--(int) 
	{
		Pointer ret = *this;
		--sub;
		return ret;
	}
	Pointer& operator+=(int n) 
	{
		sub += n;
		return *this;
	}
	Pointer& operator-=(int n) 
	{
		sub -= n;
		return *this;
	}
protected:
};


template<class T> class Array_data {

	friend class Array<T>;
	friend class Pointer<T>;

	Array_data(unsigned size = 0): 
		sz(size), data(new T[size]), use(1) { }
	~Array_data() { delete [] data; }

	const T& operator[](unsigned n) const 
	{
		if (n >= sz) 
			throw "Array subscript out of range";
		return data[n];
	}
	T& operator[](unsigned n) 
	{
		if (n >= sz) 
			throw "Array subscript out of range";
		return data[n];
	}
	
	void resize(unsigned);
	void copy(T*, unsigned);
	void grow(unsigned);
	void clone(const Array_data&, unsigned);
	Array_data(const Array_data&);	// not implemented
	Array_data& operator=(const Array_data&);	// not implemented

	T* data;
	unsigned sz;
	int use;
};

template<class T> class Array {
	friend class Pointer<T>;
public:
	Array(unsigned size): 
		data(new Array_data<T>(size)) { }
	~Array() 
	{
		if (--data->use == 0)
			delete data;
	}

	const T& operator[](unsigned n) const 
	{
		return (*data)[n];
	}
	T& operator[](unsigned n) 
	{
		return (*data)[n];
	}

	unsigned size()
	{
		return data->sz;
	}

	void resize(unsigned n)
	{
		data->resize(n);
	}

	void grow(unsigned n)
	{
		data->grow(n);
	}

	void reserve(unsigned new_sz)
	{
		if (new_sz >= data->sz)
			data->grow(new_sz);
	}

	Array(const Array& a): data(new Array_data<T>(a.data->sz))
	{
		data->copy(a.data->data, a.data->sz);
	}
	Array& operator=(const Array& a)
	{
		if (this != &a)
			data->clone(*a.data, a.data->sz);
		return *this;
	}
private:
	Array_data<T>* data;
};

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
	for (unsigned i = 0; i < n; i++)
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
int operator-(const Pointer<T>& op1, 
		 const Pointer<T>& op2) 
{
	return (int)op1.sub - (int)op2.sub;
}
/*
template <class T>
int operator==(const Pointer<T>& op1, 
	       const Pointer<T>& op2) 
{

	if (op1.ap != op2.ap) 
		return 0;
	return (op1.sub == op2.sub);
}

template <class T>
int operator!=(const Pointer<T>& op1, 
	       const Pointer<T>& op2) 
{
	return !(op1 == op2);
}
*/
template <class T>
int operator<(const Pointer<T>& op1, 
	      const Pointer<T>& op2) 
{

	if (op1.ap != op2.ap) 
		throw "== on different Arrays";
	return op1.sub < op2.sub;
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
Pointer<T> operator-(const Array<T>& p, int n) 
{
	Pointer<T> ret = p;
	return ret -= n;
}

template <class T>
Pointer<T> operator-(int n, const Array<T>& p) 
{
	Pointer<T> ret = p;
	return ret -= n;
}

template <class T>
Pointer<T> operator+ (const Array<T>& p, int n) 
{
	Pointer<T> ret = p;
	return ret += n;
}

template <class T>
Pointer<T> operator+(int n, const Array<T>& p) 
{
	Pointer<T> ret = p;
	return ret += n;
}

#endif
