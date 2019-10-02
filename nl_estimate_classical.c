#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include "nl_estimate_classical.h"

/*This code outputs a interval I  such that nl belongs
in the interval I.*/

#define vprintf(...) if(vflag==1){fprintf(stderr, __VA_ARGS__);}

long long int calls = 0;

typedef struct Queue{
	int capacity;
        int size;
        int front;
        int rear;
        long long int *elements;
}Queue;

Queue * createQueue(int maxElements){
	Queue *Q;
	Q = (Queue *)malloc(sizeof(Queue));
	Q->elements = (long long int *)malloc(sizeof(int)*maxElements);
	Q->size = 0;
	Q->capacity = maxElements;
	Q->front = 0;
	Q->rear = -1;

	return Q;
}

int pop(Queue *Q){
	if(Q->size==0){
		printf("Queue is empty!\n");
		exit(0);
	}
	int rep = Q->elements[Q->front];
	Q->size--;
	Q->front++;
	if(Q->front == Q->capacity)
       		Q->front=0;
	return rep;
}

void popall(Queue *Q){
	int k;
	for(int idx=0; idx<Q->size; idx++){
		k = pop(Q);
	}
}

void Enqueue(Queue *Q, int element){
	if(Q->size==Q->capacity)
		printf("Queue is full!\n");
	else{
		Q->size++;
		Q->rear++;
		if(Q->rear==Q->capacity)
			Q->rear=0;
		Q->elements[Q->rear]=element;
	}
}


struct Data
{
	long long int x, y;
};


//Interger to binary
long long int int_to_bin1(long long int k){
    if (k == 0) return 0;
    if (k == 1) return 1;
    return (k % 2) + 10 * int_to_bin1(k / 2);
}


