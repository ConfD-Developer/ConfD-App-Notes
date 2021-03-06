# Build ConfD target package

# Start from alpine linux base image.  The base image we use doesn't
# matter for this container, we just install ConfD and run the builder
# script.
FROM ubuntu:18.04
LABEL description="Docker image to generate the ConfD target installation package."
LABEL maintainer="jojohans@cisco.com"

ENV CONFD_DIR=/confd

# Default to latest version of ConfD.  Override on the command line
# with --build-arg ver=<different version>.
ARG ver=7.3

# Install extra packages needed to install ConfD and re-build libconfd..
RUN apt-get update && apt-get install -y --no-install-recommends \
        libssl1.0-dev \
        openssh-client && mkdir -p ${CONFD_DIR}

# Cleanup the image.
RUN apt-get autoremove -y && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# We will run ConfD as a non-root user.
RUN addgroup confd
RUN useradd confd -g confd
RUN mkdir -p ${CONFD_DIR} && chown confd:confd /confd

# Install ConfD in the container (no need for documentation or
# examples).  We expect to find the installer in a directory called
# confd/$ver.
COPY --chown=confd:confd resources/confd/$ver/* /tmp/
RUN /tmp/confd-$ver.linux.x86_64.installer.bin --skip-docs --skip-examples ${CONFD_DIR}

# Add the build script.
COPY --chown=confd:confd resources/builder /

# Remove spent resources
RUN rm -rf /tmp/confd-$ver.linux.x86_64.installer.bin

# Run the build script.
ENTRYPOINT ["/builder"]
