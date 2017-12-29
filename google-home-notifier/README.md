google-home-notifierを入手
-------------
`
git clone https://github.com/noelportugal/google-home-notifier.git
`

ラズパイのnodejs最新化しておく
------------
`
curl -sL https://deb.nodesource.com/setup_7.x | sudo -E bash -
sudo apt-get install nodejs
sudo apt-get install git-core libnss-mdns libavahi-compat-libdnssd-dev
`

nodejsで起動
----------------
`
cd google-home-notifiier
npm install
node example.js

rkatsuta@raspberrypirkatsuta:~/git/google-home-notifier $ node example.js
*** WARNING *** The program 'nodejs' uses the Apple Bonjour compatibility layer of Avahi.
*** WARNING *** Please fix your application to use the native API of Avahi!
*** WARNING *** For more information see <http://0pointer.de/avahi-compat?s=libdns_sd&e=nodejs>
*** WARNING *** The program 'nodejs' called 'DNSServiceRegister()' which is not supported (or only supported partially) in the Apple Bonjour compatibility layer of Avahi.
*** WARNING *** Please fix your application to use the native API of Avahi!
*** WARNING *** For more information see <http://0pointer.de/avahi-compat?s=libdns_sd&e=nodejs&f=DNSServiceRegister>
Endpoints:
    http://192.168.1.20:8091/google-home-notifier
    https://0c075052.ngrok.io/google-home-notifier
GET example:
curl -X GET https://0c075052.ngrok.io/google-home-notifier?text=Hello+Google+Home
POST example:
curl -X POST -d "text=Hello Google Home" https://0c075052.ngrok.io/google-home-notifier
`
テストで喋らせてみる
-----------
curl -X POST -d "text=Hello Google Home" https://495e3b08.ngrok.io/google-home-notifier
