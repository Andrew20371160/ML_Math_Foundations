## 0) High-level overview / library layout

This library implements a **binary expression tree** that can:

- Parse a math expression string into a binary tree (`calculus_tree`)
- Evaluate the expression numerically (`evaluate_at`)
- Differentiate symbolically (`diff_with`, plus vector calculus helpers)
- Simplify numeric/algebraic subtrees (`simplify`)
- Substitute variables with other expressions (`exchange`)
- Approximate definite integrals via Simpson’s rules (`simpson_rule_1_3`, `simpson_rule_3_8`)
- Generate tables of values for variables stepping across ranges (`table`)
- Heuristically test equivalence by random sampling (`random_equivalence`)

Supporting components:

- **Preprocessor**: validates and normalizes expression strings before parsing; inserts implicit `*` where needed.
- **Node**: internal tree node with linking/rewiring helpers.
- **Known functions/constants**: keyword detection and evaluation of built-in functions/constants/operators.

---

## 1) “Known functions & constants” section (`functions_and_known_constants.*`)

### Purpose
Defines:
- What tokens count as operators and brackets
- How to recognize keywords (known functions / constants)
- How to evaluate:
  - functions like `sin`, `log`, `sqrt`, etc.
  - constants like `pi`, `e`, and special placeholders `inf`, `nan`
  - operators `+ - * / ^`

Also provides an optional **COMPLEX_MODE** (compile-time macro) that adds:
- constant `i`
- function `img(...)`

### Compile-time mode: normal vs complex

In `functions_and_known_constants.h`:

- Complex mode is enabled by uncommenting:
  ```cpp
  //#define COMPLEX_MODE 1
  ```

**Normal mode:**
- `functions_count = 20`
- `constants_count = 4`
- constants: `pi`, `e`, `inf`, `nan`
- functions include: `log`, `sqrt`, `abs`, trig, inverse trig, hyperbolic, inverse hyperbolic

**Complex mode adds:**
- `IMG` function (`img(x)` behaves as `i*x` in evaluation)
- constant `i`

### Enums / arrays

#### `functions_codes` enum
Includes (at least) codes:
`LOG, SQRT, ABS, SIN, COS, TAN, SEC, CSC, COTAN, ASIN, ACOS, ATAN, EXP, LN, SINH, COSH, TANH, ASINH, ACOSH, ATANH` and in complex mode also `IMG`.

#### `constants_codes` enum
Normal: `PI, E, INF_ERR, NAN_ERR`
Complex: `PI, E, I, INF_ERR, NAN_ERR`

#### `known_functions[]`
- `"log"` is special: detected by prefix `log` and can include base digits (e.g. `log2`).
- Others are exact matches like `"sin"`, `"cos"`, `"sqrt"`, etc.

#### `known_constants[]`
Normal: `"pi"`, `"e"`, `"inf"`, `"nan"`
Complex adds `"i"`.

---

### Helper: `bool is_op(const std::string& expression, unsigned int pos)`
**File:** `functions_and_known_constants.cpp`

**What it does**
Returns `true` if `expression[pos]` is one of:
`+ - * / ^ ( )`.

Used throughout tokenizer, preprocessor, and parser.

---

### Helper: `bool is_num(const std::string& var)`
**File:** `functions_and_known_constants.cpp`

**What it does**
Validates that a string is a number consisting of:
- digits
- optional leading `+` or `-` (signs are only accepted *before* digits)
- at most one dot `.`

If a sign appears after digits have started (`found_digit == true`), it returns false.

---

### Helper: `int is_known_function(const std::string& expression, unsigned int start)`
**File:** `functions_and_known_constants.cpp`

**What it does**
- Extracts a contiguous token from `start` until an operator char (as per `is_op`) is found.
- If token begins with `"log"` it returns `LOG` immediately (even if `log2`, etc.).
- Otherwise compares token against `known_functions[1..functions_count-1]` and returns its index (which matches the enum code layout).

