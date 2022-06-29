/*
 * Definitions for the  LEDS, switches and potentiometer on the 
 * software serial Sheild
 *
 * author: nns
 * created 15 Oct 2014
 * modified 03 Nov 2019 LGT
 */

//Muntazir Rashid - mrm19

#include "ASSP.h"

// hardware port definitions
#define LED_RED 3
#define LED_YELLOW 5
#define LED_PINK 6
#define LED_WHITE 9
#define LED_GREEN 10
#define LED_BLUE 11

#define EMU A1

#define SWITCH1 A4
#define SWITCH2 A5

#define POTENTIOMETER A0

//#define PORT 6

//Array of Hosts
#define NUMNAMES 26 //number of hosts
const char *names[NUMNAMES] = {
    "Ann", "Bob", "Cat", "Del",
    "Eva", "Fey", "Gil", "Han",
    "Ivy", "Jem", "Ken", "Leo",
    "Mae", "Ned", "Obe", "Pam",
    "Qaz", "Ron", "Sam", "Tom",
    "Uno", "Val", "Wes", "Xao",
    "Yan", "Zoe"};
//Struct To Store the details of Host
struct HostDetail
{
    int Port;   //save the port number
    char *Name; //save the name of Host
    long *Time; // stores time it took for the host to respond
};
//Array of struct to store the details of all hosts
HostDetail hostDetails_list[NUMNAMES];

/**
 * configuration
 */

void setup()
{
    // initialize the digital pins as outputs.
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_PINK, OUTPUT);
    pinMode(LED_WHITE, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    // Open hardware serial communications and wait for port to open:
    Serial.begin(57600);
    // Serial.println("Software serial LED test program");

    // now setup the switches
    pinMode(SWITCH1, INPUT_PULLUP);
    pinMode(SWITCH2, INPUT_PULLUP);

    // now setup the POTENTIOMETER
    pinMode(POTENTIOMETER, INPUT);

    //this puts nano into port testing mode for port 3
    SerialShield.setEmuMode(6);

    //task1(); //uncomment this to start task 1
    //task2(); //uncomment this to start task 1
    //task3(); //uncomment this to start task 1
    //task4(); //uncomment this to start task 1
}

/*
 * ADD YOUR FUNCTIONS HERE
 */
//This function checks whether the Host if on a specified port or not
//takes in name of host and port as arguments and return true or false
boolean isHostOnPort(const char name[], int port)
{
    SerialShield.setMessageSender("mrm19");   //sender
    SerialShield.setMessageDestination(name); //destination
    SerialShield.setMessageContent("ping");   //message content
    SerialShield.sendASSPMessage(port);       // port number

    for (int i = 0; i < 50; i++)
    { //loop to run fetchASSPMessage for 50 times
        if (SerialShield.fetchASSPMessage())
        {
            return true;
            break;
        }
    }
    return false;
}

// Function to get the time Host takes to respond
unsigned long getTimeToHost(const char name[], int port)
{
    unsigned long timeBeforeMessageSent = millis(); //built in arduino function takes time in ms
    if (isHostOnPort(name, port) == true)
    {
        unsigned long timeAfterMessageRecieved = millis();
        return (timeAfterMessageRecieved - timeBeforeMessageSent);
    }
    else
    {
        return 0;
    }
}

//function to get Port number of the Host

int getPortForHost(const char name[])
{

    if (isHostOnPort(name, 1) == true)
    {
        return 1;
    }
    else if (isHostOnPort(name, 2) == true)
    {
        return 2;
    }
    else if (isHostOnPort(name, 3) == true)
    {
        return 3;
    }
    else if (isHostOnPort(name, 4) == true)
    {
        return 4;
    }
    else
    {
        return 0;
    }
}
//fetches the details of hosts
void Details()
{
    Serial.println();
    Serial.println("Fetching Details...");
    for (int i = 0; i < NUMNAMES; i++)
    {
        int hostPort = getPortForHost(names[i]);
        long hostTime = getTimeToHost(names[i], hostPort);
        hostDetails_list[i].Port = hostPort;
        hostDetails_list[i].Name = names[i];
        hostDetails_list[i].Time = hostTime;
        //to keep user informed about the progress
        if (i == 6 || i == 12 || i == 18 | i == 22)
        {
            Serial.print(i);
            Serial.println(" Completed of of 26");
        }
    }

    Serial.println("Complete");
}

