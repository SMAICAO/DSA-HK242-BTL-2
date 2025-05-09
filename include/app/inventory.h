#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "list/XArrayList.h"
#include "list/DLinkedList.h"
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

template<typename T> class List1D;
template<typename T> class List2D;
template<typename T>
inline ostream& operator<<(ostream& os, const List1D<T>& list);

template<typename T>
inline ostream& operator<<(ostream& os, const List2D<T>& matrix);

// -------------------- List1D --------------------
template <typename T>
class List1D
{
private:
    IList<T> *pList;

public:
    List1D();
    List1D(int num_elements);
    List1D(const T *array, int num_elements);
    List1D(const List1D<T> &other);
    virtual ~List1D();

    int size() const;
    T get(int index) const;
    void set(int index, T value);
    void add(const T &value);
    string toString() const;

    friend ostream& operator<< <T>(ostream& os, const List1D<T>& list);
    List1D<T> &operator=(const List1D<T> &list);
};

// -------------------- List2D --------------------
template <typename T>
class List2D
{
private:
    IList<IList<T> *> *pMatrix;

public:
    List2D();
    List2D(List1D<T> *array, int num_rows);
    List2D(const List2D<T> &other);
    virtual ~List2D();

    int rows() const;
    void setRow(int rowIndex, const List1D<T> &row);
    T get(int rowIndex, int colIndex) const;
    List1D<T> getRow(int rowIndex) const;
    string toString() const;

    friend ostream& operator<< <T>(ostream& os, const List2D<T>& matrix);
    List2D<T> &operator=(const List2D<T> &matrix);
};

struct InventoryAttribute
{
    string name;
    double value;
    InventoryAttribute() {};
    InventoryAttribute(const string &name, double value) : name(name), value(value) {}
    string toString() const { return name + ": " + to_string(value); }
    
    friend ostream &operator<<(ostream &os, const InventoryAttribute &attr)
    {
        os << attr.toString();
        return os;
    }

    bool operator==(const InventoryAttribute &other) const
    {
        return name == other.name && value == other.value;
    }
};

// -------------------- InventoryManager --------------------
class InventoryManager
{
private:
    List2D<InventoryAttribute> attributesMatrix;
    List1D<string> productNames;
    List1D<int> quantities;

public:
    InventoryManager();
    InventoryManager(const List2D<InventoryAttribute> &matrix,
                     const List1D<string> &names,
                     const List1D<int> &quantities);
    InventoryManager(const InventoryManager &other);

    int size() const;
    List1D<InventoryAttribute> getProductAttributes(int index) const;
    string getProductName(int index) const;
    int getProductQuantity(int index) const;
    void updateQuantity(int index, int newQuantity);
    void addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity);
    void removeProduct(int index);

    List1D<string> query(string attributeName, const double &minValue,
                         const double &maxValue, int minQuantity, bool ascending) const;

    void removeDuplicates();

    static InventoryManager merge(const InventoryManager &inv1,
                                  const InventoryManager &inv2);

    void split(InventoryManager &section1,
               InventoryManager &section2,
               double ratio) const;

    List2D<InventoryAttribute> getAttributesMatrix() const;
    List1D<string> getProductNames() const;
    List1D<int> getQuantities() const;
    string toString() const;

    InventoryManager &operator=(const InventoryManager &other);
};

// -------------------- List1D Method Definitions --------------------
template <typename T>
inline List1D<T>::List1D()
{
    // TODO
    pList = new XArrayList<T>();
}

template <typename T>
inline List1D<T>::List1D(int num_elements)
{
    // TODO
    pList = new XArrayList<T>(0, 0, num_elements);
}

template <typename T>
inline List1D<T>::List1D(const T *array, int num_elements)
{
    // TODO
    pList = new XArrayList<T>(0, 0, num_elements);

    for (int i = 0; i < num_elements; i++)
    {
        pList->add(array[i]);
    }
}

template <typename T>
inline List1D<T>::List1D(const List1D<T> &other)
{
    // TODO
    pList = new XArrayList<T>(0, 0, other.size());

    for (int i = 0; i < other.size(); i++) {
        pList->add(other.get(i));
    }
}

template <typename T>
inline List1D<T>::~List1D()
{
    // TODO
    delete pList;
}

template <typename T>
inline int List1D<T>::size() const
{
    // TODO
    return pList->size();
}

template <typename T>
inline T List1D<T>::get(int index) const
{
    // TODO
    return pList->get(index);
}

