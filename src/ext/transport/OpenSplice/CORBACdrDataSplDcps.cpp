#include "CORBACdrDataSplDcps.h"
#include "ccpp_CORBACdrData.h"

#include <v_copyIn.h>
#include <v_topic.h>
#include <os_stdlib.h>
#include <string.h>
#include <os_report.h>
#include <iostream>

v_copyin_result
__OpenRTM_CORBACdrData__copyIn(
    c_base base,
    const struct ::OpenRTM::CORBACdrData *from,
    struct _OpenRTM_CORBACdrData *to)
{
    v_copyin_result result = V_COPYIN_RESULT_OK;
    (void) base;

    {
/* Code generated by C:\cygwin\home\dds\OvernightTests\tmp_VzclKidJVc\build\src\tools\idlpp\code\idl_genCorbaCxxCopyin.c at line 838 */
        static c_type type0 = NULL;
        c_type subtype0;
        c_ulong length0;
        c_octet *dest0;
        const ::OpenRTM::CORBACdrData::_data_seq *src = &from->data;

        if (type0 == NULL) {
            subtype0 = c_type(c_metaResolve (c_metaObject(base), "c_octet"));
            type0 = c_metaSequenceTypeNew(c_metaObject(base),"C_SEQUENCE<c_octet>",subtype0,0);
            c_free(subtype0);
        }
        length0 = (c_ulong)(*src).length();
#ifdef OSPL_BOUNDS_CHECK
         dest0 = (c_octet *)c_newSequence_s(c_collectionType(type0),length0);
         if(dest0 != NULL) {
            /* Code generated by C:\cygwin\home\dds\OvernightTests\tmp_VzclKidJVc\build\src\tools\idlpp\code\idl_genCorbaCxxCopyin.c at line 2041 */
            const c_octet *buf0;
            buf0 = (const c_octet *)(*src).get_buffer();
            memcpy (dest0,buf0,length0* sizeof(*dest0));
            to->data = (c_sequence)dest0;
         } else {
             result = V_COPYIN_RESULT_OUT_OF_MEMORY;
          }
#else
        dest0 = (c_octet *)c_newSequence_s(c_collectionType(type0),length0);
        if(dest0 != NULL) {
            /* Code generated by C:\cygwin\home\dds\OvernightTests\tmp_VzclKidJVc\build\src\tools\idlpp\code\idl_genCorbaCxxCopyin.c at line 2041 */
            const c_octet *buf0;
            buf0 = (const c_octet *)(*src).get_buffer();
            memcpy (dest0,buf0,length0* sizeof(*dest0));
            to->data = (c_sequence)dest0;
        } else {
            result = V_COPYIN_RESULT_OUT_OF_MEMORY;
        }
#endif
    }
    return result;
}

void
__OpenRTM_CORBACdrData__copyOut(
    const void *_from,
    void *_to)
{
    const struct _OpenRTM_CORBACdrData *from = (const struct _OpenRTM_CORBACdrData *)_from;
    struct ::OpenRTM::CORBACdrData *to = (struct ::OpenRTM::CORBACdrData *)_to;
    {
        long size0;
        const c_octet *src0 = (const c_octet *)from->data;
        ::OpenRTM::CORBACdrData::_data_seq *dst = &to->data;

        size0 = c_arraySize(c_sequence(from->data));
        to->data.length(size0);
        {
            c_octet *buf0;
            buf0 = (c_octet *)(*dst).get_buffer();
            memcpy ((void *)buf0,src0,size0* sizeof(*buf0));
        }
    }
}