Returns `-1` if not a known function.

---

### Helper: `int is_known_constant(const std::string& expression, unsigned int pos)`
**File:** `functions_and_known_constants.cpp`

**What it does**
- Extracts token from `pos` until next operator
- Compares against `known_constants[]`
- Returns the constant enum index, else `-1`

---

### Helper: `int is_keyword(const std::string& expression, unsigned int pos)`
**File:** `functions_and_known_constants.cpp`

**What it does**
Attempts to recognize either:
- a known constant, else
- a known function

Returns the function/constant code (as returned by `is_known_constant` or `is_known_function`) or `-1`.

Note: implementation extracts `temp` but then calls `is_known_constant(expression,pos)` / `is_known_function(expression,pos)` using `pos` (not the extracted temp). This is exactly what the code does.

---

### Template: `DataType evaluate_function(int fn, DataType var, DataType base)`
**File:** `functions_and_known_constants.tpp`

**What it does**
Switches on `fn` and returns the corresponding math function result.
Notable cases:
- `LOG`: returns `log(var)/log(base)`
- `SEC/CSC/COTAN`: computed via reciprocal of trig functions
- Complex mode `IMG`: returns `complex<long double>(0,1) * var`

**Notes from code**
- No `default:` case; caller assumes `fn` is valid.

---

### Template: `DataType evaluate_constant(const std::string& symbol)`
**File:** `functions_and_known_constants.tpp`

**What it does**
Uses `is_known_constant(symbol,0)` to decide:
- `PI`: `M_PI`
- `E`: `exp(1)`
- `INF_ERR`: `tan(M_PI/2)`
- `NAN_ERR`: `tan(M_PI/2) / tan(M_PI/2)`
- Complex `I`: `DataType(0,1)`

---

### Template: `DataType evaluate_operator(char op, const DataType& left, const DataType& right)`
**File:** `functions_and_known_constants.tpp`

**What it does**
Computes:
- `+ - * /` with native operators
- `^` uses `pow(left,right)`

---

## 2) Preprocessor section (`preprocessor.*`)

### Purpose
`preprocessor` takes a raw expression string and produces a **normalized expression** that is safer for the parser:

- Removes spaces (by skipping them during extraction)
- Tokenizes into operands/operators/brackets/functions
- Validates token sequences using rules documented in comments
- Inserts implicit multiplication `*` in cases like:
  - `2x` -> `2*x`
  - `x(y)` -> `x*(y)`
  - `(x+y)(z+5)` -> `(x+y)*(z+5)`
  - `xsin(x)` -> `x*sin(x)` (function token after variable/close bracket)

If it finds an error, it prints an error message to `cout` and returns `"0"`.

### Token types (enum)
Defined in `preprocessor.h`:

`ERROR=-1, VAR_CONST, FUNCTION, OPERATOR, OPEN_BRACKET, CLOSE_BRACKET`

Operators are treated as: `+ - * / ^` and parentheses via `is_op` check (which includes `(` and `)`).

---

### `preprocessor::preprocessor()`
Empty constructor.

---

### Helper: `void skip_spaces(const std::string& expression, unsigned int& start) const`
Advances `start` while the current character is `' '`.

---

### Helper: `std::string preprocess_extract(const std::string& expression, unsigned int& start) const`
Extracts the next token starting at `start`, updating `start` to the next unread position (and skipping spaces).

Token extraction rules:
- If current char is an operator (`is_op`), token is that 1 char.
- Else if it starts with a digit:
  - consumes consecutive digits
  - optionally consumes a single `.` if present
  - consumes more digits
- Else (variable/function/keyword-like):
  - consumes until space or operator

This function does **not** itself validate correctness; it just extracts.

---