template <typename T>
inline void List1D<T>::set(int index, T value)
{
    // TODO
    if (0 <= index && index < pList->size()) {
        pList->removeAt(index);
        pList->add(index, value);
    } else if (index == pList->size()) {
        pList->add(value);
    } else {
        throw out_of_range("Index is out of range");
    }
}

template <typename T>
inline void List1D<T>::add(const T &value)
{
    // TODO
    pList->add(value);
}

template <typename T>
inline string List1D<T>::toString() const
{
    // TODO
    return pList->toString();
}

template <typename T>
inline ostream &operator<<(ostream &os, const List1D<T> &list)
{
    // TODO
    os << list.toString();
    return os;
}

template <typename T>
inline List1D<T> &List1D<T>::operator=(const List1D<T> &list)
{
    // TODO
    if (this != &list) {
        pList->clear();

        for (int i = 0; i < list.size(); i++) {
            pList->add(list.get(i));
        }
    }

    return *this;
}

// -------------------- List2D Method Definitions --------------------
template <typename T>
inline List2D<T>::List2D()
{
    // TODO
    pMatrix = new XArrayList<IList<T>*>();
}

template <typename T>
inline List2D<T>::List2D(List1D<T> *array, int num_rows)
{
    // TODO
    pMatrix = new XArrayList<IList<T>*>();

    for (int i = 0; i < num_rows; i++)
    {
        XArrayList<T>* row = new XArrayList<T>(0, 0, array[i].size());
        
        for (int j = 0; j < array[i].size(); j++) {
            row->add(array[i].get(j));
        }
        
        pMatrix->add(row);
    }
}

template <typename T>
inline List2D<T>::List2D(const List2D<T> &other)
{
    // TODO
    pMatrix = new XArrayList<IList<T>*>(0, 0, other.rows());

    for (int i = 0; i < other.rows(); i++)
    {
        IList<T>* row = new XArrayList<T>(0, 0, other.pMatrix->get(i)->size());
        
        for (int j = 0; j < other.pMatrix->get(i)->size(); j++) {
            row->add(other.pMatrix->get(i)->get(j));
        }
        
        pMatrix->add(row);
    }
}

template <typename T>
inline List2D<T>::~List2D()
{
    // TODO
    for (int i = 0; i < pMatrix->size(); i++) {
        delete pMatrix->get(i);
    }

    delete pMatrix;
}

template <typename T>
inline int List2D<T>::rows() const
{
    // TODO
    return pMatrix->size();
}

template <typename T>
inline void List2D<T>::setRow(int rowIndex, const List1D<T> &row)
{
    // TODO
    if (0 <= rowIndex && rowIndex < rows()) {
        IList<T>* newRow = new XArrayList<T>(0, 0, row.size());
        
        for (int i = 0; i < row.size(); i++) {
            newRow->add(row.get(i));
        }
        
        pMatrix->removeAt(rowIndex);
        pMatrix->add(rowIndex, newRow);
    } else if (rowIndex == rows()) {
        IList<T>* newRow = new XArrayList<T>(0, 0, row.size());
        
        for (int i = 0; i < row.size(); i++) {
            newRow->add(row.get(i));
        }
        
        pMatrix->add(newRow);
    } else {
        throw out_of_range("Index is out of range");
    }
}

template <typename T>
inline T List2D<T>::get(int rowIndex, int colIndex) const
{
    // TODO
    return pMatrix->get(rowIndex)->get(colIndex);
}

template <typename T>
inline List1D<T> List2D<T>::getRow(int rowIndex) const
{
    // TODO
    IList<T>* row = pMatrix->get(rowIndex);
    List1D<T> list;
    for (int i = 0; i < row->size(); i++) {
        list.add(row->get(i));
    }

    return list;
}

template <typename T>
inline string List2D<T>::toString() const
{
    // TODO
    stringstream ss;
    ss << "[";

    
    for (int i = 0; i < pMatrix->size(); i++) {
        if (i > 0) {
            ss << ", ";
        }
        ss << pMatrix->get(i)->toString();
    }
    
    ss << "]";
    return ss.str();
}

template <typename T>
inline ostream &operator<<(ostream &os, const List2D<T> &matrix)
{
    // TODO
    os << matrix.toString();
    return os;
}

