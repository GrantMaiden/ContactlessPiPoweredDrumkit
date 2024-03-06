/****************************** Module Header ******************************\
Module Name:    btb_main.h
Project:        btb
Author:         Grant Maiden
Description:    Header file for btb.main.c
\***************************************************************************/


/**
 * Initialize Leds
 **/
void initLeds();

/**
 * Initialize Interrupts
 **/
void initInterrupts();

/**
 * Parses Command line arguements. Command line inputs are used primarily for unit tests
 * \param argc - int containging the number of command line arguements
 * \param *argv[] - char* to an array of characters containing the input commands
 **/
void parseCommandLine(int argc,char *argv[]);

/**
 * Runs command line arguements
 * \param argv - char array input of characters containing the input command
 **/
void runCommandLine(char *argv[]);

/**
 * Callback that is triggered on DistanceSensors Interrupt Falling Edge.
 * \param gpio - int gpio Triggered Gpio number
 * \param level - int level GPIO input level at time of ISR
 * \param tick - uint32_t trigger time in microseconds.
 **/
void rangingISRCallback(int gpio, int level, uint32_t tick);

/**
 * Callback override virtual run method for btbThread Class
 **/
class btbThread : public CppThread {
    /**
     * Initializes the Thread
     **/
    public:
        btbThread() {}
    /**
     * Starts the Thread
     **/
    private:
        void run();
};

/**
 * Callback class to implement custom timer
 **/
class btbTimer1 : public CppTimer {

    /**
    * Timer Event called when timer is triggered. Overriden in cpp.
    **/
    private:
	void timerEvent();
};


