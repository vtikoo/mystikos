FROM openjdk:8-alpine

# https://github.com/docker-library/openjdk/issues/77#issuecomment-302871030
COPY ld-musl-x86_64.path /etc/ld-musl-x86_64.path
RUN mkdir -p /usr/lib/amd64/ \
 && cp -r \
    /usr/lib/jvm/java-1.8-openjdk/jre/lib/amd64/* \
    /usr/lib/amd64/

COPY Hello.java /app/Hello.java
WORKDIR /app

RUN javac ./Hello.java
