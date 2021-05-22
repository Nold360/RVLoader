# Install DevkitPro for GC & Wii
FROM debian:buster-slim

# DevkitPro Version
ARG VERSION=1.0.2

# DevKitPro Packages to install
ARG PKG="gamecube-dev wii-dev"

# Dependend & Additional Libraries, separated by `|`
ARG LIBS="gamecube|wii|ppc"

# Environment Variables for DevkitPro
ENV DEVKITPRO=/opt/devkitpro
ENV DEVKITARM=/opt/devkitpro/devkitARM
ENV DEVKITPPC=/opt/devkitpro/devkitPPC
ENV PATH=${DEVKITPRO}/tools/bin:$PATH

RUN apt-get update && \
                apt-get -y --no-install-recommends install wget ca-certificates cmake bzip2 xz-utils make \
                && apt-get clean && \ 
        wget -nv -O devkitpro-pacman.amd64.deb https://github.com/devkitPro/pacman/releases/download/v1.0.2/devkitpro-pacman.amd64.deb && \
        dpkg -i devkitpro-pacman.amd64.deb ; apt-get -f -y install

RUN dkp-pacman -S -y --noconfirm ${PKG}
RUN dkp-pacman -S --needed --noconfirm $(dkp-pacman -Slq dkp-libs | grep -E "^($LIBS)-") && \
    dkp-pacman -Scc --noconfirm

USER 1000
VOLUME /src
WORKDIR /src
CMD [ "make" ]
