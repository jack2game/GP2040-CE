import React from 'react';
import { useTranslation } from 'react-i18next';
import { FormCheck, Row } from 'react-bootstrap';
import * as yup from 'yup';

import Section from '../Components/Section';
import FormSelect from '../Components/FormSelect';

import FormControl from '../Components/FormControl';
import { SOCD_MODES, TILT_SOCD_MODES } from '../Data/Addons';
import { DPAD_MODES } from '../Data/Addons';

export const tiltScheme = {
	TiltInputEnabled: yup.number().required().label('Tilt Input Enabled'),
	factorTilt1LeftX: yup
		.number()
		.label('Tilt 1 Factor Left X')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt1LeftY: yup
		.number()
		.label('Tilt 1 Factor Left Y')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt1RightX: yup
		.number()
		.label('Tilt 1 Factor Right X')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt1RightY: yup
		.number()
		.label('Tilt 1 Factor Right Y')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt2LeftX: yup
		.number()
		.label('Tilt 2 Factor Left X')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt2LeftY: yup
		.number()
		.label('Tilt 2 Factor Left Y')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt2RightX: yup
		.number()
		.label('Tilt 2 Factor Right X')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt2RightY: yup
		.number()
		.label('Tilt 2 Factor Right Y')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt3LeftX: yup
		.number()
		.label('Tilt 3 Factor Left X')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt3LeftY: yup
		.number()
		.label('Tilt 3 Factor Left Y')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt3RightX: yup
		.number()
		.label('Tilt 3 Factor Right X')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt3RightY: yup
		.number()
		.label('Tilt 3 Factor Right Y')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt4LeftX: yup
		.number()
		.label('Tilt 4 Factor Left X')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt4LeftY: yup
		.number()
		.label('Tilt 4 Factor Left Y')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt4RightX: yup
		.number()
		.label('Tilt 4 Factor Right X')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorTilt4RightY: yup
		.number()
		.label('Tilt 4 Factor Right Y')
		.validateRangeWhenValue('TiltInputEnabled', 0, 100),
	factorRotate1Left: yup
		.number()
		.label('Rotate 1 Degree for Left Stick')
		.validateNumberWhenValue('TiltInputEnabled', 0, 360),
	factorRotate2Left: yup
		.number()
		.label('Rotate 2 Degree for Left Stick')
		.validateNumberWhenValue('TiltInputEnabled', 0, 360),
	factorRotate3Left: yup
		.number()
		.label('Rotate 3 Degree for Left Stick')
		.validateNumberWhenValue('TiltInputEnabled', 0, 360),
	factorRotate4Left: yup
		.number()
		.label('Rotate 4 Degree for Left Stick')
		.validateNumberWhenValue('TiltInputEnabled', 0, 360),
	factorRotate1Right: yup
		.number()
		.label('Rotate 1 Degree for Right Stick')
		.validateNumberWhenValue('TiltInputEnabled', 0, 360),
	factorRotate2Right: yup
		.number()
		.label('Rotate 2 Degree for Right Stick')
		.validateNumberWhenValue('TiltInputEnabled', 0, 360),
	factorRotate3Right: yup
		.number()
		.label('Rotate 3 Degree for Right Stick')
		.validateNumberWhenValue('TiltInputEnabled', 0, 360),
	factorRotate4Right: yup
		.number()
		.label('Rotate 4 Degree for Right Stick')
		.validateNumberWhenValue('TiltInputEnabled', 0, 360),
	tiltLeftSOCDMode: yup
		.number()
		.label('Tilt Left Stick SOCD Mode')
		.validateSelectionWhenValue('TiltInputEnabled', TILT_SOCD_MODES),
	tiltRightSOCDMode: yup
		.number()
		.label('Tilt Right Stick SOCD Mode')
		.validateSelectionWhenValue('TiltInputEnabled', TILT_SOCD_MODES),
	tilt1Then2Mode: yup
		.number()
		.label('Tilt 1 then 2 Mode')
		.validateSelectionWhenValue('TiltInputEnabled', DPAD_MODES),
	tilt2Then1Mode: yup
		.number()
		.label('Tilt 2 then 1 Mode')
		.validateSelectionWhenValue('TiltInputEnabled', DPAD_MODES),
	rotate1Then2Mode: yup
		.number()
		.label('Rotate 1 then 2 Mode')
		.validateSelectionWhenValue('TiltInputEnabled', DPAD_MODES),
	rotate2Then1Mode: yup
		.number()
		.label('Rotate 2 then 1 Mode')
		.validateSelectionWhenValue('TiltInputEnabled', DPAD_MODES),
};

