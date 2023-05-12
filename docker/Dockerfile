FROM nvidia/cudagl:11.4.2-devel-ubuntu20.04
ARG USER

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y locales \
    && sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen \
    && dpkg-reconfigure --frontend=noninteractive locales \
    && update-locale LANG=en_US.UTF-8

# nvidia-container-runtime
ENV NVIDIA_VISIBLE_DEVICES \
    ${NVIDIA_VISIBLE_DEVICES:-all}
ENV NVIDIA_DRIVER_CAPABILITIES \
    ${NVIDIA_DRIVER_CAPABILITIES:+$NVIDIA_DRIVER_CAPABILITIES,}graphics,compute,utility,display

# install lsb-release
RUN apt-get update && apt-get install -y --no-install-recommends \
    lsb-release \
    && apt clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* 

# install ROS
RUN apt-get update \ 
    && sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
RUN sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main"'
RUN apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654

RUN apt-get update \
    && apt-get install -y --no-install-recommends ros-noetic-desktop-full \
    && apt clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* 

# install libraries and tools
RUN apt-get update && apt-get install -y --no-install-recommends \
    sudo \
    dialog \
    apt-utils \
    git \
    mercurial \
    pkg-config \
    mesa-utils \
    libxau-dev \
    libxdmcp-dev \
    libxcb1-dev \
    libxext-dev \
    libx11-dev \ 
    git \
    tmux \
    wget \
    curl \
    cmake \
    vim \ 
    build-essential \
    unzip \
    autoconf \
    autogen \
    automake \
    libtool \
    mlocate \
    zlib1g-dev \
    libopenblas-dev \
    libflann-dev \
    libboost-all-dev \
    libsuitesparse-dev \
    libtbb-dev \
    libusb-1.0-0-dev \
    libgtest-dev \
    pkg-config \
    cmake-curses-gui \
    gdb \
    clang \
    emacs \
    libpcap-dev \
    gnupg2 \
    x11-xserver-utils \
    lsb-release \
    tilix \
    zsh \
    python3-dev \
    python3-pip \
    python3-tk \
    python3-matplotlib \
    valgrind \
    kcachegrind \
    && apt clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* 

# RUN wget -O - https://apt.kitware.com/kitware-archive.sh | bash \
#     && apt-get install --no-install-recommends -y cmake

RUN pip install cmake

# use current user
RUN adduser --disabled-password --gecos '' $USER
RUN adduser $USER sudo
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
USER $USER
WORKDIR /home/$USER/

# install zsh
COPY scripts/zsh zsh
RUN cd zsh && bash install_zsh.sh

# install gitstatus
RUN mkdir -p /home/$USER/.cache/gitstatus \
    && wget https://github.com/romkatv/gitstatus/releases/download/v1.5.4/gitstatusd-linux-x86_64.tar.gz -O - \
    | tar -zx -C /home/$USER/.cache/gitstatus/

# install slam libraries
RUN mkdir /home/$USER/slam_lib/
WORKDIR /home/$USER/slam_lib/

COPY scripts/debian_mkl.sh debian_mkl.sh
RUN sudo bash debian_mkl.sh

RUN git clone https://gitlab.com/libeigen/eigen.git eigen/ -b 3.4.0
RUN cd eigen/ \
    && mkdir -p build \
    && cd build \
    && cmake .. \
    && sudo make install -j6

RUN git clone https://github.com/ceres-solver/ceres-solver.git ceres-solver/ -b 2.1.0
RUN cd ceres-solver/ \
    && mkdir -p build \
    && cd build \
    && cmake .. -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DEXPORT_BUILD_DIR=ON -DMINIGLOG=ON -DPROVIDE_UNINSTALL_TARGET=OFF \
    && make -j6 && sudo make install -j6

RUN git clone https://github.com/google/or-tools
RUN cd or-tools \
    && cmake -S . -B build -DBUILD_DEPS=ON \
    && cmake --build build --config Release --target all -j6 -v \
    && sudo cmake --build build --config Release --target install -j6 -v

# RUN wget https://github.com/opencv/opencv/archive/4.5.5.zip -q -O opencv.zip \
#     && unzip -q opencv.zip \
#     && mv opencv-4.5.5 opencv \
#     && rm opencv.zip \
#     && cd opencv \
#     && wget https://github.com/opencv/opencv_contrib/archive/4.5.5.zip -q -O opencv_contrib.zip \
#     && unzip -q opencv_contrib.zip \
#     && mv opencv_contrib-4.5.5 opencv_contrib \
#     && rm opencv_contrib.zip
# RUN cd opencv/ \
#     && mkdir -p build \
#     && cd build \
#     && cmake -DCMAKE_BUILD_TYPE=RELEASE \
#     -DCMAKE_INSTALL_PREFIX=/usr \
#     -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules \
#     -DOPENCV_ENABLE_NONFREE=ON .. \
#     && make install -j6

# RUN git clone https://github.com/borglab/gtsam.git gtsam/  -b 4.1.1
# RUN cd gtsam/ \
#     && mkdir -p build \
#     && cd build \
#     && cmake .. -DGTSAM_USE_SYSTEM_EIGEN=TRUE \
#     && make install -j6

# RUN git clone https://github.com/strasdat/Sophus.git  sophus/
# RUN apt-get install -y --no-install-recommends libfmt-dev \
#     && cd sophus/ \
#     && mkdir -p build \
#     && cd build \
#     && cmake .. \
#     && make install -j6

# RUN git clone https://github.com/google/glog.git  glog/
# RUN cd glog/ \
#     && cmake -S . -B build -G "Unix Makefiles" \
#     && cmake --build build \
#     && cmake --build build --target test \
#     && cmake --build build --target install

# RUN wget https://github.com/jbeder/yaml-cpp/archive/refs/tags/yaml-cpp-0.6.0.zip \
#     && unzip yaml-cpp-0.6.0.zip && rm yaml-cpp-0.6.0.zip \
#     && mv yaml-cpp-yaml-cpp-0.6.0/ yaml-cpp-0.6.0/ 
# RUN cd ./yaml-cpp-0.6.0/ && env CFLAGS='-fPIC' CXXFLAGS='-fPIC' cmake -DCMAKE_BUILD_TYPE=Release -Bbuild && cmake --build build -v

# RUN wget https://github.com/PointCloudLibrary/pcl/archive/pcl-1.9.1.tar.gz \
#     && tar -xf pcl-1.9.1.tar.gz && rm pcl-1.9.1.tar.gz \
#     && mv pcl-pcl-1.9.1 pcl-1.9.1  \
#     && mkdir -p pcl-1.9.1/build

