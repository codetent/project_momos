#ifndef _MOMOS_STEPS_
#define _MOMOS_STEPS_

#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <vector>

#include "testif.h"

using namespace std;

typedef void(*step_function_t)(vector<int>&);


void receive(vector<int>& params) {
    TestInterface &interface = TestInterface::get_instance();
    SimulatedMessage msg((uint8_t *)"abc", 3, 0);

    interface.add_rx_message(msg);
    cout << "receive: " << params.size() << endl;
}

void transmit(vector<int>& params) {
    TestInterface &interface = TestInterface::get_instance();

    // TODO
    cout << "transmit: " << params.size() << endl;
}

void timeout(vector<int>& params) {
    assert(params.size() == 1);
    int seconds = params[0];

    cout << "Waiting for " << seconds << " seconds" << endl;
    sleep(seconds);
}


static map<string, step_function_t> step_mapping = {
    {"receive", receive},
    {"transmit", transmit},
    {"timeout", timeout}
};

#endif // _MOMOS_STEPS_
