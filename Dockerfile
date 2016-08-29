FROM emccode/mesos-build-module-dev:1.0.0
MAINTAINER EMC{code} <http://community.emccode.com>

# Install Dependencies
RUN apt-get update && apt-get install -y --no-install-recommends software-properties-common
RUN add-apt-repository ppa:masterminds/glide

# ----------------
# Install Golang
RUN apt-get update && apt-get install -y --no-install-recommends \
        curl \
        gcc \
        git \
        glide \
        libc6-dev \
    && rm -rf /var/lib/apt/lists/*

ENV GOLANG_VERSION 1.6.3
ENV GOLANG_DOWNLOAD_URL https://golang.org/dl/go$GOLANG_VERSION.linux-amd64.tar.gz
ENV GOLANG_DOWNLOAD_SHA256 cdde5e08530c0579255d6153b08fdb3b8e47caabbe717bc7bcd7561275a87aeb


RUN curl -fsSL "$GOLANG_DOWNLOAD_URL" -o golang.tar.gz \
    && echo "$GOLANG_DOWNLOAD_SHA256  golang.tar.gz" | sha256sum -c - \
    && tar -C /usr/local -xzf golang.tar.gz \
    && rm golang.tar.gz

ENV GOPATH /go
ENV PATH $GOPATH/bin:/usr/local/go/bin:$PATH

RUN mkdir -p "$GOPATH/src" "$GOPATH/bin" && chmod -R 777 "$GOPATH"


# ----------------
# Build libstorage
ENV GO_GIT_BASE /go/src/github.com/emccode
WORKDIR $GO_GIT_BASE
ENV LS_GIT_SOURCE https://github.com/emccode/libstorage.git
ENV LIBSTORAGE_VERSION=0.1.5 LS_GIT_CHECKOUT_HASH=e4cdd05 
RUN git clone $LS_GIT_SOURCE ${GO_GIT_BASE}/libstorage
WORKDIR /go/src/github.com/emccode/libstorage

RUN /usr/bin/make clean && /usr/bin/make version
RUN /usr/bin/make deps && /usr/bin/make build
# The libstorage build output will be in: /go/bin/ /go/pkg/

# This image contains a pre-built mesos with headers and binaries.
# This image also contains a pre-built libstorage with headers and shared libraries
# It is intended to support mesos isolator module development and production builds.
# A source code tree for the isolator should be mounted at /isolator if using the default entrypoint.

VOLUME ["/isolator"]

# To build Docker image:
# docker build -t <docker-user-name>/mesos-libstorage-build:<ver> -f Dockerfile .

# default COMMAND simply builds isolator
# use it like this:
# docker run -ti -v <path-to-git-clone>/mesos-module-libstorage/isolator/:/isolator <docker-user-name>/mesos-libstorage-build:<ver>

# to extract the newly built .so to the mounted source directory volume mount - you should over-ride the default CMD like this:
# docker run -ti -v <path-to-git-clone>/mesos-module-mesos-libstorage-build/isolator/:/isolator <docker-user-name>/mesos-libstorage-build:<ver> /bin/bash -c  '/usr/bin/make all && cp /isolator/build/.libs/libmesos_libstorage_isolator-${ISOLATOR_VERSION}.so /isolator/'