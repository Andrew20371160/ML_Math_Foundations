Disclaimer:This Documentation is ai generated however I reviewed it

# Matrix Algebra Library — Full Documentation

## 0) Overview

This library provides:

- A templated matrix class `matrix<DataType>` supporting **dense storage** and **compressed structural storage** (triangular/diagonal/symmetric/anti-symmetric/identity/constant).
- A custom `complex` type with numeric helper functions (notably `conjugate`) so matrix transpose naturally becomes **conjugate transpose** for complex-valued matrices.
- Core linear algebra: arithmetic, elimination (Gaussian / RREF), solving, rank/determinant/inverse, subspaces, decompositions (LU/QR/SVD), eigen methods, projections/least squares, and FFT operations.

---

# 1) Scalar type support (`complex`) and scalar helper functions

## 1.1 `class complex`
A minimal complex scalar implementation used by FFT/Fourier operations and to enable conjugate-transpose behavior in `matrix::transpose()` and inner products.

### Construction / access
- `complex(long double re=0, long double im=0)`  
  Construct from real/imaginary parts.
- `complex(int val)`  
  Real = `val`, Imag = 0.
- `complex(const complex& c)`  
  Copy.
- `get_re()`, `get_im()`  
  Return real and imaginary components.

### Arithmetic operators
- `+`, `-`, `*`, `/` with another `complex`  
  Standard complex arithmetic.
- `*`, `/` with `long double`  
  Scalar scale.
- `^ (long double power)`  
  Raises to a real power using polar form (magnitude + angle).
- Compound assignment versions: `+=`, `-=`, `*=`, `/=`, `^=` (also scalar `*=`, `/=`).

### Comparison / equality
- `operator>(const complex&)`, `operator<(const complex&)`  
  Compares by magnitude.
- `operator==`, `operator!=`  
  Tolerance-based equality for floating-point stability.

### Other
- `theta()`  
  The argument of the complex number (angle) via `atan2(im, re)`.

## 1.2 Complex free functions / numeric utilities
- `abs(const complex&) -> long double`  
  Magnitude.
- `conjugate(const complex&) -> complex`  
  Complex conjugate.
- `to_string(complex&) -> std::string`  
  Used by matrix printing/serialization.

## 1.3 `conjugate()` overloads for real/primitive types (helper)
The library defines `conjugate(x)` overloads for many primitive numeric types (int/float/double/long double/etc.) that **return x unchanged**.  
This allows `matrix::transpose()` and symmetry checks to call `conjugate()` generically regardless of scalar type.

---

# 2) Matrix core type: construction, storage, and access model

## 2.1 Primary type
- `template<typename DataType> class matrix`

### Key design features
- Supports both:
  - **Dense**: stores all `rows*cols` values.
  - **Compressed**: stores only the required entries for known structures (triangular/diagonal/symmetric/etc.) and synthesizes the rest as zeros (or sign-flipped symmetric values for anti-symmetric).
- Uses internal **function-pointer dispatch** for:
  - element access (`at_ptr`, `at_ptr_c`)
  - iteration bounds (`start_ptr`, `end_ptr`)
  so many algorithms can run efficiently without special-casing matrix type in every loop.

---

## 2.2 Construction / destruction

### Public API
- `matrix()`  
  Creates an empty 0×0 matrix.

- `matrix(int rows, int cols, DataType initialization_value = no_init, bool compressed = false)`  
  Allocates a matrix.  
  - If `compressed=false`: dense allocation.
  - If `compressed=true`: stores as a single value (constant compressed form).

- `matrix(int rows, int cols, DataType* initialization_array, int size_of_array)`  
  Fills in row-major order from a flat array and pads remaining entries with `0`.

- `matrix(const matrix& other)`  
  Deep copy of storage and metadata.

- `~matrix()`  
  Frees dense/compressed storage and any indexing tables used by compressed formats.

### Helpers / internal behavior (supporting these constructors)
- Allocation and memory ownership is centralized around the internal `vec` pointer plus (for some compressions) `row_start` and `pindex`.

---

## 2.3 Shape & validation

