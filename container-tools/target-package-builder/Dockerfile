# Build ConfD target package

# Start from alpine linux base image.  The base image we use doesn't
# matter for this container, we just install ConfD and run the builder
# script.
FROM alpine:3.10.3
LABEL description="Docker image to generate the ConfD target installation package."
LABEL maintainer="jojohans@cisco.com"

ENV CONFD_DIR=/confd

# Default to latest version of ConfD.  Override on the command line
# with --build-arg ver=<version>.
ARG ver=7.3

# Install extra packages needed to install ConfD and re-build libconfd..
RUN apk --no-cache add alpine-sdk openssh-keygen linux-headers openssl-dev

# Cleanup the image.
RUN rm -rf /var/cache/apk/*

# We will run ConfD as a non-root user.
RUN addgroup confd && adduser -D -G confd confd
RUN mkdir -p ${CONFD_DIR} && chown confd:confd ${CONFD_DIR}
USER confd

# Install ConfD in the container (no need for documentation or
# examples).  We expect to find the installer in a directory called
# confd/<ver>.
COPY --chown=confd:confd resources/confd/$ver/* /tmp/
RUN sh -c "/tmp/confd-$ver.linux.x86_64.installer.bin --skip-docs --skip-examples ${CONFD_DIR} &> /dev/null"
RUN tar xzf /tmp/confd-$ver.libconfd.tar.gz --strip-components 1 -C $CONFD_DIR

# Add the build script.
COPY --chown=confd:confd resources/builder /

# Remove spent resources
RUN rm -rf /tmp/confd-$ver.linux.x86_64.installer.bin /tmp/confd-$ver.libconfd.tar.gz

# Run the build script.
ENTRYPOINT ["/builder"]
