Non-Linearity Package Version 1.0 18/07/2019
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	The non-linearity package is a package containing various
methods for calculating and estimating the non-linearity of a Boolean
function given its truth table. The package contains code that can 
implement four methods viz.,
	i)   calculation of non-linearity
	ii)  completely-calssical estimation of non-linearity
	iii) completely-quantum estimation of non-linearity
	iv)  hybrid classical-quantum estimation of non-linearity.

	For more details on the methods please read (the paper that 
contains the methods).


AUTHORS
--------

Tharrmashastha SAPV and Debajyoti Bera
IIIT-D, India.


GENERAL USAGE NOTES
--------------------

-Implementation requires gcc, python 3.x and qiskit installed.

-The primary script is nonlinearity_estimator.c. The scripts 
nl_calc.c, nl_estimate_classical, nl_estimate_mixed are auxiliary c
scripts and nl_estimate_quantum.py and qtest.py are auxiliary python 
scripts.

-qconfig.py is the python script that contains IBMQ account API token.

-generate_function.c is the script that generates the truth table 
values of random n-bit functions given an 'n' on prompt and writes the 
same to data.txt.


IMPLEMENTATION
---------------

The package has to be compiled as follows:

gcc nl_calc.c nl_estimate_classical.c nl_estimate_mixed.c
nonlinearity_estimator.c -lm

The same is implemented as follows:

./a.out -n [bits] -e [error] -a [accuracy] -d [datafile name] [-c | -q | -m | -r] [-options]

where	
	-n: Takes the number of bits of given the function as argument.
	-d: Takes the name of the datafile that contains the truth 
            table of the function as argument. Default is data.txt.
	-e: Takes the maximum allowed error as argument. Value between
	    0 and 1 excuding 0.
	-a: Takes the required accuracy as argument. Value between 
            1/sqrt(2^(n+1)) and 0.5 excluding the lower boundary.
	-t: Takes the minimum threshold below which the the user want 
            to stop the search for (fhat_max)^2. Values from 1/2^(n/2) 
            to 1 both inclusive. Default value is 1/64.
	-c: Purely Classical mode of estimatioan.
	-q: Purely Quantum estimation.
	-m: Estimation using the mix of classical and quantum methods.
	-r: Regular trvial mode of calculation of the non-linearity of 
            the given function.
	-v: Choose for verbose. Verbose is off by default.
	-h: Provides help for implementation.

Example usage: ./a.out -n 8 -d dfile.txt -e 0.05 -a 0.0625 -c -v 

	In this example the given is an 8-bit function and the truth 
table of the function is in the file dfile.txt. The maximum allowed 
error for the estimation is 0.05 and the ouput is required to be 
within an additive accuracy of 0.0625. The method to be used is the 
complete classical estimation of non-linearity. Verbose mode is on.

NOTE: The inputs for error and accuracy may be omitted if the method
to be implemented is the trivial mode of calculation (when using -r)
of the non-linearity.


LEGAL
------

Please feel free to use, distrubute, modify or enhance the package.
But do not forget to mention the original authors of the package 
with it.

For any assistance please feel free to reach at:
E-mail : tharmasasthapv@gmail.com