### Public API
- `int get_rows() const`, `int get_cols() const`  
- `int get_size() const`  
  Number of stored values (not necessarily rows*cols if compressed).
- `int get_type() const`  
  Returns the library’s internal “matrix type” tag.
- `bool is_valid_index(int r, int c) const`  
  Bounds check.
- `bool same_shape(const matrix& other) const`  
  Ensures compatible shapes for elementwise operations.

---

## 2.4 Element access and iteration bounds

### Public API
- `DataType& at(int r, int c)`  
- `const DataType& at(int r, int c) const`  
  Returns a reference to an element or to an internal synthesized placeholder (used for out-of-storage elements in compressed matrices).

- `int start(int row) const`, `int end(int row) const`  
  Returns the valid/interesting column range for iterating across a row efficiently.

### Internal access helpers (feature-critical)
These functions materially define behavior of compressed matrices:

#### Dense/general access
- `at_general(r,c)` / `at_general_c(r,c)`  
  Direct row-major access into `vec`.

#### Triangular compressed access
- `at_utri(...)`, `at_ltri(...)`  
  Access values stored for upper/lower triangular matrices using index tables.

#### Diagonal / identity / constant access
- `at_diagonal(...)`  
  Only diagonal stored.
- `at_identity(...)`  
  Synthesizes `1` on diagonal, `0` elsewhere (stores a single value).
- `at_const(...)`  
  Returns the constant value for every position (stores a single value).

#### Symmetric / anti-symmetric compressed access
- `at_symmetric(...)`  
  Stores only one triangle and mirrors access.
- `at_anti_symmetric(...)`  
  Mirrors with sign flip and conjugation logic (via `conjugate`) as needed.

#### Row-iteration bounds helpers
- `start_general/end_general`, plus structure-aware variants (`start_utri/end_utri`, etc.).  
  Used by multiplication and elimination to avoid looping over guaranteed zeros.

---

# 3) Output / formatting / display

### Public API
- `void show() const`  
  Prints matrix elements row-by-row.
- `std::string mat_to_string() const`  
  Formats as a string. Uses `to_string()` for element conversion (works with custom `complex`).

### Helpers
- Relies on scalar `to_string()` overloads (notably `complex`).

---

# 4) Basic algebra: arithmetic, transpose, products, norms, angles

## 4.1 Elementwise operations
- `matrix operator+(const matrix& other) const`  
  Elementwise addition; requires same shape.
- `matrix operator-(const matrix& other) const`  
  Elementwise subtraction; requires same shape.
- `bool operator==(const matrix& other) const`  
  Elementwise tolerance-based equality.

## 4.2 Matrix multiply and related
- `matrix operator*(const matrix& other) const`  
  Matrix multiplication; uses `start/end` to exploit structure.
- `matrix operator/(const matrix& other) const`  
  Defined as `(*this) * other.inverse()`.

## 4.3 Scalar multiply
- `matrix operator*(DataType scalar) const`

## 4.4 Transpose (conjugate-transpose)
- `matrix transpose() const`  
  Returns `conjugate(A(j,i))`.  
  This is **transpose** for real scalars and **Hermitian conjugate** for complex scalars.

## 4.5 Inner product, norm, and angles (matrix-as-vector)
- `DataType dot(const matrix& other) const`  
  Computes Σ `other(i,j) * conjugate(this(i,j))`.
- `long double norm2() const` / `long double length() const`  
  Euclidean norm derived from `dot(*this)`.
- `long double theta(const matrix& other) const`  
  Angle (degrees) between two matrices interpreted as vectors.
- `DataType axpy(DataType alpha, const matrix& y) const`  
  Accumulates Σ `(alpha*this(i,j) + y(i,j))` into a scalar.

---

# 5) Structural classification & compression (performance + semantics)

This section is central to how the library behaves: matrix type tags and compressed storage affect speed and access.

## 5.1 Feature detection (“what kind of matrix is this?”)
### Public API
- `void fill_features(long double check_tol = check_tolerance)`  
  Classifies matrix into types such as:
  - general
  - upper-triangular / lower-triangular
  - diagonal / scalar
  - symmetric / anti-symmetric
  - identity
  - constant

