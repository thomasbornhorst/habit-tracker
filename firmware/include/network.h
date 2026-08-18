#ifndef NETWORK_H
#define NETWORK_H

bool connectToNetwork(unsigned long timeout);

bool networkIsConnected();

bool networkGetPing();

#endif