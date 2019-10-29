#define _CRT_SECURE_NO_WARNINGS
#define PROGRAM_FILE "hello_World.cl"
#define FUNC "hello"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

int main(){
        
        cl_platform_id platform;
        cl_platform_info info;
        cl_device_id device;
        cl_context context;
        cl_command_queue queue;
        cl_int err;

        cl_program program;
        FILE *program_handle;
        char *program_buffer;
        size_t program_size;
        cl_kernel kernel;

        err = clGetPlatformIDs(1, &platform, NULL);
        if(err < 0){
                perror("Can not get platform");
                exit(1);}

        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL);
        if(err < 0){
                perror("Can not get device");
                exit(1);}

        context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
        if(err < 0){
                perror("Can not create context");
                exit(1);}

        program_handle = fopen(PROGRAM_FILE, "r");
        if(program_handle == NULL){
                perror("Could not open the file");
                exit(1);}
        fseek(program_handle, 0, SEEK_END);
        program_size = ftell(program_handle);
        rewind(program_handle);
        program_buffer = (char*)malloc(program_size + 1);
        program_buffer[program_size] = "\0";
        fread(program_buffer, sizeof(char), program_size, program_handle);
        fclose(program_handle);

        program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, &program_size, &err);
        if(err < 0){
                perror("Cannot create program");
                exit(1);}

        free(program_buffer);
        err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
        if(err < 0){
                perror("Cannot build program");
                exit(1);}

        kernel = clCreateKernel(program, FUNC, &err);
        if(err < 0){
                perror("Cant create kernel");
                exit(1);}

        queue = clCreateCommandQueue(context, device, 0, &err);
        if(err < 0){
                perror("Cannot create command queue");
                exit(1);}

        size_t work_units = 4;
        err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &work_units, NULL, 0, NULL, NULL);
        if(err < 0){
                perror("Cannot enqueue kernel");
                exit(1);}

        clReleaseKernel(kernel);
        clReleaseCommandQueue(queue);
        clReleaseProgram(program);
        clReleaseContext(context);

        return 0;
}

