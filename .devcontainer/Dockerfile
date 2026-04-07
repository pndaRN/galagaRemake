FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
        gcc \
        make \
        libsdl2-dev \
        libsdl2-image-dev \
        xvfb \
        x11vnc \
        novnc \
        websockify \
    && rm -rf /var/lib/apt/lists/*
