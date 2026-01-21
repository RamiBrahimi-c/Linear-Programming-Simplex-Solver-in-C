# Linear Programming Simplex Solver

A robust, terminal-based Linear Programming (LP) solver implemented in C. It uses the **Simplex Method** and the **Two-Phase Simplex Method** to find optimal solutions for maximization and minimization problems.

To ensure high precision and avoid the rounding errors typically associated with floating-point math, this solver utilizes a custom **Fractional Arithmetic** engine.

## Features

- **Exact Precision**: Performs all calculations using fractions (numerator/denominator) with automatic simplification (GCD).
- **Two-Phase Method**: Automatically handles constraints requiring artificial variables ($\ge$ and $=$).
- **Standard Simplex**: Efficiently solves problems with only $\le$ constraints.
- **LP File Parser**: Supports reading problem definitions from simple `.lp` text files.
- **Visual Tableaus**: Prints formatted Simplex tables (iterations) to the console.
- **Cross-Platform Colors**: Color-coded terminal output for Linux, macOS, and Windows.

---

## Project Structure

- `src/`: Implementation files (`.c`).
- `include/`: Header files (`.h`).
- `bin/`: Compiled binaries (generated).
- `example.lp`: Sample input file.

---

## Compilation

You can compile the project using the provided `compile.sh` script.

### Using the script:
```bash
chmod +x compile.sh
./compile.sh
```

### Manual compilation:
```bash
mkdir -p bin
gcc src/linear_program.c src/main.c src/color_printf.c src/parser.c src/fraction.c -o bin/main -lm
```

---

## Usage

1. Prepare your linear programming problem in an `.lp` file.
2. Run the executable from the project root:
   ```bash
   ./bin/main
   ```
   *Note: The program currently looks for `example.lp` in the directory where you execute the command.*

---

## Input File Format (`.lp`)

The solver reads problems from a text file. Below is an example:

```text
OBJECTIVE: MAX
VARIABLES: 3
CONSTRAINTS: 3

COEFFS:
2   -1   -2  <= 4
2   -3    1  <= -5
-1   1   -2  <= -1

OBJ:
1   -1    1
```

### Keywords:
- **OBJECTIVE**: `MAX` or `MIN`.
- **VARIABLES**: Number of decision variables.
- **CONSTRAINTS**: Number of constraint rows.
- **COEFFS**: The matrix of coefficients, the sign (`<=`, `>=`, `=`), and the RHS value.
- **OBJ**: The coefficients of the objective function.

---

## How it Works

1. **Standardization**: Converts inequalities into equalities by adding Slack and Surplus variables.
2. **Phase 1**: If artificial variables are required, the solver first minimizes their sum to find a Basic Feasible Solution.
3. **Phase 2**: Once a feasible starting point is found, the solver optimizes the original objective function.
4. **Fractional Math**: Every pivot operation is performed on fractions to maintain 100% accuracy throughout the iterations.

## License
This project is open-source and intended for educational use.