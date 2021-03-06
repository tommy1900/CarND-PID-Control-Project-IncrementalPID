# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

Fellow students have put together a guide to Windows set-up for the project [here](https://s3-us-west-1.amazonaws.com/udacity-selfdrivingcar/files/Kidnapped_Vehicle_Windows_Setup.pdf) if the environment you have set up for the Sensor Fusion projects does not work for this project. There's also an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3).

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`.

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)


## Reflection
* PID Implementation Concept (In General)
  * The proportional term of the controller represents the position control of the steering wheel. Directly amplifies the control outputs (probably the voltage of the linear actuator of the steering axle) of the steer control angle.
  * The integral term of the controller represents the effects regarding to the accumulation of the steering offset error. Which normally brings the steady state error to zero during a certain amount time(depends on how fast the controller sampling).
  * The derivative term of the controller represents the velocity effects of the control response. Basically, it works like a damping system of the steering output, to reduce the overshoot caused by high output value(voltage).

* Tuning Step
  * The twiddle function was not implemented in this project, I do not think it will be useful in this case. Because:
    * It might be useful for fine tuning one parameter at once for optimization purpose. However the processing time it will take to optimize three parameters for a numerical solution is just not worth the time. I believe in real case, the AD vehicle research team should have the accurate vehicle dynamic/tire dynamic ready to use to serve the system identification. With the system model, a steering controller analytical solution can be easily solved.  
  * Tips on how to tune a normal PID controller:
    * Always start with P control till you see the overshoot effects appear, then introduce D to counter the overshoot till the system response is stable. (Introduce the I term after you got a reasonable response from PD controller with a constant steady state error).
  * When encounter high overshoot:
    * I would tune down D parameter first to see if the damping effect able to counter the overshoot. If this is not able to reduce the overshoot, I will then slightly decrease P and repeat this process(There might be chances that the I or D parameter is over tuned so that the system reached the "unstable frequency domain", and it will start to shack).
    This is basically based on the logic that the D parameter controls de(t)/d(t) which can reduce the impact of error/control effort changes.
  * Choice of Controller:
    * I normally use the position discrete PID for most systems, since they usually have a direct linear relationship between the output x and input voltage.  This is the most popular and normal PID algorithm but since it includes the past steps of error. However, it eats the memory and will have some integral error drifting.
    * In this particular project, an incremental PID was used for those reasons:
      * For sensitive system or actuators read incremental control inputs (like step motor), I use incremental(velocity) PID, which has less overshoot when tuning it (basically no Integral drifting since I*e(k) only, and it outputs u_delta, will not have dramatic control effort change). I think this is a perfect fit for this project, the steering angle overshoot should be as smooth as possible to drive safe. It is easier to control the amount of steering angle delta than control the entire steering performance. (But since it's not accumulating past steps, there will be some integral offset/overflow)
      * It is impossible to perform a sudden steer angle change in real life (like from -20deg to +20deg in 20ms), for example at time t, the steady state error will be reduced thanks the controller but the input u will not reduce to 0 because the car has to continue driving. So instead of tracking error and a target u, we use incremental delta_u here as new state varible to make sure robustness. 
      * A normal position PID will be too sensitive for the steering angle measurement and it will take some amount time to find the perfect PID combination.
      * Plus the high sampling step(20ms for the simulator), the derivative term (D_error/Dt) will be huge and require high damping ratio to stable the response (Put a 1.0 KP and 100.0 KD, and the controller will work).
      * Those sudden control changes will be really painful to tune when facing a curve road situation. But incremental PID controller doesn't have those problems, I only did about 2~3 iteration to make it drive really good, it probably doesn't even need an integrator.