### Helper: `unsigned int token_type(const std::string& token, int& open_brackets_c) const`
Classifies a token and updates `open_brackets_c`:
- If token is operator-like:
  - `'('` => increments open count, returns `OPEN_BRACKET`
  - `')'` => decrements open count, returns `CLOSE_BRACKET`
  - else => `OPERATOR`
- Else if `is_known_function(token, temp_start) != -1` => `FUNCTION`
- Else if token is `"."` => `ERROR`
- Else if token starts with digit:
  - if `is_num(token)` => `VAR_CONST`
  - else => `ERROR`
- Else => `VAR_CONST`

---

### Validation helpers (they can also mutate `ret_exp` by inserting `"*"`)

All in `preprocessor.cpp`:

#### `bool valid_var_const_token(unsigned int prev, const std::string& token, std::string& ret_exp) const`
Rules implemented:
- If previous token was `VAR_CONST`: append `"*"` (implicit multiplication).
- If previous token was `FUNCTION` or `CLOSE_BRACKET`: error (prints message) and returns false.

(Despite the printed message mentioning `'('`, the actual check is `previous_token == CLOSE_BRACKET`.)

#### `bool valid_function_token(unsigned int prev, const std::string& token, std::string& ret_exp) const`
Rules:
- If previous is `VAR_CONST` or `CLOSE_BRACKET`, insert `"*"` (implicit multiplication).
- If previous is `FUNCTION`, it’s invalid.

#### `bool valid_open_bracket_token(unsigned int prev, const std::string& token, std::string& ret_exp) const`
Rules:
- If previous is `VAR_CONST` or `CLOSE_BRACKET`, insert `"*"`.

#### `bool valid_close_bracket_token(unsigned int prev, const std::string& token, std::string& ret_exp) const`
Rule:
- If `prev > VAR_CONST && prev < CLOSE_BRACKET`, it’s invalid (meaning you cannot close after FUNCTION / OPERATOR / OPEN_BRACKET).

#### `bool valid_operator_token(unsigned int prev, const std::string& token, std::string& ret_exp) const`
Rules:
- If previous is `FUNCTION`: invalid (function must be followed by parentheses content)
- If previous is `OPEN_BRACKET`: only `+` or `-` are allowed
- If previous is `OPERATOR`: only `+` or `-` are allowed

---

### Public API: `std::string prepare_exp(const std::string& exp) const`
Main preprocessing pipeline.

**Algorithm (as implemented):**
1. Extract first token.
2. Determine its type and reject invalid starting tokens (errors include starting with close bracket or certain operators).
3. Append first token to `ret_exp`.
4. Loop extracting subsequent tokens while `i < expression.length()` and `open_brackets_c >= 0`:
   - classify current token and update bracket count
   - validate token sequence based on previous token type; may insert `*`
   - on error: prints error position `(i - temp.size())` and returns `"0"`
   - append token to `ret_exp`
5. After loop: if expression ends with FUNCTION/OPERATOR/OPEN_BRACKET (checked by `current_token>VAR_CONST && current_token<CLOSE_BRACKET`) it errors and returns `"0"`.
6. Returns `ret_exp`.

---

## 3) Node section (`node.*`)

### Purpose
`node` is the internal tree structure (not really a public user API). It stores:
- `symbol` (string): operand (`"3.14"`, `"x"`) or operator (`"+"`, `"*"`, `"^"`) or function name token (`"sin"`, `"log2"`, etc.)
- pointers: `parent`, `left`, `right`

`calculus_tree<DataType>` is declared as a friend, so it can access and manipulate these internals.

### `node::node(const std::string& symbol)`
Initializes:
- `this->symbol = symbol`
- parent/left/right set to `NULL`

---

### Linking helpers (private)

These are used heavily in parsing and in calculus_tree operator overloads.

#### `bool disconnect_self()`
If node has a parent:
- clears the parent’s pointer to this (`left` or `right`)
- sets `parent = NULL`
- does **not** touch children pointers

Returns `true` if it disconnected, else `false`.

