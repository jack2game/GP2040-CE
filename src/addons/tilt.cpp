#include "addons/tilt.h"
#include "drivermanager.h"
#include "storagemanager.h"
#include "helper.h"
#include "config.pb.h"
#include "math.h"

bool TiltInput::available() {
	return Storage::getInstance().getAddonOptions().tiltOptions.enabled;
}

void TiltInput::setup() {
	const TiltOptions& options = Storage::getInstance().getAddonOptions().tiltOptions;
	tiltLeftSOCDMode = options.tiltLeftSOCDMode;
	tiltRightSOCDMode = options.tiltRightSOCDMode;

	pinTilt1 = options.tilt1Pin;
	tilt1FactorLeftX = options.factorTilt1LeftX;
	tilt1FactorLeftY = options.factorTilt1LeftY;
	tilt1FactorRightX = options.factorTilt1RightX;
	tilt1FactorRightY = options.factorTilt1RightY;
	pinTilt2 = options.tilt2Pin;
	tilt2FactorLeftX = options.factorTilt2LeftX;
	tilt2FactorLeftY = options.factorTilt2LeftY;
	tilt2FactorRightX = options.factorTilt2RightX;
	tilt2FactorRightY = options.factorTilt2RightY;
	tilt3FactorLeftX = options.factorTilt3LeftX;
	tilt3FactorLeftY = options.factorTilt3LeftY;
	tilt3FactorRightX = options.factorTilt3RightX;
	tilt3FactorRightY = options.factorTilt3RightY;
	tilt4FactorLeftX = options.factorTilt4LeftX;
	tilt4FactorLeftY = options.factorTilt4LeftY;
	tilt4FactorRightX = options.factorTilt4RightX;
	tilt4FactorRightY = options.factorTilt4RightY;
	pinTiltLeftAnalogDown = options.tiltLeftAnalogDownPin;
	pinTiltLeftAnalogUp = options.tiltLeftAnalogUpPin;
	pinTiltLeftAnalogLeft = options.tiltLeftAnalogLeftPin;
	pinTiltLeftAnalogRight = options.tiltLeftAnalogRightPin;
	pinTiltRightAnalogDown = options.tiltRightAnalogDownPin;
	pinTiltRightAnalogUp = options.tiltRightAnalogUpPin;
	pinTiltRightAnalogLeft = options.tiltRightAnalogLeftPin;
	pinTiltRightAnalogRight = options.tiltRightAnalogRightPin;
	pinRotate1 = options.rotate1Pin;
	pinRotate2 = options.rotate2Pin;
	rotate1FactorLeft = options.factorRotate1Left;
	rotate2FactorLeft = options.factorRotate2Left;
	rotate3FactorLeft = options.factorRotate3Left;
	rotate4FactorLeft = options.factorRotate4Left;
	rotate1FactorRight = options.factorRotate1Right;
	rotate2FactorRight = options.factorRotate2Right;
	rotate3FactorRight = options.factorRotate3Right;
	rotate4FactorRight = options.factorRotate4Right;
	tilt1Then2Mode = options.tilt1Then2Mode;
	tilt2Then1Mode = options.tilt2Then1Mode;
	rotate1Then2Mode = options.rotate1Then2Mode;
	rotate2Then1Mode = options.rotate2Then1Mode;


	// Setup Tilt Key
	uint8_t pinTilt[12] = {
											pinTilt1,
											pinTilt2,
											pinTiltLeftAnalogDown,
											pinTiltLeftAnalogUp,
											pinTiltLeftAnalogLeft,
											pinTiltLeftAnalogRight,
											pinTiltRightAnalogDown,
											pinTiltRightAnalogUp,
											pinTiltRightAnalogLeft,
											pinTiltRightAnalogRight,
											pinRotate1,
											pinRotate2	};

	for (int i = 0; i < 12; i++) {
		if (pinTilt[i] != (uint8_t)-1) {
			gpio_init(pinTilt[i]);			 // Initialize pin
			gpio_set_dir(pinTilt[i], GPIO_IN); // Set as INPUT
			gpio_pull_up(pinTilt[i]);		  // Set as PULLUP
		}
	}

	tiltLeftState = 0;
	tiltRightState = 0;

	leftLastTiltUD = DIRECTION_NONE;
	leftLastTiltLR = DIRECTION_NONE;

	rightLastTiltUD = DIRECTION_NONE;
	rightLastTiltLR = DIRECTION_NONE;
	
	last1button = false;
	last2button = false;

	uint32_t now = getMillis();
	for (int i = 0; i < 4; i++) {
		dpadTime[i] = now;
	}
}

