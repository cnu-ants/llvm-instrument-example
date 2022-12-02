# llvm-instrument-example (source-level)

## Before getting start
Install the llvm-project on your system, following the [installation
instruction](https://github.com/llvm/llvm-project). The installation option
needs to include the following:
```
-DLLVM_ENABLE_PROJECTS="clang" 
-DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi" 
```

Set your llvm-project path in the ``Makefile``.
```
LLVM_DIR = [put the absolute path of your llvm-project here]
```
Build the example project, using ``make``. Then, an example runner script
``run_example.sh`` is automatically generated in the ``example`` directory.

## Trying an example
Execute the ``run_example.sh`` script in the ``example`` directory.
```
cd example
./run_example.sh
```
The script runs the example instrumentor that injects a print statement after
each variable declaration with initialization in ``foo.c``.
