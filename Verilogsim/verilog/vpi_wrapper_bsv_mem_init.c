/*
 * Generated by Bluespec Compiler, version 2016.03.beta1 (build 34761, 2016-03-16)
 * 
 * On Thu Jul  7 14:37:16 EDT 2016
 * 
 * To automatically register this VPI wrapper with a Verilog simulator use:
 *     #include "vpi_wrapper_bsv_mem_init.h"
 *     void (*vlog_startup_routines[])() = { bsv_mem_init_vpi_register, 0u };
 * 
 * For a Verilog simulator which requires a .tab file, use the following entry:
 * $imported_bsv_mem_init call=bsv_mem_init_calltf size=64 acc=rw:%TASK
 * 
 * For a Verilog simulator which requires a .sft file, use the following entry:
 * $imported_bsv_mem_init vpiSysFuncSized 64 unsigned
 */
#include <stdlib.h>
#include <vpi_user.h>
#include "bdpi.h"

/* the type of the wrapped function */
unsigned long long bsv_mem_init(unsigned long long , unsigned long long );

/* VPI wrapper function */
PLI_INT32 bsv_mem_init_calltf(PLI_BYTE8 *user_data)
{
  vpiHandle hCall;
  unsigned long long arg_1;
  unsigned long long arg_2;
  unsigned long long vpi_result;
  vpiHandle *handle_array;
  
  /* retrieve handle array */
  hCall = vpi_handle(vpiSysTfCall, 0);
  handle_array = vpi_get_userdata(hCall);
  if (handle_array == NULL)
  {
    vpiHandle hArgList;
    hArgList = vpi_iterate(vpiArgument, hCall);
    handle_array = malloc(sizeof(vpiHandle) * 3u);
    handle_array[0u] = hCall;
    handle_array[1u] = vpi_scan(hArgList);
    handle_array[2u] = vpi_scan(hArgList);
    vpi_put_userdata(hCall, handle_array);
    vpi_free_object(hArgList);
  }
  
  /* create return value */
  make_vpi_result(handle_array[0u], &vpi_result, DIRECT);
  
  /* copy in argument values */
  get_vpi_arg(handle_array[1u], &arg_1, DIRECT);
  get_vpi_arg(handle_array[2u], &arg_2, DIRECT);
  
  /* call the imported C function */
  vpi_result = bsv_mem_init(arg_1, arg_2);
  
  /* copy out return value */
  put_vpi_result(handle_array[0u], &vpi_result, DIRECT);
  
  /* free argument storage */
  free_vpi_args();
  vpi_free_object(hCall);
  
  return 0;
}

/* sft: $imported_bsv_mem_init vpiSysFuncSized 64 unsigned */

/* tab: $imported_bsv_mem_init call=bsv_mem_init_calltf size=64 acc=rw:%TASK */

PLI_INT32 bsv_mem_init_sizetf(PLI_BYTE8 *user_data)
{
  return 64u;
}

/* VPI wrapper registration function */
void bsv_mem_init_vpi_register()
{
  s_vpi_systf_data tf_data;
  
  /* Fill in registration data */
  tf_data.type = vpiSysFunc;
  tf_data.sysfunctype = vpiSizedFunc;
  tf_data.tfname = "$imported_bsv_mem_init";
  tf_data.calltf = bsv_mem_init_calltf;
  tf_data.compiletf = 0u;
  tf_data.sizetf = bsv_mem_init_sizetf;
  tf_data.user_data = "$imported_bsv_mem_init";
  
  /* Register the function with VPI */
  vpi_register_systf(&tf_data);
}
