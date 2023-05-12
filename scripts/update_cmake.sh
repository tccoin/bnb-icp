curl -sL https://apt.kitware.com/keys/kitware-archive-latest.asc | sudo apt-key add -
sudo apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" -y
sudo apt-get update
sudo apt-get install --no-install-recommends -y cmake