import math

#error msg instead of None for ease of use 
shape_error = "vectors passed aren't same dimentions,operation can't be performed"

tolerance = 1e-6  # Adjust this value as needed

#2d vector linear algebra operations
#working on adding these aswell
"""
transpose matrix
the upper triangular part. ( Set_to_upper_triangular_matrix_unb)
the strictly upper triangular part. ( Set_to_strictly_upper_triangular_matrix_unb)
the unit upper triangular part. ( Set_to_unit_upper_triangular_matrix_unb)
strictly lower triangular part. ( Set_to_strictly_lower_triangular_matrix_unb)
unit lower triangular part. ( Set_to_unit_lower_triangular_matrix_unb)"""

#returns shape of a vector as an array [row,col]
def vec_shape(vec):
    return [len(vec),len(vec[0])]

#returns a vector with all elements set to zeroes
#shape = [rows,cols]
def zeroes(shape):
    if shape:
        return [[0]*shape[1] for _ in range(shape[0])]

#Copies src_vec to dest_vec

def vec_copy(src_vec,dest_vec= None):
    """copies src vector (caller) to dest_vec if not None
        if dest_vec==None then its initialized inside the function
        and value is returned after coppying 
    """
    #get src_shape
    src_shape = vec_shape(src_vec)
    if dest_vec==None:
        dest_vec = zeroes(src_shape)
    #vectors must have same shape
    if src_shape == vec_shape(dest_vec):
        for row_counter in range(0,src_shape[0]) :
            for col_counter in range(0, src_shape[1]):
                #copies each element of src_vec to dest_vec src in this case is the caller object
                dest_vec[row_counter][col_counter] =src_vec[row_counter][col_counter]

        return dest_vec
        
    else :
        return shape_error

def is_equal(x,y):
    """Checks for equality of 2 vectors x,y 
        they must be of same dimentions or else error message is returned
    """
    x_shape =vec_shape(x)
    y_shape = vec_shape(y)
    if x_shape==y_shape:
        for row_counter in range(0,x_shape[0]):
            for col_counter in range(0,x_shape[1]):
                return x[row_counter][col_counter]!=y[row_counter][col_counter]
    
        return True
    return shape_error 


def vec_scale(vec,scalar = 1):
    """returns a scaled up or down vector by default scalar is 1"""
    
    #if scalar is 1 then return the vector as is
    if scalar==1:
        return vec
    else:
        shape = vec_shape(vec)
        #scale up or down the vector
        for row_counter in range(0, shape[0]):
            for col_counter in range(0, shape[1]):
                vec[row_counter][col_counter]*=scalar
    return vec

def vec_axpy(alpha,x,y):
    """ performs the axpy operation where 
        a is alpha aka the scalar
        x is the first vector 
        y is the second vector
    """
    x_shape = vec_shape(x)
    #if x and y have same shape
    if x_shape == vec_shape(y):
        #initializing a new vectors with zeroes using the zeroes function
        ret_vec = zeroes(x_shape)
        for row_counter in range(0, x_shape[0]):
            for col_counter in range(0, x_shape[1]):
                #compute Alhpa*x + y
                ret_vec[row_counter][col_counter] =alpha*x[row_counter][col_counter] +y[row_counter][col_counter]
        return ret_vec
    else :
        return shape_error

#returns inner or dot product of 2 vectors
def vec_dot(x,y):
    """returns dot product or inner product between 
       2 vectors x and y 
    """
    x_shape = vec_shape(x)
    
    if x_shape == vec_shape(y):
        #result of dot product
        res = 0
        #for each element in x and y
        for row_counter in range(0, x_shape[0]):
            for col_counter in range(0, x_shape[1]):
                #compute xi*yi and add to res
                res += x[row_counter][col_counter]*y[row_counter][col_counter]
        return res
    else:
        return shape_error

#returns norm2 of a vector
def vec_norm2(x):
    """returns length of a vector x or norm2 of it 
    """
    #remember (x)dot(x) = |x|^2
    #so norm2 or length of it is square root of ((x)dot(x))
    res = vec_dot(x,x)
    return res**0.5
    #returns norm2 or length of a vector
def vec_len(x):
    """returns length of a vector x or norm2 of it 
    """
    return vec_norm2(x)
    
#returns the theta between 2 vectors x&y
def vec_theta(x,y):
    """ this function takes 2 arguments vectors :x,y and returns 
        the angle between x and y  
    """
    if vec_shape(x)==vec_shape(y):
        #remember x.y = |x||y|cos(theta)
        #so cos(theta) = x.y/|x||y|
        #so theta = cos^-1(x.y/|x||y|)
        xlen = vec_len(x)
        ylen = vec_len(y)
        #checks if one of the arguments is zeroes vectors hence returns undefined 
        if xlen ==0 or ylen==0:
            return 'Undefined'

        xdoty = vec_dot(x,y)
        return math.acos(xdoty/(xlen*ylen))

    else :
        return shape_error

#returns true if 2 vectors are perpendicular
def vec_is_perp(x,y):
    """returns true if 2 x and y are perpendicular
    """
    #remember x.y = |x||y|cos(theta)
    #so if x.y equals 0 then theta is 90 deg hence vectors are perpendicular 
    return vec_dot(x,y) <=tolerance

def vec_is_parallel(x,y):
    """returns true if 2  x and y  are parallel
    """
    return vec_theta(x,y)<=tolerance



