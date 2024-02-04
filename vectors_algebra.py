import math
shape_error = 'Passed vectors/matrices must be of same dimensions'
square_error = 'Passed vectors/matrices must be a square matrix [mxm]'

tolerance = 1e-6  # Adjust this value as needed

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
        if dest_vec==None then its initialized anyway to be same shape as src_vec
    """
    src_shape = vec_shape(src_vec)
    if dest_vec==None:
        dest_vec = [[0]*src_shape[1] for _ in range(src_shape[0])]
        #extracts shape of dest_vec
        #vectors must have same shape
        #copies each element of src_vec to dest_vec src in this case is the caller object
    if src_shape == vec_shape(dest_vec):
        for row_counter in range(0,src_shape[0]) :
            for col_counter in range(0, src_shape[1]):
                dest_vec[row_counter][col_counter] =src_vec[row_counter][col_counter]

    return dest_vec


def vec_show(vec):
    shape = vec_shape(vec)
    for row_counter in range(0,shape[0]):
        print('')
        for col_counter in range(0,shape[1]):
            print(str(vec[row_counter][col_counter])+',' , end = '')

def is_equal(x,y):
    x_shape =vec_shape(x)
    y_shape = vec_shape(y)
    if x_shape==y_shape:
        for row_counter in range(0,x_shape[0]):
            for col_counter in range(0,x_shape[1]):
                return x[row_counter][col_counter]!=y[row_counter][col_counter]

        return True

    return shape_error


#return a scaled up or down vector by default scalar is 1
def vec_scale(vec,scalar = 1):
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

    #returns a vector that is Alhpa*x + y
def vec_axpy(alpha,x,y):
    x_shape = vec_shape(x)
    #if x and y have same shape
    if x_shape == vec_shape(y):
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
        if xlen ==0 or ylen==0:
            return 'Undefined'

        xdoty = vec_dot(x,y)

        return math.acos(xdoty/(xlen*ylen))

    else :
        return shape_error

#returns true if 2 vectors are perpendicular
def vec_is_perp(x,y):
    return vec_dot(x,y) <=tolerance

#returns true if 2 vectors are parallel
def vec_is_parallel(x,y):
    return vec_theta(x,y)<=tolerance


def vec_transpose(vec):
    #gets shape of an m x n matrix
    shape = vec_shape(vec)
    #returns n x m matrix of zeroes
    ret_vec=  zeroes([shape[1],shape[0]])
    for row_counter in range(0,shape[0]):
        for col_counter in range(0,shape[1]):
            #fill the zeroes with the transpose of vec
            #for each element in vector x the transpose of x
            #Xtranspose(J,I) = X(I,J)
            ret_vec[col_counter][row_counter] = vec[row_counter][col_counter]

    return ret_vec


#square matrices
def vec_is_square(vec):
    shape = vec_shape(vec)
    return shape[0]==shape[1]


def vec_identity(shape = None ,row = None):
    """
        returns identity matrix of size row x row
        shape is [row,col] they must be equal since identity matrix is a square matrix
        you can pass row = m only if u want since the function only needs one element to determine size
        of the returned matrix
    """
    #identity matrix is always a square matrix
    if shape :
        if isinstance(shape, list) and shape[0]==shape[1]:
            ret_vec = zeroes(shape)
        else :
            return shape_error
    else :
        ret_vec= zeroes([row,row])
    #since its a zeroes matrix we only need to set the diagonal to one so O(N) time complexity :)
    for i in range(0,vec_shape(ret_vec)[0]):
        ret_vec[i][i]=1

    return ret_vec


def vec_diagonalize(vec):
    """
        Returns the diagnal matrix of the passed function
        Vec must be a square matrix
    """
    shape = vec_shape(vec)
    if shape[0]==shape[1]:
        ret_vec = zeroes(shape)

        for i in range(0,shape[0]):
            ret_vec[i][i] = vec[i][i]

        return ret_vec

    else :
        return square_error

def vec_trace(vec):
    """
    This function calculates the trace of a square matrix, which is the sum of the diagonal elements.
    If the input matrix is not square, it returns a square_error.
    """
    shape= vec_shape(vec)
    if shape[0]==shape[1]:  # Check if the matrix is square
        res = 0
        for i in range(0,shape[0]):  # Loop over the diagonal elements
            res+= vec[i][i]  # Add the current diagonal element to the sum
        return res  # Return the sum of the diagonal elements
    else :
        return square_error  # Return an error if the matrix is not square

def vec_upper_tri(vec, unity=None, strict=None):
    """
    This function returns an upper triangular matrix derived from the input matrix.
    If the 'strict' parameter is True, it sets the diagonal elements to zero.
    If the 'unity' parameter is True and 'strict' is not True, it sets the diagonal elements to 1.
    If the input matrix is not square, it returns a square_error.
    """
    if vec_is_square(vec):  # Check if the matrix is square
        shape = vec_shape(vec)
        ret_vec = zeroes(shape)  # Initialize a zero matrix of the same shape
        for row_counter in range(0, shape[0]):  # Loop over the rows
            if strict is None:  # If 'strict' is not True,
                if unity is None or unity is False:  # If 'unity' is not True,
                    # Keep the original diagonal elements
                    ret_vec[row_counter][row_counter] = vec[row_counter][row_counter]
                else:  # If 'unity' is True,
                    # Set the diagonal elements to 1
                    ret_vec[row_counter][row_counter] = 1

            # Loop over the columns above the diagonal
            for col_counter in range(row_counter + 1, shape[1]):
                # Copy the upper triangular elements
                ret_vec[row_counter][col_counter] = vec[row_counter][col_counter]

        return ret_vec  # Return the upper triangular matrix

    return square_error  # Return an error if the matrix is not square


def vec_lower_tri(vec, unity=None, strict=None):
    """
    This function returns a lower triangular matrix derived from the input matrix.
    If the 'strict' parameter is True, it sets the diagonal elements to zero.
    If the 'unity' parameter is True and 'strict' is not True, it sets the diagonal elements to 1.
    If the input matrix is not square, it returns a square_error.
    """
    shape = vec_shape(vec)
    if shape[0] == shape[1]:  # Check if the matrix is square
        ret_vec = zeroes(shape)  # Initialize a zero matrix of the same shape
        for row_counter in range(0, shape[0]):  # Loop over the rows
            if strict is None:  # If 'strict' is not True,
                if unity is None or unity is False:  # If 'unity' is not True,
                    # Keep the original diagonal elements
                    ret_vec[row_counter][row_counter] = vec[row_counter][row_counter]
                else:  # If 'unity' is True,
                    # Set the diagonal elements to 1
                    ret_vec[row_counter][row_counter] = 1

            # Loop over the columns below the diagonal
            for col_counter in range(0, row_counter):
                # Copy the lower triangular elements
                ret_vec[row_counter][col_counter] = vec[row_counter][col_counter]

        return ret_vec  # Return the lower triangular matrix

    return square_error  # Return an error if the matrix is not square


def vec_mult(x, y):
    """
    This function performs matrix multiplication on two input matrices x and y.
    It checks if the number of columns in x is equal to the number of rows in y, which is a requirement for matrix multiplication.
    If this condition is not met, it returns an error message.
    If the condition is met, it calculates the product of x and y and returns the resulting matrix.
    """

    # Helper function to calculate the dot product of a row and a column
    def rowxcol(row, col):
        res = 0
        for i in range(0, len(row)):
            # Multiply corresponding elements and add to the result
            res += row[i] * col[i]
        return res

    # Get the shapes of the input matrices
    x_shape = vec_shape(x)
    y_shape = vec_shape(y)

    # Check if the number of columns in x is equal to the number of rows in y
    if x_shape[1] == y_shape[0]:
        # The shape of the result is determined by the number of rows in x and the number of columns in y
        ret_shape = [x_shape[0], y_shape[1]]

        # Initialize the result matrix with zeros
        ret_vec = zeroes(ret_shape)

        # Transpose y to access its columns as rows
        y_transpose = vec_transpose(y)

        # Calculate each element of the result matrix as the dot product of a row of x and a column of y
        for row_counter in range(0, ret_shape[0]):
            for col_counter in range(0, ret_shape[1]):
                ret_vec[row_counter][col_counter] = rowxcol(x[row_counter], y_transpose[col_counter])

        return ret_vec  # Return the result matrix

    else:
        # Return an error message if the number of columns in x is not equal to the number of rows in y
        return 'For 2 matrices x and y, the number of columns in x must be equal to the number of rows in y.'


