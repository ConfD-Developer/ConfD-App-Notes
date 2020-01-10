An image for creating a ConfD target package
============================================

Summary
-------

The result of running this container is a tar-ball with all files
required for a minimal ConfD target installation including support for
all northbound interfaces, as described in `section 32.2 Installing
ConfD on a target system from the ConfD User Guide`.

Note that for older ConfD releases target installation is described in
section 29.3 or 31.3 in the UG rather than 32.2.  Also note that the
exact list files required for target installation sometimes change
between releases and might need updating.  We have attempted to
describe the required files in a generic enough way to avoid version
dependencies but just be aware that it could happen that you have to
add/remove files from the package.

Prerequisites
-------------

1. The files included in this package.
2. ConfD Linux installer located in a sub directory named
   `resources/confd/<confd version>`.  Examples and documentation
   tar-ball are not needed since they aren't used in the target
   installation.  ConfD library source *is* needed if libcrypto 1.1
   support is required.

Steps
-----

1. Create the image for the specified ConfD version (Currently the
   container defaults to `ConfD 7.3`, the latest version at time of
   writing).  Pass the flag `--build-arg ver=<confd version>` to build
   for another ConfD version.

`$ docker build --tag target-package-builder:v7.3 .`

2. Generate confd target package (send the build result to stdout)

`$ docker run --rm --env STDOUT=1 target-package-builder:v7.3 > confd-target.tgz`

3. It's also possible to keep the container around and copy the
   resulting tar-ball using the `docker cp` command from the container
   to the host.  The environment variable `STDOUT=1` should not be set
   in this case.

`$ docker run --name target-builder target-package-builder:v7.3`
`$ docker cp target-builder:/confd-target.tgz .`
`$ docker rm target-builder`
