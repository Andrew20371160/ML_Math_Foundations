#include "multiset.h"
const long double M_PI =3.14159265358979323846;
const long double thresh_hold= 10e-10;

template<typename DataType>
multiset<DataType>::multiset(void){
	mode = MULTISET; 
}
template<typename DataType>
multiset<DataType>::multiset(const int& wanted_mode){
	if(wanted_mode==SET||wanted_mode==MULTISET){
		mode= wanted_mode;
	}
	else{
		mode =  MULTISET;
	}
}		
template<typename DataType>
multiset<DataType>::multiset(const DataType*arr,const uint32_t &size){
	mode=  MULTISET ;
	if(arr){
		for(uint32_t i= 0 ;i<size;i++){
			tree.insert(arr[i]);
		}
		tree.balance();
	}
}

template<typename DataType>
bool multiset<DataType>::set_mode(const int &wanted_mode){
	if(wanted_mode==SET||wanted_mode==MULTISET){
		if(wanted_mode!=mode){
			if(mode ==MULTISET){
				remove_duplicates();
			}
			mode = wanted_mode;	
			return 1;
		}
	}
	return 0 ; 
}
template<typename DataType>
multiset<DataType>::~multiset(void) {
	tree.remove_tree() ; 
}
template<typename DataType>
bool multiset<DataType>::insert(const DataType &d,const uint32_t c ) {
	if(c!=0){
		if(mode==MULTISET){
			return tree.insert(d,c);
		}
		else{
			//if not found then insert
			if(!tree.search(d)){
				return tree.insert(d,1) ; 
			}	
		}
	}
	return 0;
}

template<typename DataType>
bool multiset<DataType>::remove(const DataType& d) {
	return tree.remove(d);
}

template<typename DataType>
bool multiset<DataType>::search(const DataType &d)const {
	return tree.search(d);
}

