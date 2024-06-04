
#include <thread>
#include "Publishers.h"
#include "Subscribers.h"


int main()
{
    // initialise all Actors
    auto actors = {P0(), P1(), P2(), P3(), P4(), P5(), P6(), P7(), P8(), P9(),
                   S0(), S1(), S2(), S3(), S4(), S5(), S6(), S7(), S8(), S9()};

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    return 0;
}
