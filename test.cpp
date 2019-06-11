#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
int main()
{
	double a = 0;
	
	void *handle = dlopen("./libs.so", RTLD_LAZY);
	
	if(!handle)
	{
		printf("open lib error\n");
		cout<<dlerror()<<endl;
		return -1;
	}
    printf("open libs succedd!!!");
	// double avg(double *a, int n) ;
    // int add(int a, int b);
	typedef double (*avg_t)(int a, int b);
	avg_t avg = (avg_t) dlsym(handle, "add");
	if(!avg)
	{   
        cout<<"@@@@@@@@"<<endl;
		cout<<dlerror()<<endl;
		dlclose(handle);
		return -1;
	}
	printf("start cale!!!");

	double tab[3] = {1.0,2.0,3.0};
    
	a = avg(1, 3);
	printf("a = %f\n",a);
	dlclose(handle);
	return 0;
}