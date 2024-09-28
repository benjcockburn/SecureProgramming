rm ./SecureChat

pkill SecureChat
pkill SecureChatserver.py

cmake .
make
./SecureChat & python3 ../../server/SecureChatserver.py