export const BUTTONS = {
	gp2040: {
		label: 'GP2040',
		value: 'gp2040',
		Up: 'Up',
		Down: 'Down',
		Left: 'Left',
		Right: 'Right',
		B1: 'B1',
		B2: 'B2',
		B3: 'B3',
		B4: 'B4',
		L1: 'L1',
		R1: 'R1',
		L2: 'L2',
		R2: 'R2',
		S1: 'S1',
		S2: 'S2',
		L3: 'L3',
		R3: 'R3',
		A1: 'A1',
		A2: 'A2',
		Fn: 'Function',
	},
	arcade: {
		label: 'Arcade',
		value: 'arcade',
		Up: 'Up',
		Down: 'Down',
		Left: 'Left',
		Right: 'Right',
		B1: 'K1',
		B2: 'K2',
		B3: 'P1',
		B4: 'P2',
		L1: 'P4',
		R1: 'P3',
		L2: 'K4',
		R2: 'K3',
		S1: 'Select',
		S2: 'Start',
		L3: 'L3',
		R3: 'R3',
		A1: 'Home',
		A2: '-',
		Fn: 'Function',
	},
	xinput: {
		label: 'XInput',
		value: 'xinput',
		Up: 'Up',
		Down: 'Down',
		Left: 'Left',
		Right: 'Right',
		B1: 'A',
		B2: 'B',
		B3: 'X',
		B4: 'Y',
		L1: 'LB',
		R1: 'RB',
		L2: 'LT',
		R2: 'RT',
		S1: 'Back',
		S2: 'Start',
		L3: 'LS',
		R3: 'RS',
		A1: 'Guide',
		A2: '-',
		Fn: 'Function',
	},
	switch: {
		label: 'Nintendo Switch',
		value: 'switch',
		Up: 'Up',
		Down: 'Down',
		Left: 'Left',
		Right: 'Right',
		B1: 'B',
		B2: 'A',
		B3: 'Y',
		B4: 'X',
		L1: 'L',
		R1: 'R',
		L2: 'ZL',
		R2: 'ZR',
		S1: 'Minus',
		S2: 'Plus',
		L3: 'LS',
		R3: 'RS',
		A1: 'Home',
		A2: 'Capture',
		Fn: 'Function',
	},
	ps3: {
		label: 'PS3',
		value: 'ps3',
		Up: 'Up',
		Down: 'Down',
		Left: 'Left',
		Right: 'Right',
		B1: 'Cross',
		B2: 'Circle',
		B3: 'Square',
		B4: 'Triangle',
		L1: 'L1',
		R1: 'R1',
		L2: 'L2',
		R2: 'R2',
		S1: 'Select',
		S2: 'Start',
		L3: 'L3',
		R3: 'R3',
		A1: 'PS',
		A2: '-',
		Fn: 'Function',
	},
	ps4: {
		label: 'PS4',
		value: 'ps4',
		Up: 'Up',
		Down: 'Down',
		Left: 'Left',
		Right: 'Right',
		B1: 'Cross',
		B2: 'Circle',
		B3: 'Square',
		B4: 'Triangle',
		L1: 'L1',
		R1: 'R1',
		L2: 'L2',
		R2: 'R2',
		S1: 'Share',
		S2: 'Options',
		L3: 'L3',
		R3: 'R3',
		A1: 'PS',
		A2: 'Touchpad',
		Fn: 'Function',
	},
	dinput: {
		label: 'DirectInput',
		value: 'dinput',
		Up: 'Up',
		Down: 'Down',
		Left: 'Left',
		Right: 'Right',
		B1: '2',
		B2: '3',
		B3: '1',
		B4: '4',
		L1: '5',
		R1: '6',
		L2: '7',
		R2: '8',
		S1: '9',
		S2: '10',
		L3: '11',
		R3: '12',
		A1: '13',
		A2: '14',
		Fn: 'Function',
	},
};

export const AUX_BUTTONS = ['S1', 'S2', 'L3', 'R3', 'A1', 'A2'];
export const MAIN_BUTTONS = [
	'Up',
	'Down',
	'Left',
	'Right',
	'B1',
	'B2',
	'B3',
	'B4',
	'L1',
	'R1',
	'L2',
	'R2',
];

export const STICK_LAYOUT = [
	[null, 'Left', null],
	['Up', null, 'Down'],
	[null, 'Right', null],
	['B3', null, 'B1'],
	['B4', null, 'B2'],
	['R1', null, 'R2'],
	['L1', null, 'L2'],
];

export const STICKLESS_LAYOUT = [
	['Left', null, null],
	['Down', null, null],
	['Right', null, null],
	[null, 'Up', null],
	['B3', 'B1', null],
	['B4', 'B2', null],
	['R1', 'R2', null],
	['L1', 'L2', null],
];

export const KEYBOARD_LAYOUT = [
	[null, 'Left'],
	['Up', 'Down'],
	[null, 'Right'],
	['B3', 'B1'],
	['B4', 'B2'],
	['R1', 'R2'],
	['L1', 'L2'],
];

export const BUTTON_MASKS = [
	{ label: 'None', value: 0 },
	{ label: 'B1', value: 1 << 0 },
	{ label: 'B2', value: 1 << 1 },
	{ label: 'B3', value: 1 << 2 },
	{ label: 'B4', value: 1 << 3 },
	{ label: 'L1', value: 1 << 4 },
	{ label: 'R1', value: 1 << 5 },
	{ label: 'L2', value: 1 << 6 },
	{ label: 'R2', value: 1 << 7 },
	{ label: 'S1', value: 1 << 8 },
	{ label: 'S2', value: 1 << 9 },
	{ label: 'L3', value: 1 << 10 },
	{ label: 'R3', value: 1 << 11 },
	{ label: 'A1', value: 1 << 12 },
	{ label: 'A2', value: 1 << 13 },
	{ label: 'Up', value: 1 << 16 },
	{ label: 'Down', value: 1 << 17 },
	{ label: 'Left', value: 1 << 18 },
	{ label: 'Right', value: 1 << 19 },
];

export const ANALOG_PINS = [26, 27, 28, 29];

// deep copy and swp
export const getButtonLabels = (labelType, swapTpShareLabels = false)=> {
	const buttons = BUTTONS[labelType];

	if (labelType == 'ps4' && swapTpShareLabels) {
		const buttonLabelS1 = buttons['S1'];
		const buttonLabelA2 = buttons['A2'];
		return {
			...buttons,
			"S1": buttonLabelA2,
			"A2": buttonLabelS1,
		}
	} else {
		return buttons
	}
}