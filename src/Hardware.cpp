//*************************
// Golden Gears Robotics
// 1/24/15
// Hardware MAIN
//*************************

// Tyler Robbins - 1-25-15 -

#include <math.h>

#include "WPILib.h"

const float ELEVATOR_DEADBAND = 100;

class Hardware{
public:
	Encoder *enc;
	Talon *frontLeft;
	Talon *frontRight;
	Talon *backLeft;
	TalonSRX *backRight;
	Talon *elevatorMotor;
	RobotDrive *drive;
	Compressor *c;

	Hardware(){
		enc = new Encoder(8,9);
		frontLeft = new Talon(0);
		frontRight = new Talon(2);
		backLeft = new Talon(1);
		backRight = new TalonSRX(3);
		elevatorMotor = new Talon(9);
		drive = new RobotDrive(frontLeft,backLeft,frontRight,backRight);
		c = new Compressor(0);
	}

	void toggleSolenoid(Solenoid *sol){
		//sol->Set(!sol->Get());
		setSolenoid(sol,!sol->Get());
	}

	void setSolenoid(Solenoid *sol,bool value){
		sol->Set(value);
	}

	void moveElevator(float pwr){
		elevatorMotor->Set(pwr);
	}

	void ElevatorUp(float pwr){
		moveElevator(pwr/100);
	}

	void ElevatorDown(float pwr){
		moveElevator(-pwr/100);
	}

	void ElevatorPeriodic(bool up=false,bool down=false,bool gotop=false){
		if(up)
			ElevatorUp(30);
		else if(down)
			ElevatorDown(30);
		else if(gotop)
			ElevatorGoToPeriodic(1000);
		else
			ElevatorDown(0);
	}

	void ElevatorGoToPeriodic(float poscmd){
		float poserror = poscmd - enc->GetDistance();

		if(std::abs(poserror)<(ELEVATOR_DEADBAND/2))
			poserror = 0;

		if(poserror > ELEVATOR_DEADBAND)
			ElevatorUp(30);
		else if(poserror < -ELEVATOR_DEADBAND)
			ElevatorDown(30);
		else
			moveElevator(0);
	}

	bool swugbruh(float value){
		/*Checks if the encoder has gone past a certain value.*/
		return enc->GetDistance() >= value;
	}

	void move(Joystick &stick){
		float x = stick.GetX();
		float y = stick.GetY();
		float z = stick.GetZ();
		float turn = stick.GetTwist();
		float throttle = stick.GetThrottle();

		throttle = (-throttle + 1.0)/2.0; // Shift the throttle value from -1-1 to 0-1

		x*=throttle;
		y*=throttle;
		z*=throttle;
		turn*=throttle;

		//driveFront.ArcadeDrive(y,turn);
		//driveMiddle.ArcadeDrive(y,turn);
		//driveBack.ArcadeDrive(y,turn);

		drive->MecanumDrive_Cartesian(x,y,turn);

	}

//	SpeedController GetTalon(int port){
//		switch(port){
//			case 0:
//				return &frontLeft;
//			case 1:
//				return &backLeft;
//			case 2:
//				return &frontRight;
//			case 3:
//				return &backRight;
//			case 4:
//				return &elevatorMotor;
//			default:
//				return NULL;
//		}
//	}
//
//	Encoder GetEncoder(){ return &enc; }
//
//	Compressor GetCompressor(){ return &c; }
};


