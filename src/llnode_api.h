// llnode C++ wrapper API for lldb SB API

namespace llnode {

int initSBTarget(char *filename, char *executable);
int getSBThreadCount();
int getSBFrameCount(int threadIndex);
int getSBFrame(int threadIndex, int frameIndex, int buffer_size, char *buffer);
int handleCommands(const char* command_line, char *buffer, int buffer_size);
} // namespace llnode