template <typename T>
inline List2D<T> &List2D<T>::operator=(const List2D<T> &matrix)
{
    // TODO
    if (this != &matrix) {
        for (int i = 0; i < pMatrix->size(); i++) {
            delete pMatrix->get(i);
        }
        
        pMatrix->clear();
        
        for (int i = 0; i < matrix.rows(); i++) {
            IList<T>* row = new XArrayList<T>(0, 0, matrix.pMatrix->get(i)->size());
            
            for (int j = 0; j < matrix.pMatrix->get(i)->size(); j++) {
                row->add(matrix.pMatrix->get(i)->get(j));
            }
            
            pMatrix->add(row);
        }
    }
    return *this;
}

// -------------------- InventoryManager Method Definitions --------------------
inline InventoryManager::InventoryManager()
{
    // TODO
    attributesMatrix = List2D<InventoryAttribute>();
    productNames = List1D<string>();
    quantities = List1D<int>();
}

inline InventoryManager::InventoryManager(const List2D<InventoryAttribute> &matrix,
                                   const List1D<string> &names,
                                   const List1D<int> &quantities)
{
    // TODO
    this->attributesMatrix = matrix;
    this->productNames = List1D<string>();
    this->quantities = List1D<int>();
    
    for(int i = 0; i < names.size(); i++) {
        this->productNames.add(names.get(i));
    }
    
    for(int i = 0; i < quantities.size(); i++) {
        this->quantities.add(quantities.get(i));
    }
}

inline InventoryManager::InventoryManager(const InventoryManager &other)
{
    // TODO
    attributesMatrix = other.attributesMatrix;
    productNames = other.productNames;
    quantities = other.quantities;
}

inline int InventoryManager::size() const
{
    // TODO
    return attributesMatrix.rows();
}

inline List1D<InventoryAttribute> InventoryManager::getProductAttributes(int index) const
{
    // TODO
    if (index < 0 || index >= size()) {
        throw out_of_range("Index is invalid!");
    }

    return attributesMatrix.getRow(index);
}

inline string InventoryManager::getProductName(int index) const
{
    // TODO
    if (index < 0 || index >= size()) {
        throw out_of_range("Index is invalid!");
    }

    return productNames.get(index);
}

inline int InventoryManager::getProductQuantity(int index) const
{
    // TODO
    if (index < 0 || index >= size()) {
        throw out_of_range("Index is invalid!");
    }

    return quantities.get(index);
}

inline void InventoryManager::updateQuantity(int index, int newQuantity)
{
    // TODO
    if (index < 0 || index >= size()) {
        throw out_of_range("Index is invalid!");
    }

    quantities.set(index, newQuantity);
}

inline void InventoryManager::addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity)
{
    // TODO
    attributesMatrix.setRow(attributesMatrix.rows(), attributes);
    productNames.add(name);
    quantities.add(quantity);
}

inline void InventoryManager::removeProduct(int index)
{
    // TODO
    if (index < 0 || index >= size()) {
        throw out_of_range("Index is invalid!");
    }

    List2D<InventoryAttribute> newAttributesMatrix;
    List1D<string> newProductNames(size() - 1);
    List1D<int> newQuantities(size() - 1);

    for (int i = 0; i < size(); i++) {
        if (i != index) { // Only add elements that are not at the specified index
            newAttributesMatrix.setRow(newAttributesMatrix.rows(), attributesMatrix.getRow(i));
            newProductNames.add(productNames.get(i));
            newQuantities.add(quantities.get(i));
        }
    }

    attributesMatrix = newAttributesMatrix;
    productNames = newProductNames;
    quantities = newQuantities;
}

inline List1D<string> InventoryManager::query(string attributeName, const double &minValue,
                                       const double &maxValue, int minQuantity, bool ascending) const
{
    // TODO
    List1D<double> validAttributes;
    List1D<string> validNames;

    for (int i = 0; i < size(); i++) {
        if (quantities.get(i) >= minQuantity) {
            validNames.add(productNames.get(i));
        }
    }

    for (int i = 0; i < size(); i++) {
        for (int j = 0; j < attributesMatrix.getRow(i).size(); j++) {
            if (attributesMatrix.get(i, j).name == attributeName &&
                attributesMatrix.get(i, j).value >= minValue &&
                attributesMatrix.get(i, j).value <= maxValue) {
                validAttributes.add(attributesMatrix.get(i, j).value);
                break;
            }
        }
    }

    for (int i = 0; i < validAttributes.size() - 1; i++) {
        for (int j = i + 1; j < validAttributes.size(); j++) {
            if (ascending) {
                if (validAttributes.get(i) > validAttributes.get(j)) {
                    double temp = validAttributes.get(i);
                    validAttributes.set(i, validAttributes.get(j));
                    validAttributes.set(j, temp);

                    string tempName = validNames.get(i);
                    validNames.set(i, validNames.get(j));
                    validNames.set(j, tempName);
                }
            } else {
                if (validAttributes.get(i) < validAttributes.get(j)) {
                    double temp = validAttributes.get(i);
                    validAttributes.set(i, validAttributes.get(j));
                    validAttributes.set(j, temp);

                    string tempName = validNames.get(i);
                    validNames.set(i, validNames.get(j));
                    validNames.set(j, tempName);
                }
            }
        }
    }

    return validNames;
}