---

#### `bool append_next(const std::string& symbol)`
Adds a new node (with `symbol`) as the **right child** of this node’s parent **only if**:
- `parent != NULL`
- `parent->right == NULL`

Returns `true` on success.

---

#### `bool append_child(const std::string& symbol)`
Adds a new node as a child of `this`:
- if `left` is empty: fill `left`
- else if `right` is empty: fill `right`
- else fails

Sets new child’s parent to `this`.

---

#### `bool append_parent(const std::string& symbol)`
Creates a new node `new_parent(symbol)` and makes it the parent of `this`:
- disconnects this from its existing parent (if any)
- sets `this->parent = new_parent`
- sets `new_parent->left = this`

Returns `true` if allocation succeeded.

---

### Subtree versions (overloads taking `node*&`)

#### `bool append_next(node*& src_root)`
Attaches `src_root` as the **right child** of this node’s parent if possible:
- requires `parent != NULL` and `parent->right == NULL` and `src_root != NULL`
- disconnects `src_root` from its current parent (`disconnect_self`)
- sets `src_root->parent = parent`
- sets `parent->right = src_root`

---

#### `bool append_child(node*& src_root)`
Attaches `src_root` as a child of this node (left then right), after disconnecting it from its old parent.

---

#### `bool append_parent(node*& src_root)`
Reparents `this` under `src_root`:
- if `src_root == NULL`: just disconnects this from its parent
- else if `src_root` has room for a child: disconnects `this`, then appends it as a child of `src_root`
- always returns `true` in the implementation (even if no room branch happens, though it’s guarded by `else if`)

---

#### `bool exchange_parent(const std::string& op)`
Core tree-restructuring used by the parser for precedence handling.

Behavior:
- If `this->parent == NULL`: it calls `append_parent(op)` and returns that result.
- Else:
  - creates a new node `new_parent(op)`
  - remembers `gparent = parent`
  - disconnects `this` from its current parent
  - attaches `new_parent` as a child of `gparent`
  - attaches `this` as a child of `new_parent`
  - returns true if allocations and attachments succeed

This matches the comment diagram in `node.h`.

---

## 4) Expression tree building / parsing section (`calculus_tree` internals)

### The `calculus_tree<DataType>` class (public user type)

**File:** `calculus_tree.h` (decl), `calculus_tree.cpp` (impl)

Internal state:
- `preprocessor processor;`
- `node* root;`

Construction typically does:
1. `processor.prepare_exp(expression)`
2. `create_tree(processed_expression, start)`

---

### Token extraction used by parser: `std::string extract(const std::string&, unsigned int& start)`
**Different from preprocessor’s extractor.**
This one assumes the expression is already normalized (no spaces) and extracts:
- 1-char operators (including parentheses)
- or a substring until an operator is met

---

### Operator precedence: `int precedence(const std::string& expression, unsigned int pos)`
Returns precedence based on `expression[pos]`:
- `^` => 3
- `*` or `/` => 2
- everything else => 1 (covers `+` and `-`)

---

### Parsing entry points

#### `node* create_tree(const std::string& expression, unsigned int& start)`
Parses a full expression into a tree using `parse_block()` and operator precedence logic via `var_op_func()`.

High-level behavior:
- Parse first block into `ret_root`
- If there’s more, it creates an operator parent node for `ret_root` using the next operator token
- Then it loops:
  - parse next block
  - if operator follows, call `var_op_func` to insert it properly
  - else just attach block as child and return

---

#### `node* parse_block(const std::string& expression, unsigned int& start)`
Parses one “block” which may be:
- a **function call** (keyword function token) => `parse_function`
- a parenthesized subexpression => `parse_parenthese`
- a variable/constant/number token => node with that symbol
- a signed block at expression start or after an operator:
  - handles unary `+` or `-` by converting it to multiplication by `+1` or `-1`:
    - creates `*` node
    - left child is `+1` or `-1`
    - right child is parsed block

