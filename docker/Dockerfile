FROM buildpack-deps:xenial

MAINTAINER yjhjstz@gmail.com

USER root

VOLUME /data

WORKDIR /home

RUN apt-get update
# install lldb
RUN apt-get install -y git lldb-3.8 lldb-3.8-dev gcc g++ gdb make lldb

RUN apt-get install -y  nodejs npm

RUN git clone -b llnode-docker https://github.com/yjhjstz/llnode.git

WORKDIR llnode
RUN git clone https://chromium.googlesource.com/external/gyp.git tools/gyp \
&& git clone --depth=1 -b release_38 https://github.com/llvm-mirror/lldb.git lldb
RUN ./gyp_llnode -Dlldb_dir=/usr/lib/llvm-3.8/

# complier *.so
RUN make -C out && make install-linux

#RUN npm install --unsafe-perm

EXPOSE 2000

COPY docker-entrypoint.sh /
ENTRYPOINT [ "/docker-entrypoint.sh" ]