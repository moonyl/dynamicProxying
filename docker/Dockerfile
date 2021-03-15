FROM debian:buster-slim

COPY dynamicProxying /usr/local/bin

RUN apt-get update && \
    apt-get remove --purge --auto-remove

CMD dynamicProxying
