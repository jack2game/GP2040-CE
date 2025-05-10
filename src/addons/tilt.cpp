#include "addons/tilt.h"
#include "drivermanager.h"
#include "storagemanager.h"
#include "helper.h"
#include "config.pb.h"
#include "math.h"

bool TiltInput::available() {
    const TiltOptions& options = Storage::getInstance().getAddonOptions().tiltOptions;
    return options.enabled;
}

void TiltInput::setup() {
	const TiltOptions& options = Storage::getInstance().getAddonOptions().tiltOptions;

	tiltLeftSOCDMode = options.tiltLeftSOCDMode;
	tiltRightSOCDMode = options.tiltRightSOCDMode;

	tilt1Then2Mode = options.tilt1Then2Mode;
	tilt2Then1Mode = options.tilt2Then1Mode;
	rotate1Then2Mode = options.rotate1Then2Mode;
	rotate2Then1Mode = options.rotate2Then1Mode;

	mapAnalogLSXNeg = new GamepadButtonMapping(0);
	mapAnalogLSXPos = new GamepadButtonMapping(0);
	mapAnalogLSYNeg = new GamepadButtonMapping(0);
	mapAnalogLSYPos = new GamepadButtonMapping(0);
	mapAnalogRSXNeg = new GamepadButtonMapping(GAMEPAD_MASK_LEFT);
	mapAnalogRSXPos = new GamepadButtonMapping(GAMEPAD_MASK_RIGHT);
	mapAnalogRSYNeg = new GamepadButtonMapping(GAMEPAD_MASK_UP);
	mapAnalogRSYPos = new GamepadButtonMapping(GAMEPAD_MASK_DOWN);
	mapAnalogModOne = new GamepadButtonMapping(ANALOG_DIRECTION_MOD_ONE);
	mapAnalogModTwo = new GamepadButtonMapping(ANALOG_DIRECTION_MOD_TWO);
	mapAnalogRotateOne = new GamepadButtonMapping(ANALOG_DIRECTION_ROTATE_ONE);
	mapAnalogRotateTwo = new GamepadButtonMapping(ANALOG_DIRECTION_ROTATE_TWO);

	EventManager::getInstance().registerEventHandler(GP_EVENT_PROFILE_CHANGE, GPEVENT_CALLBACK(this->handleProfileChange(event)));

	reloadMappings();

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

	Mask_t values = Storage::getInstance().GetGamepad()->debouncedGpio;

	// Need to invert since we're using pullups
	tiltLeftState = 0
		| ((values & mapAnalogLSXNeg->pinMask)    ? mapAnalogLSXNeg->buttonMask : 0)
		| ((values & mapAnalogLSXPos->pinMask)    ? mapAnalogLSXPos->buttonMask : 0)
		| ((values & mapAnalogLSYNeg->pinMask)    ? mapAnalogLSYNeg->buttonMask : 0)
		| ((values & mapAnalogLSYPos->pinMask)    ? mapAnalogLSYPos->buttonMask : 0);

	if (tiltLeftState == 0 && sliderLeft && !sliderRight){
		tiltLeftState |= gamepad->state.dpad;
	}

	tiltRightState = 0
		| ((values & mapAnalogRSXNeg->pinMask)    ? mapAnalogRSXNeg->buttonMask : 0)
		| ((values & mapAnalogRSXPos->pinMask)    ? mapAnalogRSXPos->buttonMask : 0)
		| ((values & mapAnalogRSYNeg->pinMask)    ? mapAnalogRSYNeg->buttonMask : 0)
		| ((values & mapAnalogRSYPos->pinMask)    ? mapAnalogRSYPos->buttonMask : 0);

	if (tiltRightState == 0 && sliderRight && !sliderLeft){
		tiltRightState |= gamepad->state.dpad;
	}

	// Convert gamepad from process() output to uint8 value
	uint8_t gamepadState = gamepad->state.dpad;

	gamepad->state.dpad = gamepadState;
}

