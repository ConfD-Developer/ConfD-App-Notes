# ConfD Container Image

# Start from the latest Alpine Linux base image.
#
# Note: this assume that libconfd.so and crypto.so has been rebuilt to
# use libcrypto 1.1.
FROM alpine:3.10.3
LABEL description="Docker image that demonstrates how to run ConfD inside a Docker container."
LABEL maintainer="jojohans@cisco.com"

ENV CONFD_DIR=/confd LD_LIBRARY_PATH=/confd/lib PATH=/confd/bin:$PATH

# Default to latest glibc version.  Override on the command line with
# --build-arg glibc_ver=<version>
ARG glibc_ver=2.30-r0

# Install extra packages needed to run ConfD.  Some packages are only
# needed when we build the container and during and are removed before
# we finish to keep the size small..
RUN apk --no-cache add ca-certificates ncurses5-libs openssh-keygen sudo tini wget
RUN echo 'confd ALL=(ALL) NOPASSWD:ALL' > /etc/sudoers.d/99_confd && chmod 440 /etc/sudoers.d/99_confd

# Install glibc for alpine linux.
RUN wget -q -O /etc/apk/keys/sgerrand.rsa.pub https://alpine-pkgs.sgerrand.com/sgerrand.rsa.pub
RUN wget -q https://github.com/sgerrand/alpine-pkg-glibc/releases/download/$glibc_ver/glibc-$glibc_ver.apk
RUN wget -q https://github.com/sgerrand/alpine-pkg-glibc/releases/download/$glibc_ver/glibc-bin-$glibc_ver.apk
RUN apk add --no-cache glibc-$glibc_ver.apk glibc-bin-$glibc_ver.apk

# Cleanup the image.
RUN apk del ca-certificates wget && rm -rf /tmp/* /var/cache/apk/* /glibc-$glibc_ver.apk /glibc-bin-$glibc_ver.apk

RUN ln -svf /usr/glibc-compat/lib/libc.so.6 /lib/libz.so.1

# We will run ConfD as a non-root user.
RUN addgroup confd && adduser -D -G confd confd
RUN mkdir -p ${CONFD_DIR} && chown confd:confd ${CONFD_DIR}
USER confd

# Install ConfD in the container.  This is not a regular "ConfD
# installer" installation. We only include the files required for a
# minimal target installation, as described in section 31.3. Installing
# ConfD on a target system in the ConfD User Guide.
COPY --chown=confd:confd resources/confd-target.tgz /tmp
RUN tar xzf /tmp/confd-target.tgz -C ${CONFD_DIR} && rm /tmp/confd-target.tgz

# Generate host key and create internal "state" directories referenced
# by the default confd.conf from the package above.
RUN mkdir -p \
        ${CONFD_DIR}/etc/confd/ssh && \
        ssh-keygen -t rsa -f ${CONFD_DIR}/etc/confd/ssh/ssh_host_rsa_key -N '' >/dev/null

RUN mkdir -p \
        ${CONFD_DIR}/var/confd/candidate \
        ${CONFD_DIR}/var/confd/cdb \
        ${CONFD_DIR}/var/confd/log \
        ${CONFD_DIR}/var/confd/rollback \
        ${CONFD_DIR}/var/confd/state \
        ${CONFD_DIR}/var/confd/webui

### This confd_cmd is enhanced to support domain names.
##COPY resources/confd_cmd ${CONFD_DIR}/bin

# Add volumes and the working directory.
WORKDIR ${CONFD_DIR}

# Ports for other northbound protocols as necessary.
EXPOSE 2022 2024 4565
# NETCONF over TCP, HA replication and HTTP(S) are not used in this example.
#EXPOSE 2023 4569 8008 8088

# Finally, start ConfD.
ENTRYPOINT ["tini", "--"]
CMD ["/confd/bin/confd", "--foreground"]
