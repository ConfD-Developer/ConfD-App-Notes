# ConfD client application container image
#
# Start from the debian stretch linux base image.
FROM debian:9.8
LABEL description="Docker image that demonstrates how to connect an application inside a Docker Container to a Container running ConfD."
LABEL maintainer="jojohans@cisco.com"

ENV CONFD_APP_DIR=/app PATH=/app:$PATH

# Install extra packages needed to run the app.

# Cleanup
RUN apt-get autoremove -y && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# # We will run the example as a non-root user
RUN addgroup user
RUN useradd user -g user
RUN mkdir -p /app && chown user:user /app
USER user

# Install the app in the container.
COPY --chown=user:user resources/app /app/
WORKDIR /app

#ENTRYPOINT ["init", "--"]