Unary sign handling is only attempted when:
- current char is `+` or `-`
- and either `start == 0` or `is_op(expression, start-1)` is true

---

#### `node* parse_parenthese(const std::string& expression, unsigned int& start)`
Parses from `'('` until the matching `')'` and returns the subtree root.

Important details:
- increments `start` at entry to skip `'('`
- loops until it finds `')'`
- recursively calls itself when encountering nested `'('`
- uses `var_op_func` for operator insertion/precedence handling
- when it sees `')'`, it increments `start` to skip it and returns the built subtree root

---

#### `node* parse_function(const std::string& expression, unsigned int& start)`
Assumes current position is at a function token.
- Extracts the function token name into `var`
- Parses the following parentheses with `parse_parenthese`
- Makes the function token node the parent of that subtree:
  - `ret_root->append_parent(var)`
  - returns the new parent

In this design, a function node is identified later by:
- `ptr->left != NULL && ptr->right == NULL`

---

### Helper: `void var_op_func(const std::string& op, node*& var, node*& last_op, node*& ret_root)`
This is the core operator-precedence insertion routine used by `create_tree` and `parse_parenthese`.

Behavior summary:
- Computes precedence difference between the new operator and `last_op->symbol`.
- If `op` has higher precedence **or** `op == "^"`:
  - creates a new operator node `temp(op)`
  - attaches `var` as child of `temp`
  - attaches `temp` as child of `last_op`
  - moves `last_op` to the new operator node (right child)
- Else:
  - first attach `var` as child of `last_op`
  - walk upward while parent precedence is >= new op precedence
  - call `last_op->exchange_parent(op)` to insert the new operator above
  - adjust `ret_root` if the inserted operator becomes the new root
  - set `last_op = last_op->parent`

---

## 5) Public user API section (`calculus_tree<DataType>`)

### Constructors / destructor

#### `calculus_tree()`
Initializes `root = NULL`.

#### `calculus_tree(const std::string& expression)`
- `start = 0`
- `root = create_tree(processor.prepare_exp(expression), start)`

#### Copy constructor `calculus_tree(const calculus_tree&)`
Deep-copies the tree via `copy_tree`.

#### Destructor `~calculus_tree()`
Deletes the full tree via `remove_node(root)` and sets root to null.

---

### Assignment

#### `calculus_tree& operator=(const calculus_tree&)`
Clears current tree, deep-copies from source.

#### `template<typename input_datatype> calculus_tree& operator=(const input_datatype&)`
Clears current tree, then calls `set_exp(to_string(data))`.

Note: there is a `to_string(const string&)` overload defined in `calculus_tree.cpp` returning the input unchanged. For other types, it relies on the standard `to_string` overloads.

---

### Arithmetic operators (tree composition)
All build a new tree by copying operands and inserting a new operator node.

#### `operator+`, `operator-`, `operator*`, `operator/`, `operator^`

Notable edge behaviors (as coded):
- `*` returns `"0"` if either side is empty.
- `/` returns:
  - `"inf"` if left exists and right empty
  - `"0"` if left empty and right exists
  - `"nan"` if both empty
- `^` has special behavior if one side missing and the other is a numeric leaf (uses `stold(root->symbol)` and `threshold` comparisons) to decide `"1"`, `"0"`, or `"nan"`.

---

### Expression management

#### `void set_exp(const std::string& expression)`
Deletes existing tree (if any) then rebuilds from processed expression.

#### `bool remove_tree()`
Deletes entire tree (if root exists).

---

### Printing / serialization

#### `void print(node* ptr = NULL) const`
Prints expression (in-order) to `cout`, surrounding child expressions with parentheses.

For function nodes (`is_function_tree(ptr)` true):
- prints function name before parentheses.

#### `std::string expression(node* ptr = NULL) const`
Returns the expression string in the same style as `print`, but as a string.

