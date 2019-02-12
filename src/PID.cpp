#include "PID.h"


/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
   Kp = Kp_;
   Ki = Ki_;
   Kd = Kd_;
   //p_error = 0.0;
   //d_error = 0.0;
   //i_error = 0.0;

   Ts = 0.02;
   pre_output = 0.0;
   pre_error = 0.0;
   pre_pre_error = 0.0;
}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
   //d_error = cte - p_error;
   //p_error = cte;
   //i_error += cte;
   error = cte;
}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  //Ts = T - T_1;
  double a = Kp + Ki*Ts/2 + Kd/Ts;
  double b = -Kp + Ki*Ts/2 - 2*Kd/Ts;
  double c = Kd/Ts;

  double output = pre_output - (a*error + b*pre_error + c*pre_pre_error);
  pre_output = output;
  pre_pre_error = pre_error;
  pre_error = error;
  //T_1 = T;
  //  -(Kp*p_error + Kd*d_error + Ki*i_error);


  // satuation
  if(output > 1) output = 1;
  else if (output < -1) output = -1;
  else {
  }

  return output;  // TODO: Add your total error calc here!
}
