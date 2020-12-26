FROM ubuntu

ARG LIBSSHVERS=0.9
ARG LIBSSHVERSION=0.9.5

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Zurich

WORKDIR /build

# Get dependencies
RUN apt-get update -y
RUN apt-get install -y build-essential pkg-config qtbase5-dev cmake g++ libssl-dev xz-utils libzip-dev wget

# Manually build libssh
RUN wget https://www.libssh.org/files/$LIBSSHVERS/libssh-$LIBSSHVERSION.tar.xz && tar xf libssh-$LIBSSHVERSION.tar.xz && cd libssh-$LIBSSHVERSION && mkdir build && cd build && cmake .. && make -j$(nproc) && make install && cd ../.. && rm -rf /build/*

# Ready to build qjournalctl now :)
