FROM gcc:4.9
COPY . /upload
WORKDIR /upload
RUN g++ findrec.cpp -o findrec | g++ upload.cpp -o upload | g++ seek1.cpp -o seek1
CMD ["./main && ./findrec && ./seek1"]