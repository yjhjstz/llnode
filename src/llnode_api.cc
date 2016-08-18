#include <lldb/API/SBDebugger.h>
#include <lldb/API/SBProcess.h>
#include <lldb/API/SBTarget.h>
#include <lldb/API/SBThread.h>
#include <lldb/API/SBFrame.h>
#include <lldb/API/SBModule.h>
#include <lldb/API/SBCompileUnit.h>
#include <lldb/API/SBFileSpec.h>
#include <lldb/API/SBSymbol.h>

#include "src/llv8.h"
#include "string.h"

#include <dlfcn.h>
#include <unistd.h>

namespace llnode {

static bool loaded = false;

static lldb::SBDebugger debugger;
static lldb::SBTarget target;
static lldb::SBProcess process;

llnode::v8::LLV8 llv8;

//void (*LogOutputCallback) (const char *, void *baton);

void logCb(const char *s, void *baton) {
  fprintf(stderr, "%s", s);
  fprintf(stderr, "handle %p\n", baton);
}
/* Initialize the SB API and load the core dump */
int initSBTarget(char *filename, char *executable) {
    if ((!loaded)) {
        // lldb.so
        void* handle = dlopen("/usr/lib/x86_64-linux-gnu/liblldb-3.6.so", RTLD_LAZY | RTLD_GLOBAL);
        if (!handle) {
           printf("dlopen %s \n", dlerror());
           exit(1);
        }
        lldb::SBDebugger::Initialize();
        debugger = lldb::SBDebugger::Create(false, logCb, handle);
        loaded = true;
    }
    
    // Single instance target for now
    target = debugger.CreateTargetWithFileAndArch(executable, LLDB_ARCH_DEFAULT_64BIT);
    process = target.LoadCore(filename);

    // Load V8 constants from postmortem data
    llv8.Load(target);
    fprintf(stdout,"llnode_api.cc: SB loaded code dump %s\n",filename);
    return 0;
}

int getSBThreadCount() {
    return process.GetNumThreads();
}

int getSBFrameCount(int threadIndex) {
    lldb::SBThread thread = process.GetThreadAtIndex(threadIndex);
    return thread.GetNumFrames();
}

int getSBFrame(int threadIndex, int frameIndex, int buffer_size, char *buffer) {
    lldb::SBThread thread = process.GetThreadAtIndex(threadIndex);
    lldb::SBFrame frame = thread.GetFrameAtIndex(frameIndex);
    lldb::SBSymbol symbol = frame.GetSymbol();

    char* cursor = buffer;
    if (symbol.IsValid()) {
        cursor += sprintf(cursor,"Native: ");
        cursor += sprintf(cursor,"%s",frame.GetFunctionName());
        lldb::SBModule module = frame.GetModule();
        lldb::SBFileSpec moduleFileSpec = module.GetFileSpec();
        cursor += sprintf(cursor," [%s/%s]",moduleFileSpec.GetDirectory(),moduleFileSpec.GetFilename());
        lldb::SBCompileUnit compileUnit = frame.GetCompileUnit();
        lldb::SBFileSpec compileUnitFileSpec = compileUnit.GetFileSpec();
        if (compileUnitFileSpec.GetDirectory() != NULL || compileUnitFileSpec.GetFilename() != NULL) {
            cursor += sprintf(cursor,"\n\t [%s: %s]",compileUnitFileSpec.GetDirectory(),compileUnitFileSpec.GetFilename());
        }
    } else {
        // V8 frame
        llnode::v8::Error err;
        llnode::v8::JSFrame v8_frame(&llv8, static_cast<int64_t>(frame.GetFP()));
        std::string res = v8_frame.Inspect(true, err);

        // Skip invalid frames
        // fprintf(stdout,"JS string is [%s]\n",res.c_str());
        if (err.Fail() || strlen(res.c_str()) == 0 || strncmp(res.c_str(),"<",1) ==0) {
            if (strncmp(res.c_str(),"<",1) ==0) {
                cursor += sprintf(cursor,"Unknown: %s", res.c_str());
            } else {
                cursor += sprintf(cursor,"???");    
            }
        } else {
            // V8 symbol
            cursor += sprintf(cursor,"Javascript: %s", res.c_str());
        }
    }
    return 0;
}

int handleCommands(const char* command_line, char *buffer, int buffer_size) {
    lldb::SBCommandReturnObject command_result;
    lldb::SBCommandInterpreter interpreter = debugger.GetCommandInterpreter();
    interpreter.HandleCommand(command_line, command_result, false);
    fprintf(stderr, "cc:%s\n", command_line);
    // todo timeout
    if (!command_result.Succeeded())
    {
        fprintf (stderr, "error: couldn't exec '%s', status = %d\n", 
           command_result.GetError(), command_result.GetStatus());
        //return 1;
    }
    strncpy(buffer, command_result.GetOutput(), buffer_size - 1);
    return 0;
}

}  // namespace llnode