export const tiltState = {
	TiltInputEnabled: 0,
	factorTilt1LeftX: 35,
	factorTilt1LeftY: 35,
	factorTilt1RightX: 35,
	factorTilt1RightY: 35,
	factorTilt2LeftX: 65,
	factorTilt2LeftY: 65,
	factorTilt2RightX: 65,
	factorTilt2RightY: 65,
	factorTilt3LeftX: 0,
	factorTilt3LeftY: 0,
	factorTilt3RightX: 0,
	factorTilt3RightY: 0,
	factorTilt4LeftX: 0,
	factorTilt4LeftY: 0,
	factorTilt4RightX: 0,
	factorTilt4RightY: 0,
	factorRotate1Left: 15,
	factorRotate2Left: 345,
	factorRotate3Left: 0,
	factorRotate4Left: 0,
	factorRotate1Right: 15,
	factorRotate2Right: 345,
	factorRotate3Right: 0,
	factorRotate4Right: 0,
};

const Tilt = ({ values, errors, handleChange, handleCheckbox }) => {
	const { t } = useTranslation();
	return (
		<Section title={t('AddonsConfig:tilt-header-text')}>
			<div id="TiltOptions" hidden={!values.TiltInputEnabled}>
				<Row className="mb-3">
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-1-factor-left-x-label')}
						name="factorTilt1LeftX"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt1LeftX}
						error={errors.factorTilt1LeftX}
						isInvalid={errors.factorTilt1LeftX}
						onChange={handleChange}
						min={0}
						max={100}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-2-factor-left-x-label')}
						name="factorTilt2LeftX"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt2LeftX}
						error={errors.factorTilt2LeftX}
						isInvalid={errors.factorTilt2LeftX}
						onChange={handleChange}
						min={0}
						max={100}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-3-factor-left-x-label')}
						name="factorTilt3LeftX"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt3LeftX}
						error={errors.factorTilt3LeftX}
						isInvalid={errors.factorTilt3LeftX}
						onChange={handleChange}
						min={0}
						max={100}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-4-factor-left-x-label')}
						name="factorTilt4LeftX"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt4LeftX}
						error={errors.factorTilt4LeftX}
						isInvalid={errors.factorTilt4LeftX}
						onChange={handleChange}
						min={0}
						max={100}
					/>
				</Row>
				<Row className="mb-3">
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-1-factor-left-y-label')}
						name="factorTilt1LeftY"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt1LeftY}
						error={errors.factorTilt1LeftY}
						isInvalid={errors.factorTilt1LeftY}
						onChange={handleChange}
						min={0}
						max={100}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-2-factor-left-y-label')}
						name="factorTilt2LeftY"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt2LeftY}
						error={errors.factorTilt2LeftY}
						isInvalid={errors.factorTilt2LeftY}
						onChange={handleChange}
						min={0}
						max={100}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-3-factor-left-y-label')}
						name="factorTilt3LeftY"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt3LeftY}
						error={errors.factorTilt3LeftY}
						isInvalid={errors.factorTilt3LeftY}
						onChange={handleChange}
						min={0}
						max={100}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-4-factor-left-y-label')}
						name="factorTilt4LeftY"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt4LeftY}
						error={errors.factorTilt4LeftY}
						isInvalid={errors.factorTilt4LeftY}
						onChange={handleChange}
						min={0}
						max={100}
					/>
				</Row>
				<Row className="mb-3">
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-1-factor-right-x-label')}
						name="factorTilt1RightX"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt1RightX}
						error={errors.factorTilt1RightX}
						isInvalid={errors.factorTilt1RightX}
						onChange={handleChange}
						min={0}
						max={100}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-2-factor-right-x-label')}
						name="factorTilt2RightX"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt2RightX}
						error={errors.factorTilt2RightX}
						isInvalid={errors.factorTilt2RightX}
						onChange={handleChange}
						min={0}
						max={100}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-3-factor-right-x-label')}
						name="factorTilt3RightX"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt3RightX}
						error={errors.factorTilt3RightX}
						isInvalid={errors.factorTilt3RightX}
						onChange={handleChange}
						min={0}
						max={100}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-4-factor-right-x-label')}
						name="factorTilt4RightX"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt4RightX}
						error={errors.factorTilt4RightX}
						isInvalid={errors.factorTilt4RightX}
						onChange={handleChange}
						min={0}
						max={100}
					/>
				</Row>
				<Row className="mb-3">
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-1-factor-right-y-label')}
						name="factorTilt1RightY"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt1RightY}
						error={errors.factorTilt1RightY}
						isInvalid={errors.factorTilt1RightY}
						onChange={handleChange}
						min={0}
						max={100}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-2-factor-right-y-label')}
						name="factorTilt2RightY"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt2RightY}
						error={errors.factorTilt2RightY}
						isInvalid={errors.factorTilt2RightY}
						onChange={handleChange}
						min={0}
						max={100}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-3-factor-right-y-label')}
						name="factorTilt3RightY"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt3RightY}
						error={errors.factorTilt3RightY}
						isInvalid={errors.factorTilt3RightY}
						onChange={handleChange}
						min={0}
						max={100}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:tilt-4-factor-right-y-label')}
						name="factorTilt4RightY"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorTilt4RightY}
						error={errors.factorTilt4RightY}
						isInvalid={errors.factorTilt4RightY}
						onChange={handleChange}
						min={0}
						max={100}
					/>
				</Row>
				<Row className="mb-3">
					<FormControl
						type="number"
						label={t('AddonsConfig:rotate-1-factor-left-label')}
						name="factorRotate1Left"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorRotate1Left}
						error={errors.factorRotate1Left}
						isInvalid={errors.factorRotate1Left}
						onChange={handleChange}
						min={0}
						max={360}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:rotate-2-factor-left-label')}
						name="factorRotate2Left"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorRotate2Left}
						error={errors.factorRotate2Left}
						isInvalid={errors.factorRotate2Left}
						onChange={handleChange}
						min={0}
						max={360}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:rotate-3-factor-left-label')}
						name="factorRotate3Left"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorRotate3Left}
						error={errors.factorRotate3Left}
						isInvalid={errors.factorRotate3Left}
						onChange={handleChange}
						min={0}
						max={360}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:rotate-4-factor-left-label')}
						name="factorRotate4Left"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorRotate4Left}
						error={errors.factorRotate4Left}
						isInvalid={errors.factorRotate4Left}
						onChange={handleChange}
						min={0}
						max={360}
					/>
				</Row>
				<Row className="mb-3">
					<FormControl
						type="number"
						label={t('AddonsConfig:rotate-1-factor-right-label')}
						name="factorRotate1Right"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorRotate1Right}
						error={errors.factorRotate1Right}
						isInvalid={errors.factorRotate1Right}
						onChange={handleChange}
						min={0}
						max={360}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:rotate-2-factor-right-label')}
						name="factorRotate2Right"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorRotate2Right}
						error={errors.factorRotate2Right}
						isInvalid={errors.factorRotate2Right}
						onChange={handleChange}
						min={0}
						max={360}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:rotate-3-factor-right-label')}
						name="factorRotate3Right"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorRotate3Right}
						error={errors.factorRotate3Right}
						isInvalid={errors.factorRotate3Right}
						onChange={handleChange}
						min={0}
						max={360}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:rotate-4-factor-right-label')}
						name="factorRotate4Right"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.factorRotate4Right}
						error={errors.factorRotate4Right}
						isInvalid={errors.factorRotate4Right}
						onChange={handleChange}
						min={0}
						max={360}
					/>
				</Row>
				<Row className="mb-3">
					<FormSelect
						label={t('AddonsConfig:tilt-1-then-2-mode')}
						name="tilt1Then2Mode"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.tilt1Then2Mode}
						error={errors.tilt1Then2Mode}
						isInvalid={errors.tilt1Then2Mode}
						onChange={handleChange}
					>
						{DPAD_MODES.map((o, i) => (
							<option key={`button-tilt1Then2Mode-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>
					<FormSelect
						label={t('AddonsConfig:tilt-2-then-1-mode')}
						name="tilt2Then1Mode"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.tilt2Then1Mode}
						error={errors.tilt2Then1Mode}
						isInvalid={errors.tilt2Then1Mode}
						onChange={handleChange}
					>
						{DPAD_MODES.map((o, i) => (
							<option key={`button-tilt2Then1Mode-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>
					<FormSelect
						label={t('AddonsConfig:rotate-1-then-2-mode')}
						name="rotate1Then2Mode"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.rotate1Then2Mode}
						error={errors.rotate1Then2Mode}
						isInvalid={errors.rotate1Then2Mode}
						onChange={handleChange}
					>
						{DPAD_MODES.map((o, i) => (
							<option key={`button-rotate1Then2Mode-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>
					<FormSelect
						label={t('AddonsConfig:rotate-2-then-1-mode')}
						name="rotate2Then1Mode"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.rotate2Then1Mode}
						error={errors.rotate2Then1Mode}
						isInvalid={errors.rotate2Then1Mode}
						onChange={handleChange}
					>
						{DPAD_MODES.map((o, i) => (
							<option key={`button-rotate2Then1Mode-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>
					<FormSelect
						label={t('AddonsConfig:tilt-left-socd-mode-label')}
						name="tiltLeftSOCDMode"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.tiltLeftSOCDMode}
						error={errors.tiltLeftSOCDMode}
						isInvalid={errors.tiltLeftSOCDMode}
						onChange={handleChange}
					>
						{TILT_SOCD_MODES.map((o, i) => (
							<option key={`button-tiltLeftSOCDMode-option-${i}`} value={o.value}>
								{t(`AddonsConfig:tilt-socd-mode-${i}`)}
							</option>
						))}
					</FormSelect>
					<FormSelect
						label={t('AddonsConfig:tilt-right-socd-mode-label')}
						name="tiltRightSOCDMode"
						className="form-select-sm"
						groupClassName="col-sm-2 mb-3"
						value={values.tiltRightSOCDMode}
						error={errors.tiltRightSOCDMode}
						isInvalid={errors.tiltRightSOCDMode}
						onChange={handleChange}
					>
						{TILT_SOCD_MODES.map((o, i) => (
							<option key={`button-tiltRightSOCDMode-option-${i}`} value={o.value}>
								{t(`AddonsConfig:tilt-socd-mode-${i}`)}
							</option>
						))}
					</FormSelect>
				</Row>
			</div>
			<FormCheck
				label={t('Common:switch-enabled')}
				type="switch"
				id="TiltInputButton"
				reverse={true}
				isInvalid={false}
				checked={Boolean(values.TiltInputEnabled)}
				onChange={(e) => {
					handleCheckbox('TiltInputEnabled', values);
					handleChange(e);
				}}
			/>
		</Section>
	);
};

export default Tilt;
