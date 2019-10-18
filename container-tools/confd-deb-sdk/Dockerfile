# Generic ConfD client application builder
#
# Start from the debian linux base image.
FROM debian:9.8

LABEL description="Docker image to build ConfD client applications."
LABEL maintainer="jojohans@cisco.com"

# Default to latest ConfD version.  Override on the command line with
# --build-arg ver=<version>
ARG ver=7.2.0.1

ENV CONFD_DIR=/confd LD_LIBRARY_PATH=/confd/lib PATH=/confd/bin:$PATH

# Install extra packages needed to run ConfDs build tools.
RUN apt-get update && apt-get install -y --no-install-recommends \
        libssl1.0-dev \
        build-essential \
        openssh-client && mkdir -p ${CONFD_DIR}
RUN ln -sv /usr/lib/x86_64-linux-gnu/libcrypto.so.1.0.2 /usr/lib/x86_64-linux-gnu/libcrypto.so.1.0.0

# Install ConfD in the container.
COPY resources/confd/$ver/* /tmp/
RUN /tmp/confd-$ver.linux.x86_64.installer.bin --skip-docs /confd
COPY resources/* /

# Cleanup
RUN apt-get autoremove -y && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Build the application
ENTRYPOINT ["/builder"]
