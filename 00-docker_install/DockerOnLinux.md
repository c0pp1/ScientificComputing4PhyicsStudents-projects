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


### 1.2 Docker engine (CLI)