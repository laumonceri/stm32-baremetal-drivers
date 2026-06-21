FROM ubuntu:24.04

# Install the needed packages
RUN apt-get update && \
    apt-get install -y gcc-arm-none-eabi \
    build-essential pkg-config libcmocka-dev lcov \
    openocd picocom\
    cppcheck clang-format
 
# Create a non-root user with home directory and set working directory
ARG USERNAME=laumonceri
ARG USER_UID=1001
ARG USER_GID=1001

RUN groupadd -g ${USER_GID} ${USERNAME} || true && \
    useradd -m -u ${USER_UID} -g ${USER_GID} -s /bin/bash ${USERNAME} && \
    mkdir -p /home/${USERNAME} && \
    chown -R ${USERNAME}:${USERNAME} /home/${USERNAME}

ENV HOME=/home/${USERNAME}
USER ${USERNAME}