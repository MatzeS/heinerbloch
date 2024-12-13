#!/bin/sh
set -e -x

sudo apt-get -y install nodejs npm
sudo npm install -g typescript

cd /opt/
git clone https://github.com/glideapps/quicktype
cd quicktype
git checkout 29bb8160660a7daa2a837ac0cb1e92f99729ad8e

npm install
npm run build
sudo npm link
