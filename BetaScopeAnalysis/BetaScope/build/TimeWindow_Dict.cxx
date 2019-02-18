// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkurodICodesdIHGTD_BetaScopedIBetaScopeAnalysisdIBetaScopedIbuilddITimeWindow_Dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/home/kuro/Codes/HGTD_BetaScope/BetaScopeAnalysis/BetaScope/include/TimeWindow.hpp"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TimeWindow(void *p = 0);
   static void *newArray_TimeWindow(Long_t size, void *p);
   static void delete_TimeWindow(void *p);
   static void deleteArray_TimeWindow(void *p);
   static void destruct_TimeWindow(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TimeWindow*)
   {
      ::TimeWindow *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TimeWindow >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TimeWindow", ::TimeWindow::Class_Version(), "BetaScope/include/TimeWindow.hpp", 6,
                  typeid(::TimeWindow), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TimeWindow::Dictionary, isa_proxy, 4,
                  sizeof(::TimeWindow) );
      instance.SetNew(&new_TimeWindow);
      instance.SetNewArray(&newArray_TimeWindow);
      instance.SetDelete(&delete_TimeWindow);
      instance.SetDeleteArray(&deleteArray_TimeWindow);
      instance.SetDestructor(&destruct_TimeWindow);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TimeWindow*)
   {
      return GenerateInitInstanceLocal((::TimeWindow*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TimeWindow*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TimeWindow::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TimeWindow::Class_Name()
{
   return "TimeWindow";
}

//______________________________________________________________________________
const char *TimeWindow::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TimeWindow*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TimeWindow::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TimeWindow*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TimeWindow::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TimeWindow*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TimeWindow::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TimeWindow*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TimeWindow::Streamer(TBuffer &R__b)
{
   // Stream an object of class TimeWindow.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TimeWindow::Class(),this);
   } else {
      R__b.WriteClassBuffer(TimeWindow::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TimeWindow(void *p) {
      return  p ? new(p) ::TimeWindow : new ::TimeWindow;
   }
   static void *newArray_TimeWindow(Long_t nElements, void *p) {
      return p ? new(p) ::TimeWindow[nElements] : new ::TimeWindow[nElements];
   }
   // Wrapper around operator delete
   static void delete_TimeWindow(void *p) {
      delete ((::TimeWindow*)p);
   }
   static void deleteArray_TimeWindow(void *p) {
      delete [] ((::TimeWindow*)p);
   }
   static void destruct_TimeWindow(void *p) {
      typedef ::TimeWindow current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TimeWindow

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 339,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlETimeWindowgR_Dictionary();
   static void vectorlETimeWindowgR_TClassManip(TClass*);
   static void *new_vectorlETimeWindowgR(void *p = 0);
   static void *newArray_vectorlETimeWindowgR(Long_t size, void *p);
   static void delete_vectorlETimeWindowgR(void *p);
   static void deleteArray_vectorlETimeWindowgR(void *p);
   static void destruct_vectorlETimeWindowgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TimeWindow>*)
   {
      vector<TimeWindow> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TimeWindow>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TimeWindow>", -2, "vector", 339,
                  typeid(vector<TimeWindow>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETimeWindowgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<TimeWindow>) );
      instance.SetNew(&new_vectorlETimeWindowgR);
      instance.SetNewArray(&newArray_vectorlETimeWindowgR);
      instance.SetDelete(&delete_vectorlETimeWindowgR);
      instance.SetDeleteArray(&deleteArray_vectorlETimeWindowgR);
      instance.SetDestructor(&destruct_vectorlETimeWindowgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TimeWindow> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TimeWindow>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETimeWindowgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TimeWindow>*)0x0)->GetClass();
      vectorlETimeWindowgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETimeWindowgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETimeWindowgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TimeWindow> : new vector<TimeWindow>;
   }
   static void *newArray_vectorlETimeWindowgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TimeWindow>[nElements] : new vector<TimeWindow>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETimeWindowgR(void *p) {
      delete ((vector<TimeWindow>*)p);
   }
   static void deleteArray_vectorlETimeWindowgR(void *p) {
      delete [] ((vector<TimeWindow>*)p);
   }
   static void destruct_vectorlETimeWindowgR(void *p) {
      typedef vector<TimeWindow> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TimeWindow>

namespace {
  void TriggerDictionaryInitialization_TimeWindow_Dict_Impl() {
    static const char* headers[] = {
"BetaScope/include/TimeWindow.hpp",
0
    };
    static const char* includePaths[] = {
"/home/kuro/App/ROOT/include",
"/home/kuro/Codes/HGTD_BetaScope/BetaScopeAnalysis/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TimeWindow_Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$BetaScope/include/TimeWindow.hpp")))  TimeWindow;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TimeWindow_Dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "BetaScope/include/TimeWindow.hpp"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TimeWindow", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TimeWindow_Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TimeWindow_Dict_Impl, {}, classesHeaders, /*has no C++ module*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TimeWindow_Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TimeWindow_Dict() {
  TriggerDictionaryInitialization_TimeWindow_Dict_Impl();
}