#### `bool save(const std::string& file_path)`
Writes expression form to a file via `save_tour`.

#### `bool load(const std::string& filePath)`
Reads all lines of file into one expression string, preprocesses it, builds the tree.

---

## 6) Evaluation section

### `DataType evaluate_at(std::vector<std::string> vars_equal = {""})`
Evaluates the expression.

- If `root` exists and `prepare_variables_and_values(vars_equal)` succeeds:
  - returns `evaluate(root, vars_equal)`
- Else returns `0`

The variable/value vector format expected:
`{"x","3.5","y","2.0"}` etc.

---

### Helper: `bool prepare_variables_and_values(vector<string>&) const`
Validates and normalizes the `variables_and_values` vector:

- Vector must have even size.
- For every element:
  - if any entry matches a known function name => error
  - at even indices (variable names):
    - cannot be a number
    - cannot be a known constant token
  - at odd indices (values):
    - must be a number OR a known constant
    - if it’s a known constant, it replaces it with its numeric string value

Returns 1 on success, 0 on error.

---

### Helper: `DataType evaluate(node* ptr, const vector<string>& vars_and_values) const`
Post-order evaluation:

- Recursively compute left and right operands if children exist.
- If leaf:
  - if numeric string => `stold` to DataType
  - else if vars provided => lookup by name and return its value
  - else treat as known constant => `evaluate_constant<DataType>(symbol)`
- Else if operator node (`is_op_tree(ptr)`):
  - evaluate operator via `evaluate_operator`
- Else treat as function node:
  - detect fn code via `is_known_function(ptr->symbol, temp_start)`
  - `log` supports base encoded as suffix digits after `"log"`; default base `10`
  - call `evaluate_function`

---

## 7) Differentiation section

### Public: `calculus_tree diff_with(const std::string& variable)`
Returns a new `calculus_tree` built from the derivative expression string.

Checks:
- variable must be non-empty
- variable must not be a number
- variable must not be an operator token
- variable must not be a keyword (`is_keyword(variable,0) == -1`)

If invalid: prints an error and returns tree `"nan"`.

If valid:
- computes derivative expression string via `diff(root, variable)`
- builds a tree from that string
- calls `simplify()` on the result

---

### Core: `std::string diff(node* ptr, const std::string& var)`
Post-order symbolic differentiation:

- If operator node: `diff_op`
- Else if function node: `diff_function`
- Else leaf:
  - returns `"1"` if leaf symbol equals differentiation var
  - else `"0"`

---

### `diff_op(node* ptr, const std::string& var)`
Handles operators:
- `+` / `-`: `diff_plus_minus`
- `*`: `diff_mult`
- `/`: `diff_div`
- `^`: two cases:
  1) Exponent is numeric leaf:
     - exponent==0 => `"0"`
     - exponent==1 => derivative of base
     - else => `exponent * base' * base^(exponent-1)` (built using `simplify_mult`)
  2) General exponent:
     Implements:
     - `f = left^right`
     - `f' = left^right * ( right*(left'/left) + ln(left)*right' )`
     (built by string composition + simplify helpers)

---

### Function differentiation: `diff_function(int fn, node* ptr, const std::string& var)`
Implements derivatives for:
- `exp`, `ln`, `sin`, `cos`, `tan`, `sec`, `csc`, `cotan`, `sqrt`, `abs` (assumes `>0` per comment), `log` with base handling, `asin`, `acos`, `atan`, `sinh`, `cosh`, `tanh`, `asinh`, `acosh`, `atanh`
- complex mode: `img(f(x)) -> img(f'(x))`

All are built as strings using `expression()` for inner expressions and the `simplify_*` string helpers.

If the inner subtree is constant (and not the differentiation variable), it returns `"0"`.

---

### String simplification helpers used during differentiation:
- `simplify_add(v1, v2)`
- `simplify_sub(v1, v2)`
- `simplify_mult(v1, v2)`
- `simplify_div(v1, v2)`

