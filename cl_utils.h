

#include <oclUtils.h>

#define GPU_LOCAL_WORK_SIZE 16

void initGPU();
unsigned int RoundUp(int group_size, int global_size);
cl_kernel initGPUProgram(char * cSourceFileName, char* kernelName);
cl_mem getDeviceBufferF(int iNumElements);
cl_mem getDeviceBufferI(int iNumElements);
cl_mem getDeviceBufferF4(int iNumElements);
cl_kernel getDefaultKernel();
cl_command_queue getDefaultCommandQueue();
cl_int getLastError();
void showLastError(cl_int errorCode);
cl_context clgetContext();
cl_mem getDeviceBufferC(int iNumElements);
cl_kernel initKernel(char* kernelName);