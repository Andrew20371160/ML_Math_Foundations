
#include "bst.h"



    //turns a tree bst into a string and returns it
    //used in cout
    template<typename DataType>
    string to_string(const bst<DataType>&tree){
        string str;
        tree.to_string(str) ;
        return str ;
    }

    string to_string(const int &data){
        return std::to_string(data);
    }
    string to_string(const unsigned int&data){
        return std::to_string(static_cast<int>(data));
    }


    string to_string(const float &data){
        return std::to_string(data);
    }

    string to_string(const double &data){
        return std::to_string(data);
    }

    string to_string(const char &data){
        return std::string(1,data);
    }


    string to_string(const long &data){
        return std::to_string(static_cast<int>(data));
    }


    string to_string(const bool &data){
        return data ? "1" : "0";
    }

    string to_string(const string&data){
        return data;
    }


    template<typename DataType>
    string to_string(const vector<DataType>&vec){
        string str= "" ;
        for(int i = 0 ; i<vec.size();i++){
            str.append(::to_string(vec[i])+' ');
        }
        return str ;
    }

    //recursively add data in-order to a string str
    template <typename DataType>
    void bst<DataType>::to_string(string& str, node<DataType>* ptr) const {
        if (!ptr) {
            ptr = root ;
        }
        if(ptr){
            if (ptr->left) {
                to_string(str, ptr->left);
            }
            str.append(::to_string(ptr->data)+',');
            if (ptr->right) {
                to_string(str, ptr->right);
            }
        }
    }

    //helper function to allocate memory for a new node
    template <typename DataType>
    node<DataType>*bst<DataType>::get_node(const DataType & _data){

        node<DataType>*new_node=new node<DataType> ;

        new_node->data =_data ;

        new_node->left = NULL;
        new_node->right = NULL;
        new_node->parent = NULL;

        return new_node;
    }

    //empty tree
    template<typename DataType>
    bst<DataType>::bst(void){
        root = NULL ;
        traverser= NULL ;
        size=0;
    }
    //tree filled from an array
    //if data is sorted its filled in O(N) else O(Nlog2N)
    //note if data is sorted and there are duplicates they aren't deleted
    template <typename DataType>
    bst<DataType>::bst(const DataType* arr, const long long _size) {
        size = 0;
        root = NULL ;
        traverser=NULL;

        if (is_sorted(arr,_size)) {
            size =_size;
            root = fill_sorted(arr, 0, size-1,root);
            root->parent = NULL;
        }
        else {
            for (long long i = 0; i <_size; i++) {
               insert(arr[i]);
            }
            traverser= root;
            root->parent = NULL;
        }
    }


    template <typename DataType>
    bst<DataType>::bst(const bst<DataType>& src) {
        root = copy_bst(src.root);
        size = src.size;
    }

    template <typename DataType>
     void bst<DataType>::operator=(const bst<DataType>&src) {
        if(this!=&src&&src.root){
            if(root){
                del_tree(root) ;
                root=NULL;
            }
            root=copy_bst(src.root) ;
            size= src.size;
        }
        else{
            del_tree(root) ;
            root =NULL;
        }
     }

    //tree filled from an array
    //if data is sorted its filled in O(N) else O(Nlog2N)
    template<typename DataType>
    bst<DataType>::bst(const vector<DataType>&arr) {
        size = 0;
        root = NULL ;
        traverser=NULL;
        if (is_sorted(arr)){
            size =arr.size();
            root = fill_sorted(arr, 0, size-1,root);
            root->parent = NULL;
        }
        else {
            for (long long i = 0; i <arr.size(); i++) {
               insert(arr[i]);
            }
            traverser= root;
            root->parent = NULL;
        }
    }

    //destructor
   template <typename DataType>
    bst<DataType>::~bst() {
        del_tree(root);
        root = NULL;
        traverser=NULL;
        size = 0;
    }
    //functions that remove the whole tree or a subtree
   template <typename DataType>//removes the whole bst
    bool bst<DataType>:: remove_tree(void){
         if(root){
           del_tree(root) ;
            root =NULL;
            traverser=NULL;
            size = 0 ;

        return 1;
     }
     return 0;
    }
    //remove the subtree beneath root containing data
    template <typename DataType>
    bool bst<DataType>:: remove_subtree(const DataType&data){
         if(search(data)){
            fix_parent(traverser,NULL) ;
            del_tree(traverser);
            traverser= root ;
            return 1 ;
         }
         return 0 ;
    }
    //checks if data in an array is sorted
    template <typename DataType>
    bool bst<DataType>::is_sorted(const DataType*arr,long long _size){
        for(long long i = 0  ; i <_size-1;i++){
            if(arr[i]>arr[i+1]){
                return 0;
            }
        }
        return 1 ;
    }

    //checks if vector of datatype is sorted
        template<typename DataType>

    bool bst<DataType>::is_sorted(const vector<DataType>&arr){
            for(long long i = 0  ; i <arr.size()-1;i++){
                if(arr[i]>arr[i+1]){
                    return 0;
                }
            }
            return 1 ;
        }

        template <typename DataType>
    node<DataType>*bst<DataType>::fill_sorted(const vector<DataType>&arr,long long beg,long long end,node<DataType>*ptr){
        if(ptr==NULL){
           if(beg<=end){
                long long mid =(end-beg)/2+beg;

                ptr = get_node(arr[mid]) ;

                ptr->left = fill_sorted(arr,beg,mid-1,ptr->left) ;

                if(ptr->left){
                    ptr->left->parent=ptr;
                }
                ptr->right=fill_sorted(arr,mid+1,end,ptr->right);
                if(ptr->right){
                    ptr->right->parent=ptr;
                }
                return ptr ;
            }
            else{
                return NULL;
            }
        }
        return NULL ;

    }//passed


    /*
    these functions fill a bst from sorted data in linear time (O(N))
    and returns the root of the tree
    one from an array and the other one from a file but the indexing file must exist(used in load)
    if indexing file doesn't exist and data is sorted it loads the data in a vector of the same data type
    and then puts it in linear time in the bst
    */
    template <typename DataType>
    node<DataType>* bst<DataType>:: fill_sorted(const DataType*arr,long long beg ,long long end ,node<DataType>*ptr){
        if(ptr==NULL){
           if(beg<=end){
                long long mid =(end-beg)/2+beg;

                ptr = get_node(arr[mid]) ;

                ptr->left = fill_sorted(arr,beg,mid-1,ptr->left) ;

                if(ptr->left){
                    ptr->left->parent=ptr;
                }
                ptr->right=fill_sorted(arr,mid+1,end,ptr->right);
                if(ptr->right){
                    ptr->right->parent=ptr;
                }
                return ptr ;
            }
            else{
                return NULL;
            }
        }
        return NULL ;
    }



    //deletes a binary tree where root is ptr
    template <typename DataType>//no recusion
     void bst<DataType>:: del_tree(node<DataType>*ptr){
        if(ptr){
            queue<node<DataType>*>q ;
            q.push(ptr) ;

            if(ptr->parent){
                fix_parent(ptr,NULL);
            }

            long long counter =1 ;
            while(!q.empty()){
                node<DataType>*temp= q.front() ;
                q.pop() ;
                if(temp->left){
                    q.push(temp->left);
                    counter++;
                }

                if(temp->right){
                    q.push(temp->right);
                    counter++ ;
                }

                if(temp){
                    delete temp;
                    temp=NULL;
                }
            }
            if(ptr==root){
               root=NULL;
            }
            ptr=NULL;
            size-=counter;
        }
    }



    //data inserted must have the id or the feature of comparison not repeated
    //ex: id of a person...etc
    //no recursion
    template <typename DataType>
    bool bst<DataType>::insert(const DataType&_data){
        node<DataType>*ptr= root;
        if(root==NULL){
            root=get_node(_data);
            size=1;
            return 1 ;
        }
        else{
            size++;
            while(1){
                if(_data==ptr->data){
                    size--;
                    return 0  ;
                }
                else if(_data<ptr->data){
                    if(ptr->left){
                        ptr=ptr->left;
                    }
                    else{
                        ptr->left= get_node(_data) ;
                        ptr->left->parent=ptr;
                        return 1  ;
                    }
                }
                else{
                    if(ptr->right){
                        ptr=ptr->right;
                    }
                    else{
                        ptr->right= get_node(_data) ;
                        ptr->right->parent=ptr;
                        return 1  ;
                    }
                }
            }
        }
    }

    //this function puts traverser pointer on the node containing data if found
    //and returns true
    template <typename DataType>    //no recursion
    bool bst<DataType>::search(const DataType&_data)const{
         traverser=root;
         if(traverser==NULL){
             return 0 ;
         }
        else{
            while(1){
                if(_data==traverser->data){
                    return 1  ;
                }
                else if(_data<traverser->data){
                    if(traverser->left){
                        traverser=traverser->left;
                    }
                    else{
                        return 0 ;
                    }
                }
                else{
                    if(traverser->right){
                        traverser=traverser->right;
                    }
                    else{
                        return 0  ;
                    }
                }
            }
        }
    }

    /*
    next set of functions are used in deletion of a node in a bst
    */
    //returns node with max data where root is ptr
    template <typename DataType>
    node<DataType>*bst<DataType>::get_max(node<DataType>*ptr)const {
        if(ptr){
            while(ptr->right){
                ptr=ptr->right;
            }
            return ptr ;
        }
    }

    //returns node with min data where root is ptr
    template <typename DataType>
    node<DataType>* bst<DataType>::get_min(node<DataType>*ptr)const {
        if(ptr){
            while(ptr->left){
                ptr=ptr->left;
            }
            return ptr ;
        }
    }

    //checks if ptr exists and its the left child
    template <typename DataType>
    bool  bst<DataType>::is_left(node<DataType>*ptr)const{
        return(ptr&&ptr->parent&&ptr->parent->left==ptr);
    }


    //checks if ptr exists and its the right child
    template <typename DataType>
    bool  bst<DataType>::is_right(node<DataType>*ptr)const {
        return(ptr&&ptr->parent&&ptr->parent->right==ptr);
    }

    /*
    this function makes ptr's parent's link pointing at ptr point at another destiniation
    for example ptr is left child of a parent left and we want it to point to another node
    this function takes care of it
    */
    template <typename DataType>
    void bst<DataType>::fix_parent(node<DataType>*ptr,node<DataType>*dest){
        if(is_left(ptr)){
            ptr->parent->left=dest;
        }
        else if(is_right(ptr)){
            ptr->parent->right=dest ;
        }
    }
    //checks if ptr is the root
    template <typename DataType>
    bool bst<DataType>::is_root(node<DataType>*ptr)const {
          return (ptr==root) ;
    }

    //deletes a node from a bst
    template <typename DataType>//no recursion
    bool bst<DataType>::remove(const DataType&data){
        if(root){
            //if data exists traverser sits on it now
            if(search(data)){
                size--;//decrement if data is found
                //leaf node condition
                if(traverser->left==NULL&&traverser->right==NULL){
                    if(is_root(traverser)){
                        delete root ;
                        traverser=NULL;
                        root= NULL;
                    }
                    else{
                        //update paren't link to point to null
                        fix_parent(traverser,NULL) ;
                        delete traverser ;
                        traverser = NULL;
                    }

                }
                //no right child condition
                else if(traverser->right==NULL){
                     traverser->left->parent=traverser->parent;
                     if(!is_root(traverser)){
                        fix_parent(traverser,traverser->left);
                      }
                      else{
                          root =root->left ;
                      }
                      delete traverser;
                      traverser=NULL;
                    }
                //no left child condition
                else if(traverser->left==NULL){
                     traverser->right->parent=traverser->parent;
                     if(!is_root(traverser)){
                        fix_parent(traverser,traverser->right);
                      }
                      else{
                          root =root->right ;
                      }
                      delete traverser;
                      traverser=NULL;
                    }
                    //2 children condition
                else{
                    node<DataType>*temp=(traverser->left)?get_max(traverser->left):get_min(traverser->right);
                    //save data in temp_data
                    //and saves traverser location in a temp pointer
                    //so that after removing temp data
                    //the temp_ptr is assigned the value
                    DataType temp_data=temp->data;
                    node<DataType>*temp_ptr= traverser;
                    remove(temp_data);
                    temp_ptr->data=temp_data ;
                }
                traverser=root;
                return 1 ;
                }
            }
            else{
                traverser=root;
            }
            return 0;
        }

    //copies a tree where ptr is the root of that tree
    template <typename DataType>    //no recursion
    node<DataType>*bst<DataType>::copy_bst(const node<DataType>* ptr) {
        if (ptr) {
            node<DataType>* new_root = get_node(ptr->data);
            queue<const node<DataType>*> src;

            queue<node<DataType>*> dest;
            size = 1 ;
            src.push(ptr);
            dest.push(new_root);

            while (!src.empty()) {
                const node<DataType>* src_node = src.front();

                node<DataType>* dest_node = dest.front();

                src.pop();
                dest.pop();

                if (src_node->left) {
                    dest_node->left = get_node(src_node->left->data);
                    dest_node->left->parent = dest_node;
                    src.push(src_node->left);
                    dest.push(dest_node->left);
                    size++;
                }

                if (src_node->right) {
                    dest_node->right = get_node(src_node->right->data);
                    dest_node->right->parent = dest_node;
                    src.push(src_node->right);
                    dest.push(dest_node->right);
                    size++;
                    }
                }
                return new_root;
            }
            //if src is null the tree is deleted
            else{
                del_tree(root) ;
                root =  NULL;
            }
            return NULL;
        }

    //inorder print the data of the bst where root is ptr
    //by default its root of the phantom
    template <typename DataType>//recursion used
    void  bst<DataType>::inorder(node<DataType>*ptr)const {
        if(!ptr){
            ptr = root ;
        }
        if(ptr){
            if(ptr->left){
                inorder(ptr->left);
            }
            cout<<ptr->data<<" , ";
            if(ptr->right){
                inorder(ptr->right)  ;
            }
        }
    }

    template <typename DataType>//recursion used
    void  bst<DataType>::preorder(node<DataType>*ptr)const {
        if(!ptr){
            ptr = root ;
        }
        if(ptr){
            cout<<ptr->data<<" , " ;
            if(ptr->left){
                preorder(ptr->left);
            }
            if(ptr->right){
                preorder(ptr->right)  ;
            }
        }
    }
    template <typename DataType>//recursion used
    void bst<DataType>:: postorder(node<DataType>*ptr)const {
        if(!ptr){
            ptr = root ;
        }
        if(ptr){
            if(ptr->left){
                postorder(ptr->left);
            }
            if(ptr->right){
                postorder(ptr->right)  ;
            }
            cout<<ptr->data<<" , " ;
        }
    }
    //prints the tree in level order or breadth-first
    template <typename DataType>//no recursion
    void  bst<DataType>::breadth_first(node<DataType>*ptr)const{
        if(root){
            if(!ptr){
                ptr = root ;
            }
            queue<const node<DataType>*>q ;
            q.push(ptr) ;
            while(!q.empty()){
                const node<DataType>*temp= q.front() ;
                q.pop() ;
                if(temp){
                    if(temp->left){
                        q.push(temp->left);
                    }
                    if(temp->right){
                        q.push(temp->right);
                    }
                    if(temp){
                        cout<<temp->data<<" , ";
                    }
                }
            }
        }
    }


    //interface with the tree
    template <typename DataType>
    void bst<DataType>:: move(void)const{
        if(root){
            traverser= root;
            while(1){
                cout<<"\n(Enter->stops at current)\n(w->go up)\n(a->go left)\n(d->go right)\n(s->show the whole tree)\n(q->quit)\n>>";

                cout<<"\n\nCurrent : "<<traverser->data<<"\n\n";
                cout<<"\n>>";

                char ch = getch();
                switch(ch){
                    case 'w':{
                        if(traverser->parent)
                           traverser =traverser->parent;
                        }break;
                    case 'd':{
                        if(traverser->right)
                           traverser =traverser->right;
                        }break;

                    case 'a':{
                        if(traverser->left)
                           traverser =traverser->left;
                        }break;
                    case 's':{
                        cout<<"\n\nLevel Order\n\n";
                        breadth_first(root);
                    }break;
                    case 'r':{
                            traverser=root;
                        }break;

                    case 'q':{
                        traverser=root;
                        return ;
                        }break;
                    case '\r' :{
                        return ;
                        }

                    }
                }
            }
            else{
                cout<<"\nTree is empty\n" ;
            }
        }



    //cout a bst
    template <typename DataType>
    ostream& operator<<(ostream& os, const bst<DataType>&obj) {
        string output= "";
        obj.to_string(output);
        os<<output ;
        return os ;
    }

    template <typename DataType>
    istream& operator>>(istream& is, bst<DataType>& obj) {
        string line;
        while (getline(is, line) &&!line.empty()) {
            std::istringstream lineStream(line);
            DataType data;
            while (lineStream >> data) {
                obj.insert(data);
            }
        }
        return is;
    }