void TiltInput::preprocess()
{
	bool sliderLeft;
	bool sliderRight;
	
	Gamepad* gamepad = Storage::getInstance().GetGamepad();
	sliderLeft = gamepad->getOptions().dpadMode == DPAD_MODE_LEFT_ANALOG;
	sliderRight = gamepad->getOptions().dpadMode == DPAD_MODE_RIGHT_ANALOG;

	// Need to invert since we're using pullups
	tiltLeftState = 0;
	if ((pinTiltLeftAnalogUp == (uint8_t)-1) && (pinTiltLeftAnalogDown == (uint8_t)-1) && (pinTiltLeftAnalogLeft == (uint8_t)-1) && (pinTiltLeftAnalogRight == (uint8_t)-1) && sliderLeft) {
		tiltLeftState |= gamepad->state.dpad;
	}
	if (pinTiltLeftAnalogUp != (uint8_t)-1) {
		tiltLeftState |= (!gpio_get(pinTiltLeftAnalogUp) ? gamepad->mapDpadUp->buttonMask : 0);
	}
	if (pinTiltLeftAnalogDown != (uint8_t)-1) {
		tiltLeftState |= (!gpio_get(pinTiltLeftAnalogDown) ? gamepad->mapDpadDown->buttonMask : 0);
	}
	if (pinTiltLeftAnalogLeft != (uint8_t)-1) {
		tiltLeftState |= (!gpio_get(pinTiltLeftAnalogLeft) ? gamepad->mapDpadLeft->buttonMask : 0);
	}
	if (pinTiltLeftAnalogRight != (uint8_t)-1) {
		tiltLeftState |= (!gpio_get(pinTiltLeftAnalogRight) ? gamepad->mapDpadRight->buttonMask : 0);
	}

	tiltRightState = 0;
	if ((pinTiltRightAnalogUp == (uint8_t)-1) && (pinTiltRightAnalogDown == (uint8_t)-1) && (pinTiltRightAnalogLeft == (uint8_t)-1) && (pinTiltRightAnalogRight == (uint8_t)-1) && sliderRight) {
		tiltRightState |= gamepad->state.dpad;
	}
	if (pinTiltRightAnalogUp != (uint8_t)-1) {
		tiltRightState |= (!gpio_get(pinTiltRightAnalogUp) ? gamepad->mapDpadUp->buttonMask : 0);
	}
	if (pinTiltRightAnalogDown != (uint8_t)-1) {
		tiltRightState |= (!gpio_get(pinTiltRightAnalogDown) ? gamepad->mapDpadDown->buttonMask : 0);
	}
	if (pinTiltRightAnalogLeft != (uint8_t)-1) {
		tiltRightState |= (!gpio_get(pinTiltRightAnalogLeft) ? gamepad->mapDpadLeft->buttonMask : 0);
	}
	if (pinTiltRightAnalogRight != (uint8_t)-1) {
		tiltRightState |= (!gpio_get(pinTiltRightAnalogRight) ? gamepad->mapDpadRight->buttonMask : 0);
	}

	// Convert gamepad from process() output to uint8 value
	uint8_t gamepadState = gamepad->state.dpad;

	gamepad->state.dpad = gamepadState;
}

void TiltInput::process()
{
	SOCDTiltClean(tiltLeftSOCDMode, tiltRightSOCDMode);

	Gamepad* gamepad = Storage::getInstance().GetGamepad();

	// Set Tilt Output
	OverrideGamepad(gamepad, tiltLeftState, tiltRightState);
}