void TiltInput::process()
{
	const TiltOptions& options = Storage::getInstance().getAddonOptions().tiltOptions;
	
	// don't process if no pins are bound. we can pause by disabling the addon, but pins are required.
	if (!options.enabled || ((mapAnalogModOne->pinMask == 0) && (mapAnalogModTwo->pinMask == 0) && (mapAnalogRotateOne->pinMask == 0) && (mapAnalogRotateTwo->pinMask == 0))) return;

	SOCDTiltClean(tiltLeftSOCDMode, tiltRightSOCDMode);

	Gamepad* gamepad = Storage::getInstance().GetGamepad();

	// Set Tilt Output
	OverrideGamepad(gamepad, tiltLeftState, tiltRightState);
}

void TiltInput::reloadMappings() {
    const TiltOptions& options = Storage::getInstance().getAddonOptions().tiltOptions;

	tilt1FactorLeftX = options.factorTilt1LeftX;
	tilt1FactorLeftY = options.factorTilt1LeftY;
	tilt1FactorRightX = options.factorTilt1RightX;
	tilt1FactorRightY = options.factorTilt1RightY;
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
	rotate1FactorLeft = options.factorRotate1Left;
	rotate2FactorLeft = options.factorRotate2Left;
	rotate3FactorLeft = options.factorRotate3Left;
	rotate4FactorLeft = options.factorRotate4Left;
	rotate1FactorRight = options.factorRotate1Right;
	rotate2FactorRight = options.factorRotate2Right;
	rotate3FactorRight = options.factorRotate3Right;
	rotate4FactorRight = options.factorRotate4Right;

	mapAnalogLSXNeg->pinMask  = 0;
	mapAnalogLSXPos->pinMask  = 0;
	mapAnalogLSYNeg->pinMask  = 0;
	mapAnalogLSYPos->pinMask  = 0;
	mapAnalogRSXNeg->pinMask  = 0;
	mapAnalogRSXPos->pinMask  = 0;
	mapAnalogRSYNeg->pinMask  = 0;
	mapAnalogRSYPos->pinMask  = 0;
	// mapAnalogModOne->pinMask  = 0;
	// mapAnalogModTwo->pinMask  = 0;
	// mapAnalogRotateOne->pinMask  = 0;
	// mapAnalogRotateTwo->pinMask  = 0;

	GpioMappingInfo* pinMappings = Storage::getInstance().getProfilePinMappings();
	for (Pin_t pin = 0; pin < (Pin_t)NUM_BANK0_GPIOS; pin++)
	{
		switch (pinMappings[pin].action) {
			case GpioAction::ANALOG_DIRECTION_LS_X_NEG:	mapAnalogLSXNeg->pinMask |= 1 << pin; break;
			case GpioAction::ANALOG_DIRECTION_LS_X_POS:	mapAnalogLSXPos->pinMask |= 1 << pin; break;
			case GpioAction::ANALOG_DIRECTION_LS_Y_NEG:	mapAnalogLSYNeg->pinMask |= 1 << pin; break;
			case GpioAction::ANALOG_DIRECTION_LS_Y_POS:	mapAnalogLSYPos->pinMask |= 1 << pin; break;
			case GpioAction::ANALOG_DIRECTION_RS_X_NEG:	mapAnalogRSXNeg->pinMask |= 1 << pin; break;
			case GpioAction::ANALOG_DIRECTION_RS_X_POS:	mapAnalogRSXPos->pinMask |= 1 << pin; break;
			case GpioAction::ANALOG_DIRECTION_RS_Y_NEG:	mapAnalogRSYNeg->pinMask |= 1 << pin; break;
			case GpioAction::ANALOG_DIRECTION_RS_Y_POS:	mapAnalogRSYPos->pinMask |= 1 << pin; break;
			case GpioAction::ANALOG_DIRECTION_MOD_ONE:	mapAnalogModOne->pinMask |= 1 << pin; break;
			case GpioAction::ANALOG_DIRECTION_MOD_TWO:	mapAnalogModTwo->pinMask |= 1 << pin; break;
			case GpioAction::ANALOG_DIRECTION_ROTATE_ONE:	mapAnalogRotateOne->pinMask |= 1 << pin; break;
			case GpioAction::ANALOG_DIRECTION_ROTATE_TWO:	mapAnalogRotateTwo->pinMask |= 1 << pin; break;
			default:	break;
		}
	}

	tiltLeftState = 0;
	tiltRightState = 0;

	last1button = false;
	last2button = false;

	leftLastTiltUD = DIRECTION_NONE;
	leftLastTiltLR = DIRECTION_NONE;

	rightLastTiltUD = DIRECTION_NONE;
	rightLastTiltLR = DIRECTION_NONE;
}

