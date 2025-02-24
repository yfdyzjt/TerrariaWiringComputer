FROM ubuntu:latest

# xmake
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository ppa:xmake-io/xmake && \
    apt update && \
    apt install xmake
ENV XMAKE_ROOT=y

# tmake
RUN apt-get install -y curl dotnet-runtime-8.0 && \
    curl -L https://github.com/yfdyzjt/TMake/releases/latest/download/tmake-linux-x64.elf -o /usr/local/bin/tmake && \
    chmod +x /usr/local/bin/tmake

# riscv-none-elf-gcc
RUN curl -fsSL https://deb.nodesource.com/setup_22.x | bash - && \
    apt-get install -y nodejs && \
    npm install -g xpm@latest && \
    xpm init && \
    xpm install @xpack-dev-tools/riscv-none-elf-gcc@latest --verbose
ENV PATH=$PATH:/xpacks/@xpack-dev-tools/riscv-none-elf-gcc/.content/bin

WORKDIR /TerrariaWiringComputer

ENTRYPOINT ["/usr/bin/env"]
CMD ["/bin/bash"]
