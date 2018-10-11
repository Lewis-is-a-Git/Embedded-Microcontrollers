/*
 * servo.h
 */
#ifndef SERVO_H_
#define SERVO_H_
/**
 * Initialises the servo motor.
 * Initial position is middle (50%).
 */
void initialiseServo();
/**
 * Set servo position.
 *
 * @param position Position to set (0-100)
 *
 */
void setServoPosition(unsigned position);
#endif /* SERVO_H_ */
