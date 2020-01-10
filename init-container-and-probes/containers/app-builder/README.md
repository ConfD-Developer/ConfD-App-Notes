An image for Containerizing ConfD client applications
=====================================================

Summary
-------
Create the app builder image

Prerequisites
-------------
Put application files to include in the container in the resources
directory.

Steps
-----
1. Build the application builder container (override ConfD version
   with `--build-arg ver=<ver>`).

`$ docker build --tag app-builder:v7.3 .`

2. Generate a ConfD app by running the builder container.  The result,
   a container image containing the ConfD client application, is sent
   to stdout.

`$ docker run --rm --env STDOUT=1 -v path/to/src-code:/src-code app-builder:v7.3 /src-code > app.tgz`

3. It's also possible to keep the container around and copy the
   resulting tar-ball using the docker cp command from the container
   to the host

`$ docker run --name app-builder app-builder:v7.3`
`$ docker cp app-builder:/app.tgz .`
`$ docker container rm app-builder`
