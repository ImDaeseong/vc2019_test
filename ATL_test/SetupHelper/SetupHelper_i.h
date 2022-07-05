

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for SetupHelper.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __SetupHelper_i_h__
#define __SetupHelper_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IShareFun_FWD_DEFINED__
#define __IShareFun_FWD_DEFINED__
typedef interface IShareFun IShareFun;

#endif 	/* __IShareFun_FWD_DEFINED__ */


#ifndef __ShareFun_FWD_DEFINED__
#define __ShareFun_FWD_DEFINED__

#ifdef __cplusplus
typedef class ShareFun ShareFun;
#else
typedef struct ShareFun ShareFun;
#endif /* __cplusplus */

#endif 	/* __ShareFun_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IShareFun_INTERFACE_DEFINED__
#define __IShareFun_INTERFACE_DEFINED__

/* interface IShareFun */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IShareFun;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("341a0f93-dc36-42fb-93dc-da97ac84f45f")
    IShareFun : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSystemPath( 
            /* [retval][out] */ VARIANT *vtPath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetProgramFilesPath( 
            /* [retval][out] */ VARIANT *vtPath) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IShareFunVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IShareFun * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IShareFun * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IShareFun * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IShareFun * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IShareFun * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IShareFun * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IShareFun * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSystemPath )( 
            IShareFun * This,
            /* [retval][out] */ VARIANT *vtPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetProgramFilesPath )( 
            IShareFun * This,
            /* [retval][out] */ VARIANT *vtPath);
        
        END_INTERFACE
    } IShareFunVtbl;

    interface IShareFun
    {
        CONST_VTBL struct IShareFunVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IShareFun_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IShareFun_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IShareFun_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IShareFun_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IShareFun_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IShareFun_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IShareFun_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IShareFun_GetSystemPath(This,vtPath)	\
    ( (This)->lpVtbl -> GetSystemPath(This,vtPath) ) 

#define IShareFun_GetProgramFilesPath(This,vtPath)	\
    ( (This)->lpVtbl -> GetProgramFilesPath(This,vtPath) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IShareFun_INTERFACE_DEFINED__ */



#ifndef __SetupHelperLib_LIBRARY_DEFINED__
#define __SetupHelperLib_LIBRARY_DEFINED__

/* library SetupHelperLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_SetupHelperLib;

EXTERN_C const CLSID CLSID_ShareFun;

#ifdef __cplusplus

class DECLSPEC_UUID("6da4f7dc-a952-4936-a557-433e324a3ac6")
ShareFun;
#endif
#endif /* __SetupHelperLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

unsigned long             __RPC_USER  VARIANT_UserSize64(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal64(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal64(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree64(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


