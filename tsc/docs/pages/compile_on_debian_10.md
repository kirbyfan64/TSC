# Example for Debian 10 (with precompiled CEGUI) #

Debian 10 does not have libcegui-mk2-dev,
so it can be installed from Debian testing this way:

1) Edit install-build-dependencies.sh and remove from it `libcegui-mk2-dev`.
   Then run it:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
./install-build-dependencies.sh
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

2) Edit apt repos:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
sudo nano /etc/apt/sources.list
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

3) There is Debian 10 repos similar to these:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
deb http://www.nic.funet.fi/debian/ buster main non-free contrib
deb-src http://www.nic.funet.fi/debian/ buster main non-free contrib
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

4) Change those temporarily from buster to testing like this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
deb http://www.nic.funet.fi/debian/ testing main non-free contrib
deb-src http://www.nic.funet.fi/debian/ testing main non-free contrib
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

4) Update repos and install cegui:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
sudo apt update && sudo apt -y install libcegui-mk2-dev
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

5) Change repos back from testing to buster, see above steps 2 and 3, and then:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
sudo apt update
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

6) Restore install dependencies script to original:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
git checkout -- install-build-dependencies.sh
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

7) Build and run TSC:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
./build-tsc.sh
./run-tsc.sh
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