These implement lightweight simplifications like:
- multiplying by zero -> `"0"`
- dividing zero by zero -> `"nan"`
- division by zero -> `"inf"`
- additions with zero -> return other side
…and always wrap combined expressions in parentheses.

---

## 8)higher-level calculus utilities

### Independent variables

#### `std::vector<std::string> independent_variables()`
Returns a sorted list (because it uses `std::set`) of variable symbols found in leaf nodes that are:
- not numbers
- not keywords (functions/constants)

Uses `independent_variables_tour`.

#### `void independent_variables_tour(node* ptr, std::set<std::string>& ret_set)`
Recursive traversal adding eligible variables.

---

### Gradient / divergence / curl / Laplacian

#### `std::vector<calculus_tree<DataType>> gradient(const vector<string>& ind_vars)`
Builds vector field where each component is `d/d(ind_vars[i]) (this tree)`.

Implementation builds each component using `create_tree(diff(root, var), start)`.

#### `calculus_tree divergence(const vector<calculus_tree<DataType>>& gradient_field, const vector<string>& independent_variables)`
Builds scalar tree representing:
`d(fx)/dx + d(fy)/dy + ...`

It concatenates derivative strings with `+`, then parses.

#### `std::vector<calculus_tree<DataType>> curl(const vector<calculus_tree<DataType>>& gradient_field, const vector<string>& independent_variables)`
Only defined for `gradient_field.size() == 3`.
Returns:
- component 0: `d(fz)/dy - d(fy)/dz`
- component 1: `-1 * ( d(fz)/dx - d(fx)/dz )`
- component 2: `d(fy)/dx - d(fx)/dy`

It uses the string simplify helpers and parses the resulting strings.

#### `calculus_tree laplacian(const vector<string>& ind_vars)`
Computes:
`f_xx + f_yy + f_zz + ...` over the provided variable list
by:
- `gradient(ind_vars)`
- then `diff(gradient_field[i].root, ind_vars[i])` for each component
- sums them as a string and parses

---

### Simplification (numeric + structural)

#### `void simplify()`
Calls `simplify_tree(root)`.

#### `std::string simplify_tree(node* ptr)`
Performs a post-order simplification pass:
- If operator node: tries `simplify_tree_add/sub/mult/div/power`
- If function node and child simplifies to a numeric string: it evaluates the function and replaces the subtree with its numeric value node
- If constant leaf: returns numeric value string for known constants

When it simplifies, it actually **rewrites the tree**:
- removes the old subtree with `remove_node(ptr)`
- appends a new constant child to the original parent, or replaces root

#### Operator simplifiers:
- `simplify_tree_add(v1,v2)`
- `simplify_tree_sub(v1,v2)`
- `simplify_tree_mult(v1,v2)`
- `simplify_tree_div(v1,v2)`
- `simplify_tree_power(v1,v2)`

These do numeric folding when both operands are numbers and also apply algebraic rules like:
- `x + 0 => x`
- `x - x => 0`
- `x * 0 => 0`
- `x / x => 1`
- `x ^ 1 => x`
- `x ^ 0 => 1`
- and propagate `"inf"` in various cases (as coded)

Threshold comparisons use the global `threshold = 10e-6`.

---

### Substitution (“exchange variable”)

#### `calculus_tree exchange(const string& old_variable, const string& new_var) const`
Returns a new tree where all leaves equal to `old_variable` are replaced by the parsed tree of `new_var`.

Only performs substitution if `old_variable` is:
- not a keyword (`is_keyword(var,0) == -1`)
- not a number

#### Helper: `void exchange_variable_tour(node* ptr, const string& old_var, const calculus_tree<DataType>& new_var)`
Traverses the tree; when a matching leaf is found:
- if leaf has a parent: replaces the correct child pointer with `copy_tree(new_var.root)`
- if leaf is root: replaces root entirely with `copy_tree(new_var.root)`

