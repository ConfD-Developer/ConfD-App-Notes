An image for containerizing ConfD examples
==========================================

Prerequistes
------------

Steps
-----
1. Create the example builder image

`$ docker build --tag example-builder:v1 [--build-arg ver=<confd-version>] .`

2. Generate a ConfD example (the result is sent to stdout).  The
   output is the binaries, fxs-files, confd.conf etc.

`$ docker run --rm --env STDOUT=1 example-builder:v1 path/to/confd-example > example.tgz`

3. It's also possible to keep the container around and copy the
resulting tar-ball using the docker cp command from the container to
the host:
`$ docker run --name example-buider example-builder:v1`
`$ docker cp example-builder:/example.tgz .`
`$ docker container rm example-builder`
