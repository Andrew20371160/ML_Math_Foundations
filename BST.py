class bst:

    def __init__(self,data=None):
        """initiates the bst root and its left and right nodes with None"""
        self.left = None
        self.right = None
        self.parent =None
        if data==None:
            #temporary value for self.data untill user inserts one
            self.data=None
        else:
            self.data=data

    def insert(self,data):
        """Inserts a new node into the bst"""
        #if user didn't initalize the root node earlier
        if self.data== None :
            self.data=data
            return self
        else:
            #this pointer will traverse the tree untill its position
            bst_traverse =self
            while True :
                if data <=bst_traverse.data :
                    #if left child is None then its the place for putting the new node
                    if bst_traverse.left ==None:
                        bst_traverse.left=bst(data)
                        bst_traverse.left.parent= bst_traverse
                        #traversing the bst is finished by this condition
                        break
                    else :
                        #go left
                        bst_traverse = bst_traverse.left
                elif data > bst_traverse.data and bst_traverse.right==None:
                    #inserts a new node at right child
                    bst_traverse.right=bst(data)
                    bst_traverse.right.parent= bst_traverse
                    break
                else:
                    #go right
                    bst_traverse = bst_traverse.right

    def search(self,data):
        """Searchs for data on the bst"""
        bst_traverse = self
        while True :
            if data ==bst_traverse.data:
                return [True,bst_traverse]

            elif data <bst_traverse.data:
                #checks if there is a left child
                if bst_traverse.left==None:
                    return [False,None]
                else:
                    #keep traversing
                    bst_traverse =bst_traverse.left
            else:
                #checks if there is a right child
                if bst_traverse.right==None:
                    return [False,None]
                else:
                    bst_traverse=bst_traverse.right

    def replace(self,dest_data,src_data):
        #search first
        logic,temp_node= self.search(dest_data)
        if logic ==True :
            if src_data!=temp_node.data:
                temp_node.del_node(temp_node.data)
                self.insert(src_data)
            return True

        return False



    def min(self):
        """This function returns minimum data or leftmost node's data """
        bst_traverse=self
        while bst_traverse.left !=None :
            bst_traverse=bst_traverse.left
        return bst_traverse.data

    def max(self):
        """This function returns maximum data or rightmost node's data"""
        bst_traverse = self
        while bst_traverse.right !=None :
            bst_traverse=bst_traverse.right
        return bst_traverse.data


    def is_left_child(self):
        """checks if current node is a left child of the parent node"""
        return self.parent!=None and self.data<=self.parent.data

    def is_right_child(self):
        """checks if current node is a right child of the parent node"""
        return self.parent!=None and self.data>self.parent.data

    def update_parent_nodes(self,dest_node):
        """
        Updates the parent node's left or right child according to the destination node.

        If the current node is the left child of its parent, the parent's left child is updated to be the destination node.
        If the current node is the right child of its parent, the parent's right child is updated to be the destination node.

        This method is primarily used in the `del_node` function to maintain the binary search tree properties after a node deletion.

        Parameters:
        dest_node: The node to replace the current node in the parent's children.
        """
        if  self.parent != None :
            if self.is_left_child() :
                self.parent.left=dest_node
            else:
                self.parent.right=dest_node

    def del_node(self, data):
        bst_traverse = self
        while True:
            if data == bst_traverse.data:
                break
            elif data < bst_traverse.data:
                if bst_traverse.left is not None:
                    bst_traverse = bst_traverse.left
                else:
                    return False
            else:
                if bst_traverse.right is not None:
                    bst_traverse = bst_traverse.right
                else:
                    return False

        if bst_traverse.left is None and bst_traverse.right is None:
            bst_traverse.update_parent_nodes(None)
        elif bst_traverse.left is None:
            bst_traverse.right.parent = bst_traverse.parent  # Update child's parent reference
            bst_traverse.update_parent_nodes(bst_traverse.right)
        elif bst_traverse.right is None:
            bst_traverse.left.parent = bst_traverse.parent  # Update child's parent reference
            bst_traverse.update_parent_nodes(bst_traverse.left)
        else:
            min_data = bst_traverse.right.min()
            bst_traverse.right.del_node(min_data)
            bst_traverse.data = min_data


        return True


    def del_bst(self,data = None):
        def del_all(self):
            if self != None:
                if self.left is not None:
                    del_all(self.left)
                    self.left = None
                if self.right is not None:
                    del_all(self.right)
                    self.right = None
                del self

        if self.data!=None :
            del_all(self)
            self.data=None



    def parent(self,data):
        """Returns parent of data if it exists in the bst"""
        logic,temp_node=self.search(data)
        if logic==True and temp_node !=None :
            return temp_node.parent.data
        else:
            return None

    def predecessor(self,data):
        """in a sorted bst this function returns the previous element
            in an inorder display of the elements
            ex : 1 ,2 ,3 ,4 ,5
            predecessor(3) = 2
        """
        logic,temp_node = self.search(data)
        if logic==True:
            temp_data = temp_node.data
            if  temp_node.left!=None:
                return temp_node.left.max()
            #left node is None at this point and if its the left child then this is minimum node
            #at the bst (worst case senario)
            elif temp_node.is_left_child():
            # while value is bigger visit parents
            #if the loop doesn't start then it must be minimum data
                while temp_node!=None:
                    if  temp_data > temp_node.data:
                        return temp_node.data

                    temp_node= temp_node.parent
                else:
                    return None

            return temp_node.parent.data
        return None

    def successor(self,data):
        """in a sorted bst this function returns the previous element
            in an inorder display of the elements
            ex : 1 ,2 ,3 ,4 ,5
            predecessor(3) = 2
        """
        logic,temp_node = self.search(data)
        if logic==True:
            temp_data = temp_node.data
            if  temp_node.right!=None:
                return temp_node.right.min()
            #left node is None at this point and if its the left child then this is minimum node
            #at the bst (worst case senario)
            elif temp_node.is_right_child():
            # while value is bigger visit parents
            #if the loop doesn't start then it must be minimum data
                while temp_node!=None:
                    if  temp_data < temp_node.data:
                        return temp_node.data

                    temp_node= temp_node.parent
                else:
                    return None

            return temp_node.parent.data
        return None

    def inorder(self):
        """This function performs inorder traversal of the bst"""
        #first check if left child is not None to prevent errors
        if self.left != None:
            #go left
            self.left.inorder()
        #prints of the node
        print(self.data)
        #again check if left child isn't None
        if self.right != None:
            #go right
            self.right.inorder()

    def postorder(self):
        """Children are vested first then the data is printed"""
        if self.left != None:
            self.left.postorder()
        if self.right  != None:
            self.right.postorder()
        print(self.data)


    def reverse_order(self):
        """Displays data on the bst in decending order"""
        if self.right != None:
            self.right.reverse_order()
        print(self.data)
        if self.left != None:
            self.left.reverse_order()


    def choose_order(self,order=None):
        """
        Chooses the order of traversal for the binary tree.
        Parameters:
        order (str): The order of traversal. Options are:
        'in' for in-order traversal,
        'rev' for reverse order traversal,
        'post' for post-order traversal.
        By default, it performs in-order traversal.
        """
        if order=='in' or order==None:
            self.inorder()
        elif order=='rev':
            self.reverse_order()
        elif order =='post' :
            self.postorder()
        else :
            self.inorder()

    def height(self,data=None):

        def get_height(self):
            left_height = -1
            right_height = -1

            if self.left!=None:
                left_height=get_height(self.left)
            if self.right !=None:
                right_height =get_height(self.right)

            return max([left_height,right_height])+1

        if data :
            logic,temp_node =self.search(data)
            if logic:
                return get_height(temp_node)
        else :
            return get_height(self)

    def depth(self,data=None):
        if data:
            bst_traverse = self
            counter = 0
            while True :
                if data ==bst_traverse.data:
                    return counter

                elif data<bst_traverse.data:
                    if bst_traverse.left !=None:
                        bst_traverse=bst_traverse.left
                    else :
                        return -1
                else :
                    if bst_traverse.right !=None:
                        bst_traverse = bst_traverse.right
                    else :
                        return-1
                counter+=1

            return counter
        else:
            return 0

    def count(self,data =None):
        counter=0
        if data==None:
            bst_traverse= self
        else :
            logic,bst_traverse =self.search(data)

        if bst_traverse!=None:
            if bst_traverse.left!=None:
               counter += bst_traverse.left.count()
            if bst_traverse.right!=None:
                counter += bst_traverse.right.count()

            return counter+1





root= bst(20)
root.insert(15)
root.insert(25)
root.insert(18)
root.insert(11)
root.insert(23)
root.insert(27)
root.insert(3)
root.insert(12)
root.insert(21)
root.insert(24)
root.insert(22)
root.insert(29)
root.insert(1)
root.insert(2)
root.insert(7)
root.insert(5)
root.insert(4)
root.insert(6)

print(root.count(15))

root.del_bst()

