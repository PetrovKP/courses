//
// Created by kirill on 18.11.17.
//

#ifndef UTILS_OPENCL_H
#define UTILS_OPENCL_H

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

cl_context createContext(void);
cl_command_queue createCommandQueue(cl_context context, cl_device_id *device);
cl_program createProgram(cl_context context, cl_device_id device);

void cleanup(cl_context context, cl_command_queue commandQueue, cl_program program, cl_kernel kernel, cl_mem* memObjects, cl_uint nmem);

size_t* default_wg_sizes(size_t* num_wg_sizes,const size_t max_wg_size, size_t *global_size);
void __checkError(int line,
                  const char *file,
                  cl_int error,
                  const char *msg,
                  ...);
// Print the error associciated with an error code
void printErrorCl(cl_int error);

void __checkError(int line,
                 const char *file,
                 cl_int error,
                 const char *msg,
                 ...);

#define checkError(status, ...) __checkError(__LINE__, __FILE__, status, __VA_ARGS__)

#endif //UTILS_OPENCL_H
