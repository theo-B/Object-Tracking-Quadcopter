#include <iostream>
#include <ctime>
#include <unistd.h>
#include <chrono>
#include <thread>

#define I_LIMIT 50
#define MAX_LOOP_TIME 110

using namespace std;
using namespace std::chrono;

struct pid
{
    float input;
    float set_pt;
    float error;
    float error_sum = 0;
    float prev_error = 0;
    float P;
    float I;
    float D;
    float kp = 1.0;
    float ki = 1.0;
    float kd = 1.0;
    int output_adjust;
};


pid pid_calculate(pid pid_a, milliseconds time_elapsed)
{
    if(time_elapsed.count() > MAX_LOOP_TIME)
    {
        cout << " Warning: loop time is longer than sample time ";
    }

    // Calculate P value (error)
    if(pid_a.input == 0)
    {
        pid_a.error == 0;
        cout << " No input to PID. ";
    }
    else
    {
        // Calculate error
        pid_a.error = pid_a.set_pt - pid_a.input;

        // Calculate P value
        pid_a.P = pid_a.error * pid_a.kp;
        cout << "P: " << pid_a.P;
    }

    // Add error*time to total error sum
    pid_a.error_sum += pid_a.error * static_cast<float>(time_elapsed.count());
    // Calculate I value
    pid_a.I = pid_a.error_sum * pid_a.ki;
    // Catch any error_sum values resulting in I outside of determined range
    if(pid_a.I > I_LIMIT)
    {
        pid_a.error_sum = I_LIMIT/pid_a.ki;
    }
    else if(pid_a.I < -I_LIMIT)
    {
        pid_a.error_sum = -I_LIMIT/pid_a.ki;
    }
    cout << " I: " << pid_a.I;

    // Calculate D value (rate of change of error)
    pid_a.D = pid_a.kd * (pid_a.P - pid_a.prev_error) / time_elapsed.count();
    cout << " D: " << pid_a.D << endl;
    // Set previous error
    pid_a.prev_error = pid_a.error;

    // Set output
    pid_a.output_adjust = static_cast<int>(pid_a.P + pid_a.I + pid_a.D);
    // If output too small, ignore it
    if(pid_a.output_adjust < 1 and pid_a.output_adjust > -1)
    {
        pid_a.output_adjust = 0;
    }
    //*/
    cout << "PID adjustment: " << pid_a.output_adjust << endl;

    return pid_a;
}
