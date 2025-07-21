#ifndef _multiset_h_
#define _multiset_h_
#include "bst.h"
#include <vector>
#include <cmath>

enum mode {
	SET=0,MULTISET=1
};

template<typename DataType>
bool remove_duplicates_function (DataType&d1,uint32_t&c1,DataType*v1,uint32_t*v2,const uint32_t){
	c1 = 1 ;
	return 1 ;
}
template<typename DataType>
bool standardize_element (DataType&d1,uint32_t&c1,DataType*v1,uint32_t*v2,const uint32_t){
	d1=d1-v1[0];
	d1 =d1/v1[1] ;
	return 1 ;
}

uint32_t choose(const uint32_t &num);

template<typename DataType>
class multiset{

	private:
		//bst holding elements where frequency of each element is stored in each node
		bst<DataType>tree;
		//either 2 modes : set or multiset
		//if set then it doesn't allow for duplicates as normal sets
		bool is_multiset ;
		bool intersection_tour(node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
							   node<DataType>**match_vector,uint32_t& match_counter)const;
		bool union_tour(node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
							   node<DataType>**match_vector,uint32_t& match_counter)const;
		bool unique_elements_tour(node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
					   node<DataType>**match_vector,uint32_t& match_counter)const;

		bool difference_tour(node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
			   node<DataType>**match_vector,uint32_t& match_counter)const;

		bool merge(node<DataType>**dest, node<DataType>**, node<DataType>** ,
				   const uint32_t &v1_c,const uint32_t &v2_c)const;
		multiset<DataType> form_tree(node<DataType>**vec,uint32_t size)const;
		bool contain_tour(node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
					      uint32_t& match_counter)const;
		bool equality_tour(node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
					      uint32_t& match_counter)const;

		bool draw_no_replace_tour(node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
				     	long double&p,uint32_t &remaining_elements)const;

		bool draw_replace_tour(node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
				     	long double&p)const;

		bool avg_tour(node<DataType>*src_tree_ptr,DataType& avg)const;

		bool powered_avg_tour(node<DataType>*src_tree_ptr,DataType& avg,double power)const;

		bool variance_tour(node<DataType>*src_tree_ptr,DataType& avg1,DataType&avg2)const;

		bool mode_tour (node<DataType>*src_tree_ptr,const node<DataType>*& mode_ptr)const ;

		bool pmf_tour(node<DataType>*src_tree_ptr,std::vector<double>&pmf_vec,uint32_t &counter)const;
		bool cmf_tour(node<DataType>*src_tree_ptr,std::vector<double>&pmf_vec,uint32_t &counter)const;
		bool addition_tour(node<DataType>*src_tree_ptr,const bst<DataType>*intersect_tree)const;
		void apply_function_tour(node<DataType>*src_tree_ptr,bool(*f_ptr)(DataType&,uint32_t&,
														  	  		  DataType*,uint32_t*,const uint32_t),
																	  DataType*v1,uint32_t*v2,uint32_t size);


	public:
		//empty multiset by default mode is multiset
		multiset(void);
		//copy constructor same mode of input set is used
		multiset(const multiset&) ;
		//assignment operator
		multiset&operator =(const multiset&) ;

		//by defaults mode is multiset initializes the multiset with array elements
		//then balances the tree for efficient computations
		multiset(const DataType*,const uint32_t &size);
		//initializes an empty set with wanted mode either set or multiset
		multiset(const int&);
		//destructor
		~multiset(void) ;
		//sets mode of the current set either set or multiset
		//if it was multiset then duplicates are removed
		bool set_mode(const int&);
		//insert an element with a count of it
		//if mode is set then this count is ignored and only inserts one element
		bool insert(const DataType& ,const uint32_t c=1) ;
		//removes all instances of an element in the set
		bool remove(const DataType& ) ;
		//removes every element in the set
		bool clear(void);
		//returns 1 if element is found in the set
		bool search(const DataType&)const ;
		//adjusts count of an element in a set
		//if 0 then element is removed
		//else behaviour is based on the mode
		bool adjust_count(const DataType&,const uint32_t&);
		//returns count of an element in the set/multiset
		uint32_t count(const DataType&)const ;
		//returns intersection of 2 multisets/sets
		multiset intersect(const multiset&) const;
		////returns union of 2 multisets/sets
		multiset unite(const multiset&) const;
		//retuns difference of 2 multisets/sets
		multiset difference(const multiset&) const;

		multiset operator/(const multiset&) const;
		multiset operator+(const multiset&) const;
		multiset operator*(const multiset&) const;
		multiset operator-(const multiset&) const;

		//returns 1 if phantom contains or is equal to input
		bool operator>=(const multiset&) const;
		//returns 1 if input contains or is equal to phantom
		bool operator<=(const multiset&) const;
		//returns 1 if phantom equals input
		bool operator==(const multiset&) const;
		//returns weighted average of elements within a set/multiset
		DataType average(void)const ;
		//returns E(x^power) or expected value of power of elements
		DataType average(const double&power)const;
		//returns variance of elements
		DataType variance(void)const ;
		//prints the elements of the set/multiset
		void display(void)const;
		//returns the most frequent element in the set/multiset
		DataType mode(void)const ;
		//returns probability mass function of the elements
		//from minimum element till max elements in a vector
		std::vector<double> pmf(void)const ;
		//returns Cumulative mass function of the elements
		//from minimum element till max elements in a vector
		std::vector<double> cmf(void)const ;
		//returns standard deviation of the elements
		DataType standard_deviation(void)const;
		//returns max_elements - min_element in the set
		DataType range(void) const ;
		//removes each duplicates of every element and sets each to 1 only
		bool remove_duplicates(void);
		//replaces an element in the set with new data
		bool replace(const DataType&old_data,const DataType&new_data) ;
		//z-score normalization returns it in a set
		multiset normalize(void)const  ;
		
		multiset axpy(const DataType&,const DataType&)const;

		//without order
		/*
		for in-order
		long double val = sample_space.prob_draw_replace(event)/tgamma(event.nodes_count()+1) ;
		since tgamma(n)=(n-1)!
		*/
    	long double prob_draw_no_replace(const multiset&event)const ;
    	long double prob_draw_replace(const multiset&)const;

    	//returns number of unique elements in the multiset
    	uint32_t nodes_count(void)const;
    	//returns total number of elements in the multiset
    	uint32_t total_count(void)const;
    	//conditional probability of drawing without replacement
	    long double prob_cond_draw_no_replace(const multiset&src,const multiset&condition)const;
	    //conditional probability of drawing with replacement
	    long double prob_cond_draw_replace(const multiset&src,const multiset&condition)const;

	    //the following set of functions treats whole set as an individual event
	    //if it's contained within the sample space it reutnrs

	    //total number of elements in src /total number of elements in sample space
	    long double prob(const multiset&src)const;
	    //conditional probability using prob function
	    long double prob_cond(const multiset&src,const multiset&condition);
	    //retuns true if event1 and event2 are independent in the sample space of phantom
		bool independence(const multiset&e1,const multiset&e2)const ;
		//bayse rule using prob function
	    long double bayes(const multiset&src,const multiset&condition)const;

};

#endif
