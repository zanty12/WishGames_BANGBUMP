/*==============================================================================

	Title		: LIST
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _LIST_H_
#define _LIST_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* define宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* extern宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototype宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class宣言
------------------------------------------------------------------------------*/
namespace COLLECTION {
	template<class T>
	class Node {
	public:
		Node<T> *previous = nullptr;
		Node<T> *next = nullptr;
		void *collection = nullptr;
		T data;



	public:
		Node(void *collection) : collection(collection) { }
		Node(void *collection, const T data) : collection(collection), data(data) { }

		void push_next(Node<T> *node) {
			if (node == nullptr || node == this) return;

			if (next) next->previous = node;
			node->next = next;
			node->previous = this;
			next = node;
			node->collection = collection;
		}

		void push_previous(Node<T> *node) {
			if (node == nullptr || node == this) return;

			if (previous) previous->next = node;
			node->previous = previous;
			node->next = this;
			previous = node;
			node->collection = collection;
		}

		void pop() {
			if (next && next->previous) next->previous = previous;
			if (previous && previous->next) previous->next = next;
			next = nullptr;
			previous = nullptr;
			collection = nullptr;
		}
	};

	template<class T>
	class Iterator {
	private:
		Node<T> *current;

	public:
		Iterator(Node<T> *node) : current(node) { }

		Iterator<T> operator=(Node<T> *node) { return Iterator<T>(current = node); }
		Iterator<T> operator=(Iterator<T> iterator) { return Iterator<T>(current = iterator.current); }
		T operator*() { return current->data; }
		Iterator<T> operator++() { return Iterator<T>(current = current->next); }
		Iterator<T> operator--() { return Iterator<T>(current = current->previous); }
		bool operator==(Iterator<T> iterator) { return current == iterator.current; }
		bool operator!=(Iterator<T> iterator) { return current != iterator.current; }
		T operator->() { return current->data; }
		operator T() { return current->data; }

		bool IsEmpty(void) { return current == nullptr; }
	};

	template<class T>
	class Collection {
	protected:
		Node<T> *head = nullptr;
		Node<T> *tail = nullptr;
		Node<T> *current = nullptr;
		int count = 0;
		int currentIndex = -1;



	protected:
		void push_back(T data) {
			Node<T> *node = new Node<T>(this, data);

			if (count == 0) {
				head = tail = current = node;
				currentIndex = 0;
			}
			else {
				tail->push_next(node);
				tail = node;
			}

			count++;
		}

		void push_next(Node<T> *node, T data) {
			if (node) {
				node->push_next(new Node<T>(this, data));
				if (tail == node) tail = node;

				current = nullptr;
				count++;
			}
		}

		void push_previous(Node<T> *node, T data) {
			if (node) {
				node->push_previous(new Node<T>(this, data));
				if (head == node) head = node;

				current = nullptr;
				count++;
			}
		}

		void push_front(T data) {
			Node<T> *node = new Node<T>(this, data);

			if (count == 0) {
				head = tail = current = node;
				currentIndex = 0;
			}
			else {
				head->push_previous(node);
				head = node;
				currentIndex++;
			}

			count++;
		}



		Node<T> *get(T data) {
			if (current == nullptr) current = head, currentIndex = 0;



			while (current) {
				if (data == current->data) break;
				current = current->next;
				currentIndex++;
			}

			return current;
		}

		Node<T> *get_index(unsigned int index) {
			if (index < 0 || count <= index) return nullptr;
			if (current == nullptr) current = head, currentIndex = 0;


			if (index == 0) return head;
			else if (index == count - 1) return tail;
			while (current && currentIndex != index) {
				if (currentIndex < index) {
					currentIndex++;
					current = current->next;
				}
				else {
					currentIndex--;
					current = current->previous;
				}
			}

			return current;
		}

		Node<T> *pop(Node<T> *node) {
			if (node->collection != this) return nullptr;

			if (node) {
				node->pop();
				count--;

				if (count <= 0) {
					head = tail = current = nullptr;
					count = 0;
				}
			}

			return node;
		}

		void remove(Node<T> *node) {
			node = pop(node);
			if (node) delete node;
		}

		void remove_index(unsigned int index) {
			Node<T> *node = pop(get_index(index));
			if (node) delete node;
		}

		void all_remove(void) {
			current = head;
			while (current) {
				Node<T> *next = current->next;
				delete current;
				current = next;
			}
		}



	public:
		Collection() { }
		~Collection() { all_remove(); }


		int Count(void) {
			return count;
		}

		Iterator<T> begin(void) {
			return Iterator<T>(head);
		}

		Iterator<T> end(void) {
			return Iterator<T>(nullptr);
		}
	};

	template<class T>
	class List : public Collection<T> {
	public:
		void Add(T data) {
			Collection<T>::push_back(data);
		}

		void Insert(unsigned int index, T data) {
			Node<T> *node = Collection<T>::get_index(index);
			if (node) Collection<T>::push_previous(node, data);
		}

		void Remove(unsigned int index) {
			Collection<T>::remove_index(index);
		}

		T &GetIndex(unsigned int index) {
			return Collection<T>::get_index(index)->data;
		}

		T &operator[](unsigned int index) {
			return Collection<T>::get_index(index)->data;
		}
	};
}

#endif
