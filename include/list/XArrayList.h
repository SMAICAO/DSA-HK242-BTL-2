/*
* File:   XArrayList.h
*/

#ifndef XARRAYLIST_H
#define XARRAYLIST_H
#include "IList.h"
#include <memory.h>
#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

// REMOVE BEFORE SUBMITTING
#include <utility>
inline std::ostream& operator<<(ostream& os, const pair<char, int>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

template <class T>
class XArrayList : public IList<T>
{
public:
    class Iterator; // forward declaration

protected:
    T *data;                                 // dynamic array to store the list's items
    int capacity;                            // size of the dynamic array
    int count;                               // number of items stored in the array
    bool (*itemEqual)(T &lhs, T &rhs);       // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(XArrayList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    XArrayList(
        void (*deleteUserData)(XArrayList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0,
        int capacity = 10);
    XArrayList(const XArrayList<T> &list);
    XArrayList<T> &operator=(const XArrayList<T> &list);
    ~XArrayList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size() const;
    void clear();
    T &get(int index) const;
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: BEGIN

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(XArrayList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    Iterator begin()
    {
        return Iterator(this, 0);
    }
    Iterator end()
    {
        return Iterator(this, count);
    }

    /** free:
     * if T is pointer type:
     *     pass THE address of method "free" to XArrayList<T>'s constructor:
     *     to:  remove the user's data (if needed)
     * Example:
     *  XArrayList<Point*> list(&XArrayList<Point*>::free);
     *  => Destructor will call free via function pointer "deleteUserData"
     */
    static void free(XArrayList<T> *list)
    {
        typename XArrayList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

protected:
    void checkIndex(int index) const;     // check validity of index for accessing
    void ensureCapacity(int index); // auto-allocate if needed

    /** equals:
     * if T: primitive type:
     *      indexOf, contains: will use native operator ==
     *      to: compare two items of T type
     * if T: object type:
     *      indexOf, contains: will use native operator ==
     *      to: compare two items of T type
     *      Therefore, class of type T MUST override operator ==
     * if T: pointer type:
     *      indexOf, contains: will use function pointer "itemEqual"
     *      to: compare two items of T type
     *      Therefore:
     *      (1): must pass itemEqual to the constructor of XArrayList
     *      (2): must define a method for comparing
     *           the content pointed by two pointers of type T
     *          See: definition of "equals" of class Point for more detail
     */
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }

    void copyFrom(const XArrayList<T> &list);

    void removeInternalData();

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    // Iterator: BEGIN
    class Iterator
    {
    private:
        int cursor;
        XArrayList<T> *pList;

    public:
        Iterator(XArrayList<T> *pList = 0, int index = 0)
        {
            this->pList = pList;
            this->cursor = index;
        }
        Iterator &operator=(const Iterator &iterator)
        {
            cursor = iterator.cursor;
            pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            T item = pList->removeAt(cursor);
            if (removeItemData != 0)
                removeItemData(item);
            cursor -= 1; // MUST keep index of previous, for ++ later
        }

        T &operator*()
        {
            return pList->data[cursor];
        }
        bool operator!=(const Iterator &iterator)
        {
            return cursor != iterator.cursor;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            this->cursor++;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
    // Iterator: END
};

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline XArrayList<T>::XArrayList(
    void (*deleteUserData)(XArrayList<T> *),
    bool (*itemEqual)(T &, T &),
    int capacity)
{
    // TODO
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;
    this->capacity = capacity;

    data = new T[capacity];
    count = 0;
}

template <class T>
inline void XArrayList<T>::copyFrom(const XArrayList<T> &list)
{
    /*
    * Copies the contents of another XArrayList into this list.
    * Initializes the list with the same capacity as the source list and copies all elements.
    * Also duplicates user-defined comparison and deletion functions, if applicable.
    */
    // TODO
    removeInternalData();

    this->capacity = list.capacity;
    this->count = list.count;
    
    data = new T[capacity];
    for (int i = 0; i < list.count; i++)
    {
        data[i] = list.data[i];
    }

    // Copy functions
    this->itemEqual = list.itemEqual;
    this->deleteUserData = list.deleteUserData;
}

template <class T>
inline void XArrayList<T>::removeInternalData()
{
    /*
    * Clears the internal data of the list by deleting the dynamic array and any user-defined data.
    * If a custom deletion function is provided, it is used to free the stored elements.
    * Finally, the dynamic array itself is deallocated from memory.
    */
    // TODO
    if (data != nullptr) {
        if (deleteUserData != nullptr) {
            deleteUserData(this);
        }
        
        delete[] data;
    }
    capacity = 0;
    count = 0;
}

template <class T>
inline XArrayList<T>::XArrayList(const XArrayList<T> &list)
{
    // TODO
    copyFrom(list);
}

template <class T>
inline XArrayList<T> &XArrayList<T>::operator=(const XArrayList<T> &list)
{
    // TODO
    if (this != &list) {
        copyFrom(list);
    }

    return *this;
}

template <class T>
inline XArrayList<T>::~XArrayList()
{
    // TODO
    removeInternalData();

    itemEqual = nullptr;
    deleteUserData = nullptr;
}

template <class T>
inline void XArrayList<T>::add(T e)
{
    // TODO
    ensureCapacity(count + 1);
    data[count++] = e;
}

template <class T>
inline void XArrayList<T>::add(int index, T e)
{
    // TODO
    if (index < 0 || index > count) {
        throw out_of_range("Index is out of range!");
    }

    ensureCapacity(count + 1);

    for (int i = count; i > index; i--) {
        data[i] = data[i - 1];
    }

    data[index] = e;
    count++;
}

template <class T>
inline T XArrayList<T>::removeAt(int index)
{
    // TODO
    checkIndex(index);

    T removedValue = data[index];
    for (int i = index; i < count - 1; i++) {
        data[i] = data[i + 1];
    }

    count--;
    return removedValue;
}

template <class T>
inline bool XArrayList<T>::removeItem(T item, void (*removeItemData)(T))
{
    // TODO
    int index = indexOf(item);
    
    if (index == -1) {
        T removedValue = removeAt(index);

        if (removeItemData != nullptr) {
            removeItemData(removedValue);
        }

        return true;
    } else {
        return false;
    }
}

template <class T>
inline bool XArrayList<T>::empty()
{
    // TODO
    return count == 0;
}

template <class T>
inline int XArrayList<T>::size() const
{
    // TODO
    return count;
}

template <class T>
inline void XArrayList<T>::clear()
{
    // TODO
    removeInternalData();

    capacity = 10;
    data = new T[capacity];
}

template <class T>
inline T &XArrayList<T>::get(int index) const
{
    // TODO
    checkIndex(index);

    return data[index];
}

template <class T>
inline int XArrayList<T>::indexOf(T item)
{
    // TODO
    for (int i = 0; i < count; i++) {
        if (equals(data[i], item, itemEqual)) {
            return i;
        }
    }

    return -1;
}
template <class T>
inline bool XArrayList<T>::contains(T item)
{
    // TODO
    return indexOf(item) != -1;
}

template <class T>
inline string XArrayList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the array list into a string representation, formatting each element using a user-defined function.
     * If no function is provided, it uses the default string representation for each element.
     * Example: Given an array list with elements {1, 2, 3} and a function that converts integers to strings, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function pointer for converting items of type T to strings. If null, default to the string conversion of T.
     * @return A string representation of the array list with elements separated by commas and enclosed in square brackets.
     */

    // TODO
    stringstream ss;
    ss << "[";
    for (int i = 0; i < count; i++) {
        if (i > 0) {
            ss << ", ";
        }

        if (item2str != nullptr) {
            ss << item2str(data[i]);
        } else {
            ss << data[i];
        }
    }
    ss << "]";
    return ss.str();
}

//////////////////////////////////////////////////////////////////////
//////////////////////// (private) METHOD DEFNITION //////////////////
//////////////////////////////////////////////////////////////////////
template <class T>
inline void XArrayList<T>::checkIndex(int index) const
{
    /**
     * Validates whether the given index is within the valid range of the list.
     * Throws an std::out_of_range exception if the index is negative or exceeds the number of elements.
     * Ensures safe access to the list's elements by preventing invalid index operations.
     */
    // TODO
    if (index < 0 || index >= count) {
        throw out_of_range("Index is out of range!");
    }
}
template <class T>
inline void XArrayList<T>::ensureCapacity(int index)
{
    /**
     * Ensures that the list has enough capacity to accommodate the given index.
     * If the index is out of range, it throws an std::out_of_range exception. If the index exceeds the current capacity,
     * reallocates the internal array with increased capacity, copying the existing elements to the new array.
     * In case of memory allocation failure, catches std::bad_alloc.
     */
    // TODO
    if (index < 0) {
        throw out_of_range("Index is out of range!");
    }

    if (index >= capacity) {
        int newCapacity = capacity * 1.5, newCount = count;

        T *newData = new T[newCapacity];
        for (int i = 0 ; i < count; i++) {
            newData[i] = data[i];
        }

        removeInternalData();
        data = newData;
        capacity = newCapacity;
        count = newCount;
    }
}

#endif /* XARRAYLIST_H */
