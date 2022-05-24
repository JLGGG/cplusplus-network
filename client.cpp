#include <iostream>
#include "jl_net.h"

enum class CustomMsgTypes : uint32_t
{
    FireBullet,
    MovePlayer
};

class CustomClient : public jl::net::client_interface<CustomMsgTypes>
{
    public:
        bool FireBullet(float x, float y)
        {
            jl::net::message<CustomMsgTypes> msg;
            msg.header.id = CustomMsgTypes::FireBullet;
            msg << x << y;
            Send(msg);
        }
}
int main()
{
    CustomClient c;
    // In progress

    return 0;
    
}