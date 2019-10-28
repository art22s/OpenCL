#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

int main(){

        cl_platform_id platforms;
        char p_name[40];
        cl_int err;

        err = clGetPlatformIDs(1,&platforms,NULL);
        if(err<0){
                perror("Platform error");
                exit(1);
        }
        
        err = clGetPlatformInfo(platforms, CL_PLATFORM_VENDOR, sizeof(p_name), &p_name, NULL);

        printf("%s",p_name);
        return 0;
        }


