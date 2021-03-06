# Generic ConfD client application builder
#
# Start from the ubuntu linux base image.  The particular base image
# is not important, we use ubuntu because it makes it easy to create a
# complete build environment.
FROM ubuntu:18.04

LABEL description="Docker image to build ConfD examples."
LABEL maintainer="jojohans@cisco.com"

# Default to latest ConfD version.  Override on the command line with
# --build-arg ver=<version>
ARG ver=7.3

ENV CONFD_DIR=/confd LD_LIBRARY_PATH=/confd/lib PATH=/confd/bin:$PATH

# Install extra packages needed to run ConfDs build tools.
RUN apt-get update && apt-get install -y --no-install-recommends \
        libssl1.0-dev \
        build-essential \
        openssh-client && mkdir -p ${CONFD_DIR}

# Install ConfD in the container.
COPY resources/confd/$ver/* /tmp/
RUN /tmp/confd-$ver.linux.x86_64.installer.bin --skip-docs /confd
RUN mv /confd/src/confd/tools/Makefile.confd_cmd /confd/src/confd/tools/Makefile

# Add the build script.
COPY resources/builder /

# Cleanup
RUN apt-get autoremove -y && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Build an example
ENTRYPOINT ["/builder"]