void TiltInput::OverrideGamepad(Gamepad* gamepad, uint8_t dpad1, uint8_t dpad2) {
	Mask_t values = gamepad->debouncedGpio;

	bool pinTilt1Pressed = (values & mapAnalogModOne->pinMask);
	bool pinTilt2Pressed = (values & mapAnalogModTwo->pinMask);
	bool pinRotate1Pressed = (values & mapAnalogRotateOne->pinMask);
	bool pinRotate2Pressed = (values & mapAnalogRotateTwo->pinMask);

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
	else if (pinTilt1Pressed && !pinTilt2Pressed && !pinRotate1Pressed && !pinRotate2Pressed) {
		last1button = true;
		last2button = false;
		// gamepad->state.lx = dpadToAnalogX(dpad1) + (midValue - dpadToAnalogX(dpad1)) * scaledTilt1FactorLeftX;
		// gamepad->state.ly = dpadToAnalogY(dpad1) + (midValue - dpadToAnalogY(dpad1)) * scaledTilt1FactorLeftY;
		// gamepad->state.rx = dpadToAnalogX(dpad2) + (midValue - dpadToAnalogX(dpad2)) * scaledTilt1FactorRightX;
		// gamepad->state.ry = dpadToAnalogY(dpad2) + (midValue - dpadToAnalogY(dpad2)) * scaledTilt1FactorRightY;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt1FactorLeftY);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt1FactorRightY);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
	}

	// Tilt2
	else if (!pinTilt1Pressed && pinTilt2Pressed && !pinRotate1Pressed && !pinRotate2Pressed) {
		last1button = false;
		last2button = true;
		// gamepad->state.lx = dpadToAnalogX(dpad1) + (midValue - dpadToAnalogX(dpad1)) * scaledTilt2FactorLeftX;
		// gamepad->state.ly = dpadToAnalogY(dpad1) + (midValue - dpadToAnalogY(dpad1)) * scaledTilt2FactorLeftY;
		// gamepad->state.rx = dpadToAnalogX(dpad2) + (midValue - dpadToAnalogX(dpad2)) * scaledTilt2FactorRightX;
		// gamepad->state.ry = dpadToAnalogY(dpad2) + (midValue - dpadToAnalogY(dpad2)) * scaledTilt2FactorRightY;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt2FactorLeftY);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt2FactorRightY);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
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

	// Tilt1 + Rotate1 + Rotate 2 = Tilt3
	else if (pinTilt1Pressed && !pinTilt2Pressed && pinRotate1Pressed && pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		// gamepad->state.lx = dpadToAnalogX(dpad1) + (midValue - dpadToAnalogX(dpad1)) * scaledTilt3FactorLeftX;
		// gamepad->state.ly = dpadToAnalogY(dpad1) + (midValue - dpadToAnalogY(dpad1)) * scaledTilt3FactorLeftY;
		// gamepad->state.rx = dpadToAnalogX(dpad2) + (midValue - dpadToAnalogX(dpad2)) * scaledTilt3FactorRightX;
		// gamepad->state.ry = dpadToAnalogY(dpad2) + (midValue - dpadToAnalogY(dpad2)) * scaledTilt3FactorRightY;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt3FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt3FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt3FactorLeftY);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt3FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt3FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt3FactorRightY);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
	}

	// Tilt2 + Rotate1 + Rotate 2 = Tilt4
	else if (!pinTilt1Pressed && pinTilt2Pressed && pinRotate1Pressed && pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		// gamepad->state.lx = dpadToAnalogX(dpad1) + (midValue - dpadToAnalogX(dpad1)) * scaledTilt4FactorLeftX;
		// gamepad->state.ly = dpadToAnalogY(dpad1) + (midValue - dpadToAnalogY(dpad1)) * scaledTilt4FactorLeftY;
		// gamepad->state.rx = dpadToAnalogX(dpad2) + (midValue - dpadToAnalogX(dpad2)) * scaledTilt4FactorRightX;
		// gamepad->state.ry = dpadToAnalogY(dpad2) + (midValue - dpadToAnalogY(dpad2)) * scaledTilt4FactorRightY;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt4FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt4FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt4FactorLeftY);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*0/2) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/2) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*2/2) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/2) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*1/4) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*3/4) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*5/4) * (1-scaledTilt4FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt4FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(0 / 180 * M_PI + M_PI*7/4) * (1-scaledTilt4FactorRightY);
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

	// Tilt1 + Rotate1
	else if (pinTilt1Pressed && !pinTilt2Pressed && pinRotate1Pressed && !pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt1FactorLeftY);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt1FactorRightY);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
	}

	// Tilt2 + Rotate1
	else if (!pinTilt1Pressed && pinTilt2Pressed && pinRotate1Pressed && !pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate1degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate1degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt2FactorLeftY);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate1degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate1degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt2FactorRightY);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
	}

	// Tilt1 + Rotate2
	else if (pinTilt1Pressed && !pinTilt2Pressed && !pinRotate1Pressed && pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt1FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt1FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt1FactorLeftY);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt1FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt1FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt1FactorRightY);
			break;
		default:
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
			break;
		}
	}

	// Tilt2 + Rotate2
	else if (!pinTilt1Pressed && pinTilt2Pressed && !pinRotate1Pressed && pinRotate2Pressed) {
		last1button = false;
		last2button = false;
		switch (tiltLeftState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*0/2) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*1/2) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*2/2) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*3/2) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*1/4) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*3/4) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*5/4) * (1-scaledTilt2FactorLeftY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.lx = midValue + midValue * sin(rotate2degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt2FactorLeftX);
			gamepad->state.ly = midValue - midValue * cos(rotate2degreeLeft / 180 * M_PI + M_PI*7/4) * (1-scaledTilt2FactorLeftY);
			break;
		default:
			gamepad->state.lx = dpadToAnalogX(dpad1);
			gamepad->state.ly = dpadToAnalogY(dpad1);
			break;
		}

		switch (tiltRightState) {
		case (GAMEPAD_MASK_UP):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*0/2) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*1/2) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*2/2) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*3/2) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*1/4) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*3/4) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*5/4) * (1-scaledTilt2FactorRightY);
			break;
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT):
			gamepad->state.rx = midValue + midValue * sin(rotate2degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt2FactorRightX);
			gamepad->state.ry = midValue - midValue * cos(rotate2degreeRight / 180 * M_PI + M_PI*7/4) * (1-scaledTilt2FactorRightY);
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

uint16_t TiltInput::getAnalogValue(bool isMin, bool isMax) {
	uint16_t midValue = GAMEPAD_JOYSTICK_MID;
	if ( DriverManager::getInstance().getDriver() != nullptr ) {
		midValue = DriverManager::getInstance().getDriver()->GetJoystickMidValue();
	}

	if (isMin && !isMax) {
		return GAMEPAD_JOYSTICK_MIN;
	} else if (!isMin && isMax) {
		return GAMEPAD_JOYSTICK_MAX;
	} else {
		return midValue;
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

void TiltInput::handleProfileChange(GPEvent* e) {
	reloadMappings();
}
