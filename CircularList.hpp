#ifndef CIRCULAR_LIST
#define CIRCULAR_LIST

#include "CircularIterator.hpp"
#include "CLImpl.hpp"

template< typename T >
class CircularList {
public:
	typedef typename CLImpl<T>::value_type		value_type;
	typedef typename CLImpl<T>::reference		reference;
	typedef typename CLImpl<T>::const_reference	const_reference;
	typedef typename CLImpl<T>::pointer			pointer;
	typedef typename CLImpl<T>::const_pointer	const_pointer;
	typedef typename CLImpl<T>::size_type		size_type;
	typedef typename CLImpl<T>::difference_type	difference_type;
	typedef typename CLImpl<T>::iterator		iterator;
	typedef typename CLImpl<T>::const_iterator	const_iterator;
	typedef typename CircularIterator<T>		circular_iterator;
	typedef typename CircularIterator<const T>	const_circular_iterator;

	CircularList();
	CircularList( size_type n, const_reference value = T() );
	CircularList( const CircularList<T>& other );
	template< typename InputIterator >
		CircularList( InputIterator first, InputIterator last );

	~CircularList();

	CircularList<T>& operator=( const CircularList<T>& rhs );

	void assign( size_type n, const_reference value );
	template< typename InputIterator >
		void assign( InputIterator first, InputIterator last );

	iterator		begin();
	const_iterator	begin() const;
	iterator		end();
	const_iterator	end() const;

	bool empty() const;
	size_type size() const;

	reference		front();
	const_reference	front() const;
	reference		back();
	const_reference	back() const;

	void push_front( const_reference value );
	void pop_front();
	void push_back( const_reference value );
	void pop_back();

	iterator insert( iterator position, const_reference value );
	void insert( iterator position, size_type n, const_reference value );
	template< typename InputIterator >
		void insert( iterator position, InputIterator first, InputIterator last );

	iterator erase(	iterator position );
	iterator erase( iterator position, iterator last );

	void swap( CircularList<T>& other );
	void clear();

	void remove( const_reference value );
	template< typename Predicate >
		void remove( Predicate pred );

	void unique();
	template< typename BinaryPredicate >
		void unique( BinaryPredicate binary_pred );

private:
	CLImpl<T> impl_;
};

template< typename T >
CircularList<T>::CircularList()
{}

template< typename T >
CircularList<T>::CircularList( size_type n, const_reference value ) : 
	impl_( n, value )
{}

template< typename T >
CircularList<T>::CircularList( const CircularList<T>& other ) : impl_( other.impl_ )
{}

template< typename T >
template< typename InputIterator >
CircularList<T>::CircularList( InputIterator first, InputIterator last ) : impl_( first, last )
{}

template< typename T >
CircularList<T>::~CircularList()
{}

template< typename T >
CircularList<T>& CircularList<T>::operator=( const CircularList<T>& rhs )
{
	impl_ = rhs.impl_;
	return (*this);
}

template< typename T >
void CircularList<T>::assign( size_type n, const_reference value )
{
	CLImpl<T> temp( n, value );
	temp.swap( impl_ );
}

template< typename T >
template< typename InputIterator >
void CircularList<T>::assign( InputIterator first, InputIterator last )
{
	CLImpl<T> temp( first, last );
	temp.swap( impl_ );
}

template< typename T >
typename CircularList<T>::iterator CircularList<T>::begin()
{
	return iterator( impl_.begin_ );
}

template< typename T >
typename CircularList<T>::const_iterator CircularList<T>::begin() const
{
	return const_iterator( impl_.begin_ );
}

template< typename T >
typename CircularList<T>::iterator CircularList<T>::end()
{
	return iterator( impl_.end_ );
}

template< typename T >
typename CircularList<T>::const_iterator CircularList<T>::end() const
{
	return const_iterator( impl_.end_ );
}

template< typename T >
bool CircularList<T>::empty() const
{
	return (impl_.size_ == 0);
}

template< typename T >
typename CLImpl<T>::size_type CircularList<T>::size() const
{
	return (impl_.size_);
}

template< typename T >
typename CLImpl<T>::reference CircularList<T>::front()
{
	return (impl_.begin_)->value;
}

template< typename T >
typename CLImpl<T>::const_reference CircularList<T>::front() const
{
	return (impl_.begin_)->value;
}

template< typename T >
typename CLImpl<T>::reference CircularList<T>::back()
{
	return (impl_.end_)->prev->value;
}

template< typename T >
typename CLImpl<T>::const_reference CircularList<T>::back() const
{
	return (impl_.end_)->prev->value;
}

template< typename T >
void CircularList<T>::push_front( const_reference value )
{
	impl_.insert( begin(), value );
}

template< typename T >
void CircularList<T>::pop_front()
{
	impl_.erase( begin() );
}

template< typename T >
void CircularList<T>::push_back( const_reference value )
{
	impl_.insert( end(), value );
}

template< typename T >
void CircularList<T>::pop_back()
{
	impl_.erase( --end() );
}

template< typename T >
typename CircularList<T>::iterator CircularList<T>::insert( iterator position, const_reference value )
{
	return impl_.insert( position, value );
}

template< typename T >
void CircularList<T>::insert( iterator position, size_type n, const_reference value )
{
	for( ; n > 0; --n )
		position = insert( position, value );
}

template< typename T >
template< typename InputIterator >
void CircularList<T>::insert( iterator position, InputIterator first, InputIterator last )
{
	while( first != last ) {
		insert( position, *first );
		++first;
	}
}

template< typename T >
typename CircularList<T>::iterator CircularList<T>::erase( iterator position )
{
	return impl_.erase( position );
}

template< typename T >
typename CircularList<T>::iterator CircularList<T>::erase( iterator position, iterator last )
{
	iterator iter;
	while( position != last ) {
		iter = erase( position );
		position = iter;
		++position;
	}
	return iter;
}

template< typename T >
void CircularList<T>::swap( CircularList<T>& other )
{
	impl_.swap( other.impl_ );
}

template< typename T >
void CircularList<T>::clear()
{
	CLImpl<T> temp;
	temp.swap( impl_ );
}

template< typename T >
void CircularList<T>::remove( const_reference value )
{
	for( iterator i = begin(); i != end(); ++i )
		if( *i == value )
			i = erase( i );
}

template< typename T >
template< typename Predicate >
void CircularList<T>::remove( Predicate pred )
{
	for( iterator i = begin(); i != end(); ++i )
		if( pred(*i) )
			i = erase( i );
}

template< typename T >
void CircularList<T>::unique()
{
	if( size_ > 1 ) {
		iterator iter = begin();
		++iter;
		while( iter != end() ) {
			iterator prev = iter;
			--prev;
			if( *iter == *prev ) {
				erase(iter);
				iter = prev;
			}
			++iter;
		}
	}
}

template< typename T >
template< typename BinaryPredicate >
void CircularList<T>::unique( BinaryPredicate binary_pred )
{
	if( size_ > 1 ) {
		iterator iter = begin();
		++iter;
		while( iter != end() ) {
			iterator prev = iter;
			--prev;
			if( pred(*iter, *prev) ) {
				erase(iter);
				iter = prev;
			}
			++iter;
		}
	}
}

#endif