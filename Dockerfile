FROM ubuntu:latest

RUN mkdir -p /home/tp2

COPY . /home/tp2

WORKDIR /home/tp2/
