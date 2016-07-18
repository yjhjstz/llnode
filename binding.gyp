{
  "targets": [
    {
      "target_name": "llnode_module",
      "sources": [ "src/llnode_module.cc", "src/llnode_api.cc", "src/llv8.cc", "src/llv8-constants.cc" ],
      "include_dirs": ["."],
      "conditions": [
        ["OS=='mac'", {
          "libraries": [ "/usr/local/opt/llvm36/lib/llvm-3.6/lib/liblldb.dylib" ],
          "include_dirs": ["./lldb/include"],
          "make_global_settings": [
            ["CXX","/usr/bin/clang++"],
            ["LINK","/usr/bin/clang++"],
          ],
          "xcode_settings": {
            "OTHER_CPLUSPLUSFLAGS" : ["-stdlib=libc++"],
           },
           # cflags as recommended by 'llvm-config --cxxflags'
           "cflags": [ "--enable-cxx11", "--enable-libcpp", "-std=c++11", "-stdlib=libc++", "-I/usr/local/opt/llvm36/lib/llvm-3.6/include  -DNDEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -g -O2 -fomit-frame-pointer -std=c++11 -fvisibility-inlines-hidden -fno-exceptions -fPIC -ffunction-sections -fdata-sections -Wcast-qual", ],
           # ldflags as recommended by 'llvm-config --ldflags'
           "ldflags": [ "-L/usr/local/opt/llvm36/lib/llvm-3.6/lib -llldb-3.6", ],
        }],
        ["OS=='linux'", {
          "libraries": [ "/usr/lib/llvm-3.6/lib/liblldb.so.1" ],
           # cflags as recommended by 'llvm-config --cxxflags'
           "cflags": [ "-I/usr/lib/llvm-3.6/include  -DNDEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -g -O2 -fomit-frame-pointer -std=c++11 -fvisibility-inlines-hidden -fno-exceptions -fPIC -ffunction-sections -fdata-sections -Wcast-qual", ],
           # ldflags as recommended by 'llvm-config --ldflags'
           "ldflags": [ "-L/usr/lib/llvm-3.6/lib -llldb-3.6", ],
        }],
      ],
    },
  ],
}

