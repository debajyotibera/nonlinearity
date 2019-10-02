/*This script is used to generate random n-bit
 * functions where 'n' is user provided and writes 
 * the truth table in an external file named
 * data.txt which could be used in the nonlinearity 
 * estimation program.*/

#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>

/*struct Data
{
	long long int
};
*/
long long int int_to_bin(long long int k){
    if (k == 0) return 0;
    if (k == 1) return 1;
    return (k % 2) + 10 * int_to_bin(k / 2);
}

int bent(long long int k){
	int r=0, r1=0,r2=0;
	while(k){
		r1 = k%10;
		r2 = (k/10)%10;
		r += r1*r2;
		k = k/100;
	}
	return r%2;
}

int main(){
	int n, cond, r;
	time_t t;
        srand((unsigned) time(&t));
	FILE *file;
	file = fopen("data.txt", "w+");
	printf("Enter the number of bits of the function : ");
	scanf("%d", &n);
	fprintf(file,"%d\n",n);
	printf("Please enter 1 if you would like it to be a bent function, else enter 0 : ");
	scanf("%d", &cond);
	for(int i=0; i<pow(2,n); i++){
		int i_b = int_to_bin(i);
		if(cond)
			r = bent(i_b);
		else
			r = rand()%2;
		fprintf(file, "%0*lld\t%d\n", n, int_to_bin(i), r);
	}
	fclose(file);
}