template<typename DataType>
uint32_t multiset<DataType>::count(const DataType& d)const {
	node<DataType>*ptr = tree.get_node(d) ;
	return (ptr)?ptr->count:0 ;
}	
template<typename DataType>
bool multiset<DataType>::adjust_count(const DataType& d ,const uint32_t &c){
	if(c!=0){
		if(mode==MULTISET){
			return tree.adjust_count(d,c);
		}
		return 0 ;
	}
	else{
		return tree.remove(d) ;
	}
}
template<typename DataType>
bool multiset<DataType>::intersection_tour(const node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
					   const node<DataType>**match_vector,uint32_t& match_counter)const{
	if(src_tree_ptr&&other_tree){
		if(src_tree_ptr->left){
			intersection_tour(src_tree_ptr->left,other_tree,match_vector,match_counter) ;
		}
		node<DataType>* temp_ptr = other_tree->get_node(src_tree_ptr->data) ;
		if(temp_ptr){
			match_vector[match_counter] =(temp_ptr->counter<src_tree_ptr->counter)?temp_ptr:src_tree_ptr; 
			match_counter ++;
		}
		if(src_tree_ptr->right){
			intersection_tour(src_tree_ptr->right,other_tree,match_vector,match_counter) ;
		}
		return 1;
	}
	return 0;

}
template<typename DataType>
bool multiset<DataType>::union_tour(const node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
					   	 const node<DataType>**match_vector,uint32_t& match_counter)const{
	if(src_tree_ptr&&other_tree){
		if(src_tree_ptr->left){
			union_tour(src_tree_ptr->left,other_tree,match_vector,match_counter) ;
		}
		node<DataType>*temp_ptr = other_tree->get_node(src_tree_ptr->data) ;
		if(temp_ptr){
			match_vector[match_counter] =(temp_ptr->counter>src_tree_ptr->counter)?temp_ptr:src_tree_ptr; 
			match_counter ++;
		}
		else{
			//unique to phantom
			match_vector[match_counter] =src_tree_ptr;
			match_counter ++;
		}
		if(src_tree_ptr->right){
			union_tour(src_tree_ptr->right,other_tree,match_vector,match_counter) ;
		}
		return 0;
	}
	return 1 ; 
}
template<typename DataType>
bool multiset<DataType>::unique_elements_tour(const node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
					   	 const node<DataType>**match_vector,uint32_t& match_counter)const {
	if(src_tree_ptr&&other_tree){
		if(src_tree_ptr->left){
			unique_elements_tour(src_tree_ptr->left,other_tree,match_vector,match_counter) ;
		}
		node<DataType>*temp_ptr = other_tree->get_node(src_tree_ptr->data) ;
		if(!temp_ptr){
			match_vector[match_counter] =src_tree_ptr;
			match_counter ++;
		}
		if(src_tree_ptr->right){
			unique_elements_tour(src_tree_ptr->right,other_tree,match_vector,match_counter) ;
		}
		return 0;
	}
	return 1 ; 
}
	template<typename DataType>
	bool multiset<DataType>::difference_tour(const node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
						   	 const node<DataType>**match_vector,uint32_t& match_counter)const {
		if(src_tree_ptr&&other_tree){
			if(src_tree_ptr->left){
				difference_tour(src_tree_ptr->left,other_tree,match_vector,match_counter) ;
			}
			node<DataType>*temp_ptr = other_tree->get_node(src_tree_ptr->data) ;
			if(temp_ptr){
				if(src_tree_ptr->counter>temp_ptr->counter){
					match_vector[match_counter] = new node<DataType>(src_tree_ptr->data,(src_tree_ptr->counter-temp_ptr->counter));
					match_counter ++;
				}
			}
			if(src_tree_ptr->right){
				difference_tour(src_tree_ptr->right,other_tree,match_vector,match_counter) ;
			}
			return 0;
		}
		return 1 ; 
	}		
			
	template<typename DataType>
	multiset<DataType>::multiset(const multiset<DataType>&mset) {
		tree= mset.tree;
	}
	template<typename DataType>
	multiset<DataType>&multiset<DataType>::operator =(const multiset<DataType>&mset) {
		tree=mset.tree ;
		return *this; 
	}
	template<typename DataType>
	multiset <DataType> multiset<DataType>:: intersect(const multiset&src_mset) const{
		if(tree.nodes_count&&src_mset.tree.nodes_count){
			const node<DataType>**match_vector = new const node<DataType>*[(src_mset.tree.nodes_count <tree.nodes_count)?src_mset.tree.nodes_count:tree.nodes_count] ;
			uint32_t match_counter = 0;
			intersection_tour(tree.root,&src_mset.tree,match_vector,match_counter);
			multiset<DataType>ret_set ;
			if(match_counter){			
				ret_set = form_tree(match_vector,match_counter);
			}
			delete[]match_vector ;
			match_vector= NULL; 
			return ret_set ; 
		}
		return multiset();
	}

	template<typename DataType>
	multiset<DataType> multiset<DataType>::form_tree(const node<DataType>**vec,uint32_t size)const{
		if(vec&&size){
			multiset<DataType>ret_set ;
			uint32_t start = 0;
			ret_set.tree.root= ret_set.tree.balance_tour(vec,start,size-1) ;
			ret_set.tree.nodes_count= size ; 
			for(uint32_t i = 0;i<size;i++){
				ret_set.tree.total_count +=vec[i]->counter;
			}
			return ret_set;
		}
		return multiset() ; 
	}


	template<typename DataType>
	multiset <DataType> multiset<DataType>:: unite(const multiset&src_mset) const{
		if(tree.nodes_count&&src_mset.tree.nodes_count){
			//what's in a only and in both a and b 
			const node<DataType>**intersection_vec = new const node<DataType>*[tree.nodes_count] ;
			uint32_t intersection_counter = 0;
			union_tour(tree.root,&src_mset.tree,intersection_vec,intersection_counter);
			//what's unique to source
			const node<DataType>**unique_in_src = new const node<DataType>*[src_mset.tree.nodes_count] ;
			uint32_t unique_in_src_counter = 0;
			src_mset.unique_elements_tour(src_mset.tree.root,&tree,unique_in_src,unique_in_src_counter);
			//merge 2 vectors into one union vector
			uint32_t union_size=intersection_counter+unique_in_src_counter;
			const node<DataType>**union_vec=  new const node<DataType>*[union_size] ;
			merge(union_vec,intersection_vec,unique_in_src,intersection_counter,unique_in_src_counter) ;
			//delete allocated memory
			delete[]intersection_vec ; intersection_vec = NULL;
			delete[]unique_in_src ;unique_in_src= NULL; 
			//finaly create the union set
			multiset<DataType>ret_set = form_tree(union_vec,union_size);
			delete[]union_vec ; 
			union_vec=NULL ;
			return ret_set;
		}
		if(tree.nodes_count){
			return *this;
		}
		else{
			return src_mset ;
		}
	}

	template<typename DataType>
	multiset <DataType> multiset<DataType>:: operator-(const multiset&src_mset) const{
		if(tree.nodes_count&&src_mset.tree.nodes_count){
			//fill diff vec with elements in a that has bigger count than b where the count in subtracted
			const node<DataType>**diff_vec = new const node<DataType>*[(src_mset.tree.nodes_count<tree.nodes_count)?src_mset.tree.nodes_count:tree.nodes_count] ;
			uint32_t diff_counter = 0;			
			difference_tour(tree.root,&src_mset.tree,diff_vec,diff_counter);
			//elements in a not in b
			const node<DataType>**unique_in_phantom =NULL;
			uint32_t unique_in_phantom_counter=0;
			if(diff_counter<tree.nodes_count){
				//in case they exist
				unique_in_phantom= new const node<DataType>*[tree.nodes_count-diff_counter] ;
				unique_elements_tour(tree.root,&src_mset.tree,unique_in_phantom,unique_in_phantom_counter);
			}	
			//merge into final vector
			const node<DataType>**ret_vec =NULL;
			if(diff_counter+unique_in_phantom_counter){ 
				ret_vec= new const node<DataType>*[diff_counter+unique_in_phantom_counter] ;
			}

			merge(ret_vec,diff_vec,unique_in_phantom,diff_counter,unique_in_phantom_counter);
			multiset<DataType>ret_set= form_tree(ret_vec,diff_counter+unique_in_phantom_counter);
			for(uint32_t i =0 ; i <diff_counter;i++){
				//delete each allocated element (with new count)
				delete diff_vec[i] ;
				diff_vec[i] = NULL; 
			}
			//delete diff vec then unique in phantom if exists
			delete[]diff_vec ; diff_vec=NULL;
			if(unique_in_phantom){
				delete[]unique_in_phantom; 
				unique_in_phantom= NULL;
			}			
			//form final set and delete ret_vec  
			if(ret_vec){
				delete[]ret_vec; 
				ret_vec=NULL; 
			}
			return ret_set;

		}
		if(tree.nodes_count){
			return *this;
		}
		else{
			return multiset() ;
		}
	}	

	template<typename DataType>
	bool multiset<DataType>::merge(const node<DataType>**dest,const node<DataType>**vec1,const node<DataType>**vec2 ,
			   const uint32_t &v1_c,const uint32_t &v2_c)const{
		uint32_t i1= 0,i2=0 ;
		uint32_t dest_c = 0;
		while(i1<v1_c&&i2<v2_c){
			if(vec1[i1]->data<vec2[i2]->data){
				dest[dest_c] = vec1[i1];
				i1++;
			}
			else{
				dest[dest_c] = vec2[i2] ;
				i2++; 
			}
			dest_c++; 
		}
		while(i1<v1_c){
			dest[dest_c] = vec1[i1];
			i1++ ; 
			dest_c++ ;
		}
		while(i2<v2_c){
			dest[dest_c] = vec2[i2];
			i2++ ; 
			dest_c++ ;
		}
	}
	template<typename DataType>
	void multiset<DataType>::display(void)const{
		std::cout<<"\n{";
		tree.display();
		std::cout<<"}";
	}	
	template<typename DataType>
	bool multiset<DataType>::operator>=(const multiset&mset) const{
		if(tree.nodes_count&&mset.tree.nodes_count){
			uint32_t is_contain =1  ;
			contain_tour(mset.tree.root,&tree,is_contain) ; 
			return (is_contain==1);
		}
		else if(tree.nodes_count){
			return 1; 
		}
		else if(mset.tree.nodes_count){
			return 0;
		}
		return 1;
	}
	template<typename DataType>
	bool multiset<DataType>::operator<=(const multiset&mset) const{		
		return mset>=(*this);
	}
	template<typename DataType>
	bool multiset<DataType>::operator==(const multiset&mset) const{
		if(tree.nodes_count&&mset.tree.nodes_count){
			uint32_t is_equal =1;
			equality_tour(tree.root,&mset.tree,is_equal) ; 
			return is_equal==1;
		}
		if(tree.nodes_count==mset.tree.nodes_count&&tree.nodes_count==0){
			return 1; 
		}
		return 0;
	}

	template<typename DataType>
	bool multiset<DataType>::contain_tour(const node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
						   	 			  uint32_t& match_counter)const {
		if(src_tree_ptr&&other_tree){
			if(src_tree_ptr->left){
				contain_tour(src_tree_ptr->left,other_tree,match_counter) ;
			}
			if(match_counter){
				node<DataType>*temp_ptr = other_tree->get_node(src_tree_ptr->data) ;
				if(temp_ptr){
					if(src_tree_ptr->counter<=temp_ptr->counter){
						match_counter = 1; 
					}
					else{
						match_counter= 0;
					}
				}
				else{
					match_counter= 0;						
				}
			}				
			if(src_tree_ptr->right){
				contain_tour(src_tree_ptr->right,other_tree,match_counter) ;
			}
			return 0;
		}
		return 1 ; 
	}		
	template<typename DataType>
	bool multiset<DataType>::equality_tour(const node<DataType>*src_tree_ptr,const bst<DataType>*other_tree,
						   	 			  uint32_t& match_counter)const {
		if(src_tree_ptr&&other_tree){
			if(src_tree_ptr->left){
				equality_tour(src_tree_ptr->left,other_tree,match_counter) ;
			}
			if(match_counter){
				node<DataType>*temp_ptr = other_tree->get_node(src_tree_ptr->data) ;
				if(temp_ptr){
					if(src_tree_ptr->counter==temp_ptr->counter){
						match_counter = 1; 
					}
					else{
						match_counter= 0;
					}
				}
				else{
					match_counter= 0;						
				}
			}
			if(src_tree_ptr->right){
				equality_tour(src_tree_ptr->right,other_tree,match_counter) ;
			}
			return 0;
		}
		return 1 ; 
	}			
	template<typename DataType>
	bool multiset<DataType>::avg_tour(const node<DataType>*src_tree_ptr,DataType& match_counter)const {
		if(src_tree_ptr){
			if(src_tree_ptr->left){
				avg_tour(src_tree_ptr->left,match_counter) ;
			}
			match_counter +=src_tree_ptr->data*src_tree_ptr->counter;
			if(src_tree_ptr->right){
				avg_tour(src_tree_ptr->right,match_counter) ;
			}
			return 0;
		}
		return 1 ; 
	}	
	template<typename DataType>
	bool multiset<DataType>::powered_avg_tour(const node<DataType>*src_tree_ptr,DataType& match_counter,double power)const {
		if(src_tree_ptr){
			if(src_tree_ptr->left){
				powered_avg_tour(src_tree_ptr->left,match_counter,power) ;
			}
			match_counter +=pow(src_tree_ptr->data,power)*src_tree_ptr->counter;
			if(src_tree_ptr->right){
				powered_avg_tour(src_tree_ptr->right,match_counter,power) ;
			}
			return 0;
		}
		return 1 ; 
	}		
	//E[x^2]-E[x]
	template<typename DataType>
	bool  multiset<DataType>::variance_tour(const node<DataType>*src_tree_ptr,DataType& avg1,DataType&avg2)const{
		if(src_tree_ptr){
			if(src_tree_ptr->left){
				variance_tour(src_tree_ptr->left,avg1,avg2) ;
			}
			avg1+=pow(src_tree_ptr->data,2)*src_tree_ptr->counter;
			avg2+=src_tree_ptr->data*src_tree_ptr->counter;
			if(src_tree_ptr->right){
				variance_tour(src_tree_ptr->right,avg1,avg2) ;
			}
			return 0;
		}
		return 1 ; 
	}

	template<typename DataType>
	bool multiset<DataType>::moden_tour(const node<DataType>*src_tree_ptr,const node<DataType>*&moden_ptr)const {
		if(src_tree_ptr){
			if(src_tree_ptr->left){
				moden_tour(src_tree_ptr->left,moden_ptr) ;
			}
			if(moden_ptr==NULL){
				moden_ptr=src_tree_ptr;  
			}
			else{
				if(src_tree_ptr->counter>moden_ptr->counter){
					moden_ptr = src_tree_ptr ; 
				}
			}
			if(src_tree_ptr->right){
				moden_tour(src_tree_ptr->right,moden_ptr) ;
			}
			return 0;
		}
		return 1 ; 
	}	
	template<typename DataType>
	bool multiset<DataType>::pmf_tour(const node<DataType>*src_tree_ptr,std::vector<double>&pmf_vec,uint32_t &counter)const {
		if(src_tree_ptr){
			if(src_tree_ptr->left){
				pmf_tour(src_tree_ptr->left,pmf_vec,counter) ;
			}
			pmf_vec[counter] = src_tree_ptr->counter ;
			counter++ ;

			if(src_tree_ptr->right){
				pmf_tour(src_tree_ptr->right,pmf_vec,counter) ;
			}
			return 0;
		}
		return 1 ; 
	}		


	template<typename DataType>
	bool multiset<DataType>::cmf_tour(const node<DataType>*src_tree_ptr,std::vector<double>&cmf_vec,uint32_t &counter)const {
		if(src_tree_ptr){
			if(src_tree_ptr->left){
				cmf_tour(src_tree_ptr->left,cmf_vec,counter) ;
			}

			cmf_vec[counter] = src_tree_ptr->counter ;
			if(counter!=0){
				cmf_vec[counter] +=cmf_vec[counter-1] ;
			}
			counter++ ;
			if(src_tree_ptr->right){
				cmf_tour(src_tree_ptr->right,cmf_vec,counter) ;
			}
			return 0;
		}
		return 1 ; 
	}
	template<typename DataType>
	DataType multiset<DataType>::average(void)const {
		DataType ret_val =0;
		if(tree.nodes_count){
			avg_tour(tree.root,ret_val) ;
			return ret_val / DataType(tree.total_count) ; 
		}
		return DataType(0);
	}	
	template<typename DataType>
	DataType multiset<DataType>::average(const double& power)const{
		if(tree.nodes_count){
			DataType ret_val =0;
			powered_avg_tour(tree.root,ret_val,power) ;
			return ret_val /DataType(tree.total_count) ;
		}
		return DataType(0);
	}
	template<typename DataType>
	DataType multiset<DataType>::variance(void)const {
		if(tree.nodes_count){
			DataType x_squared = 0,x = 0 ; 
			variance_tour(tree.root,x_squared,x);
			x =x/DataType(tree.total_count) ;
			x = DataType(pow(x,2)) ;
			x_squared= x_squared/DataType(tree.total_count) ; 
			return x_squared-x; 
		}
		return 0;
	}
	template<typename DataType>
	DataType multiset<DataType>::moden(void)const {
		if(tree.nodes_count){
			const node<DataType>*ret_moden =NULL; 
			moden_tour(tree.root,ret_moden) ; 
			return ret_moden->data ;
		}
		std::cout<<"\nemtpy tree";
		DataType garbage_val ; 
		return garbage_val ;
	}		

	template<typename DataType>
	std::vector<double> multiset<DataType>::pmf(void)const {
		if(tree.nodes_count){
			std::vector<double>ret_vec(tree.nodes_count) ; 
			uint32_t start = 0;
			pmf_tour(tree.root,ret_vec,start);
			for(uint32_t i = 0; i <ret_vec.size() ;i++){
				ret_vec[i] =ret_vec[i]/static_cast<double>(tree.total_count) ;
			}
			return ret_vec ;
		}
		std::cout<<"\nEmpty tree" ; 
		return std::vector<double>(0) ;
	}	
	template<typename DataType>
	std::vector<double> multiset<DataType>::cmf(void)const {
		if(tree.nodes_count){
			std::vector<double>ret_vec(tree.nodes_count) ; 
			uint32_t start = 0;
			cmf_tour(tree.root,ret_vec,start);
			for(uint32_t i = 0; i <ret_vec.size() ;i++){
				ret_vec[i] =ret_vec[i]/static_cast<double>(tree.total_count) ;
			}
			return ret_vec ;
		}
		std::cout<<"\nEmpty tree" ; 
		return std::vector<double>(0) ;
	}	
	template<typename DataType>
	bool multiset<DataType>::addition_tour(node<DataType>*src_tree_ptr,const bst<DataType>*intersect_tree)const{
		if(src_tree_ptr&&intersect_tree){
			if(src_tree_ptr->left){
				addition_tour(src_tree_ptr->left,intersect_tree) ;
			}
			node<DataType>*temp_node = intersect_tree->get_node(src_tree_ptr->data); 			
			if(temp_node){
				src_tree_ptr->counter +=temp_node->counter; 
			}
			if(src_tree_ptr->right){
				addition_tour(src_tree_ptr->right,intersect_tree) ;
			}
			return 0;
		}
		return 1 ; 
	}

	template<typename DataType>
	multiset<DataType> multiset<DataType>::operator+(const multiset&src) const{
		if(tree.nodes_count&&src.tree.nodes_count){
			multiset<DataType>ret_set = this->unite(src) ; 
			multiset<DataType>intersection  = this->intersect(src) ;	
			addition_tour(ret_set.tree.root,&intersection.tree);

			return ret_set ; 
		}
		if(tree.nodes_count){
			return *this;
		}
		else if(src.tree.nodes_count){
			return src ; 
		}
		return multiset() ; 
	}
	template<typename DataType>
	DataType multiset<DataType>::standard_deviation(void)const{
		return DataType(sqrt(variance()));
	}
	template<typename DataType>
	bool multiset<DataType>::clear(void){
		tree.remove_tree() ; 
		return 1 ; 
	}		
	template<typename DataType>
	DataType multiset<DataType>::range(void) const {
		return tree.max()->data-tree.min()->data;
	} 
	template<typename DataType>
    bool multiset<DataType>::replace(const DataType&old_data,const DataType&new_data) {
    	
    	node<DataType>*ptr = tree.get_node(old_data)  ;
    	if(ptr){
	    	uint32_t c = ptr->counter  ;
	    	tree.remove_node(old_data) ; 
	    	ptr =NULL;
	   		tree.insert(new_data,c) ; 
		return 1 ;
		}	
		return 0  ;
    }
	template<typename DataType>
	bool multiset<DataType>::apply_function_tour(node<DataType>*src_tree_ptr,bool(*f_ptr)(DataType&,uint32_t&,
														  	  		  DataType*,uint32_t*,const uint32_t),
																	  DataType*v1,uint32_t*v2,uint32_t size){
		if(src_tree_ptr){
			if(src_tree_ptr->left){
				apply_function_tour(src_tree_ptr->left,f_ptr,v1,v2,size) ;
			}
			f_ptr(src_tree_ptr->data,src_tree_ptr->counter,v1,v2,size) ; 
			if(src_tree_ptr->right){
				apply_function_tour(src_tree_ptr->right,f_ptr,v1,v2,size) ;
			}
		}
	}
	template<typename DataType>
	bool  multiset<DataType>::remove_duplicates(void){
		if(tree.nodes_count){
			apply_function_tour(tree.root,remove_duplicates_function,NULL,NULL,0) ;
			tree.total_count = tree.nodes_count ;
			return 1 ;
		}
		return 0; 
	}  
	template<typename DataType>
	multiset<DataType> multiset<DataType>::normalize(void) const{
		if(tree.nodes_count){
			multiset<DataType>ret_set = *this;
			DataType *avg_and_stddev= new DataType[2] ;
			avg_and_stddev[0] = average() ;
			avg_and_stddev[1] = standard_deviation();
			ret_set.apply_function_tour(ret_set.tree.root,standardize_element,avg_and_stddev,NULL,2) ;
			delete[]avg_and_stddev ;avg_and_stddev =NULL;
			return ret_set; 
		}
		return multiset(); 
	}
	template<typename DataType>
	bool multiset<DataType>::draw_no_replace_tour(const node<DataType>*src_tree_ptr,const bst<DataType>*sample_space,
				     	long double&p,uint32_t &remaining_elements)const{
		if(src_tree_ptr&&sample_space){
			if(src_tree_ptr->left){
				draw_no_replace_tour(src_tree_ptr->left,sample_space,p,remaining_elements);
			}
			node<DataType>*temp_node = sample_space->get_node(src_tree_ptr->data); 			
			if(temp_node){
				if(temp_node->counter>=src_tree_ptr->counter){
					for(uint32_t i =temp_node->counter;i>(temp_node->counter-src_tree_ptr->counter);i--){
						p*=static_cast<long double>(i)/static_cast<long double>(remaining_elements);
						remaining_elements--;
					}
				}
				else{
					p=0 ;
				}
			}
			else{
				p=0;
			}
			if(src_tree_ptr->right){
				draw_no_replace_tour(src_tree_ptr->right,sample_space,p,remaining_elements);
			}
			return 1 ; 
		}
		return 0; 
	}	

	template<typename DataType>
	bool multiset<DataType>::draw_replace_tour(const node<DataType>*src_tree_ptr,const bst<DataType>*sample_space,
				     	long double&p)const{
		if(src_tree_ptr&&sample_space){
			if(src_tree_ptr->left){
				draw_replace_tour(src_tree_ptr->left,sample_space,p);
			}
			node<DataType>*temp_node = sample_space->get_node(src_tree_ptr->data); 			
			if(temp_node){
				if(temp_node->counter>=src_tree_ptr->counter){
					p*=static_cast<long double>(pow(temp_node->counter,src_tree_ptr->counter))/
					static_cast<long double>(pow(sample_space->total_count,src_tree_ptr->counter));
				}
				else{
					p=0 ;
				}
			}
			else{
				p=0;
			}
			if(src_tree_ptr->right){
				draw_replace_tour(src_tree_ptr->right,sample_space,p);
			}
			return 1 ; 
		}
		return 0; 
	}	

	template<typename DataType>
    long double multiset<DataType>::prob_draw_no_replace(const multiset&event)const {
    	if(event.tree.root&&tree.root){
	    	long double ret_val =1; 
	    	uint32_t remaining_elements =tree.total_count; 
	    	draw_no_replace_tour(event.tree.root,&tree,ret_val, remaining_elements);
	    	return ret_val*tgamma(event.tree.nodes_count+1);
	    }
	    else if(event.tree.root){
	    	//can't draw event from empty set
	    	return 0 ; 
	    }
	    else if(tree.root){
	    	return 1 ; 
	    }
	    //can draw empty set from empty set
	    return 1; 
    }  

	template<typename DataType>
    long double multiset<DataType>::prob_draw_replace(const multiset&event)const {
    	if(event.tree.root&&tree.root){
	    	long double ret_val =1; 
	    	draw_replace_tour(event.tree.root,&tree,ret_val);
	    	return ret_val*tgamma(event.tree.nodes_count+1);
	    }
	    else if(event.tree.root){
	    	//can't draw event from empty set
	    	return 0 ; 
	    }
	    else if(tree.root){
	    	return 1 ; 
	    }
	    //can draw empty set from empty set
	    return 1; 
    }  
	template<typename DataType>

	uint32_t multiset<DataType>::nodes_count(void)const{
		return tree.nodes_count; 
	}	
	template<typename DataType>
	uint32_t multiset<DataType>::total_count(void)const{
		return tree.total_count;
	}

	template<typename DataType>
    long double  multiset<DataType>::prob_cond_draw_no_replace(const multiset&src,const multiset&condition)const{
    	if(*this>=condition){    		
    		return condition.prob_draw_no_replace(src);
    	} 
    	std::cout<<"\ncondition can't be drawn from sample space";
    	return -1;
    }

	template<typename DataType>
    long double  multiset<DataType>::prob_cond_draw_replace(const multiset&src,const multiset&condition)const{
    	if(*this>=condition){
    		return condition.prob_draw_replace(src);
    	} 
    	std::cout<<"\ncondition can't be drawn from sample space";
    	return -1;
    }

	uint32_t choose(const uint32_t& n,const uint32_t &k){
		return tgamma(n+1)/(tgamma(n-k+1)*tgamma(k+1));
	}    

	template<typename DataType>
    long double multiset<DataType>::prob(const multiset&src)const{
        if(*this>=src){
            return static_cast<double>(src.tree.total_count)/static_cast<double>(tree.total_count);
        }
        return 0 ;
    }

	template<typename DataType>
    long double  multiset<DataType>::prob_cond(const multiset&src,const multiset&condition){
    	if(*this>=condition){
    		return condition.prob(src);
    	}
    	std::cout<<"\ncondition can't be drawn from sample space";
    	return -1;
    }

	template<typename DataType>
	bool multiset<DataType>::independence(const multiset&e1,const multiset&e2)const {
		long double p_e1 = prob(e1);
		long double p_e2 = prob(e2);
		long double p_e1_inter_e2 = prob(e1.intersect(e2));
		return fabs(p_e1*p_e2-p_e1_inter_e2)<=thresh_hold;
	}

	template<typename DataType>
	long double multiset<DataType>::bayes(const multiset&src,const multiset&condition)const{
		long double p_cond = prob(condition);
		if(p_cond>0){
			return prob_cond(condition,src)*prob(src)/p_cond;
		}    	
		std::cout<<"\ncondition can't be drawn from sample space";
		return -1; 
	}    