### Helpers
- Type tests used internally here:
  - `is_zero`, `is_identity`, `is_upper_tri`, `is_lower_tri`, `is_symmetric`, `is_anti_symmetric`, `is_scalar`.

## 5.2 Compression/decompression pipeline
### Public API
- `void compress()`  
  Compresses the matrix according to `fill_features()` and swaps in the correct accessor dispatch.
- `matrix decompress() const`  
  Returns a dense general matrix with all entries materialized.

### Helper functions (implementation detail but feature-defining)
- `compress_utri`, `compress_ltri`, `compress_diagonal`, `compress_identity`, `compress_const`, `compress_symmetric`, `compress_anti_symmetric`
- `set_at_ptr()`, `set_start_end_ptr()`, `set_feature()`
- Index-table builders for triangular compression: `row_start` and `pindex` arrays.

---

# 6) Matrix property predicates (“is_*”)

These functions answer “what structure or algebraic property does this matrix have?”

### Public API (structure)
- `bool is_square() const`
- `bool is_zero() const`
- `bool is_identity() const`
- `bool is_upper_tri() const`
- `bool is_lower_tri() const`
- `bool is_diagonal() const`
- `bool is_scalar() const`
- `bool is_symmetric() const`
- `bool is_anti_symmetric() const`

### Public API (geometric / algebraic)
- `bool is_orthogonal()`  
  Checks if `A * A^T` equals identity. If true, sets internal type to orthonormal for optimizations (notably inverse).
- `bool is_orthogonal(const matrix& other) const`  
  Checks “perpendicular” relationship between two matrices treated as vectors/spaces.
- `bool is_parallel(const matrix& other) const`
- `bool is_idempotent() const` (`A^2 = A`)
- `bool is_involutory() const` (`A^2 = I`)
- `bool is_nilpotent() const`
- `bool is_independent() const` (rank == cols)
- `bool is_positive_definite() const`  
  Symmetry requirement + elimination-based positivity test.

### Helpers used by these checks
- `transpose()`, multiplication, `det()`/`rank()`/`gauss_down()`, and tolerance constants (`check_tolerance`, `tolerance`).

---

# 7) Gaussian elimination, RREF, solving systems

This group covers row operations, echelon forms, and solving linear systems.

## 7.1 Core elimination
### Public API
- `matrix gauss_down(std::vector<int>* pivots_indices=nullptr, std::vector<int>* pivots_locations=nullptr)`  
  Produces an upper-triangular-like form and optionally returns pivot metadata.

- `matrix gauss_up(std::vector<int>* pivots_indices=nullptr)`  
  Eliminates upward (used for RREF-style cleanup).

- `matrix rref(std::vector<int>* pivots_indices=nullptr)`  
  Reduced row echelon form.

### Helper functions (pivot management)
- `int is_pivot(int row, int col)`  
  Finds/creates a pivot (may swap rows) for downward elimination.
- `int is_pivot_up(int row, int col)`  
  Pivot check for upward elimination.

These pivot helpers are essential to:
- rank computation
- determinant sign changes
- extracting pivot columns
- solving and null space construction

## 7.2 Substitution helpers
- `void back_sub(int selected_row, matrix& solution_matrix)`  
  Back substitution step.
- `void fwd_sub(int selected_row, matrix& solution_matrix)`  
  Forward substitution step.

## 7.3 Solving
- `matrix solve()`  
  Solves an augmented system `[A|b]` by elimination + back substitution. Returns the solution column.

## 7.4 Rank
- `int rank() const`  
  Computed by counting pivots after elimination.

---

# 8) Determinant, inverse, trace

## 8.1 Determinant
- `DataType det() const`  
  Uses elimination to compute diagonal product of triangular form and adjusts sign for row swaps.

## 8.2 Inverse
- `matrix inverse() const`  
  - Fast path: if matrix is marked orthonormal → inverse is `transpose()`.
  - Otherwise performs a Gauss–Jordan-like method with an identity tracking matrix.

## 8.3 Trace
- `DataType trace() const`  
  Sum of diagonal entries.

