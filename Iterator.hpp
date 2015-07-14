#ifndef CL_ITERATOR
#define CL_ITERATOR

#include "Node.hpp"
#include <iterator>
#include <cstddef>

template< typename T >
class CLImpl;

template< typename T >
class CircularList;

template< typename T >
class CircularIterator;

template< typename T >
class Iterator : 
	public std::iterator< std::bidirectional_iterator_tag, T, std::size_t, T*, T& > {
public:
    using typename std::iterator<std::bidirectional_iterator_tag, T, std::size_t, T*, T&>::reference;
    using typename std::iterator<std::bidirectional_iterator_tag, T, std::size_t, T*, T&>::pointer;
	Iterator( );
	Iterator( const Iterator<T>& );

	Iterator<T>& operator=( const Iterator<T>& );

	bool eq( const Iterator<T>& other ) const { return (node_ == other.node_); }

	reference operator*();
	pointer operator->();

	Iterator<T>& operator++();
	Iterator<T> operator++(int);

	Iterator<T>& operator--();
	Iterator<T> operator--(int);
private:
	explicit Iterator( Node<T>* ip );
	friend class CLImpl<T>;
	friend class CircularList<T>;
	friend class CircularIterator<T>;
	Node<T>* node_;
};

template< typename T >
bool operator==( const Iterator<T>& lhs, const Iterator<T>& rhs )
{
	return lhs.eq(rhs) ;
}

template< typename T >
bool operator!=( const Iterator<T>& lhs, const Iterator<T>& rhs )
{
	return !(lhs == rhs);
}

template< typename T >
Iterator<T>::Iterator() : node_(0)
{}

template< typename T >
Iterator<T>::Iterator( const Iterator<T>& other ) : node_( other.node_ )
{}

template< typename T >
Iterator<T>::Iterator( Node<T>* ip ) : node_(ip )
{}

template< typename T >
Iterator<T>& Iterator<T>::operator=( const Iterator<T>& rhs )
{
	node_ = rhs.node_;
	return *this;
}

template< typename T >
typename Iterator<T>::reference Iterator<T>::operator*()
{
	return *(node_->value);
}

template< typename T >
typename Iterator<T>::pointer Iterator<T>::operator->()
{
	return node_->value;
}

template< typename T >
Iterator<T>& Iterator<T>::operator++()
{
	node_ = node_->next;
	return (*this);
}

template< typename T >
Iterator<T> Iterator<T>::operator++(int)
{
	Iterator<T> temp(*this);
	++(*this);
	return temp;
}

template< typename T >
Iterator<T>& Iterator<T>::operator--()
{
	node_ = node_->prev;
	return (*this);
}

template< typename T >
Iterator<T> Iterator<T>::operator--(int)
{
	Iterator<T> temp(*this);
	--(*this);
	return temp;
}

#endif
