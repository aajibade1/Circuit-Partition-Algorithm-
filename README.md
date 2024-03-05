# Circuit Partitioning Hybrid Accelerator
Advanced Circuit Partitioning Algorithm featuring hybrid Accelerator for EIG and KL algorithms. By utilising sparse matrix methods, eigenvector computation and ratio cut optimisation, runtime is reduced for dense circuits.

# How to Run the Code
1. Clone this repository and run the following commands in the terminal:
```
    cd EIG-KL-Hybrid-Algorithm-Accelerator
    ./run.sh
```

2. For Eigen algorithm:
Compile the code:
```
    g++ cEIG.cpp -std=c++17 -I eigen -o EIG
```
Run the code:
```
    ./EIG <dataset_input_file>
```

3. For KL algorithm Single Thread:
Compile the code:
```
    g++ cKL.cpp -std=c++17 -I eigen -o KL
```
Run the code:
```
    ./KL_single_thread <dataset_input_file>         # produce the Cutsize for each iteration
    ./KL_single_thread <dataset_input_file> -EIG    # produces the Cutsize for each iteration with Eigen soltion as the initial split
```

4. For KL algorithm Multi Thread:
Compile the code:
```
    g++ cKL.cpp -std=c++17 -o KL_multi_thread_omp
```
Run the code:
```
    ./KL_multi_thread_omp <dataset_input_file>         # produce the Cutsize for each iteration
    ./KL_multi_thread_omp <dataset_input_file> -EIG    # produces the Cutsize for each iteration with Eigen soltion as the initial split
```

5. The resutls are saved in the folder (results/) for each implementation 