---

# 9) The Four Fundamental Subspaces (grouped documentation)

This section groups everything related to:
- **Column space** (image / range)
- **Row space**
- **Null space** (kernel)
- **Left null space** (null space of Aᵀ)

## 9.1 Dimensions
- `int dim() const`  
  Dimension of the column space (rank).
- `int dim_null_cols() const`  
  Nullity = `cols - rank` (dimension of null space).
- `int dim_null_rows() const`  
  `rows - rank` (dimension of left null space in typical theory; this library exposes it directly).

## 9.2 Bases for the row/column spaces
- `matrix basis_cols() const`  
  Returns a matrix whose columns are the **pivot columns of the original matrix**, forming a basis for the **column space**.

- `matrix basis_rows() const`  
  Returns pivot rows from the RREF process, forming a basis for the **row space**.

### Helpers used
- `gauss_down()`, `rref()`, pivot index tracking via `pivots_indices`.

## 9.3 Null space and left null space
- `matrix null_cols() const`  
  Constructs a basis for the **null space** `N(A)` by:
  - computing RREF
  - identifying pivot and free variables
  - building special solution vectors

- `matrix null_rows(matrix* elementary=nullptr) const`  
  Computes a basis for the **left null space** `N(A^T)` (or, equivalently, null space of the row space).
  Optionally returns an “elementary” transformation matrix used during elimination steps.

### Pivot-ordering helper
- `matrix fix_pivots() const`  
  Attempts to reorder pivot rows to the top (stabilizes later basis/null computations that assume pivot rows appear first).

## 9.4 Independence / basis validation
- `bool is_independent() const`  
  True if rank equals number of columns.
- `bool is_basis(int dimension) const`  
  Checks whether the matrix columns form a basis for a specified dimension (size + independence).

---

# 10) Projections and least squares (applied linear algebra)

## 10.1 Projection matrix
- `matrix projection() const`  
  Returns the projection matrix onto the column space of `A`:
  \[
    P = A(A^T A)^{-1}A^T
  \]
  Marks the result as symmetric.

## 10.2 Least squares fitting
- `matrix fit_least_squares(const matrix& output) const`  
  Solves least squares with normal equations:
  \[
    (A^T A) x = A^T b
  \]
  Internally builds an augmented system and calls `solve()`.

### Helpers used
- `transpose()`, `operator*`, `append_cols()`, `solve()`, `inverse()`.

---

# 11) Matrix decompositions (LU, QR, Gram–Schmidt)

## 11.1 LU factorization (with permutation)
- `void lu_fact(matrix& lower, matrix& permutation, matrix& upper) const`  
  Produces `L`, `P`, `U` (with row-swap tracking).  
  For already upper-triangular matrices it can shortcut.

### Helpers
- Elimination-like steps and row swap bookkeeping.

## 11.2 Gram–Schmidt orthonormalization
- `matrix gram_shmidt() const`  
  Orthonormalizes columns. Marks output as orthonormal.

### Helpers used
- `projection()`, `dot()`, `col_length()`, `filter()` (numerical cleanup behavior).

## 11.3 QR factorization
- `void qr_fact(matrix& q, matrix& r) const`  
  Uses Gram–Schmidt to compute:
  - `Q` (orthonormal columns)
  - `R = Q^T A` (upper-triangular)

---

# 12) Spectral methods: eigenvalues, eigenvectors, matrix powers

## 12.1 Eigenvalues (iterative QR)
- `matrix eigen_values(int max_iteration, long double min_diff = check_tolerance) const`  
  Uses repeated QR steps (`A ← RQ`) until convergence/triangularity.  
  Returns a diagonal matrix containing the eigenvalues (approximate).

## 12.2 Eigenvectors (via null spaces)
- `matrix eigen_vectors(const matrix& eigen_values) const`  
  For each eigenvalue λ, forms `(A - λI)` and computes its null space; those vectors are eigenvectors.

### Helpers used
- `null_cols()`, `inverse()` (indirectly in other steps), `compress()/fill_features()` may optimize.

