#include <oclUtils.h>
#include <iostream>

#include "cl_utils.h"



using namespace std;


// OpenCL Vars
cl_context cxGPUContext;        // OpenCL context
cl_command_queue cqCommandQueue = NULL;// OpenCL command que
cl_platform_id cpPlatform;      // OpenCL platform
cl_device_id cdDevice;          // OpenCL device
cl_program cpProgram;           // OpenCL program
cl_kernel ckKernel;             // OpenCL kernel
cl_mem cmDevSrcA;               // OpenCL device source buffer A
cl_mem cmDevSrcB;               // OpenCL device source buffer B 
cl_mem cmDevDst;                // OpenCL device destination buffer 
unsigned int szGlobalWorkSize;        // 1D var for Total # of work items
unsigned int szLocalWorkSize;		    // 1D var for # of work items in the work group	
unsigned int szParmDataBytes;			// Byte size of context information
unsigned int szKernelLength;			// Byte size of kernel code
cl_int ciErr1;			// Error code var
char* cPathAndName = NULL;      // var for full paths to data, src, etc.
char* cSourceCL = NULL;         // Buffer to hold source for compilation 


// Helper function to get error string
// *********************************************************************
const char* ErrorString(cl_int error)
{
    static const char* errorString[] = {
        "CL_SUCCESS",
        "CL_DEVICE_NOT_FOUND",
        "CL_DEVICE_NOT_AVAILABLE",
        "CL_COMPILER_NOT_AVAILABLE",
        "CL_MEM_OBJECT_ALLOCATION_FAILURE",
        "CL_OUT_OF_RESOURCES",
        "CL_OUT_OF_HOST_MEMORY",
        "CL_PROFILING_INFO_NOT_AVAILABLE",
        "CL_MEM_COPY_OVERLAP",
        "CL_IMAGE_FORMAT_MISMATCH",
        "CL_IMAGE_FORMAT_NOT_SUPPORTED",
        "CL_BUILD_PROGRAM_FAILURE",
        "CL_MAP_FAILURE",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "CL_INVALID_VALUE",
        "CL_INVALID_DEVICE_TYPE",
        "CL_INVALID_PLATFORM",
        "CL_INVALID_DEVICE",
        "CL_INVALID_CONTEXT",
        "CL_INVALID_QUEUE_PROPERTIES",
        "CL_INVALID_COMMAND_QUEUE",
        "CL_INVALID_HOST_PTR",
        "CL_INVALID_MEM_OBJECT",
        "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
        "CL_INVALID_IMAGE_SIZE",
        "CL_INVALID_SAMPLER",
        "CL_INVALID_BINARY",
        "CL_INVALID_BUILD_OPTIONS",
        "CL_INVALID_PROGRAM",
        "CL_INVALID_PROGRAM_EXECUTABLE",
        "CL_INVALID_KERNEL_NAME",
        "CL_INVALID_KERNEL_DEFINITION",
        "CL_INVALID_KERNEL",
        "CL_INVALID_ARG_INDEX",
        "CL_INVALID_ARG_VALUE",
        "CL_INVALID_ARG_SIZE",
        "CL_INVALID_KERNEL_ARGS",
        "CL_INVALID_WORK_DIMENSION",
        "CL_INVALID_WORK_GROUP_SIZE",
        "CL_INVALID_WORK_ITEM_SIZE",
        "CL_INVALID_GLOBAL_OFFSET",
        "CL_INVALID_EVENT_WAIT_LIST",
        "CL_INVALID_EVENT",
        "CL_INVALID_OPERATION",
        "CL_INVALID_GL_OBJECT",
        "CL_INVALID_BUFFER_SIZE",
        "CL_INVALID_MIP_LEVEL",
        "CL_INVALID_GLOBAL_WORK_SIZE",
    };

    const int errorCount = sizeof(errorString) / sizeof(errorString[0]);

    const int index = -error;

    return (index >= 0 && index < errorCount) ? errorString[index] : "";
}


cl_context clgetContext()
{
	return cxGPUContext;
}

void initGPU()
{
	//Get an OpenCL platform
    ciErr1 = clGetPlatformIDs(1, &cpPlatform, NULL);
	
    
    if (ciErr1 != CL_SUCCESS)
    {
		showLastError(ciErr1);
		return ;        
	}

    //Get the devices
    ciErr1 = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &cdDevice, NULL);
    
    if (ciErr1 != CL_SUCCESS)
    {
        return;
        
    }

    //Create the context
    cxGPUContext = clCreateContext(0, 1, &cdDevice, NULL, NULL, &ciErr1);
    
    if (ciErr1 != CL_SUCCESS)
    {
       showLastError(ciErr1);
		return;
        
    }

    // Create a command-queue
    cqCommandQueue = clCreateCommandQueue(cxGPUContext, cdDevice, 0, &ciErr1);
    
    if (ciErr1 != CL_SUCCESS)
    {
        showLastError(ciErr1);
		return; 
    }
	

}

cl_int getLastError()
{
  return ciErr1;
}

void showLastError(cl_int errorCode)
{
  cout<< ErrorString(errorCode)<<"\n";
}


cl_kernel getDefaultKernel()
{
	return ckKernel;
}
// Round Up Division function
unsigned int RoundUp(int group_size, int global_size) 
{
    int r = global_size % group_size;
    if(r == 0) 
    {
        return global_size;
    } else 
    {
        return global_size + group_size - r;
    }
}

