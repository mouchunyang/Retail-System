#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

template <
         typename T,
         typename TComparator = std::equal_to<T>,
         typename PComparator = std::less<double>, //a min heap
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const TComparator& tcomp = TComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item with the provided priority
    void push(double pri, const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey reduces the current priority of
    /// item to newpri, moving it up in the heap
    /// as appropriate.
    void decreaseKey(double newpri, const T& item);

    double getPriority(const T item);

private:
    /// Add whatever helper functions you need below
    // return the location of the current loc;
    void trickleUp(unsigned int loc);

    void heapify(unsigned int loc);


    // These should be all the data members you need.
    std::vector< std::pair<double, T> > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, TComparator> keyToLocation_;

};

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const TComparator& tcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, tcomp)

{

}

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::~Heap()
{

}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::push(double priority, const T& item)
{
    // You complete.
    store_.push_back(std::pair<double, T>(priority, item));
    keyToLocation_[item] = store_.size()-1; //insert to unorder_map
    trickleUp(store_.size() - 1);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::trickleUp(unsigned int loc){
  if (loc == 0) { return; }
  unsigned int parent = (loc-1)/m_;
  while( loc > 0 && c_(store_[loc].first, store_[parent].first) ){
    keyToLocation_[store_[parent].second] = loc;
    keyToLocation_[store_[loc].second] = parent;

    std::pair<double, T> parentItem = store_[parent];
    store_[parent] = store_[loc];
    store_[loc] = parentItem;
    loc = parent;
    parent = (parent-1)/m_;
  } 
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::decreaseKey(double priority, const T& item)
{
    // You complete
    if (keyToLocation_.find(item) == keyToLocation_.end())
        return;
    else {
        unsigned int loc = keyToLocation_[item];    
        double oldPriority = store_[loc].first;
        store_[loc].first = priority;
        if (priority > oldPriority){
            heapify(loc);
        }
        else{
            trickleUp(loc);
        }
    }
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
T const & Heap<T,TComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }
    // You complete
    return store_[0].second;
}

/// Removes the top element
template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    //unordered_map
    keyToLocation_.erase(store_[0].second);
    keyToLocation_[store_.back().second] = 0;
    store_[0] = store_.back();
    store_.pop_back();
    heapify(0);
    //keyToLocation_[store_[newLoc].second] = newLoc;
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::heapify(unsigned int loc){
  if (store_.size() <= loc*m_+ 1){
    //it is a leaf node.
    return;
  }
  unsigned int leftchild = m_*loc + 1;
  unsigned int targetchild = leftchild;
  for (int i = 1; i < m_; i++){
    if (store_.size() > (unsigned int)(i + leftchild)){ //node exists
      unsigned int ichild = leftchild + i;
      if (c_(store_[ichild].first, store_[targetchild].first)){
        targetchild = ichild;
      }
    }
  }
  if (c_(store_[targetchild].first, store_[loc].first)){ //swap and process
    keyToLocation_[store_[loc].second] = targetchild;
    keyToLocation_[store_[targetchild].second] = loc; 
    
    std::pair<double, T> item = store_[loc];
    store_[loc] = store_[targetchild];
    store_[targetchild] = item;
    heapify(targetchild);
  }
}

/// returns true if the heap is empty
template <typename T, typename TComparator, typename PComparator, typename Hasher >
bool Heap<T,TComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
double Heap<T,TComparator,PComparator,Hasher>::getPriority(const T item){
    if (keyToLocation_.find(item) != keyToLocation_.end())  
        return store_[keyToLocation_[item]].first;
    else
        throw std::out_of_range("Out of Bound");    
}


#endif