---

## 9) Numerical methods

### Simpson’s rule evaluation support (vectorized traversal)

#### Helper: `vector<DataType> simpson_rule_tour(...) const`
Evaluates the expression for a *range* of x values starting at `x_val` with step `step_size`.

- Returns a `vector<DataType>` of length `fx_size`
- Leaf handling:
  - If symbol matches integration variable: generates `x_val + step_size*i`
  - If other variable: uses `variables_and_values` lookup
  - Else constant/number as constant vector
- Operator nodes: elementwise compute across arrays
- Function nodes: apply function elementwise

This is used by both Simpson’s 1/3 and 3/8.

---

### `DataType simpson_rule_1_3(...) const`
Computes definite integral using Simpson’s 1/3 rule.

Key behaviors from code:
- Checks: `variable.size()`, `sub_intervals_count>0`, `beg<end`, `traversal_array_size<=sub_intervals_count`
- If `traversal_array_size==0`, it sets it to `sub_intervals_count` and keeps dividing by 2 until <= 1000
- Splits the evaluation into `traversal_count = sub_intervals_count / traversal_array_size` chunks
- Builds sums with weights 4 and 2 according to Simpson’s 1/3 pattern
- Returns `(step_size/3)*value`

---

### `DataType simpson_rule_3_8(...) const`
Computes integral using Simpson’s 3/8 rule.

Differences:
- Auto `traversal_array_size` reduces by dividing by 3 until <= 1000
- Weights: if `j % 3 == 0` then weight 2 else weight 3 (except endpoints)
- Returns `((3/8)*step_size)*value`

---

### Table generation

#### Helper: `vector<DataType> table_tour(...) const`
Similar to `simpson_rule_tour`, but:
- instead of one special integration variable, it checks all provided variables
- if a leaf matches a variable, it increments it using `step_size[index]*i`
- otherwise constants are broadcast

#### Public: `vector<DataType> table(vector<string>& variables_and_values, unsigned int fx_size, const vector<DataType>& step_size, unsigned int traversal_array_size=0) const`
Generates `fx_size` evaluations while stepping the variables.

Behavior:
- Validates:
  - `variables_and_values.size() != 0`
  - `fx_size > 0`
  - `step_size.size() == variables_and_values.size()/2`
- Auto sets `traversal_array_size` down to <= 1000 by dividing by 2
- Computes full chunks and a remainder (`extra_round_count`)
- Updates variable values between chunks by adding `traversal_array_size*step_size[j]*(i+1)` to each initial value (as strings via `to_string(stold(...)+...)`)

Returns empty vector on invalid input.

---

### Random equivalence testing

#### `bool random_equivalence(calculus_tree& other_tree, unsigned int trial_count)`
Heuristic equivalence by random evaluation:

- Gets independent vars for both trees and checks:
  - same count
  - same variable names in the same order (after set-based extraction)
- Builds a `vars_and_values` vector of size `2*n` (var name then random value)
- Uses `rand()` seeded with `srand(time(0))`
- Generates random values scaled by `M_PI/e`
- Runs approximately `trial_count/2` iterations, each iteration tests:
  1) random positive samples
  2) random negative samples
- Compares `abs(evaluate_at(...) - other.evaluate_at(...)) > threshold`

If no independent variables exist, it just compares constants: `abs(evaluate_at()-other.evaluate_at()) <= threshold`.

---

## 10) Misc helpers / global utilities

### `const long double threshold = 10e-6;`
Used for “close enough to zero/one” checks and equivalence comparisons.

### `template<typename T> std::ostream& operator<<(std::ostream&, const std::vector<T>& vec)`
Defined in `calculus_tree.cpp` (declared in `calculus_tree.h`).
Prints:
- `<elem0,\n\n elem1,\n\n ...>` style (note the two newlines after commas).

---
