# Installing Docker on Linux-based OS and running AlmaLinux9

In this guide we provide step-by-step instructions for installing [docker](https://www.docker.com/) on Linux-based OS and spinning up a container with `AlmaLinux9`, the base environment we will use for developing the projects of this course.

For doubts or problems, please contact me [here](mailto:alberto.coppi@studenti.unipd.it) or open an issue [here](https://github.com/c0pp1/ScientificComputing4PhyicsStudents-projects/issues/new/choose).

## Table of Contents

- [Installing Docker on Linux-based OS and running AlmaLinux9](#installing-docker-on-linux-based-os-and-running-almalinux9)
  - [Table of Contents](#table-of-contents)
  - [1. Install Docker](#1-install-docker)
    - [1.0 Prerequisites](#10-prerequisites)
    - [1.1 Docker Desktop (GUI)](#11-docker-desktop-gui)
      - [1.1.1 Ubuntu](#111-ubuntu)
      - [1.1.2 Arch Linux](#112-arch-linux)
      - [1.1.3 Fedora](#113-fedora)
    - [1.2 Docker engine (CLI)](#12-docker-engine-cli)
      - [1.2.1 Ubuntu](#121-ubuntu)
      - [1.2.2 Arch Linux](#122-arch-linux)
      - [1.2.3 Fedora](#123-fedora)
    - [1.3 Post-installation (optional)](#13-post-installation-optional)
      - [1.3.1 Avoid running explicitly docker as su](#131-avoid-running-explicitly-docker-as-su)
      - [1.3.2 Daemon configuration file](#132-daemon-configuration-file)
  - [2. Run AlmaLinux 9 in a container](#2-run-almalinux-9-in-a-container)
    - [2.1 Using Docker Desktop](#21-using-docker-desktop)
    - [2.2 Using Docker CLI](#22-using-docker-cli)
  - [3. Attach Visual Studio Code to a container](#3-attach-visual-studio-code-to-a-container)
    - [3.0 Install the Remote - Containers extension](#30-install-the-remote---containers-extension)
    - [3.1 Attach to a running container](#31-attach-to-a-running-container)
    - [3.2 Configure the `devcontainer.json`](#32-configure-the-devcontainerjson)

## 1. Install Docker

### 1.0 Prerequisites

To install Docker a Linux system you need the following:

- A Linux system :)
- A processor with virtualization capabilities and the virtualization feature enabled in your BIOS/UEFI

There are two ways to install Docker on your system: Docker Desktop is a graphical interface running its containers in a separate and dedicated VM storage; Docker engine is the command line interface.  
In principle you can have both installed on your system, but you can incur in problems in some particular circumstances, therefore this is discouraged.

For each install method the instructions for some specific distributions are provided

- [Install Docker Desktop](#11-docker-desktop-gui)
- [Install Docker engine](#12-docker-engine-cli)

### 1.1 Docker Desktop (GUI)

Docker Desktop is available for Ubuntu/Debian based distributions, Red Hat Enterprise Linux (RHEL) and Fedora. There is also an experimental package for ArchLinux but this is not officially supported.
We will provide instructions for Ubuntu, Arch Linux, and Fedora. For other systems refer to the [official documentation](https://docs.docker.com/desktop/setup/install/linux/).

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
>
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

#### 1.1.3 Fedora

Docker Desktop is available for Fedora as an RPM package.

1. Set up Docker's repository. This step requires the package `dnf-plugins-core`, which is downloaded in the first command. The second command adds the repository.

    ```bash
    sudo dnf -y install dnf-plugins-core
    sudo dnf-3 config-manager --add-repo https://download.docker.com/linux/fedora/docker-ce.repo
    ```

2. Download the latest [RPM package](https://desktop.docker.com/linux/main/amd64/docker-desktop-x86_64.rpm?utm_source=docker&utm_medium=webreferral&utm_campaign=docs-driven-download-linux-amd64). If you prefer to use the terminal, just run:

    ```bash
    wget https://desktop.docker.com/linux/main/amd64/docker-desktop-x86_64.rpm
    ```

3. Install the package with dnf as follows:

    ```bash
    sudo dnf install ./docker-desktop-x86_64.rpm
    ```

### 1.2 Docker engine (CLI)

This section describes how to install Docker Engine on Linux, also known as Docker CE.  
As before, we provide installation instructions for Ubuntu and Arch Linux.

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

#### 1.2.3 Fedora

To install docker-ce do the following:

1. Set up Docker's repository. This step requires the package `dnf-plugins-core`, which is downloaded in the first command. The second command adds the repository.

    ```bash
    sudo dnf -y install dnf-plugins-core
    sudo dnf-3 config-manager --add-repo https://download.docker.com/linux/fedora/docker-ce.repo
    ```

2. Install the Docker packages.

    ```bash
    sudo dnf install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
    ```

3. Start docker engine.

    ```bash
    sudo systemctl start docker
    ```

> [!NOTE]  
> The previous command will start the Docker Engine services for your current boot. If you wish the service to start automatically, you can enable the service.
>
> ```bash
> sudo systemctl enable docker
> ```

### 1.3 Post-installation (optional)

You can verify that the installation is successful by running the Docker hello-world image:

```bash
sudo docker run hello-world
```

Following the installation, it is recommended to do some follow-up configuration.
The post-installation actions are more thoroughly documented in the [official documentation](https://docs.docker.com/engine/install/linux-postinstall/), but we will show here two very useful configuration steps...

#### 1.3.1 Avoid running explicitly docker as su

To avoid the need of running `docker` with `sudo`, it is convenient to create and add the current user to the `docker` group. This procedure is generally not dependent on the Linux system you are running on. Follow these steps:

1. Create a new `docker` group.

    ```bash
    sudo groupadd docker
    ```

2. Add your user to the `docker` group.

    ```bash
    sudo usermod -aG docker $USER
    ```

3. Log out and log back in so that your group membership is re-evaluated, or run `newgrp docker` to achieve the same result.

#### 1.3.2 Daemon configuration file

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

## 2. Run AlmaLinux 9 in a container

In the following we explain how to run a docker container with the AlmaLinux 9 image. We provide instructions for both [Docker Desktop](#21-using-docker-desktop) and [Docker CLI](#22-using-docker-cli)

### 2.1 Using Docker Desktop

1. **Open Docker Desktop.**

2. **Pull the AlmaLinux 9 image:**
    - Go to the "Images" tab.
    - Click on the "Pull" button.
    - Enter `almalinux:9` and click "Pull".

3. **Run a container using the pulled image:**
    - Go to the "Containers" tab.
    - Click on the "Run" button next to the `almalinux:9` image.
    - In the dialog that appears, you can set the container name (e.g., `almalinux9-container`).
    - Ensure the "Interactive & TTY" option is checked to open an interactive terminal session.
    - Click "Run".

4. **Verify the container is running:**
    - Go to the "Containers" tab.
    - You should see `almalinux9-container` listed and running.

### 2.2 Using Docker CLI

1. **Pull the AlmaLinux 9 image from Docker Hub:**

    ```bash
    sudo docker pull almalinux:9
    ```

2. **Run a container using the pulled image:**

    ```bash
    sudo docker run -it --name almalinux9-container almalinux:9
    ```

    This command will start a new container named `almalinux9-container` and open an interactive terminal session inside it. Optionally you can

    ```
    dnf -y upgrade
    ```

    To upgrade all the installed packages to the latest version.

3. **Verify the container is running:**

    ```bash
    sudo docker ps
    ```

    You should see `almalinux9-container` listed in the output.

## 3. Attach Visual Studio Code to a container

Visual Studio Code provides a convenient way to develop inside a Docker container using the Remote - Containers extension. You can either attach to a running container or let VS Code crete one for you, configuring the `devcontainer.json` for your repository.

### 3.0 Install the Remote - Containers extension

To attach to a Docker container first of all you need to install the required extension:

- Open Visual Studio Code.
- Go to the Extensions view by clicking on the Extensions icon in the Activity Bar on the side of the window.
- Search for "Remote - Containers" and install the extension.

### 3.1 Attach to a running container

Follow these steps to attach VS Code to your running AlmaLinux 9 container:

1. **Open the Command Palette:**
    - Press `Ctrl+Shift+P` to open the Command Palette.

2. **Attach to the running container:**
    - In the Command Palette, type `Remote-Containers: Attach to Running Container...` and select it.
    - You will see a list of running containers. Select `almalinux9-container` from the list.

3. **Start developing inside the container:**
    - VS Code will open a new window attached to the `almalinux9-container`.
    - You can now open folders, edit files, and run commands inside the container as if you were working on your local machine.

### 3.2 Configure the `devcontainer.json`

To configure a development container using `devcontainer.json`, follow these steps:

1. **Create a `.devcontainer` folder in your project root:**

    ```bash
    mkdir .devcontainer
    ```

2. **Create a `devcontainer.json` file inside the `.devcontainer` folder:**

    ```bash
    touch .devcontainer/devcontainer.json
    ```

    Otherwise you can just create a `.devcontainer.json` in your project root. (pay attention to the initial dot!)

3. **Edit the `devcontainer.json` file with the following content:**

    ```json
    {
        "name": "AlmaLinux 9 Development Container",
        "image": "almalinux:9",
        "runArgs": ["--name", "almalinux9-container"],
        "settings": {
            "terminal.integrated.shell.linux": "/bin/bash"
        },
        "extensions": [
            "ms-vscode.cpptools",
            "ms-python.python"
        ],
        "postCreateCommand": "dnf -y upgrade && dnf install -y git",
        "remoteUser": "root"
    }
    ```

    This configuration specifies:
    - The name of the development container.
    - The Docker image to use (`almalinux:9`).
    - Additional arguments to pass when running the container.
    - VS Code settings to apply inside the container.
    - Extensions to install in the container (I put extension for C++ and Python but you can customize this with your preferred extensions).
    - A command to run after the container is created (e.g., installing Git).
    - The user to use inside the container (`root`).

4. **Open the project in VS Code:**
    - Open Visual Studio Code.
    - Open the project folder containing the `.devcontainer` folder.

5. **Reopen the folder in the container:**
    - Press `Ctrl+Shift+P` to open the Command Palette.
    - Type `Remote-Containers: Reopen in Container` and select it.

VS Code will build the container according to the `devcontainer.json` configuration and reopen the project inside the container. You can now develop inside the AlmaLinux 9 container with all the specified settings and extensions.

By following these steps, you can leverage the full power of Visual Studio Code to develop inside your AlmaLinux 9 container.
