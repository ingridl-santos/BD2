FROM ubuntu:latest

RUN mkdir -p /home/BD2

COPY . /home/BD2

CMD /home/BD2/upload 