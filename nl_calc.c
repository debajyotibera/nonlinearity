#include<stdio.h>
#include<math.h>
#include "nl_calc.h"

#define vprintf(...) if(vflag==1){fprintf(stderr,  __VA_ARGS__);}


struct Values
{
	float mx;
	int k;
};

//Interger to binary
long long int int_to_bin(long long int k){
    if (k == 0) return 0;
    if (k == 1) return 1;
    return (k % 2) + 10 * int_to_bin(k / 2);
}

//Dot product.
int dot(long long int p, long long int x, int n){
        int b1[n];
        long long int temp = int_to_bin(p);
        for(int i=0; i<n; i++){
                int rem = temp%10;
                b1[i]=rem;
                temp = (temp-rem)/10;
        }
        int b2[n];
        temp = int_to_bin(x);
        for(int i=0; i<n; i++){
                int rem = temp%10;
                b2[i]=rem;
                temp = (temp-rem)/10;
        }
        int dot_sum = 0;
        for(int i=0;i<n;i++){
                dot_sum += b1[i]*b2[i];
        }
        //printf("Dot of %lld and %lld is : %d\n", p,x,dot_sum%2);
        return dot_sum%2;
}

float fhat(int a, int f[], int n){
	float val = 0;
	int expon;
	for(int j=0; j<pow(2,n); j++){
		expon = f[j]+dot(a,j, n);
		val += pow(-1,expon);
	}

	return val/pow(2,n);
}

struct Values maxi(float arr[], int n){
	float mx = -1;
	int m = 0;
	for(int k=0; k<pow(2,n); k++){
		if(fabs(arr[k])>mx){
			mx = fabs(arr[k]);
			m = k;
		}
	}
	struct Values val;
	val.mx = mx;
	val.k = m;
	return val;
}

int nl_calc(char datfile[], int vflag){
	//char *datfile = argv[1];
	int fval, n;
	long long int pos;
	float mx;
        FILE *file;
        file = fopen(datfile, "r");
        fscanf(file, "%d", &n);
	int size = pow(2,n);
	int f[size];
	for(int i=0; i<pow(2,n); i++){
                fscanf(file, "%lld\t%d", &pos, &fval);
                f[i] = fval;
        }
	fclose(file);

	float arr[size];
	for(int i=0; i<pow(2,n); i++){
		arr[i]=fhat(i, f, n);
	}
	struct Values val1;
	/*for(int i=0; i<pow(2,n); i++){
		printf("%f\n",arr[i]);
	}*/
	val1 = maxi(arr, n);
	mx = val1.mx;
	int m = val1.k;
	float nl = pow(2,n-1)*(1.-mx);
	long long int calls = pow(2,2*n);
	vprintf("\nTotal calls : %lld\n", calls);
	vprintf("\nMaximum f_hat is: %f and it occurs at %0*lld\n.", mx, n, int_to_bin(m));
	printf("\nThe nl(f) is : %f\n", nl/(pow(2,n)));
}
