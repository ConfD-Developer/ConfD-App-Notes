# ConfD Daemon Container Image

# Start from the debian stretch linux base image.
FROM debian:9
LABEL description="Docker image that demonstrates how to run ConfD inside a Docker container."
LABEL maintainer="jojohans@cisco.com"

# The environment needed by ConfD
ENV CONFD_DIR=/confd LD_LIBRARY_PATH=/confd/lib PATH=/confd/bin:$PATH

# Install extra packages needed to run ConfD.
RUN apt-get update && apt-get install -y --no-install-recommends \
        libssl1.0-dev \
        openssh-client \
        procps && mkdir -p ${CONFD_DIR}
RUN cd /usr/lib/x86_64-linux-gnu && ln -sv libcrypto.so.1.0.2 libcrypto.so.1.0.0

### Cleanup the image.
RUN apt-get autoremove -y && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# We will run ConfD as a non-root user.
RUN addgroup confd
RUN useradd confd -g confd
RUN mkdir -p ${CONFD_DIR} && chown confd:confd /confd

# Install ConfD in the container.  Note that this isn't a regular ConfD "installler"
# installation, instead we only include the files required for a minimal
# target installation as described in section 31.3. Installing ConfD on a
# target system in the ConfD User Guide.
COPY --chown=confd:confd resources/confd-target.tgz /tmp
USER confd
RUN tar xzf /tmp/confd-target.tgz -C ${CONFD_DIR} && rm /tmp/confd-target.tgz

# Generate host key and create internal "state" directories referenced
# by the default confd.conf from the package above.
RUN mkdir -p \
        ${CONFD_DIR}/etc/confd/ssh && \
        ssh-keygen -t rsa -f ${CONFD_DIR}/etc/confd/ssh/ssh_host_rsa_key -N '' > /dev/null

RUN mkdir -p \
        ${CONFD_DIR}/var/confd/candidate \
        ${CONFD_DIR}/var/confd/cdb \
        ${CONFD_DIR}/var/confd/log \
        ${CONFD_DIR}/var/confd/rollback \
        ${CONFD_DIR}/var/confd/state \
        ${CONFD_DIR}/var/confd/webui

# Add volumes and the working directory.
WORKDIR ${CONFD_DIR}

# Expose ports for required northbound protocols as necessary.
EXPOSE 2022 2024 4565
# NETCONF over TCP, HA replication and HTTP(S) are not used in this example.
#EXPOSE 2023 4569 8008 8088

# Start ConfD.
CMD ["/confd/bin/confd", "--foreground"]