//task1 to check which host is connected on Port 4
void task1()
{
    Serial.print("HOST");
    Serial.println("  Port 4?");
    for (int i = 0; i < NUMNAMES; i++)
    {
        if (isHostOnPort(names[i], 4) == true)
        {
            Serial.print(names[i]);
            Serial.println("   YES");
        }
        else
        {
            Serial.print(names[i]);
            Serial.println("   NO");
        }
    }
}

//task2 checks how which Host is connected on Port 2 and how much time it takes to respond
void task2()
{
    Serial.print("HOST");
    Serial.println("  Port2Time");
    for (int i = 0; i < NUMNAMES; i++)
    {
        if (isHostOnPort(names[i], 2) == true)
        {
            Serial.print(names[i]);
            Serial.print("   ");
            Serial.println(getTimeToHost(names[i], 2));
        }
        else
        {
            Serial.print(names[i]);
            Serial.print("   ");
            Serial.println("NC");
        }
    }
}
//checks which Host in on which port and prints it
void task3()
{

    Details(); //fetches the details of hosts

    Serial.print("Port");
    Serial.println("   Hosts");

    //prints the result for port 1-4
    for (int i = 1; i < 5; i++)
    {
        Serial.print(i);
        Serial.print("      ");
        for (int j = 0; j < NUMNAMES; j++)
        {
            if (hostDetails_list[j].Port == i)
            {
                Serial.print(hostDetails_list[j].Name);
                Serial.print(" ");
            }
        }
        Serial.println();
    }
    //prints the result for 0 Hosts not connected to any port
    Serial.print("NC");
    Serial.print("     ");
    for (int i = 0; i < NUMNAMES; i++)
    {
        if (hostDetails_list[i].Port == 0)
        {
            Serial.print(hostDetails_list[i].Name);
            Serial.print(" ");
        }
    }
}
void task4()
{
    //wasnt able to figure out how to get the names of fastest and slowest hosts

    //if You want to try task 3 and 4 simultaneously comment out Details(); here to save time
    Details(); //fetches the details of hosts

    Serial.print("Port");
    Serial.print("    Hosts");
    Serial.print("    Slowest");
    Serial.println("    Time");

    //prints the results for slowest
    for (int i = 1; i < 5; i++)
    {
        Serial.print(i);
        Serial.print("        ");
        int hostNum = 0;
        long max = hostDetails_list[0].Time;
        //calculates number of hosts on particular Port
        for (int j = 0; j < NUMNAMES; j++)
        {
            if (hostDetails_list[j].Port == i)
            {
                hostNum++;
                //gets the speed of slowest host connected on a port
                if (hostDetails_list[j].Time > max)
                {
                    max = hostDetails_list[j].Time;
                }
            }
        }

        Serial.print(hostNum);
        Serial.print("        ");
        Serial.print("Name");
        Serial.print("          ");
        Serial.print(max);
        Serial.println();
    }
    //prints the results for fastest

    Serial.print("Port");
    Serial.print("    Hosts");
    Serial.print("    Fastest");
    Serial.println("    Time");

    for (int i = 1; i < 5; i++)
    {
        Serial.print(i);
        Serial.print("        ");
        int hostNum = 0;
        long min = hostDetails_list[0].Time;
        //calculates number of hosts on particular Port
        for (int j = 0; j < NUMNAMES; j++)
        {
            if (hostDetails_list[j].Port == i)
            {
                hostNum++;
                //gets the speed of fastest host connected on a port
                if (hostDetails_list[j].Time < min)
                {
                    min = hostDetails_list[j].Time;
                }
            }
        }

        Serial.print(hostNum);
        Serial.print("        ");
        Serial.print("Name");
        Serial.print("          ");
        Serial.print(min);
        Serial.println();
    }
}

/**
 * main loop
 */

void loop() // run over and over
{
}
