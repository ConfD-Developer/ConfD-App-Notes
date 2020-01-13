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
1. Build the application builder container.  Note: this is a generic
   container and we must provide the application binary as a build
   argument as there's no way for the Dockerfile to know which file to
   include.

`$ docker build --tag name-of-app:v<ver> --build-arg app=name-of-app .`

2. Run the resulting container as usual.
`$ docker run -it --rm name-of-app:v<ver>`