inline void InventoryManager::removeDuplicates()
{
    for (int i = 0; i < size() - 1; i++) {
        for (int j = i + 1; j < size();) { // Do not increment `j` here
            bool isDuplicate = false;

            // Check if product names match
            if (productNames.get(i) == productNames.get(j)) {
                // Check if attributes match
                List1D<InventoryAttribute> row1 = attributesMatrix.getRow(i);
                List1D<InventoryAttribute> row2 = attributesMatrix.getRow(j);

                if (row1.toString() == row2.toString()) {
                    isDuplicate = true;
                }
            }

            if (isDuplicate) {
                // Sum the quantities of the duplicate products
                int newQuantity = quantities.get(i) + quantities.get(j);
                quantities.set(i, newQuantity);

                // Remove the duplicate product
                removeProduct(j);
                // Do not increment `j` because the size has changed
            } else {
                j++; // Increment `j` only if no duplicate was removed
            }
        }
    }
}

inline InventoryManager InventoryManager::merge(const InventoryManager &inv1,
                                         const InventoryManager &inv2)
{
    // TODO
    List2D<InventoryAttribute> newAttributesMatrix;
    List1D<string> newProductNames;
    List1D<int> newQuantities;

    for (int i = 0; i < inv1.size(); i++) {
        newAttributesMatrix.setRow(newAttributesMatrix.rows(), inv1.getProductAttributes(i));
        newProductNames.add(inv1.getProductName(i));
        newQuantities.add(inv1.getProductQuantity(i));
    }

    for (int i = 0; i < inv2.size(); i++) {
        newAttributesMatrix.setRow(newAttributesMatrix.rows(), inv2.getProductAttributes(i));
        newProductNames.add(inv2.getProductName(i));
        newQuantities.add(inv2.getProductQuantity(i));
    }

    return InventoryManager(newAttributesMatrix, newProductNames, newQuantities);
}

inline void InventoryManager::split(InventoryManager &section1,
                             InventoryManager &section2,
                             double ratio) const
{
    // TODO
    double size1 = size() * ratio;
    int size1int = size1;
    if (size1 > size1int) {
        size1 = size1int + 1;
    }

    int size2 = size() - size1;

    int curr = 0;
    for (int i = 0; i < size1; i++) {
        section1.addProduct(getProductAttributes(curr), getProductName(curr), getProductQuantity(curr));
        curr++;
    }
    for (int i = 0; i < size2; i++) {
        section2.addProduct(getProductAttributes(curr), getProductName(curr), getProductQuantity(curr));
        curr++;
    }
}

inline List2D<InventoryAttribute> InventoryManager::getAttributesMatrix() const
{
    // TODO
    return attributesMatrix;
}

inline List1D<string> InventoryManager::getProductNames() const
{
    // TODO
    return productNames;
}

inline List1D<int> InventoryManager::getQuantities() const
{
    // TODO
    return quantities;
}

inline string InventoryManager::toString() const
{
    // TODO
    stringstream ss;
    ss << "InventoryManager[" << endl;
    ss << "  AttributesMatrix: " << attributesMatrix.toString() << "," << endl;
    ss << "  ProductNames: " << productNames.toString() << "," << endl;
    ss << "  Quantities: " << quantities.toString() << endl;
    ss << "]";

    return ss.str();
}

inline InventoryManager &InventoryManager::operator=(const InventoryManager &other)
{
    // TODO
    if (this != &other) {
        attributesMatrix = other.attributesMatrix;
        productNames = other.productNames;
        quantities = other.quantities;
    }

    return *this;
}

#endif /* INVENTORY_MANAGER_H */
