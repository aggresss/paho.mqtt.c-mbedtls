#!/usr/bin/env bash
# When OpenSSL prompts you for the Common Name for each certificate, use different names.

# CA key
openssl genrsa -out ca.key 2048
# CA csr
openssl req -new -subj "/CN=ca" -key ca.key -out ca.csr
# CA crt
openssl x509 -req -in ca.csr -out ca.crt -signkey ca.key -days 3650

# server key
openssl genrsa -out server.key 2048
# server.csr
openssl req -new -subj "/CN=server" -key server.key -out server.csr
# server.crt
openssl x509 -req -in server.csr -out server.crt -CA ca.crt -CAkey ca.key -CAcreateserial -days 3650
# server.crt verify
openssl verify -CAfile ca.crt  server.crt

# client key
openssl genrsa -out client.key 2048
# client.csr
openssl req -new -subj "/CN=client" -key client.key -out client.csr
# client.crt
openssl x509 -req -in client.csr -out client.crt -CA ca.crt -CAkey ca.key -CAcreateserial -days 3650
# client.crt verify
openssl verify -CAfile ca.crt  client.crt

# md5 checksum
md5sum * > md5.txt