//The character's movement changes depending on the degree to which the stick is tilted.
//I added the functionality to allow the all - button controller to perform the operations that can be performed by the sticks.
//Two buttons, tilt1 and tilt2, have been implemented.
//While pressing these buttons, pressing the left or right analog stick will cause the character to walk or walk more slowly.
//Since this is an auxiliary function for appeals and such,
//pressing Tilt1 and Tilt2 at the same time will cause the light analog stick to correspond to each of the DPad methods.

void TiltInput::OverrideGamepad(Gamepad* gamepad, uint8_t dpad1, uint8_t dpad2) {
	bool pinTilt1Pressed = (pinTilt1 != (uint8_t)-1) ? !gpio_get(pinTilt1) : false;
	bool pinTilt2Pressed = (pinTilt2 != (uint8_t)-1) ? !gpio_get(pinTilt2) : false;
	bool pinRotate1Pressed = (pinRotate1 != (uint8_t)-1) ? !gpio_get(pinRotate1) : false;
	bool pinRotate2Pressed = (pinRotate2 != (uint8_t)-1) ? !gpio_get(pinRotate2) : false;

	// Scales input from 0-100% of maximum input
	double scaledTilt1FactorLeftX  = 1.0 - (tilt1FactorLeftX  / 100.0);
	double scaledTilt1FactorLeftY  = 1.0 - (tilt1FactorLeftY  / 100.0);
	double scaledTilt1FactorRightX = 1.0 - (tilt1FactorRightX / 100.0);
	double scaledTilt1FactorRightY = 1.0 - (tilt1FactorRightY / 100.0);
	double scaledTilt2FactorLeftX  = 1.0 - (tilt2FactorLeftX  / 100.0);
	double scaledTilt2FactorLeftY  = 1.0 - (tilt2FactorLeftY  / 100.0);
	double scaledTilt2FactorRightX = 1.0 - (tilt2FactorRightX / 100.0);
	double scaledTilt2FactorRightY = 1.0 - (tilt2FactorRightY / 100.0);
	double scaledTilt3FactorLeftX  = 1.0 - (tilt3FactorLeftX  / 100.0);
	double scaledTilt3FactorLeftY  = 1.0 - (tilt3FactorLeftY  / 100.0);
	double scaledTilt3FactorRightX = 1.0 - (tilt3FactorRightX / 100.0);
	double scaledTilt3FactorRightY = 1.0 - (tilt3FactorRightY / 100.0);
	double scaledTilt4FactorLeftX  = 1.0 - (tilt4FactorLeftX  / 100.0);
	double scaledTilt4FactorLeftY  = 1.0 - (tilt4FactorLeftY  / 100.0);
	double scaledTilt4FactorRightX = 1.0 - (tilt4FactorRightX / 100.0);
	double scaledTilt4FactorRightY = 1.0 - (tilt4FactorRightY / 100.0);

	double rotate1degreeLeft = rotate1FactorLeft;
	double rotate2degreeLeft = rotate2FactorLeft;
	double rotate3degreeLeft = rotate3FactorLeft;
	double rotate4degreeLeft = rotate4FactorLeft;
	double rotate1degreeRight = rotate1FactorRight;
	double rotate2degreeRight = rotate2FactorRight;
	double rotate3degreeRight = rotate3FactorRight;
	double rotate4degreeRight = rotate4FactorRight;

	uint16_t midValue = GAMEPAD_JOYSTICK_MID;
	if ( DriverManager::getInstance().getDriver() != nullptr ) {
		midValue = DriverManager::getInstance().getDriver()->GetJoystickMidValue();
	}

	// (Tilt1+Tilt2) = Selected Mode
	if (pinTilt1Pressed && pinTilt2Pressed && !pinRotate1Pressed && !pinRotate2Pressed) {
		if (last1button){
			switch (tilt1Then2Mode){
			case (DPAD_MODE_LEFT_ANALOG):
				gamepad->state.lx = dpadToAnalogX(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.ly = dpadToAnalogY(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.dpad = 0;
				gamepad->state.rx = midValue;
				gamepad->state.ry = midValue;
				break;
			case (DPAD_MODE_RIGHT_ANALOG):
				gamepad->state.rx = dpadToAnalogX(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.ry = dpadToAnalogY(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.dpad = 0;
				gamepad->state.lx = midValue;
				gamepad->state.ly = midValue;
				break;
			default:
				gamepad->state.dpad |= dpad1|dpad2;
				gamepad->state.lx = midValue;
				gamepad->state.ly = midValue;
				gamepad->state.rx = midValue;
				gamepad->state.ry = midValue;
				break;
			}
		}
		else if (last2button){
			switch (tilt2Then1Mode){
			case (DPAD_MODE_LEFT_ANALOG):
				gamepad->state.lx = dpadToAnalogX(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.ly = dpadToAnalogY(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.dpad = 0;
				gamepad->state.rx = midValue;
				gamepad->state.ry = midValue;
				break;
			case (DPAD_MODE_RIGHT_ANALOG):
				gamepad->state.rx = dpadToAnalogX(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.ry = dpadToAnalogY(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.dpad = 0;
				gamepad->state.lx = midValue;
				gamepad->state.ly = midValue;
				break;
			default:
				gamepad->state.dpad |= dpad1|dpad2;
				gamepad->state.lx = midValue;
				gamepad->state.ly = midValue;
				gamepad->state.rx = midValue;
				gamepad->state.ry = midValue;
				break;
			}
		}
	}
	
	// (Rotate1+Rotate2) = Selected Mode
	if (!pinTilt1Pressed && !pinTilt2Pressed && pinRotate1Pressed && pinRotate2Pressed) {
		if (last1button){
			switch (rotate1Then2Mode){
			case (DPAD_MODE_LEFT_ANALOG):
				gamepad->state.lx = dpadToAnalogX(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.ly = dpadToAnalogY(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.dpad = 0;
				gamepad->state.rx = midValue;
				gamepad->state.ry = midValue;
				break;
			case (DPAD_MODE_RIGHT_ANALOG):
				gamepad->state.rx = dpadToAnalogX(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.ry = dpadToAnalogY(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.dpad = 0;
				gamepad->state.lx = midValue;
				gamepad->state.ly = midValue;
				break;
			default:
				gamepad->state.dpad |= dpad1|dpad2;
				gamepad->state.lx = midValue;
				gamepad->state.ly = midValue;
				gamepad->state.rx = midValue;
				gamepad->state.ry = midValue;
				break;
			}
		}
		else if (last2button){
			switch (rotate2Then1Mode){
			case (DPAD_MODE_LEFT_ANALOG):
				gamepad->state.lx = dpadToAnalogX(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.ly = dpadToAnalogY(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.dpad = 0;
				gamepad->state.rx = midValue;
				gamepad->state.ry = midValue;
				break;
			case (DPAD_MODE_RIGHT_ANALOG):
				gamepad->state.rx = dpadToAnalogX(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.ry = dpadToAnalogY(gamepad->state.dpad|dpad1|dpad2);
				gamepad->state.dpad = 0;
				gamepad->state.lx = midValue;
				gamepad->state.ly = midValue;
				break;
			default:
				gamepad->state.dpad |= dpad1|dpad2;
				gamepad->state.lx = midValue;
				gamepad->state.ly = midValue;
				gamepad->state.rx = midValue;
				gamepad->state.ry = midValue;
				break;
			}
		}
	}

	// Tilt1
	else if ( pinTilt1Pressed && !pinTilt2Pressed && !pinRotate1Pressed && !pinRotate2Pressed) {
		last1button = true;
		last2button = false;
		gamepad->state.lx = dpadToAnalogX(dpad1) + (midValue - dpadToAnalogX(dpad1)) * scaledTilt1FactorLeftX;
		gamepad->state.ly = dpadToAnalogY(dpad1) + (midValue - dpadToAnalogY(dpad1)) * scaledTilt1FactorLeftY;
		gamepad->state.rx = dpadToAnalogX(dpad2) + (midValue - dpadToAnalogX(dpad2)) * scaledTilt1FactorRightX;
		gamepad->state.ry = dpadToAnalogY(dpad2) + (midValue - dpadToAnalogY(dpad2)) * scaledTilt1FactorRightY;
	}

	// Tilt2
	else if (!pinTilt1Pressed && pinTilt2Pressed && !pinRotate1Pressed && !pinRotate2Pressed) {
		last1button = false;
		last2button = true;
		gamepad->state.lx = dpadToAnalogX(dpad1) + (midValue - dpadToAnalogX(dpad1)) * scaledTilt2FactorLeftX;
		gamepad->state.ly = dpadToAnalogY(dpad1) + (midValue - dpadToAnalogY(dpad1)) * scaledTilt2FactorLeftY;
		gamepad->state.rx = dpadToAnalogX(dpad2) + (midValue - dpadToAnalogX(dpad2)) * scaledTilt2FactorRightX;
		gamepad->state.ry = dpadToAnalogY(dpad2) + (midValue - dpadToAnalogY(dpad2)) * scaledTilt2FactorRightY;
	}

	// Tilt1 + Rotate1 + Rotate 2 = Tilt3
	else if (pinTilt1Pressed && !pinTilt2Pressed && pinRotate1Pressed && pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		gamepad->state.lx = dpadToAnalogX(dpad1) + (midValue - dpadToAnalogX(dpad1)) * scaledTilt3FactorLeftX;
		gamepad->state.ly = dpadToAnalogY(dpad1) + (midValue - dpadToAnalogY(dpad1)) * scaledTilt3FactorLeftY;
		gamepad->state.rx = dpadToAnalogX(dpad2) + (midValue - dpadToAnalogX(dpad2)) * scaledTilt3FactorRightX;
		gamepad->state.ry = dpadToAnalogY(dpad2) + (midValue - dpadToAnalogY(dpad2)) * scaledTilt3FactorRightY;
	}

	// Tilt2 + Rotate1 + Rotate 2 = Tilt4
	else if (!pinTilt1Pressed && pinTilt2Pressed && pinRotate1Pressed && pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		gamepad->state.lx = dpadToAnalogX(dpad1) + (midValue - dpadToAnalogX(dpad1)) * scaledTilt4FactorLeftX;
		gamepad->state.ly = dpadToAnalogY(dpad1) + (midValue - dpadToAnalogY(dpad1)) * scaledTilt4FactorLeftY;
		gamepad->state.rx = dpadToAnalogX(dpad2) + (midValue - dpadToAnalogX(dpad2)) * scaledTilt4FactorRightX;
		gamepad->state.ry = dpadToAnalogY(dpad2) + (midValue - dpadToAnalogY(dpad2)) * scaledTilt4FactorRightY;
	}

	// Rotate1
	else if (!pinTilt1Pressed && !pinTilt2Pressed && pinRotate1Pressed && !pinRotate2Pressed) {
		last1button = true;
		last2button = false;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*0/2);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*0/2);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*1/2);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*1/2);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*2/2);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*2/2);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*3/2);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*3/2);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*1/4);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*1/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*3/4);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*3/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*5/4);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*5/4);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*7/4);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*7/4);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*0/2);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*0/2);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*1/2);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*1/2);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*2/2);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*2/2);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*3/2);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*3/2);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*1/4);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*1/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*3/4);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*3/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*5/4);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*5/4);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*7/4);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*7/4);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
	}

	// Rotate2
	else if (!pinTilt1Pressed && !pinTilt2Pressed && !pinRotate1Pressed && pinRotate2Pressed) {
		last1button = false;
		last2button = true;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*0/2);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*0/2);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*1/2);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*1/2);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*2/2);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*2/2);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*3/2);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*3/2);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*1/4);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*1/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*3/4);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*3/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*5/4);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*5/4);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*7/4);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*7/4);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*0/2);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*0/2);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*1/2);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*1/2);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*2/2);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*2/2);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*3/2);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*3/2);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*1/4);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*1/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*3/4);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*3/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*5/4);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*5/4);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*7/4);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*7/4);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
	}

	// Tilt1 + Tilt2 + Rotate1 = Rotate3
	else if (pinTilt1Pressed && pinTilt2Pressed && pinRotate1Pressed && !pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*0/2);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*0/2);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*1/2);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*1/2);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*2/2);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*2/2);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*3/2);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*3/2);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*1/4);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*1/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*3/4);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*3/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*5/4);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*5/4);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*7/4);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*7/4);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*0/2);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*0/2);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*1/2);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*1/2);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*2/2);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*2/2);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*3/2);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*3/2);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*1/4);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*1/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*3/4);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*3/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*5/4);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*5/4);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*7/4);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*7/4);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
	}

	// Tilt1 + Tilt2 + Rotate2 = Rotate4
	else if (pinTilt1Pressed && pinTilt2Pressed && !pinRotate1Pressed && pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*0/2);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*0/2);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*1/2);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*1/2);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*2/2);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*2/2);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*3/2);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*3/2);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*1/4);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*1/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*3/4);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*3/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*5/4);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*5/4);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*7/4);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*7/4);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*0/2);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*0/2);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*1/2);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*1/2);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*2/2);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*2/2);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*3/2);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*3/2);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*1/4);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*1/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*3/4);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*3/4);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*5/4);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*5/4);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*7/4);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*7/4);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
	}

	// Tilt1 + Rotate1 = Tilt3 + Rotate3
	else if (pinTilt1Pressed && !pinTilt2Pressed && pinRotate1Pressed && !pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt3FactorLeftY);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt3FactorRightY);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
	}

	// Tilt2 + Rotate1 = Tilt4 + Rotate3
	else if (!pinTilt1Pressed && pinTilt2Pressed && pinRotate1Pressed && !pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate3degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate3degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt4FactorLeftY);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate3degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate3degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt4FactorRightY);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
	}

	// Tilt1 + Rotate2 = Tilt3 + Rotate4
	else if (pinTilt1Pressed && !pinTilt2Pressed && !pinRotate1Pressed && pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt3FactorLeftY);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt3FactorRightY);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
	}

	// Tilt2 + Rotate2 = Tilt4 + Rotate4
	else if (!pinTilt1Pressed && pinTilt2Pressed && !pinRotate1Pressed && pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate4degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate4degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt4FactorLeftY);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate4degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate4degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt4FactorRightY);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
	}

	// Not presssing any tilt or rotate buttons
	else if (!pinTilt1Pressed && !pinTilt2Pressed && !pinRotate1Pressed && !pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		gamepad->state.lx = dpadToAnalogX(dpad1);
		gamepad->state.ly = dpadToAnalogY(dpad1);
		gamepad->state.rx = dpadToAnalogX(dpad2);
		gamepad->state.ry = dpadToAnalogY(dpad2);
	}
}


