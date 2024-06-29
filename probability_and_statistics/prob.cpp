    #include "prob.h"
    /*
    prints a set in a visible way->{1,5,53,56,155} or {} for empty sets
    */
    template<typename DataType>
    ostream& operator<<(ostream& os, const set<DataType>& obj) {

        string output= "{";
        output.append(obj.to_string());
        if(output=="{"){
            //never was filled
            output = "{}"  ;
        }
        else{
            output[output.size()-1]='}' ;
        }
        os<<output ;
        return os ;
    }

    //turns a set into a string
    //for printing purpose
    template<typename DataType>
    string set<DataType>::to_string(void)const{
        string str ="" ;
        tree.to_string(str) ;
        return str  ;
    }
/*
in cin first you enter the size or number of elements you are  going to insert
then insert them
*/
    template<typename DataType>
    istream& operator>>(istream& is, set<DataType>& obj){
            int n;
            is>>n;
            DataType data ;
            for(int j= 0 ;j<obj.get_size();j++){
                is>>data ;
                obj.insert(data) ;
            }
        return is ;
    }



    //empty bst
    template<typename DataType>
    set<DataType>::set(void){
        tree = bst<DataType>() ;
    }

    /*
    if the array inserted into the set is both sorted and contains duplicates this may cause error
    since the array is filled in O(N) apporach to maintain property of a bst
    //check out fill_sorted in bst.tpp for more context
    so make sure to use remove_duplicates(void) if calculations doesn't match what you expected
    */
    template<typename DataType>
    set<DataType>::set(DataType*_arr,int _size){
        if(_arr&&_size>0){
            tree= bst<DataType>(_arr,_size);
        }
        else{
            tree=bst<DataType>() ;
        }
    }


    /*
    if the vector inserted into the set is both sorted and contains duplicates this may cause errors
    since the array is filled in O(N) apporach to maintain property of a bst
    //check out fill_sorted in bst.tpp for more context
    so make sure to use remove_duplicates(void) if calculations doesn't match what you expected
    */
    template<typename DataType>
    set<DataType>::set(vector<DataType>&_arr){
        if(_arr.size()>0){
            tree =bst<DataType>(_arr) ;
        }
        else{
            tree = bst<DataType>() ;
        }
    }
    //copy constructor
    template<typename DataType>
    set<DataType>::set(const set&src){
        tree= src.tree ;
    }
    //assignment operator
    template<typename DataType>
    void set<DataType>::operator=(const set&src) {
        tree = src.tree ;
    }
    //destructor of the set
    //though bst is deleted anyway but just in case :)
    template<typename DataType>
    set<DataType>::~set(void){
        tree.remove_tree() ;
    }
    //insert an element into the set
    template<typename DataType>
    bool set<DataType>::insert(DataType data){
        return tree.insert(data) ;
    }
    //insert an array into the set

    template<typename DataType>
    bool set<DataType>::insert(DataType *_arr,int n) {
        if(n>0){
                /*
    if tree is empty or set is empty it treats at as a constructor for the bst
    */
            if(tree.get_size()==0){
                tree =bst<DataType>(_arr,n) ;
            }
            else{
                //else insert each element
                for(int i = 0 ; i<n;i++){
                    tree.insert(_arr[i]);
                }
            }
            return 1 ;
        }
        return 0  ;
    }


    template<typename DataType>
    bool set<DataType>::search(const DataType&data)const {
        return search(data) ;
    }
    //remove an element from the set
    template<typename DataType>
    bool set<DataType>::remove(const DataType&data){
        return tree.remove(data) ;
     }
     //get number of elements in the set
    template<typename DataType>
    long long set<DataType>::get_size(void)const{
        return tree.get_size();
    }

    /*
    euler tour section
    */
    template<typename DataType>
    bool exist(const DataType&data,const bst<DataType>&other_tree){
        return other_tree.search(data) ;
    }

    template<typename DataType>
    bool not_exist(const DataType&data,const bst<DataType>&other_tree){
        return !other_tree.search(data) ;
    }

    template<typename DataType>
    bool always_exist(const DataType&data,const bst<DataType>&other_tree){
        return 1;
    }

    /*
    this function fills vector/increase counter based on a conditino between phantom and caller
    if data in phantom exists in data in caller
    if data in phantom not exist
    etc etc
    so if we calculate intersection we push every element that's in phantom and in other_tree
    into the match_vec
    using same logic this function can be used to calculate any property needed
    */
    template<typename DataType>
    void set<DataType>::euler_tour(node<DataType>*ptr,const bst<DataType>&other_tree,
        DataType*match_vec,long long &match_counter,bool (*func_ptr)(const DataType&,const bst<DataType>&))const{
        if(ptr==NULL){
            ptr= tree.root ;
        }
        if(ptr){
            if(ptr->left){
                euler_tour(ptr->left,other_tree,match_vec,match_counter,func_ptr) ;
            }
            if(func_ptr(ptr->data,other_tree)){
                 match_vec[match_counter]=ptr->data;
                 match_counter++;
            }
            if(ptr->right){
                euler_tour(ptr->right,other_tree,match_vec,match_counter,func_ptr) ;
            }
        }
    }

    template<typename DataType>
    void set<DataType>::euler_tour(node<DataType>*ptr,const bst<DataType>&other_tree,
        long long& match_counter,bool (*func_ptr)(const DataType&,const bst<DataType>&))const{
        if(ptr==NULL){
            ptr= tree.root ;
        }
        if(ptr){
            if(ptr->left){
                euler_tour(ptr->left,other_tree,match_counter,func_ptr) ;
            }
            if(func_ptr(ptr->data,other_tree)){
                match_counter++ ;
            }
            if(ptr->right){
                euler_tour(ptr->right,other_tree,match_counter,func_ptr) ;
            }
        }
    }


    /*
    all the following functions are applications of euler tour
    */
    //returns intersection between caller and src
    template<typename DataType>
    set<DataType> set<DataType>::intersect(const set&src)const{
        //if both aren't phi or empty sets
        if(tree.get_size()&&src.get_size()){
            //vector to store elements in both sets
            //it has size of smaller set
            DataType *v = new DataType[min(src.get_size(),get_size())];
            //number of elements in both sets
            long long final_size =0 ;
            //get elements that are in phantom and src and put it into v
            euler_tour(NULL,src.tree,v,final_size,exist) ;
            //create the set
            set<DataType> s(v,final_size) ;
            //remove allocated memory
            delete[]v ;
             v = NULL;

            return s ;
        }
        //else its an empty set
        return set();
    }

    template<typename DataType>
    void merge(const vector<DataType>&left_vec,const vector<DataType>&right_vec,vector<DataType>&dest){
            long long left = 0 , right  =0 ,counter=0;

                while(left<left_vec.size()&&right<right_vec.size()){
                    if(left_vec[left]<=right_vec[right]){
                        //check that it's a new element and not repeated
                            dest[counter++] = left_vec[left] ;
                            left++;
                    }
                    else if(left_vec[left]>right_vec[right]){
                        dest[counter++] = right_vec[right] ;
                        right++;
                    }

                }
            //fill rest
            while(left<left_vec.size()){
                dest[counter++] = left_vec[left];
                left ++;
            }

            while(right<right_vec.size()){
                    dest[counter++] = right_vec[right];
                right ++;
            }

    }
    //returns union of caller and src
    template<typename DataType>
    set<DataType> set<DataType>::unite(const set&src)const{
        //if both exist
        if(tree.get_size()&&src.get_size()){
            //assuming all elements of phantom are unique
            DataType*unique_in_phantom=new DataType[get_size()] ;
            //counter of uniqie elements that are in phantom and not in src
            long long  counter = 0;
            //vec is filled with data that's in phantom and not in src
            euler_tour(NULL,src.tree,unique_in_phantom,counter ,not_exist) ;
            //copy of the src set
            set <DataType>ret_set =src;
            //in that copy insert rest of elements that are unique in the phantom
            for(int i = 0 ; i<counter;i++){
                ret_set.tree.insert(unique_in_phantom[i]);
            }
            //delete allocated memory
            delete[]unique_in_phantom ;
            unique_in_phantom = NULL;

            return ret_set;
        }
        //if one of them is empty set return the other one
        //if both are empty same thing applies
        else if(tree.get_size()){
            return *this ;
        }
        else{
            return src ;
        }
    }
    //returns true if sample-space(phantom contains src set)
    template<typename DataType>
    bool set<DataType>::contain(const set&src)const{
        if(tree.get_size()){
            long long counter = 0 ;
            //search if every node in src is in the phantom(sample-space)
            //if exist increase the counter
            src.euler_tour(NULL,tree,counter,exist) ;

            return counter==src.get_size();
        }
        return false ;
    }
    /*
    in the sample-space (phantom ) calculate complement of src set
    */
    template<typename DataType>
    set<DataType> set<DataType>::comp(const set &src)const{
        //if sample space contains src then we get its complement
        //else result doesn't make sense
        if(contain(src)){
            DataType*vec = new DataType[get_size()-src.get_size()];
            long long counter  = 0 ;
            //fill with data that's in the sample space (phantom)and not in src
            euler_tour(NULL,src.tree,vec,counter,not_exist) ;
            set s(vec,counter);
            //delete the array
            delete[]vec;
            vec =NULL ;
            return s ;
        }
        return set(NULL,0)  ;
    }

    template<typename DataType>
    bool set<DataType>::not_contain(const set&src)const{
        return !contain(src);
    }
    //retunrs true if both sets contain same elements
    template<typename DataType>
    bool set<DataType>::equal(const set &src)const{
        if(src.get_size()==get_size()){
            return contain(src);
        }
        return 0 ;
    }
    //turns a set into empty set (phi)
    template<typename DataType>
    bool set<DataType>::clear(void){
        tree.remove_tree();
        return true ;
    }
    //returns true if set is empty
    template<typename DataType>
    bool set<DataType>::empty(void)const{
        return get_size()==0 ;
    }
    /*
    probability of an event (src) to occur in sample space (phantom)
    */
    template<typename DataType>
    double set<DataType>::prob(const set&src)const{
        long  long  counter =0;
        //count number of elements that are in both
        src.euler_tour(NULL,tree,counter,exist);
        return static_cast<double>(counter)/static_cast<double>(get_size()) ;
    }

    /*
    probability of an event (src) to occur in sample space (phantom) knowing that event condition occurred
    */
    template<typename DataType>
    double set<DataType>::prob_cond(const set&src,const set&condition)const{
        return prob(src.intersect(condition))/prob(condition);
    }
    /*
    in sample space (phantom ) check independence between s1 and s2
    */
    template<typename DataType>
    bool set<DataType>::independence(const set&s1,const set&s2)const{
        return prob(s1)*prob(s2)==prob(s1.intersect(s2));
    }
    /*
    in sample space(phantom ) check independence of events s1,s2,......sn
    */
    template<typename DataType>
    bool set<DataType>::independence(const set*set_arr,int n)const{
        double res =1;
        for(int i =0 ; i<n;i++){
            res *=prob(set_arr[i]) ;
        }
        return res==prob_intersect(set_arr,n) ;

    }
    //returns set of elements that are in phantom and not in src
    //different than comp that phantom doesn't have to contain src set
    //same when phantom is the sample space containing src
    template<typename DataType>
    set<DataType> set<DataType>::diff(const set&src)const {
        if(get_size()){
            DataType*vec =new DataType[get_size()];
            long long counter= 0   ;
            //for each element in phantom
            //check if it doesn't exist in src
            euler_tour(NULL,src.tree,vec,counter,not_exist);

            set ret_set(vec,counter);

            delete[]vec ; vec= NULL;
            return ret_set ;
        }
        return set(NULL,0);
    }
    /*
    retunrs p(s1 U s2 U s3 ........U sN)
    in the sample space (phantom)
    */
    template<typename DataType>
    double set<DataType>::prob_unite(const set*set_arr,int n)const{
        set s =set_arr[0];
        for(int i =1; i<n; i++){
            s= s.unite(set_arr[i]);
        }
        return prob(s);
    }
    /*
    retunrs p(s1 intersects s2 intersects s3 ........intersects sN)
    in the sample space (phantom)
    */
    template<typename DataType>
    double  set<DataType>::prob_intersect(const set*set_arr,int n)const{
        set s =set_arr[0];
        for(int i =1; i<n; i++){
            s= s.intersect(set_arr[i]);
        }
        return prob(s);
    }
    /*
    replaces an element with a new element
    */
    template<typename DataType>
    bool set<DataType>::replace(const DataType&old_data,const DataType&new_data){
        if(tree.remove(old_data)){
            return tree.insert(new_data);
        }
        return false ;
    }
    //removes duplicates of an element
    template<typename DataType>
    bool set<DataType>::remove_duplicates(const DataType&data){
         if(tree.remove(data)){
            while(tree.remove(data)){

            }
            tree.insert(data) ;
            return  1;
         }
         return 0 ;
    }
    //remove all duplicates of every element
    template<typename DataType>
    bool set<DataType>::remove_duplicates(void){
        if(get_size()){
            DataType*arr =new DataType[get_size()];
            long long counter=0;
            //for each element in phantom push it into the array
            //don't seach = always_exist

            euler_tour(NULL,tree,arr,counter,always_exist);

            for(long long  i = 0 ; i<get_size() -1; i++){
                if(arr[i]==arr[i+1]){
                    int j = i+1;
                    while(arr[j]==arr[i]&&j<get_size()){
                        remove(arr[j]);
                        j++;
                    }
                    i =j-1;
                }
            }
            delete[]arr;
            arr= NULL;
            return 1;
        }
        return 0  ;
    }


int main(){

    srand(time(0)) ;
    set<int>s1, s2 ;
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed); // Standard mersenne_twister_engine seeded with high_resolution_clock
    std::uniform_int_distribution<> distrib(0, 100000000);
    for(int i = 0 ;i< 1000000; i++){
        int random_number = distrib(gen);
        s1.insert(random_number);
        random_number = distrib(gen);
        s2.insert(random_number);
    }
    //cout<<s1.get_size();
    //cout<<s1<<endl;
    //cout<<s2.get_size()<<endl;

    //cout<<s2<<endl  ;
    s1.intersect(s2) ;

    s1.unite(s2) ;

    set<int>s3 = s1.unite(s2).comp(s1) ;

    cout<<s1.unite(s2).get_size()<<","<<s1.get_size()<<","<<s2.get_size()<<","<<s3.get_size() ;

}
