#ifndef CIRCULAR_LIST_IMPL
#define CIRCULAR_LIST_IMPL

#include "Iterator.hpp"
#include "Node.hpp"
#include <cstddef>
#include <algorithm>

template< typename T >
class NodeSentry {
public:
	NodeSentry( Node<T>* p );
	void release();
	~NodeSentry();
private:
	Node<T>* ptr_;
};

template< typename T >
NodeSentry<T>::NodeSentry( Node<T>* p ) : ptr_( p )
{ }

template< typename T >
void NodeSentry<T>::release()
{
	ptr_ = 0;
}

template< typename T >
NodeSentry<T>::~NodeSentry()
{
	while( ptr_ ) {
		if( ! ptr_->value ) {
            /* At the tail of the list */
			delete ptr_;
			ptr_ = 0;
		}
		else {
			Node<T>* prev = ptr_;
			ptr_ = ptr_->next;
			delete prev;
		}
	}
}

template< typename T >
class CLImpl {
public:
	typedef T						value_type;
	typedef T&						reference;
	typedef const T&				const_reference;
	typedef T*						pointer;
	typedef const T*				const_pointer;
	typedef std::size_t				size_type;
	typedef std::size_t				difference_type;
	typedef Iterator<T>				iterator;
	typedef Iterator<const T>		const_iterator;

	CLImpl();
	CLImpl( const CLImpl<T>& other );
	explicit CLImpl( size_type n, const T& value );
	template< typename InputIterator >
		CLImpl( InputIterator first, InputIterator last );

	~CLImpl();

	iterator insert( iterator position, const T& value );

	iterator erase( iterator element );

	void swap( CLImpl<T>& other );

	Node<T>* begin_;
	Node<T>* end_;
	size_type size_;
};

template< typename T >
CLImpl<T>::CLImpl() : begin_( new Node<T>(0, 0) ), end_( 0 ), size_( 0 )
{ 
	end_ = begin_;
}

template< typename T >
CLImpl<T>::CLImpl( const CLImpl<T>& other ) : begin_( new Node<T>(0, 0) ), end_( 0 ), size_( 0 )
{
	end_ = begin_;
    /* NodeSentry is used for exception safety.
     * For example, what if an exception is throwed during the allocation?
     * To avoid memory leaks, we need to delete the allocated memory
     */
	NodeSentry<T> sentry( begin_ );
	Node<T>* temp = begin_;
	Node<T>* pOther = other.begin_;
	for( ; size_ < other.size_; ++size_ ) {
		temp->next = new Node<T>( temp, 0, *(pOther->value) );
		temp = temp->next;
		pOther = pOther->next;
	}
	begin_ = end_->next;
	temp->next = end_;
	end_->prev = temp;
	sentry.release();
}

template< typename T >
CLImpl<T>::CLImpl( typename CLImpl<T>::size_type n, const T& value ) : begin_( new Node<T>(0, 0) ), end_( 0 ), size_( n )
{
	end_ = begin_;
	NodeSentry<T> sentry( begin_ );
	Node<T>* temp = begin_;
	for( ; n > 0; --n ) {
		temp->next = new Node<T>( temp, 0, value );
		temp = temp->next;
	}
	begin_ = end_->next;
	temp->next = end_;
	end_->prev = temp;
	sentry.release();
}

template< typename T >
template< typename InputIterator >
CLImpl<T>::CLImpl( InputIterator first, InputIterator last ) : begin_( new Node<T>(0, 0) ), end_( 0 ), size_( 0 )
{
	end_ = begin_;
	NodeSentry<T> sentry( begin_ );
	Node<T>* temp = begin_;
	for(; first != last; ++first ) {
		temp->next = new Node<T>( temp, 0, *first );
		temp = temp->next;
		++size_;
	}
	begin_ = end_->next;
	temp->next = end_;
	end_->prev = temp;
	sentry.release();
}

template< typename T >
CLImpl<T>::~CLImpl()
{
    /* A smart implementation. The destructor of NodeSentry will
     * automatically release the memory
     */
	NodeSentry<T> sentry( begin_ );
}

template< typename T >
typename CLImpl<T>::iterator CLImpl<T>::insert( typename CLImpl<T>::iterator position, const T& value )
{
	Node<T>* after = position.node_;
	Node<T>* before = after->prev;
	Node<T>* temp = new Node<T>( before, after, value );
	after->prev = temp;
	if( after == begin_ )
		begin_ = temp;
	if( !before ) {
		end_->next = temp;
	} else
		before->next = temp;
	return iterator( temp );
}

template< typename T >
typename CLImpl<T>::iterator CLImpl<T>::erase( typename CLImpl<T>::iterator element )
{
	Node<T>* after = (element.node_)->next;
	Node<T>* before = (element.node_)->prev;
	after->prev = before;
	before->next = after;
	--size_;
	if( element.node_ == begin_ )
		begin_ = after;
	delete element.node_;
	return iterator(after);
}

template< typename T >
void CLImpl<T>::swap( CLImpl<T>& other )
{
	std::swap( begin_, other.begin_ );
	std::swap( end_, other.end_ );
	std::swap( size_, other.size_ );
}

#endif
