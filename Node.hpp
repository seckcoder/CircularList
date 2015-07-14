#ifndef CL_NODE
#define CL_NODE

template< typename T >
struct Node {
public:
	Node( Node<T>* p, Node<T>* n, const T& v );
	Node( Node<T>* p, Node<T>* n );
	~Node();

	Node<T>*	prev;
	Node<T>*	next;
	T*			value;
private:
	Node( const Node<T>& );
	Node<T>& operator=( const Node<T>& );
};

/* I don't quite like this part since for every node we will allocate a new 
 * block of memory for value. Are there any better methods?
 */
template< typename T >
Node<T>::Node( Node<T>* p, Node<T>* n, const T& v ) : prev(p), next(n), value(new T(v) )
{}

template< typename T >
Node<T>::Node( Node<T>* p, Node<T>* n ) : prev(p), next(n), value(0)
{}

template< typename T >
Node<T>::~Node( )
{
	delete value;
}

#endif
