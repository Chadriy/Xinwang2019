#pragma once

class PrepareChannel {
private:
    int source, target;
    float timeout;
public:
    PrepareChannel(int source, int target, float timeout);

    int getHash() const;

    bool operator < (const PrepareChannel &channel) const;

    bool isTimeout();

    int getSource();

    int getTarget();

    float getTimeout();
};
