// Javascript module API for llnode/lldb
#include <string>
#include <node.h>
#include <node_object_wrap.h>
#include "src/llnode_api.h"

namespace llnode {

using v8::FunctionCallbackInfo;
using v8::Exception;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Number;




void LoadDump(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() < 2) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong amount of args")));
    return;
  }

  String::Utf8Value filename(args[0]);
  String::Utf8Value executable(args[1]);
  initSBTarget(*filename, *executable);
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "OK"));
}

void GetThreadCount(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  args.GetReturnValue().Set(Number::New(isolate, getSBThreadCount()));
}

void GetFrameCount(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    args.GetReturnValue().Set(Number::New(isolate, getSBFrameCount(args[0]->NumberValue())));
}

void GetFrame(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  char buffer[4096];

  getSBFrame(args[0]->NumberValue(), args[1]->NumberValue(), 4096, buffer);
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, buffer));
}


#define FUNC_TYPE_LIST(V)   \
  V("jssource", Jssource)   \
  V("nodeinfo", Nodeinfo)   \
  V("findjsobjects", Findjsobjects) \

#define FUNC_TYPE_LIST_ARG1(V)   \
  V("jsstack", Jsstack)          \
  V("jsprint", JsPrint)          \
  V("findjsinstances", Findjsinstances) 

// not args
#define F(cmd, name)                                  \
  void GetV8##name(const FunctionCallbackInfo<Value>& args) {        \
    char buffer[4096];                                               \
    Isolate* isolate = args.GetIsolate();                            \
    handleCommands(cmd, buffer, 4096);                               \
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, buffer)); \
  }
FUNC_TYPE_LIST(F)
#undef F

// args0
#define F1(cmd, name)                                  \
  void GetV8##name(const FunctionCallbackInfo<Value>& args) {        \
    char buffer[4096];                                               \
    Isolate* isolate = args.GetIsolate();                            \
    if (args.Length() < 1) {                                         \
      isolate->ThrowException(Exception::TypeError(                  \
        String::NewFromUtf8(isolate, "Wrong amount of args")));      \
      return;                                                        \
    }                                                                \
    String::Utf8Value expr(args[0]);                                 \
    std::string cmdline = cmd + std::string(" ") + std::string(*expr);\
    handleCommands(cmdline.c_str(), buffer, 4096);                   \
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, buffer)); \
  }
FUNC_TYPE_LIST_ARG1(F1)
#undef F1


void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "loadDump", LoadDump);
  NODE_SET_METHOD(exports, "getThreadCount", GetThreadCount);
  NODE_SET_METHOD(exports, "getFrameCount", GetFrameCount);
  NODE_SET_METHOD(exports, "getFrame", GetFrame);
#define S(cmd, name) NODE_SET_METHOD(exports, cmd, GetV8##name);
FUNC_TYPE_LIST(S)
FUNC_TYPE_LIST_ARG1(S)
#undef S

}

NODE_MODULE(llnode_module, init)

#undef FUNC_TYPE_LIST
#undef FUNC_TYPE_LIST_ARG1

}  // namespace llnode