//Dot product.
int dot1(int n, long long int p, long long int x){
	int b1[n];              
        long long int temp = int_to_bin1(p);
	for(int i=0; i<n; i++){
                int rem = temp%10;
                b1[i]=rem;
                temp = (temp-rem)/10;
        }
	int b2[n];              
        temp = int_to_bin1(x);
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


//Estimation function.
float estimate2_v(int n, long long int p, int k, float threshold, double eps, double delta, int maxRound, int f[]){
	
	FILE *fp;
	struct Data dat;
	long int x, y;
	long int maxIter = ceil(8*log(maxRound/((threshold-eps)*delta))/pow(eps,2));
	calls += maxIter;
	//printf("maxIter : %li\n", maxIter);
	float v = 0, v1, est=0, mean=0;
	for(int i=0; i<maxIter; i++){
		int pow1 = pow(2,n-k);
		long int s = rand()%pow1;
		//long int z = i;
		//printf("s : %ld\n\n", s);
		int pow2 = pow(2,n);
		//fread(&dat, sizeof(dat), 1, fp);
		//printf("x : %ld\n",x);
		//printf("y : %ld\n",y);
		//x = dat.x; y = dat.y;
		long int x = rand()%pow2;
		long int y = rand()%pow2;
		//long int x = j;
		//printf("x : %ld\n",x);
		//printf("p is : %lld\n",(p<<n-k)+s );
		int expon1 = f[x]+(dot1(n,(p<<n-k)+s,x));
		int expon2 = f[y]+(dot1(n,(p<<n-k)+s,y));
		//printf("f(xz) : %d\n", f[(x<<n-k)+z]);
		float addition = pow(-1,expon1)*pow(-1,expon2);
		est += addition;
		//printf("est : %f\n", est);
	}
	//fclose(fp);
	mean = (est/maxIter)*pow(2,n-k);
	//printf("mean : %f\n", mean);
	return mean;
}


int nl_estimate_classical(char datfile[], double epsilon, double delta, double threshold_min, int vflag){
	int n;
	//double epsilon, delta;
	FILE *file;
	int cond = 1;
	file = fopen(datfile,"r");
	fscanf(file,"%d", &n);
	/*while(cond){
		printf("Please enter the accuracy required : \n");
		scanf("%lf",&epsilon);
		if(epsilon>=1./sqrt(pow(2,n+1)) && epsilon<=0.5){
			cond = 0;
		}
		else{
			printf("The minimum possible accuracy is %lf and the maximum possible accuracy is %lf.\n", 1./sqrt(pow(2,n+1)), 0.5);
		}
	}
	printf("Please enter the maximum error allowed (0.0 to 1.0) : ");
	scanf("%lf",&delta);*/
	double new_eps = pow(2*epsilon,2);
	long long int size = pow(2,n), shots=0;
	Queue *list = createQueue(size);
	//printf("%lld\n", size);
	int f[size], fval, maxRound;
	long long int pos;
	for(int fsize=0; fsize<pow(2,n); fsize++){
		fscanf(file, "%lld", &pos);
		fscanf(file, "%d", &fval);
		f[fsize] = fval;
	}
	//for(int fsize1=0; fsize1<pow(2,n); fsize1++)
	//	printf("%d\n", f[fsize1]);
	time_t t;
	srand((unsigned) time(&t));
	//printf("new_eps = %lf\n", new_eps);
	if(ceil(log(1/new_eps)/log(2))-log(1/new_eps)/log(2)==0){
		maxRound = ceil(log(1/new_eps)/log(2))+1;
	}
	else{
		maxRound = ceil(log(1/new_eps)/log(2))+1;
	}
	if(n<maxRound)
		maxRound=n;
	vprintf("MaxRound : %d\n", maxRound);
	//double threshold_min = pow((1./8),2);                         //Minimum for threshold
	double eps = new_eps - (1./pow(2,maxRound));                  //Accuracy

	double threshold = 0.5;
	double lower=1./pow(2,2*n), upper=1.0;
	vprintf("Threshold Minimum : %lf,\nEpsilon : %lf,\nLower Bound: %f,\nUpper Bound : %f\n", threshold_min, eps, lower, upper);
	clock_t start = clock();
	for(int i=0;i<maxRound;i++){
		vprintf("\nSearching for max f_hat above threshold %f.\n",threshold);
		int maxInvoc = ceil(2/threshold);
	        Queue *Q = createQueue(pow(2,n));
		Queue *L = createQueue(2*pow(2,n));
		int index = 0;
        	float v = estimate2_v(n,0,1, threshold, eps, delta, maxRound, f);
		//printf("v_est for p=0 at k=1 %f\n", v);
        	if(v>=threshold-eps)
                	Enqueue(Q,0);
        	v = estimate2_v(n,1,1, threshold, eps, delta, maxRound, f);
		//printf("v_est for p=1 at k=1 %f\n", v);
        	if(v>=threshold-eps)
                	Enqueue(Q,1);
        	for(int k=2;k<=n;k++){
			//printf("\nk is : %d\n", k);
			
			//printf("Q-size : %d\n", Q->size);
                        //for(int qsize=0;
			//qsize<Q->size; qsize++){
                        //        printf("Q[%d] : %lld\n", qsize, Q->elements[(Q->front)]);
                        //}

			int q_size = Q->size;
			for(int cnt=0; cnt<q_size; cnt++){

                	long long int p = pop(Q);
			//printf("\np is : %lld\n", p);

                	v = estimate2_v(n, p<<1,k, threshold, eps, delta,  maxRound, f);
			//printf("v est for p=%lld : %f\n", p<<1, v);
                	if(v>=threshold-eps){
                        	if(k==n){
					Enqueue(L,p<<1);
					//printf("the element that goes inside L is : %lld\n", p<<1);
				}
				else{
                                	Enqueue(Q,(p<<1));
					//printf("the element that goes inside Q is : %lld\n", (p<<1));
					//printf("Q now has : %lld\n", Q->elements[Q->front]);
				}
                	}
			v = estimate2_v(n, (p<<1)+1,k, threshold, eps, delta,  maxRound, f);
			//printf("v est for p=%lld : %f\n", (p<<1)+1, v);
                	if(v>=threshold-eps){
                        	if(k==n){
                                	Enqueue(L,(p<<1)+1);
					//printf("the element that goes inside L is : %lld\n", (p<<1)+1);
				}
                        	else{
                                	Enqueue(Q,(p<<1)+1);
					//printf("the element that goes inside Q is : %lld\n", (p<<1)+1);
					//printf("Q now has : %lld\n", Q->elements[Q->front]);
			}
                	}
			}	
        	}
		free(Q);
		//printf("\n");
		//printf("L has %d elements.\n\n",L->size);
		vprintf("Search Complete!\n");
		if(L->size!=0){
			vprintf("Max f_hat is above %lf.\n", threshold-eps);
			if(lower >= 1./pow(2,2*n))
				lower = fmax(1./pow(2,2*n),threshold-(eps));
			if(list->size)
				popall(list);
			for(int idx=0; idx<L->size; idx++){
				Enqueue(list, L->elements[L->front+idx]);
				}
			//vprintf("Lower bound changes to %lf\n", lower);
			threshold = threshold + (1./pow(2,i+2));
		}
		else if(L->size==0){
			upper = threshold;
			//vprintf("Upper Bound changes to %lf\n", threshold);
			vprintf("Max f_hat is below %f\n", threshold);
			threshold = threshold - (1./pow(2,i+2));
		}
		else
			//printf("This is else\n\n.");
		
		//printf("\nlower : %lf\n", lower);
			
		vprintf("Threshold : %f\n", threshold);
		if(threshold<threshold_min){
			break;
		}
		free(L);
	}
	clock_t end = clock()-start;
	double duration = ((double)end)/CLOCKS_PER_SEC;//start - end;
	vprintf("\nTime taken in classical rounds : %f seconds\n", duration);
	//printf("\nlower : %lf , upper : i%lf\n", lower, upper);
	vprintf("Total classical calls taken : %lld\n", calls);
	//double nl_upper = pow(2,n-1)*(1-sqrt(lower));
	//double nl_lower = pow(2,n-1)*(1-sqrt(upper));
	double nl_upper = (1-sqrt(lower))/2.;
	double nl_lower = (1-sqrt(upper))/2.;
	if(threshold>=threshold_min){
		printf("\nnl(f) belongs to (%lf , %lf]\n\n", nl_lower, nl_upper);
		int idx=0;
		//printf("List size -> %d\n",list->size);
		printf("The point 'a' such that a.x is the closest to the given f belongs to the list { ");
		for(int idx=0;idx<list->size-1; idx++){
			printf("%0*lld, ",n,int_to_bin1(list->elements[idx]));
		}
	}
	printf("%0*lld }\n\n", n, int_to_bin1(list->elements[list->size-1]));
}