/*
    logical operators for the bst
    if user wants to create a bst of bsts it needs more work since how do you get max element of those trees ?
*/
    //equality of 2 bst is if they have same structure and same data
    //so breadth first equality is checked via this function
    template<typename DataType>
    bool bst<DataType>::operator==(const bst&src)const{
        if(root&&src.root){
            queue<const node<DataType>*>b1,b2;
            b1.push(root) ;
            b2.push(src.root);
            bool l1=0,l2=0,r1=0,r2=0 ;
            while(!b1.empty()&&!b2.empty()){
                l1=0;l2=0;
                r1=0;r2=0;
                const node<DataType>*temp= b1.front() ;
                const node<DataType>*temp2= b2.front() ;

                    b1.pop() ;
                    b2.pop() ;

                    if(temp->left){
                        l1 =1;
                        b1.push(temp->left)  ;
                    }
                    if(temp2->left){
                        l2= 1 ;
                        b2.push(temp2->left)  ;
                    }
                    if(l1!=l2){
                        return 0 ;
                    }
                    if(temp->right){
                        r1 =1;
                        b1.push(temp->right)  ;
                    }
                    if(temp2->right){
                        r2= 1 ;
                        b2.push(temp2->right)  ;
                    }
                    if(r1!=r2){
                        return 0 ;
                    }
                    if(!(temp->data==temp2->data)){
                        return 0;
                    }
                }
                return b1.empty()&&b2.empty() ;
            }

        return (root==NULL&&src.root==NULL);
    }


    //compares max of phantom is less than max of src
     template <typename DataType>
     bool bst<DataType>::operator <(const bst&src) const {
          if(root&&src.root){
              return get_max()->data<src.get_max()->data;
          }
          return (root==NULL);
     }
    //compares if max of phantom is bigger than max of src
    template <typename DataType>
    bool bst<DataType>::operator >(const bst&src) const {
          if(root&&src.root){
              return get_max()->data>src.get_max()->data;
          }
          return (src==NULL);
    }
    //get number of elements in the tree
    template<typename DataType>
    long long  bst<DataType>::get_size(void)const{
        return size;
    }
    template<typename DataType>
    DataType bst<DataType>:: access_traverser(void){
        DataType data ;
        if(traverser){
            data = traverser->data ;
        }
        return data;
    }


