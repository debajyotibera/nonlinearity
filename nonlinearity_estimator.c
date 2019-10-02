#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<string.h>
#include<math.h>
#include "nl_calc.h"
#include "nl_estimate_classical.h"
#include "nl_estimate_mixed.h"


void print_usage(){
	printf("\n\nUsage: a.out -n [bits] -e [error] -a [accuracy] -d [datafile name] [-c | -q | -m | -r] [-options]\n");
	printf("\t-n: Takes the number of bits of givenfunction as argument.\n");
	printf("\t-d: Takes the name of the datafile that contains the truth table of the function as argument. Default is data.txt.\n");
	printf("\t-e: Takes the maximum allowed error as argument. Value between 0 and 1 excuding 0.\n");
	printf("\t-a: Takes the required accuracy as argument. Value between 1/sqrt(2^(n+1)) and 0.5 excluding the lower boundary.\n");
	printf("\t-t: Takes the minimum threshold below which the the user want to stop the search for (fhat_max)^2. Values from 1/2^(n/2) to 1 both inclusive. Default value is 1/64.\n");
	printf("\t-c: Purely Classical mode of estimatioan.\n");
	printf("\t-q: Purely Quantum estimation.\n");
	printf("\t-m: Estimation using the mix of classical and quantum methods.\n");
       printf("\t-r: Regular trvial mode of calculation of the nl of the given function.\n");
       printf("\t-v: Choose for verbose. Verbose is off by default.\n");
       printf("\t-h: Provides help with the parsers.\n");
       printf("\nExample usage: ./a.out -n 8 -d dfile.txt -e 0.05 -a 0.0625 -c -v \n");
       printf("\n\tIn this example the given is an 8-bit function and the truth table of the function is in the file dfile.txt. The maximum allowed error for the estimation is 0.05 and the ouput is required to be within an additive accuracy of 0.0625. The method to be used is the complete classical estimation of non-linearity. Verbose mode is on.\n");
       printf("\n\tNOTE: The inputs for error and accuracy may be omitted if the method to be implemented is the trivial mode of calculation (when using -r) of the non-linearity.\n\n");
}

int main(int argc, char **argv){
	int option_index = 0;
	char *error = NULL;
	char *accuracy = NULL;
	//float accuracy = 0;
	float thresh_min = 1/64;
	char dat_file[100] = "data.txt";
	char *mode = NULL;
	int vflag = 0;
	int n = 0;
	while((option_index = getopt(argc, argv, "e:a:d:t:n:cqrmvh")) != -1){
		switch (option_index){
			case 'n':
				n = atoi(optarg);
				break;
			case 'e':
				error = optarg;
				break;
			case 'a':
				accuracy = optarg;
				break;
			case 'd':
				strcpy(dat_file, optarg);
				break;
			case 't':
				thresh_min = atof(optarg);
				break;
			case 'c':
				mode = "c";
				break;
			case 'q':
				mode = "q";
				break;
			case 'm':
				mode = "m";
				break;
			case 'r':
				mode = "r";
				break;
			case 'v':
				vflag = 1;
				break;
			case 'h':
				print_usage();
				return -1;
			default:
				printf("Values missing\n");
		}
	}
	if(mode=="r" && n){
		printf("\n");
	}
	else if(error && accuracy && mode && n){
		printf("\n");
	}
	else{
		printf("\nValues missing!!! Please check the input!\n\n");
		return -1;
	}
	double epsilon = atof(accuracy);
        double delta = atof(error);
	if(delta<=0 || delta>1){
		printf("Improper error value. Error Value has to be between 0 and 1, excluding 0.\n");
		return -1;
	}
	double eps_uplim = 1/(sqrt(pow(2,(n+1))));
	if(epsilon>0.5 || epsilon <= eps_uplim){
		printf("Improper accuracy value. Accuracy value has to be greater than %lf and less than or equal to 0.5.\n", eps_uplim);
		return -1;
	}
	if(mode == "c"){
		printf("Mode : Purely Classical Estimation\n");
		nl_estimate_classical(dat_file, epsilon, delta, thresh_min, vflag);
	}
	else if(mode=="q"){
	 	printf("Mode : Purely Quantum Estimation\n");
		float thresh_abv = 0.99;
		char out_string[50], command[50], str[50];
		sprintf(out_string, "%lf %lf %d", thresh_abv, delta, vflag);
		strcpy(str, "python3 nl_estimate_quantum.py ");
                strcat(str,out_string );
                //const char *command = str.c_str();
                strcpy(command,str);
                system(command);	
	}
	else if(mode=="m"){
		printf("Mode : Estimation using Mixed Method\n");
		nl_estimate_mixed(dat_file, epsilon, delta, thresh_min, vflag);
	}
	else if(mode=="r"){
		printf("Mode : Trivial Calculation\n");
		nl_calc(dat_file, vflag);
	}

	return 0;
}
