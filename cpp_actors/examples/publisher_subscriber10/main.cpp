
#include <thread>
#include "Publisher0.h"
#include "Subscriber0.h"
#include "Subscriber1.h"
#include "Subscriber2.h"
#include "Subscriber3.h"
#include "Subscriber4.h"
#include "Subscriber5.h"
#include "Subscriber6.h"
#include "Subscriber7.h"
#include "Subscriber8.h"
#include "Subscriber9.h"


int main()
{
    // initialise all Actors
    auto actors = {PUBLISHER0(), SUBSCRIBER0(), SUBSCRIBER1(), SUBSCRIBER2(), SUBSCRIBER3(), SUBSCRIBER4(), SUBSCRIBER5(), SUBSCRIBER6(), SUBSCRIBER7(), SUBSCRIBER8(), SUBSCRIBER9()};

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    return 0;
}
