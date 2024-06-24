
#ifndef prob_h_
#define prob_h_

#include "bst.h"

#include <chrono>
//sample space is phantom
//in sample space s find complement of A
template<typename DataType>
class set{
    /*
    bst stores unique elements only and with O(logn) for essential operations needed
    for a set class it's good
    */
    bst<DataType>tree ;

    //mutable DataType dummy_arr[1] ;
    /*
    these functions till now built all functions necessary of the set clss
    based on euler tour i learned in college
    the idea is simple while inorder traversing elements of phantom if that matches a property we need
    int the other tree or (set) then we push it into the match_vector
    for example the intersection of 2 sets is elements that exist in both sets
    so for each element in first tree1 if it exists in tree2 put it into match_Vector
    same idea is used in union of 2 sets ,probability of events ,conditional probability ,every function till now
    all applicable using one line just foucs on the functionality or what to do when visiting node of tree1
    if it has a property related to tree2
    while traversing tree1 if func_ptr(data ,three2) do something either fill a vector ,increase a counter...etc
    */
    void euler_tour(node<DataType>*ptr,const bst<DataType>&other_tree,
    DataType*match_vec,long long &match_counter,bool (*func_ptr)(const DataType&,const bst<DataType>&))const;

    void euler_tour(node<DataType>*ptr,const bst<DataType>&other_tree,
    long long& match_counter,bool (*func_ptr)(const DataType&,const bst<DataType>&))const;

public:
    /*
    constructors
    */
    //empty constructor
    set(void);
    /*
    if the array inserted into the set is both sorted and contains duplicates this may cause error
    since the array is filled in O(N) apporach to maintain property of a bst 
    //check out fill_sorted in bst.tpp for more context  
    so make sure to use remove_duplicates(void) if calculations doesn't match what you expected 
    */
    //fill a set with initialized array
    set(DataType*_arr,int _size);
    //fill a set with initialized vector
    set(vector<DataType>&_arr);
    //copy constructor
    set(const set&src);
    //assignment operator
    void operator=(const set&) ;
    //destructor
    ~set(void) ;
    //insert an element into the set
    //if it exists already then it returns false
    bool insert(DataType data) ;
    //insert an array into the set
    //if size or n <0 it doesn't insert elements
    bool insert(DataType *data,int n) ;
    //search for a data in the set
    bool search(const DataType&data)const  ;
    //remove data from a set
    bool remove(const DataType&data) ;
    //get number of elements of a set
    long long get_size(void) const;
    /*
    they assume that sample-space is phantom always
    so ->s1.comp(s2) ->in sample space s1 get the complement of s2

    all these functions use the concept of euler_tour in them
    */
    /*
        returns a set of elements that exist in both set1 and set2
    */
    set intersect(const set&)const;
    /*
    returns a set of unique elements that represent unino of 2 sets
    */
    set unite(const set&)const;
    //in sample-space phantom get complement of set
    set comp(const set &)const;
    //returns set of elements that exist in phantom and not set
    set diff(const set&)const ;
    //replaces an element with new element
    bool replace(const DataType&old_data,const DataType&new_data) ;
    //retunrs true if sample-space or phantom contains the set src
    bool contain(const set&)const;

    //retunrs true if sample-space or phantom doesn't contain the set src
    bool not_contain(const set&)const;
    //returns true if set1 equals set2
    bool equal(const set &) const;
    //removes every element in the set
    //turns it into phi
    bool clear(void) ;
    //returns true if a set is phi or empty
    bool empty(void) const;
    //in sample-space (phantom) calculate proability of event set
    double prob(const set&)const;
    //in sample-space (phantom) calculate proability of event src knowing that condition even occurred
    double prob_cond(const set&src,const set&condition)const;

    /*
    in sample space (phantom) returns true if s1 and s2 are independent events
    p(s1)*p(s2)==p(s1 intersects s2)
    */
    bool independence(const set&s1,const set&s2)const;
    /*
    in sample space (phantom) returns true if group of sets are independent
    */
    bool independence(const set*,int n)const;
    /*
    in sample-space phantom calculate the probability of union of events
    */
    double prob_unite(const set*,int n)const;
    /*
    in sample-space phantom calculate the probability of intersection of events
    */
    double prob_intersect(const set*,int n)const;
/*
    removes duplicates if exist in a set
    if an array inserted into a bst is sorted and there are duplicates this may cause error in the calculations
    so make sure to use remove_duplicates(void) to make sure
*/
    //removes duplicate of data if exists
    bool remove_duplicates(const DataType&data);
    //removes all duplicates
    bool remove_duplicates(void);
    //turns a set into a string
    string to_string(void)const  ;



};

//you can cin and cout a set easily
/*
in cin first you enter the size or number of elements you are  going to insert
then insert them
*/
    template<typename DataType>
    ostream& operator<<(ostream& os, const set<DataType>& obj);
    /*
    prints a set in a visible way->{1,5,53,56,155} or {} for empty sets
    */
    template<typename DataType>
    istream& operator>>(istream& is, set<DataType>& obj);


#endif
