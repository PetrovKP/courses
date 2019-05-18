//
// Created by kirill on 21.10.17.
//

#include "gemm.h"
#include "utils_opencl.h"

void cblas_sgemm(const int m, const int n, const int k, const float *a,
                 const float *b, float *c, mode device) {
  if (device == GPUCUDA) {
    #ifdef GPU_CUDA
      printf("run to GPU CUDA\n");
      cblas_sgemm_gpu_cuda(m, n, k, a, b, c, DEFAULT);
    #endif
  } else if (device == GPUCUDA_OPTIMAZE) {
    #ifdef GPU_CUDA
      printf("run to GPU CUDA OPTIMIZE\n");
      cblas_sgemm_gpu_cuda(m, n, k, a, b, c, OPTIMIZED);
    #endif
  } else if (device == GPUCL) {
    printf("run to GPU CL\n");
    cblas_sgemm_gpu_cl(m, n, k, a, b, c, DEFAULT);
  } else if (device == GPUCL_OPTIMAZE) {
    printf("run to GPU CL OPTIMIZE\n");
    cblas_sgemm_gpu_cl(m, n, k, a, b, c, OPTIMIZED);
  }
  else {
    printf("run to CPU\n");
    cblas_sgemm_cpu(m, n, k, a, b, c);
  }
}

void cblas_sgemm_cpu(const int m, const int n, const int k, const float *a,
                     const float *b, float *c) {
  float resC;
  for (int i = 0; i < m; ++i)
    for (int j = 0; j < k; ++j) {
     resC = 0.0;
      for (int l = 0; l < n; ++l)
        resC += a[i*n + l] * b[l*k + j];
      c[i*k + j] = resC;
    }
}

void cblas_sgemm_gpu_cl(const int m, const int n, const int k,
                        const float *a, const float *b, float *c,
                        const mode_opt opt) {
  cl_context context = 0;
  cl_command_queue commandQueue = 0;
  cl_device_id device = 0;
  cl_kernel kernel = 0;
  cl_program program = 0;

  context = createContext();
  commandQueue = createCommandQueue(context, &device);
  program = createProgram(context, device);
  if (opt == OPTIMIZED)
    kernel = clCreateKernel(program, "cblas_sgemm_optimized", NULL);
  else
    kernel = clCreateKernel(program, "cblas_sgemm", NULL);

  cl_int err;
  cl_mem memC = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float)*m*k, NULL, &err);
  checkError(err,"mem Result");
  cl_mem memA = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*m*n, a, &err);
  checkError(err,"mem A");
  cl_mem memB = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*n*k, b, &err);
  checkError(err,"mem B");

  err =  clSetKernelArg(kernel, 0, sizeof(int), &m);
  err |= clSetKernelArg(kernel, 1, sizeof(int), &n);
  err |= clSetKernelArg(kernel, 2, sizeof(int), &k);
  err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &memA);
  err |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &memB);
  err |= clSetKernelArg(kernel, 5, sizeof(cl_mem), &memC);
  checkError(err,"clSetKernelArg");

  const size_t BLOCK_SIZE = 16;

  size_t globalWorkSize[] = { m, k };
  size_t localWorkSize[]  = { BLOCK_SIZE, BLOCK_SIZE };

  err = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
  checkError(err,  "clEnqueueNDRangeKernel");

  clFinish(commandQueue);

  err = clEnqueueReadBuffer(commandQueue, memC, CL_TRUE, 0, sizeof(float)*m*k, c, 0, NULL, NULL);
  checkError(err,  "clEnqueueReadBuffer: out");
  clFinish(commandQueue);

  clReleaseMemObject(memA);
  clReleaseMemObject(memB);
  clReleaseMemObject(memC);

  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseCommandQueue(commandQueue);
  clReleaseContext(context);
}