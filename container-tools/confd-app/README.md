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
1. Build the application builder container

`$ docker build --tag name-of-app:v<ver> .`

2. Run the resulting container as usual.
`$ docker run -it --rm name-of-app:v<ver>`
