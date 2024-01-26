import math

#returns shape of a vector as an array [row,col]
def vec_shape(vec):
    #extracts row size
    row = len(vec)
    #extracts col size
    col = len(vec[0])
    return [row,col]

#Copies src_vec to dest_vec
def vec_copy(dest_vec,src_vec):
    #extracts shape of dest_vec
    dest_shape=  vec_shape(dest_vec)
    #vectors must have same shape
    if dest_shape==vec_shape(src_vec):
        #copies each element of src_vec to dest_vec
        for row_counter in range(0,dest_shape[0]) :
            for col_counter in range(0, dest_shape[1]):
                  dest_vec[row_counter,col_counter] = src_vec[row_counter,col_counter]
    else :
        return None

#return a scaled up or down vector by default scalar is 1
def vec_scale(vec,scalar = 1):
        vector_shape = vec_shape(vec)
        #if scalar is 1 then return the vector as is
        if scalar==1:
            return vec
        else:
            #scale up or down the vector
            for row_counter in range(0, vector_shape[0]):
                for col_counter in range(0, vector_shape[1]):
                    vec[row_counter,col_counter]*=scalar
        return vec

#returns a vector that is Alhpa*x + y
def vec_axpy(alpha , x,y):
    x_shape = vec_shape(x)
    #if x and y have same shape
    if x_shape == vec_shape(y):
        ret_vec = [[0]*x_shape[1] for _ in range(x_shape[0])]        #for each element in x and y
        for row_counter in range(0, x_shape[0]):
            for col_counter in range(0, x_shape[1]):
                #compute Alhpa*x + y
                ret_vec[row_counter,col_counter] =alpha*x[row_counter,col_counter] +y[row_counter,col_counter]
        return ret_vec
    else :
        return None
#returns inner or dot product of 2 vectors
def vec_dot(x,y):
    x_shape = vec_shape(x)
    if x_shape == vec_shape(y):
        ret_vec = x
        #result of dot product
        res = 0
        #for each element in x and y
        for row_counter in range(0, x_shape[0]):
            for col_counter in range(0, x_shape[1]):
                #compute xi*yi and add to res
                res += x[row_counter,col_counter]*y[row_counter,col_counter]
        return res
    else:
        return None
#returns norm2 of a vector
def vec_norm2(x):
    res = vec_dot(x,x)
    return res**0.5
#returns norm2 or length of a vector
def vec_len(x):
    return vec_norm2(x)
#returns the theta between 2 vectors x&y
def vec_theta(x,y):
    if vec_shape(x)==vec_shape(y):
        #remember x.y = |x||y|cos(theta)
        #so cos(theta) = x.y/|x||y|
        #so theta = cos^-1(x.y/|x||y|)
        xlen = vec_len(x)
        ylen = vec_len(y)
        xdoty = vec_dot(x,y)
        return math.acos(xdoty/(xlen*ylen))
    else :
        return None

#returns true if 2 vectors are perpendicular
def vec_is_perp(x,y):
    return vec_dot(x,y) ==0
#returns true if 2 vectors are parallel
def vec_is_parallel(x,y):
    return vec_theta(x,y)==0

#under construction

