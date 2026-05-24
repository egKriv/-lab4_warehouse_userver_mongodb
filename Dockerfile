FROM ghcr.io/userver-framework/ubuntu-22.04-userver-pg-dev:latest

ENV CCACHE_DIR=/home/user/inventory-api/.ccache \
    CORES_DIR=/cores \
    PREFIX=~/.local \
    SHELL=/bin/bash

USER user

WORKDIR /home/user/inventory-api

COPY . .

RUN make build-release

EXPOSE 8080

CMD ["/home/user/inventory-api/build-release/inventory-api", "--config", "/home/user/inventory-api/configs/static_config.yaml"]