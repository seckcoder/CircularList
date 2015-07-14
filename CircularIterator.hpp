#ifndef CL_CircularIterator
#define CL_CircularIterator

#include "Node.hpp"
#include "Iterator.hpp"
#include <iterator>
#include <cstddef>

template< typename T >
class CLImpl;

template< typename T >
class CircularIterator : 
	public std::iterator< std::bidirectional_iterator_tag, T, std::size_t, T*, T& > {
public:
	CircularIterator( );
	CircularIterator( const CircularIterator<T>& );
	CircularIterator( Iterator<T> other );

	CircularIterator<T>& operator=( const CircularIterator<T>& );

	Iterator<T> toIterator() const;

	bool eq( const CircularIterator<T>& other ) const { return (node_ == other.node_); }

	reference operator*();
	pointer operator->();

	CircularIterator<T>& operator++();
	CircularIterator<T> operator++(int);

	CircularIterator<T>& operator--();
	CircularIterator<T> operator--(int);
private:
	Node<T>* node_;
};

template< typename T >
bool operator==( const CircularIterator<T>& lhs, const CircularIterator<T>& rhs )
{
	return lhs.eq(rhs) ;
}

template< typename T >
bool operator!=( const CircularIterator<T>& lhs, const CircularIterator<T>& rhs )
{
	return !(lhs == rhs);
}

template< typename T >
CircularIterator<T>::CircularIterator() : node_(0)
{}

template< typename T >
CircularIterator<T>::CircularIterator( const CircularIterator<T>& other ) : node_( other.node_ )
{}

template< typename T >
CircularIterator<T>::CircularIterator( Iterator<T> other )
{
	node_ = other.node_;
	if( ! node_->value )
		node_ = node_->next;
}

template< typename T >
CircularIterator<T>& CircularIterator<T>::operator=( const CircularIterator<T>& rhs )
{
	node_ = rhs.node_;
	return *this;
}

template< typename T >
Iterator<T> CircularIterator<T>::toIterator() const 
{
	return Iterator<T>( node_ );
}

template< typename T >
typename CircularIterator<T>::reference CircularIterator<T>::operator*()
{
	return *(node_->value);
}

template< typename T >
typename CircularIterator<T>::pointer CircularIterator<T>::operator->()
{
	return node_->value;
}

template< typename T >
typename CircularIterator<T>& CircularIterator<T>::operator++()
{
	node_ = node_->next;
	if( ! node_->value )
		node_ = node_->next;
	return (*this);
}

template< typename T >
CircularIterator<T> CircularIterator<T>::operator++(int)
{
	CircularIterator<T> temp(*this);
	++(*this);
	return temp;
}

template< typename T >
CircularIterator<T>& CircularIterator<T>::operator--()
{
	node_ = node_->prev;
	if( ! node_->value )
		node_ = node_->prev;
	return (*this);
}

template< typename T >
CircularIterator<T> CircularIterator<T>::operator--(int)
{
	CircularIterator<T> temp(*this);
	--(*this);
	return temp;
}

#endif