FROM ubuntu:latest
RUN apt-get update
RUN apt-get install -y gcc make binutils libc6-dev gdb sudo
WORKDIR /workspace