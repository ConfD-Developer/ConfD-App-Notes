# ConfD Example application
#
# Install the glibc alpine linux base image.
FROM alpine:3.10.2
LABEL description="Docker image that demonstrates how to connect an application inside a Docker Container to a Container running ConfD."
LABEL maintainer="jojohans@cisco.com"

ENV CONFD_APP_DIR=/app PATH=/app:$PATH

# Default to latest glibc version.  Override on the command line with
# --build-arg ver=<version>
ARG glibc_ver=2.30.r0

# Install extra packages needed to run ConfD. We also need wget in order to be
# able to fetch glibc libraries.
RUN apk --no-cache add ca-certificates libcrypto1.0 tini wget
RUN wget -q -O /etc/apk/keys/sgerrand.rsa.pub https://alpine-pkgs.sgerrand.com/sgerrand.rsa.pub
RUN wget -q https://github.com/sgerrand/alpine-pkg-glibc/releases/download/$glibc_ver/glibc-$glibc_ver.apk
RUN wget -q https://github.com/sgerrand/alpine-pkg-glibc/releases/download/$glibc_ver/glibc-bin-$glibc_ver.apk
RUN apk add --no-cache glibc-$glibc_ver.apk glibc-bin-$glibc_ver.apk
RUN mkdir -p ${CONFD_APP_DIR}
x
# Cleanup
RUN apk del wget ca-certificates && rm -rf /tmp/* /var/cache/apk/* /glibc-$glibc_ver.apk /glibc-bin-$glibc_ver.apk

# # We will run the example as a non-root user
RUN addgroup user && adduser -D -G user user
USER user

# Install the app in the container.
COPY --chown=user:user resources/* ${CONFD_APP_DIR}/
WORKDIR /app

ENTRYPOINT ["tini", "--"]
