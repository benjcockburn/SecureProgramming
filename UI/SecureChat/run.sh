#Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)
rm ./SecureChat

pkill SecureChat
pkill SecureChatserver.py

cmake .
make
./SecureChat & python3 ../../server/SecureChatserver.py
