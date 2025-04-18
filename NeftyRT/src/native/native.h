//
// Created by NeftyOliver on 25. 4. 18.
//

#ifndef NATIVE_H
#define NATIVE_H
#include <cstdint>

#ifdef _WIN64
#include <windows.h>
#endif

#define MAKE_FUNCTION_TYPE(returnType, funcPtr, param ...) typedef returnType (*funcPtr##_T)(param)

typedef union PTR64_ {
  void * asVoidPtr;
  uint64_t asUint64;
} PTR64;

inline PTR64 ptr64FromVoidPtr(void * paramPtr) {
  PTR64 ptr;
  ptr.asVoidPtr = paramPtr;
  return ptr;
};

inline PTR64 ptr64FromAddress(const uint64_t paramPtr) {
  PTR64 ptr;
  ptr.asUint64 = paramPtr;
  return ptr;
};

typedef union WindowPointer_ {
  void * asVoidPtr;
  PTR64 asPointer;

/*
 * TODO: X11 and Cocoa support
 */

  HWND * adWin32Window;
} WindowPointer;

typedef PTR64 * ( * GenericFunctionPtr)();

typedef union FunctionPTR64_ {
  void * asVoidPtr;
  uint64_t asUint64;
  PTR64 asPTR64;
  GenericFunctionPtr asFunction;
} FunctionPTR64; //You have to remember the parameters.

inline FunctionPTR64 ptr64ToFunctionPtr(PTR64 ptr64) {
  FunctionPTR64 functionPtr;
  functionPtr.asVoidPtr = ptr64.asVoidPtr;
  return functionPtr;
}


typedef struct WindowObject_ {
  const char * name;
  struct {
    uint32_t width;
    uint32_t height;
    uint32_t x;
    uint32_t y;
  } Dimension;
  bool resizable;
  WindowPointer window;
} WindowObject;

void * createWindowAsPointer(const char * windowName, uint32_t width, uint32_t height, uint32_t x, uint32_t y, bool resizable);

#endif //NATIVE_H
