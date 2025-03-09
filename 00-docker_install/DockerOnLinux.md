# Installing Docker on Linux-based OS and running AlmaLinux9
In this guide we provide step-by-step instructions for installing [docker](https://www.docker.com/) on linux-based OS and spinning up a container with `AlmaLinux9`, the base environment we will use for developing the projects of this course.

For doubts or problems, please contact me [here](mailto:alberto.coppi@studenti.unipd.it) or open an issue [here](https://github.com/c0pp1/ScientificComputing4PhyicsStudents-projects/issues/new/choose).

## 1. Install Docker
### 1.0 Prerequisites
To install Docker a linux system you need the following:
- A linux system :)
- A processor with virtualization capabilities and the virtualization feature enabled in your BIOS/UEFI

There are two ways to install Docker on your system: Docker Desktop is a graphical interface running its containers in a separate and dedicated VM storage; Docker engine is the command line interface.  
In principle you can have both installed on your system, but you can incur in problems in some particular circumstances, therefore this is discouraged.

For each install method the instructions for some specific distributions are provided

- [Install Docker Desktop](#11-docker-desktop-gui)
- [Install Docker engine](#12-docker-engine-cli)

### 1.1 Docker Desktop (GUI)
Docker Desktop is available for Ubuntu/Debian based distributions, Red Hat Enterprise Linux (RHEL) and Fedora. There is also an experimental package for ArchLinux but this is not officially supported.  
We will provide instructions for Ubuntu and Arch Linux (my personal system), for other systems refer to the [official documentation](https://docs.docker.com/desktop/setup/install/linux/).

#### 1.1.1 Ubuntu
Be sure to be on Ubuntu 22.04, 24.04 or the latest non-LTS and to have `gnome-terminal` installed if you are a non-gnome DE.  
Recommended approach to install Docker Desktop on Ubuntu:

1. Set up Docker's package repository.
    ```bash
    # Add Docker's official GPG key:
    sudo apt-get update
    sudo apt-get install ca-certificates curl
    sudo install -m 0755 -d /etc/apt/keyrings
    sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
    sudo chmod a+r /etc/apt/keyrings/docker.asc

    # Add the repository to Apt sources:
    echo \
        "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
        $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}") stable" | \
        sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
    sudo apt-get update
    ```
2. Download the latest [DEB package](https://desktop.docker.com/linux/main/amd64/docker-desktop-amd64.deb?utm_source=docker&utm_medium=webreferral&utm_campaign=docs-driven-download-linux-amd64).
3. Install the package with apt as follows:
    ```bash
    sudo apt-get update
    sudo apt-get install ./docker-desktop-amd64.deb
    ```
    > [!NOTE]  
    > At the end of the installation process, apt displays an error due to installing a downloaded package. You can ignore this error message.  
    > ```bash
    > N: Download is performed unsandboxed as root, as file '/home/user/Downloads/docker-desktop.deb' couldn't be accessed by user '_apt'. - pkgAcquire::Run (13: Permission denied)
    > ```

#### 1.1.2 Arch Linux
Docker Desktop for Arch Linux is still experimental. If you want to install it you can follow these steps:

1. Install the Docker client binary on Linux. Static binaries for the Docker client are available for Linux as docker. You can use:
    ```bash
    wget https://download.docker.com/linux/static/stable/x86_64/docker-28.0.1.tgz -qO- | tar xvfz - docker/docker --strip-components=1
    mv ./docker /usr/local/bin
    ```
2. Download the latest Arch package from the [Release notes](https://docs.docker.com/desktop/release-notes/) (this is the [direct link](https://desktop.docker.com/linux/main/amd64/184744/docker-desktop-x86_64.pkg.tar.zst) for version 4.39.0).
3. Install the package:
    ```bash
    sudo pacman -U ./docker-desktop-x86_64.pkg.tar.zst
    ```

See the [ArchWiki](https://wiki.archlinux.org/title/Docker) for more information.


### 1.2 Docker engine (CLI)
This section describes how to install Docker Engine on Linux, also known as Docker CE.  
As before, we provide installation onstructions for Ubuntu and Arch Linux.

#### 1.2.1 Ubuntu
To install docker-ce do the following:
1. Set up Docker's package repository.
    ```bash
    # Add Docker's official GPG key:
    sudo apt-get update
    sudo apt-get install ca-certificates curl
    sudo install -m 0755 -d /etc/apt/keyrings
    sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
    sudo chmod a+r /etc/apt/keyrings/docker.asc

    # Add the repository to Apt sources:
    echo \
        "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
        $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}") stable" | \
        sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
    sudo apt-get update
    ```
2. Install the Docker packages.
    ```bash
    sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
    ```
3. Verify that the installation is successful by running the hello-world image:
    ```bash
    sudo docker run hello-world
    ```
To avoid the need of running `docker` with `sudo`, it is convenient to create and add the current user to the `docker` group. To do that follow these steps:
1. Create the `docker` group.
    ```bash
    sudo groupadd docker
    ```
2. Add your user to the `docker` group.
    ```bash
    sudo usermod -aG docker $USER
    ```
3. Log out and log back in so that your group membership is re-evaluated or run `newgrp docker`.

#### 1.2.2 Arch Linux
1. Install the `docker` package.
    ```bash
    sudo pacman -S docker
    ```
2. Next enable/start `docker.service` or `docker.socket`. Note that `docker.service` starts the service on boot, whereas `docker.socket` starts docker on first usage which can decrease boot times.
    ```bash
    sudo systemctl enable docker.socket
    sudo systemctl start docker.socket
    ```

### 1.3 Daemon configuration file
The Docker daemon can be configured to meet your specific needs through a configuration file. The default location of the configuration file on Linux is `/etc/docker/daemon.json`.  
As an example, you can specify a different location for storing docker data (which can easily occupy $\mathcal{O}(10)$ GB os disk space), like a secondary hard disk. To do so specify the `"data-root": "<your-preferred-path>"` property in the config file.

The following is a full example of the allowed configuration options on Linux:
```json
{
  "authorization-plugins": [],
  "bip": "",
  "bip6": "",
  "bridge": "",
  "builder": {
    "gc": {
      "enabled": true,
      "defaultKeepStorage": "10GB",
      "policy": [
        { "keepStorage": "10GB", "filter": ["unused-for=2200h"] },
        { "keepStorage": "50GB", "filter": ["unused-for=3300h"] },
        { "keepStorage": "100GB", "all": true }
      ]
    }
  },
  "cgroup-parent": "",
  "containerd": "/run/containerd/containerd.sock",
  "containerd-namespace": "docker",
  "containerd-plugins-namespace": "docker-plugins",
  "data-root": "",
  "debug": true,
  "default-address-pools": [
    {
      "base": "172.30.0.0/16",
      "size": 24
    },
    {
      "base": "172.31.0.0/16",
      "size": 24
    }
  ],
  "default-cgroupns-mode": "private",
  "default-gateway": "",
  "default-gateway-v6": "",
  "default-network-opts": {},
  "default-runtime": "runc",
  "default-shm-size": "64M",
  "default-ulimits": {
    "nofile": {
      "Hard": 64000,
      "Name": "nofile",
      "Soft": 64000
    }
  },
  "dns": [],
  "dns-opts": [],
  "dns-search": [],
  "exec-opts": [],
  "exec-root": "",
  "experimental": false,
  "features": {
    "cdi": true,
    "containerd-snapshotter": true
  },
  "fixed-cidr": "",
  "fixed-cidr-v6": "",
  "group": "",
  "host-gateway-ip": "",
  "hosts": [],
  "proxies": {
    "http-proxy": "http://proxy.example.com:80",
    "https-proxy": "https://proxy.example.com:443",
    "no-proxy": "*.test.example.com,.example.org"
  },
  "icc": false,
  "init": false,
  "init-path": "/usr/libexec/docker-init",
  "insecure-registries": [],
  "ip": "0.0.0.0",
  "ip-forward": false,
  "ip-masq": false,
  "iptables": false,
  "ip6tables": false,
  "ipv6": false,
  "labels": [],
  "live-restore": true,
  "log-driver": "json-file",
  "log-format": "text",
  "log-level": "",
  "log-opts": {
    "cache-disabled": "false",
    "cache-max-file": "5",
    "cache-max-size": "20m",
    "cache-compress": "true",
    "env": "os,customer",
    "labels": "somelabel",
    "max-file": "5",
    "max-size": "10m"
  },
  "max-concurrent-downloads": 3,
  "max-concurrent-uploads": 5,
  "max-download-attempts": 5,
  "mtu": 0,
  "no-new-privileges": false,
  "node-generic-resources": [
    "NVIDIA-GPU=UUID1",
    "NVIDIA-GPU=UUID2"
  ],
  "pidfile": "",
  "raw-logs": false,
  "registry-mirrors": [],
  "runtimes": {
    "cc-runtime": {
      "path": "/usr/bin/cc-runtime"
    },
    "custom": {
      "path": "/usr/local/bin/my-runc-replacement",
      "runtimeArgs": [
        "--debug"
      ]
    }
  },
  "seccomp-profile": "",
  "selinux-enabled": false,
  "shutdown-timeout": 15,
  "storage-driver": "",
  "storage-opts": [],
  "swarm-default-advertise-addr": "",
  "tls": true,
  "tlscacert": "",
  "tlscert": "",
  "tlskey": "",
  "tlsverify": true,
  "userland-proxy": false,
  "userland-proxy-path": "/usr/libexec/docker-proxy",
  "userns-remap": ""
}
```
