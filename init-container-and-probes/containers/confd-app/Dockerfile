# ConfD Example application
#
# Install the glibc alpine linux base image.
FROM ubuntu:18.04
LABEL description="Docker image that demonstrates how to connect an application inside a Docker Container to a Container running ConfD."
LABEL maintainer="jojohans@cisco.com"

ARG app

ENV CONFD_APP_DIR=/app PATH=/app:$PATH

### Cleanup
RUN apt-get autoremove -y && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Install the app in the container.
RUN mkdir -p ${CONFD_APP_DIR}

# We will run the app as a non-root user.
RUN addgroup user
RUN useradd user -g user
USER user

COPY --chown=user:user resources/$app ${CONFD_APP_DIR}/
WORKDIR /app

CMD ["$app"]
