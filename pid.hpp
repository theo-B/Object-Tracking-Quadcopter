#include <iostream>
#include <ctime>
#include <unistd.h>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

milliseconds sample_rate_ms(160);

struct pid
{
    float P = 0;
    float I = 0;
    float D = 0;
    float kp = 1.0;
    float ki = 1.0;
    float kd = 1.0;
    float input;
    float set_pt;
    float prev_error = 0;
    int output_adjust;
};


pid pid_calculate(pid pid_a, milliseconds time_elapsed)
{
    // Ensure that sample rate has elapsed
    milliseconds t_diff = sample_rate_ms - time_elapsed;
    if(t_diff.count() > 0)
    {
        this_thread::sleep_for(t_diff);
    }
    else if(t_diff.count() < 0)
    {
        cout << "Time difference is negative; sample rate too low" << endl;
        return pid_a;
    }

    // Calculate P value (error)
    if(pid_a.input == 0)
    {
        pid_a.P == 0;
        cout << "No input to PID." << endl;
    }
    else
    {
        pid_a.P = pid_a.set_pt - pid_a.input;
        cout << "kp*P: " << pid_a.P*pid_a.kd;
    }

    // Add error to total error sum
    pid_a.I += pid_a.P * static_cast<float>(sample_rate_ms.count())/1000;
    cout << " ki*I: " << pid_a.I*pid_a.ki;

    // Calculate rate of change of error
    pid_a.D = (pid_a.P - pid_a.prev_error) / (static_cast<float>(sample_rate_ms.count())/1000);
    cout << " ki*D: " << pid_a.D*pid_a.kd << endl;

    // Set previous error
    pid_a.prev_error = pid_a.P;

    pid_a.output_adjust = static_cast<int>((pid_a.P*pid_a.kp) + (pid_a.I*pid_a.ki) + (pid_a.D*pid_a.kd));

    cout << "PID adjustment: " << pid_a.output_adjust << endl;
    cout << "kP: " << pid_a.kp << endl;
    cout << "kI: " << pid_a.ki << endl;
    cout << "kD: " << pid_a.kd << endl;

    return pid_a;
}