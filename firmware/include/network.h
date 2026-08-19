#ifndef NETWORK_H
#define NETWORK_H

namespace Network {
    bool connectToNetwork(unsigned long timeout);

    bool networkIsConnected();

    bool networkGetPing();
}

#endif