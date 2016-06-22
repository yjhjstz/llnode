{
  "targets": [
    {
      "target_name": "llnode_module",
      "sources": [ "src/llnode_module.cc", "src/llnode_api.cc", "src/llv8.cc", "src/llv8-constants.cc" ],
      "include_dirs": ["."],
      "libraries": [ "/usr/lib/llvm-3.6/lib/liblldb.so.1" ],
      "conditions": [
        ["OS=='linux'", {
           # cflags as recommended by 'llvm-config --cxxflags'
           "cflags": [ "-I/usr/lib/llvm-3.6/include  -DNDEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -g -O2 -fomit-frame-pointer -std=c++11 -fvisibility-inlines-hidden -fno-exceptions -fPIC -ffunction-sections -fdata-sections -Wcast-qual", ],
           # ldflags as recommended by 'llvm-config --ldflags'
           "ldflags": [ "-L/usr/lib/llvm-3.6/lib -llldb-3.6", ],
        }],
      ],
    },
  ],
}

