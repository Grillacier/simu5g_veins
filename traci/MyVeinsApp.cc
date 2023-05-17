//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "veins/modules/application/traci/MyVeinsApp.h"
#include <fstream>

using namespace veins;

Define_Module(veins::MyVeinsApp);

void MyVeinsApp::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        // Initializing members and pointers of your application goes here
        EV << "Initializing " << par("appName").stringValue() << std::endl;
    }
    else if (stage == 1) {
        // Initializing members that require initialized other modules goes here
    }
}

void MyVeinsApp::finish()
{
    calculateAndSaveAverageSpeed();
    DemoBaseApplLayer::finish();
}


void MyVeinsApp::onBSM(DemoSafetyMessage* bsm)
{
    // Your application has received a beacon message from another car or RSU
    // code for handling the message goes here

    DemoBaseApplLayer::onBSM(bsm);
    //traciVehicle->setColor(RGB(0, 255, 0));
    getParentModule()->getDisplayString().setTagArg("i", 1, "green");
    traciVehicle->setSpeed(traciVehicle->getSpeed()/2);
    std::cout << "slowing down" << std::endl;
    //traciVehicle->slowDown(10, 30);
}

void MyVeinsApp::onWSM(BaseFrame1609_4* wsm)
{
    // Your application has received a data message from another car or RSU
    // code for handling the message goes here, see TraciDemo11p.cc for examples
}

void MyVeinsApp::onWSA(DemoServiceAdvertisment* wsa)
{
    // Your application has received a service advertisement from another car or RSU
    // code for handling the message goes here, see TraciDemo11p.cc for examples
}

void MyVeinsApp::handleSelfMsg(cMessage* msg)
{
    DemoBaseApplLayer::handleSelfMsg(msg);
    // this method is for self messages (mostly timers)
    // it is important to call the DemoBaseApplLayer function for BSM and WSM transmission
}

void MyVeinsApp::handlePositionUpdate(cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);
    double currentSpeed = traciVehicle->getSpeed();
    // put current speed in vector with other speeds
    speeds.push_back(currentSpeed);
    vehicleCount++;
}

void MyVeinsApp::calculateAndSaveAverageSpeed()
{
    // calculate mean speed
    double totalSpeed = 0.0;
    for (double speed : speeds) {
        totalSpeed += speed;
    }
    double averageSpeed = totalSpeed / vehicleCount;

    std::ofstream outFile("average_speed.txt");

    if (outFile.is_open()) {
        outFile << "Average Speed" << std::endl;
        outFile << averageSpeed << std::endl;
        outFile.close();

        EV << "Average speed saved to average_speed.csv" << std::endl;
    }
    else {
        EV << "Error: Unable to open file for writing" << std::endl;
    }
}