## 12.3 Matrix power
- `matrix operator^(long double power) const`  
  Forms something equivalent to:
  \[
    A^p = S \Lambda^p S^{-1}
  \]
  using eigenvalues/eigenvectors and inversion.

---

# 13) Singular Value Decomposition (SVD)

- `void svd(matrix& u, matrix& s, matrix& vt) const`  
  Computes an SVD-like factorization by:
  - building `A A^T`
  - taking eigenvalues/eigenvectors
  - converting eigenvalues → singular values by sqrt
  - normalizing vectors

Produces:
- `U` (orthonormal)
- `S` (diagonal)
- `V^T` (orthonormal)

### Helpers
- `eigen_values()`, `eigen_vectors()`, `col_length()`, `transpose()`, `filter()`.

---

# 14) FFT / Fourier operations (complex-friendly)

This library includes a recursive FFT implementation operating column-wise on matrices.

## 14.1 Fourier helpers (free functions)
- `fourier_diagonal(dimension, n)`  
  Generates twiddle-factor-like values (powers of primitive root) used in combining FFT halves.
- `fourier_mat(dimension)`  
  Generates a Fourier-like matrix.

## 14.2 Matrix permutation and splitting (FFT helpers)
- `matrix arrange(const std::vector<int>& seq) const`  
  Reorders rows according to `seq` (permutation).
- `matrix split(bool half) const`  
  Returns the top half (`half=false`) or bottom half (`half=true`) of the rows (as implemented).

## 14.3 FFT core
- `matrix fft_col(int dimension) const`  
  Recursive Cooley–Tukey style FFT for a column:
  - reorder even/odd indices
  - recursively FFT halves
  - combine using Fourier diagonal twiddle factors

- `matrix fft() const`  
  Applies `fft_col` to each column.  
  If number of rows is not a power of 2, it pads rows to the next power of 2 with zeros before transforming.

### Helpers
- `resize()` (for padding), `extract_col()`, `append_cols()` (reconstruct result), `complex` arithmetic + `conjugate`.

---

# 15) Matrix building blocks & utilities

## 15.1 Concatenation and extraction
- `matrix append_cols(const matrix& other) const`  
  Horizontal concatenation.
- `matrix append_rows(const matrix& other) const`  
  Vertical concatenation.
- `matrix extract_col(int index) const`  
  Returns column `index` as a `(rows×1)` matrix.

## 15.2 Resizing and filtering
- `matrix resize(int wanted_rows, int wanted_cols, DataType padding_value=0) const`  
  Resize with padding value.
- `matrix filter(long double filter_tolerance = check_tolerance) const`  
  Sets very small entries to zero (numerical cleanup).

## 15.3 Column metrics
- `long double col_length(int col_i) const`  
  Euclidean length of a column.

## 15.4 Transformation inference
- `matrix transformer(const matrix& output) const`  
  Attempts to compute a transformation matrix `T` such that:
  \[
    T \cdot (\text{this}) = output
  \]
  Typically assumes some independence / solvability; internally relies on elimination and solving logic.

---

# 16) Internal helper functions (important for understanding behavior)

These are not “headline” algorithms, but they are core to correctness/performance and are used throughout:

## 16.1 Pivoting helpers
- `is_pivot()`, `is_pivot_up()`  
  Used by elimination, rank, determinant, inverse, null space.

## 16.2 Dispatch configuration helpers (compression support)
- `set_feature()`, `set_at_ptr()`, `set_start_end_ptr()`  
  Configure which access/iteration functions are used depending on matrix structure.

## 16.3 Compressed storage builders
- `compress_utri/ltri/diagonal/symmetric/anti_symmetric/identity/const`  
  Convert dense storage to minimal storage and generate indexing metadata (triangular).

## 16.4 Specialized `at_*`, `start_*`, `end_*`
- Provide correct synthesized values for:
  - implicit zeros
  - mirrored values (symmetric)
  - mirrored sign-flipped values (anti-symmetric)
  - identity/constant implicit fill

These helpers are the reason that high-level algorithms (multiply, elimination, etc.) can be written mostly without hardcoding the matrix type.
