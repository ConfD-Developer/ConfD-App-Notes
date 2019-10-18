An image for Containerizing ConfD examples
==========================================

Summary
-------
Create the example image

Prerequisites
-------------
Put application files to include in the container in the resources
directory.

Steps
-----
Build the application builder container

$ docker build --tag app-builder:v6.7.3 [--build-arg ver=6.7.2] .

Generate a ConfD app container image by running the builder container.
The result, a container image containing the ConfD client application,
is sent to stdout.
$ docker run --rm --env STDOUT=1 example-builder:v1 path/to/confd-example > app.tgz

It's also possible to keep the container around and copy the resulting
tar-ball using the docker cp command from the container to the host:
$ docker run --name app-builder app-builder:v6.7.3
$ docker cp app-builder:/app.tgz .
$ docker container rm app-builder