void TiltInput::SOCDTiltClean(SOCDMode leftSOCDMode, SOCDMode rightSOCDMode) {
	// Left Stick SOCD Cleaning
	switch (tiltLeftState & (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN)) {
	case (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN):
		if (leftSOCDMode == SOCD_MODE_UP_PRIORITY)
		{
			tiltLeftState ^= GAMEPAD_MASK_DOWN; // Remove Down
			leftLastTiltUD = DIRECTION_UP; // We're in UP mode
		}
		else if (leftSOCDMode == SOCD_MODE_SECOND_INPUT_PRIORITY && leftLastTiltUD != DIRECTION_NONE)
			tiltLeftState ^= (leftLastTiltUD == DIRECTION_UP) ? GAMEPAD_MASK_UP : GAMEPAD_MASK_DOWN;
		else if (leftSOCDMode == SOCD_MODE_FIRST_INPUT_PRIORITY && leftLastTiltUD != DIRECTION_NONE)
			tiltLeftState ^= (leftLastTiltUD == DIRECTION_UP) ? GAMEPAD_MASK_DOWN : GAMEPAD_MASK_UP;
		else
			leftLastTiltUD = DIRECTION_NONE;
		break;

	case GAMEPAD_MASK_UP:
		leftLastTiltUD = DIRECTION_UP;
		break;

	case GAMEPAD_MASK_DOWN:
		leftLastTiltUD = DIRECTION_DOWN;
		break;

	default:
		leftLastTiltUD = DIRECTION_NONE;
		break;
	}

	switch (tiltLeftState & (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT)) {
	case (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT):
		if (leftSOCDMode == SOCD_MODE_SECOND_INPUT_PRIORITY && leftLastTiltLR != DIRECTION_NONE)
			tiltLeftState ^= (leftLastTiltLR == DIRECTION_LEFT) ? GAMEPAD_MASK_LEFT : GAMEPAD_MASK_RIGHT;
		else if (leftSOCDMode == SOCD_MODE_FIRST_INPUT_PRIORITY && leftLastTiltLR != DIRECTION_NONE)
			tiltLeftState ^= (leftLastTiltLR == DIRECTION_LEFT) ? GAMEPAD_MASK_RIGHT : GAMEPAD_MASK_LEFT;
		else
			leftLastTiltLR = DIRECTION_NONE;
		break;

	case GAMEPAD_MASK_LEFT:
		leftLastTiltLR = DIRECTION_LEFT;
		break;

	case GAMEPAD_MASK_RIGHT:
		leftLastTiltLR = DIRECTION_RIGHT;
		break;

	default:
		leftLastTiltLR = DIRECTION_NONE;
		break;
	}

	// Right Stick SOCD Cleaning
	switch (tiltRightState & (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN)) {
	case (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN):
		if (rightSOCDMode == SOCD_MODE_UP_PRIORITY)
		{
			tiltRightState ^= GAMEPAD_MASK_DOWN; // Remove Down
			rightLastTiltUD = DIRECTION_UP; // We're in UP mode
		}
		else if (rightSOCDMode == SOCD_MODE_SECOND_INPUT_PRIORITY && rightLastTiltUD != DIRECTION_NONE)
			tiltRightState ^= (rightLastTiltUD == DIRECTION_UP) ? GAMEPAD_MASK_UP : GAMEPAD_MASK_DOWN;
		else if (rightSOCDMode == SOCD_MODE_FIRST_INPUT_PRIORITY && rightLastTiltUD != DIRECTION_NONE)
			tiltRightState ^= (rightLastTiltUD == DIRECTION_UP) ? GAMEPAD_MASK_DOWN : GAMEPAD_MASK_UP;
		else
			rightLastTiltUD = DIRECTION_NONE;
		break;

	case GAMEPAD_MASK_UP:
		rightLastTiltUD = DIRECTION_UP;
		break;

	case GAMEPAD_MASK_DOWN:
		rightLastTiltUD = DIRECTION_DOWN;
		break;

	default:
		rightLastTiltUD = DIRECTION_NONE;
		break;
	}

	switch (tiltRightState & (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT)) {
	case (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT):
		if (rightSOCDMode == SOCD_MODE_SECOND_INPUT_PRIORITY && rightLastTiltLR != DIRECTION_NONE)
			tiltRightState ^= (rightLastTiltLR == DIRECTION_LEFT) ? GAMEPAD_MASK_LEFT : GAMEPAD_MASK_RIGHT;
		else if (rightSOCDMode == SOCD_MODE_FIRST_INPUT_PRIORITY && rightLastTiltLR != DIRECTION_NONE)
			tiltRightState ^= (rightLastTiltLR == DIRECTION_LEFT) ? GAMEPAD_MASK_RIGHT : GAMEPAD_MASK_LEFT;
		else
			rightLastTiltLR = DIRECTION_NONE;
		break;

	case GAMEPAD_MASK_LEFT:
		rightLastTiltLR = DIRECTION_LEFT;
		break;

	case GAMEPAD_MASK_RIGHT:
		rightLastTiltLR = DIRECTION_RIGHT;
		break;

	default:
		rightLastTiltLR = DIRECTION_NONE;
		break;
	}
}