//////////////////////////////////////////////////////////////////////////////
//! Loads a Program file and prepends the cPreamble to the code.
//!
//! @return the source string if succeeded, 0 otherwise
//! @param cFilename        program filename
//! @param cPreamble        code that is prepended to the loaded file, typically a set of #defines or a header
//! @param szFinalLength    returned length of the code string
//////////////////////////////////////////////////////////////////////////////
char* _LoadProgSource(const char* cFilename, const char* cPreamble, unsigned int* szFinalLength)
{
    // locals 
    FILE* pFileStream = NULL;
    unsigned int szSourceLength;

    // open the OpenCL source code file
    #ifdef _WIN32   // Windows version
        if(fopen_s(&pFileStream, cFilename, "rb") != 0) 
        {       
            return NULL;
        }
    #else           // Linux version
        pFileStream = fopen(cFilename, "rb");
        if(pFileStream == 0) 
        {       
            return NULL;
        }
    #endif

    unsigned int szPreambleLength = strlen(cPreamble);

    // get the length of the source code
    fseek(pFileStream, 0, SEEK_END); 
    szSourceLength = ftell(pFileStream);
    fseek(pFileStream, 0, SEEK_SET); 

    // allocate a buffer for the source code string and read it in
    char* cSourceString = (char *)malloc(szSourceLength + szPreambleLength + 1); 
    memcpy(cSourceString, cPreamble, szPreambleLength);
    if (fread((cSourceString) + szPreambleLength, szSourceLength, 1, pFileStream) != 1)
    {
        fclose(pFileStream);
        free(cSourceString);
        return 0;
    }

    // close the file and return the total length of the combined (preamble + source) string
    fclose(pFileStream);
    if(szFinalLength != 0)
    {
        *szFinalLength = szSourceLength + szPreambleLength;
    }
    cSourceString[szSourceLength + szPreambleLength] = '\0';

    return cSourceString;
}

cl_command_queue getDefaultCommandQueue()
{
	return cqCommandQueue;
}

cl_kernel initGPUProgram(char * cSourceFileName, char* kernelName)
{
// Read the OpenCL kernel in from source file
    char* cSourceCL = _LoadProgSource(cSourceFileName, "", &szKernelLength);
	cl_kernel _ck;
    // Create the program
    cpProgram = clCreateProgramWithSource(cxGPUContext, 1, (const char **)&cSourceCL, &szKernelLength, &ciErr1);
    if (ciErr1 != CL_SUCCESS)
    {
		return NULL;
    }

    // Build the program with 'mad' Optimization option
    #ifdef MAC
        char* flags = "-cl-fast-relaxed-math -DMAC";
    #else
        char* flags = "-cl-fast-relaxed-math";
    #endif
    ciErr1 = clBuildProgram(cpProgram, 0, NULL, NULL, NULL, NULL);
    if (ciErr1 != CL_SUCCESS)
    {
        return NULL;
    }

    // Create the kernel
    _ck = clCreateKernel(cpProgram, kernelName, &ciErr1);
    if (ciErr1 != CL_SUCCESS)
    {
        return NULL;
    }
	ckKernel = _ck;
	return _ck;
}

cl_kernel initKernel(char* kernelName)
{
	cl_kernel _ck;
	// Create the kernel
    _ck = clCreateKernel(cpProgram, kernelName, &ciErr1);
    if (ciErr1 != CL_SUCCESS)
    {
        return NULL;
    }
	
	return _ck;
}

cl_mem getDeviceBufferF(int iNumElements)
{
	 // set and log Global and Local work size dimensions
    szLocalWorkSize = GPU_LOCAL_WORK_SIZE;
    szGlobalWorkSize = RoundUp((int)szLocalWorkSize, iNumElements);  // rounded up to the nearest multiple of the LocalWorkSize

	 // Allocate the OpenCL buffer memory objects for source and result on the device GMEM
    cl_mem cmDevSrcA = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY, sizeof(cl_float) * szGlobalWorkSize, NULL, &ciErr1);
	return cmDevSrcA;
}

cl_mem getDeviceBufferC(int iNumElements)
{
	 // set and log Global and Local work size dimensions
    szLocalWorkSize = GPU_LOCAL_WORK_SIZE;
    szGlobalWorkSize = RoundUp((int)szLocalWorkSize, iNumElements);  // rounded up to the nearest multiple of the LocalWorkSize

	 // Allocate the OpenCL buffer memory objects for source and result on the device GMEM
	cl_mem cmDevSrcA = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY, sizeof(cl_char) * szGlobalWorkSize, NULL, &ciErr1);
	return cmDevSrcA;
}

cl_mem getDeviceBufferI(int iNumElements)
{
	 // set and log Global and Local work size dimensions
    szLocalWorkSize = GPU_LOCAL_WORK_SIZE;
    szGlobalWorkSize = RoundUp((int)szLocalWorkSize, iNumElements);  // rounded up to the nearest multiple of the LocalWorkSize

	 // Allocate the OpenCL buffer memory objects for source and result on the device GMEM
    cl_mem cmDevSrcA = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY, sizeof(cl_int) * szGlobalWorkSize, NULL, &ciErr1);
	return cmDevSrcA;
}

cl_mem getDeviceBufferF4(int iNumElements)
{
	 // set and log Global and Local work size dimensions
    szLocalWorkSize = GPU_LOCAL_WORK_SIZE;
    szGlobalWorkSize = RoundUp((int)szLocalWorkSize, iNumElements);  // rounded up to the nearest multiple of the LocalWorkSize

	 // Allocate the OpenCL buffer memory objects for source and result on the device GMEM
    cl_mem cmDevSrcA = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY, sizeof(cl_float4) * szGlobalWorkSize, NULL, &ciErr1);
	return cmDevSrcA;
}
