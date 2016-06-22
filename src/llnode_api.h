// llnode C++ wrapper API for lldb SB API

namespace llnode {

int initSBTarget(char *filename);
int getSBThreadCount();
int getSBFrameCount(int threadIndex);
int getSBFrame(int threadIndex, int frameIndex, int buffer_size, char *buffer);

} // namespace llnode
