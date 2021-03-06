/*
 *  ACMidiDefines.h
 *
 *  Created by Michael Markert, audiocommander on 29.07.05
 *  Adapted to MIOS on 21.05.06.
 *  Copyright 2006 Michael Markert, http://www.audiocommander.de
 *
 */

/*
 * Released under GNU General Public License
 * http://www.gnu.org/licenses/gpl.html
 *
 * This program is free software; you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation
 *
 * YOU ARE ALLOWED TO COPY AND CHANGE
 * BUT YOU MUST RELEASE THE SOURCE TOO (UNDER GNU GPL) IF YOU RELEASE YOUR PRODUCT
 * YOU ARE NOT ALLOWED NOT USE IT WITHIN PROPRIETARY CLOSED-SOURCE PROJECTS
 */


#ifndef _ACMIDIDEFINES_H
#define _ACMIDIDEFINES_H




// **************************** //
// ** Channel Voice Messages ** //
// **************************** //
#ifdef _DEBUG_C
	#pragma mark -
	#pragma mark Channel Voice Messages
#endif

// MIDI_NOTE_OFF:		128..143
#define MIDI_NOTE_OFF	0x80
// MIDI_NOTE_ON:		144..159
#define MIDI_NOTE_ON	0x90
// MIDI_POLY_AFTER:		160..175
#define MIDI_POLY_AFTER 0xA0
// MIDI_CC:				176..191
#define MIDI_CC			0xB0
// MIDI_PRG:			192..207
#define MIDI_PRG		0xC0
// MIDI_CH_AFTER:		208..223
#define MIDI_CH_AFTER	0xD0
// MIDI_PITCH:			224..239	// Pitch value is 14bit (-8192..+8191)
#define MIDI_PITCH		0xE0



// ****************************** //
// ** Channel Control Messages ** //
// ****************************** //
#ifdef _DEBUG_C
	#pragma mark -
	#pragma mark Channel Control Messages
#endif
// MIDI_CC_BANK_SELECT				0
#define MIDI_CC_BANK_SELECT			0x00

// MIDI_CC_MODULATION				1
// MIDI_CC_BREATH					2
// MIDI_CC_FOOT						4
// MIDI_CC_PORTAMENTO				5
// MIDI_CC_DATA_ENTRY_MSB			6
// MIDI_CC_VOLUME					7
// MIDI_CC_BALANCE					8
// MIDI_CC_PAN						10
// MIDI_CC_EXPRESSION				11
// MIDI_CC_FX1						12
// MIDI_CC_FX2						13
#define MIDI_CC_MODULATION			0x01
#define MIDI_CC_BREATH				0x02
#define MIDI_CC_FOOT				0x04
#define MIDI_CC_PORTAMENTO			0x05
#define MIDI_CC_DATA_ENTRY_MSB		0x06
#define MIDI_CC_VOLUME				0x07
#define MIDI_CC_BALANCE				0x08
#define MIDI_CC_PAN					0x0A
#define MIDI_CC_EXPRESSION			0x0B
#define MIDI_CC_FX1					0x0C
#define MIDI_CC_FX2					0x0D

// 14..31 undefined & general purpose
// 32..63 LSB MODES:				(Byte b = LSByte - 32)
// MIDI_CC_BANK_SELECT_LSB			32
// MIDI_CC_MODULATION_LSB			33
// MIDI_CC_BREATH_LSB				34
// MIDI_CC_FOOT_LSB					36
// MIDI_CC_PORTAMENTO_LSB			37
// MIDI_CC_DATA_ENTRY_LSB			38
// MIDI_CC_VOLUME_LSB				39
// MIDI_CC_BALANCE_LSB				40
// MIDI_CC_PAN_LSB					42
// MIDI_CC_EXPRESSION_LSB			43
// MIDI_CC_FX1_LSB					44
// MIDI_CC_FX2_LSB					45
#define MIDI_CC_BANK_SELECT_LSB		0x20
#define MIDI_CC_MODULATION_LSB		0x21
#define MIDI_CC_BREATH_LSB			0x22
#define MIDI_CC_FOOT_LSB			0x24
#define MIDI_CC_PORTAMENTO_LSB		0x25
#define MIDI_CC_DATA_ENTRY_LSB		0x26
#define MIDI_CC_VOLUME_LSB			0x27
#define MIDI_CC_BALANCE_LSB			0x28
#define MIDI_CC_PAN_LSB				0x2A
#define MIDI_CC_EXPRESSION_LSB		0x2B
#define MIDI_CC_FX1_LSB				0x2C
#define MIDI_CC_FX2_LSB				0x2D

#ifdef _DEBUG_C
	#pragma mark -
#endif
// ________ < 63 off, > 64 on ________
// MIDI_CC_PEDAL (=SUSTAIN)			64
// MIDI_CC_PORTAMENTO_ONOFF			65
// MIDI_CC_SUSTENUTO				66
// MIDI_CC_SOFT_PEDAL				67
// MIDI_CC_LEGATO					68
// MIDI_CC_HOLD						69
#define MIDI_CC_PEDAL				0x40
#define MIDI_CC_PORTAMENTO_ONOFF	0x41
#define MIDI_CC_SUSTENUTO			0x42
#define MIDI_CC_SOFT_PEDAL			0x43
#define MIDI_CC_LEGATO				0x44
#define MIDI_CC_HOLD				0x45

#ifdef _DEBUG_C
	#pragma mark -
#endif
// ______ LSB Sound Controllers ______
// MIDI_CC_VARIATION				70
// MIDI_CC_TIMBRE					71
// MIDI_CC_RELEASE					72
// MIDI_CC_ATTACK					73
// MIDI_CC_BRIGHTNESS				74
// MIDI_CC_DECAY					75
// MIDI_CC_VIBRATO_RATE				76
// MIDI_CC_VIBRATO_DEPTH			77
// MIDI_CC_VIBRATO_DELAY			78
#define MIDI_CC_VARIATION			0x46
#define MIDI_CC_TIMBRE				0x47
#define MIDI_CC_RELEASE				0x48
#define MIDI_CC_ATTACK				0x49
#define MIDI_CC_BRIGHTNESS			0x4A
#define MIDI_CC_DECAY				0x4B
#define MIDI_CC_VIBRATO_RATE		0x4C
#define MIDI_CC_VIBRATO_DEPTH		0x4D
#define MIDI_CC_VIBRATO_DELAY		0x4E

// MIDI_CC_FX1_DEPTH				91
// MIDI_CC_FX2_DEPTH				92
// MIDI_CC_FX3_DEPTH				93
// MIDI_CC_FX4_DEPTH				94
// MIDI_CC_FX5_DEPTH				95
#define MIDI_CC_FX1_DEPTH			0x5B
#define MIDI_CC_FX2_DEPTH			0x5C
#define MIDI_CC_FX3_DEPTH			0x5D
#define MIDI_CC_FX4_DEPTH			0x5E
#define MIDI_CC_FX5_DEPTH			0x5F

#ifdef _DEBUG_C
	#pragma mark -
#endif
// ______ Registered Parameters ______
// MIDI_CC_DATA_INCREMENT			96
// MIDI_CC_DATA_DECREMENT			97
// MIDI_CC_NRPN_LSB					98
// MIDI_CC_NRPN_MSB					99
// MIDI_CC_RPN_LSB					100
// MIDI_CC_RPN_MSB					101
#define MIDI_CC_DATA_INCREMENT		0x60
#define MIDI_CC_DATA_DECREMENT		0x61
#define MIDI_CC_NRPN_LSB			0x62
#define MIDI_CC_NRPN_MSB			0x63
#define MIDI_CC_RPN_LSB				0x64
#define MIDI_CC_RPN_MSB				0x65

// 102..119	Undefined

#ifdef _DEBUG_C
	#pragma mark -
#endif
// _______ Channel Controllers _______
// MIDI_CC_ALL_SOUND_OFF			120
// MIDI_CC_RESET					121
// MIDI_CC_LOCAL					122
// MIDI_CC_ALL_NOTES_OFF			123
// MIDI_CC_OMNI_OFF					124
// MIDI_CC_OMNI_ON					125
// MIDI_CC_POLY_ONOFF				126
// MIDI_CC_POLY_ON					127
#define MIDI_CC_ALL_SOUND_OFF		0x78
#define MIDI_CC_RESET				0x79
#define MIDI_CC_LOCAL				0x7A
#define MIDI_CC_ALL_NOTES_OFF		0x7B
#define MIDI_CC_OMNI_OFF			0x7C
#define MIDI_CC_OMNI_ON				0x7D
#define MIDI_CC_POLY_ONOFF			0x7E
#define MIDI_CC_POLY_ON				0x7F


// **************************** //
// ** System Common Messages ** //
// **************************** //
#ifdef _DEBUG_C
	#pragma mark -
	#pragma mark System Common Messages
#endif

// MIDI_SYSEX:						240
#define MIDI_SYSEX					0xF0

// MIDI_TIMECODE:					241
// MIDI_SONG_POSITION_POINTER		242
// MIDI_SONG_SELECT					243
// MIDI_TUNE_REQUEST				246
// MIDI_EOX (END OF SYSEX)			247
#define MIDI_TIMECODE				0xF1
#define MIDI_SONG_POSITION_POINTER	0xF2
#define MIDI_SONG_SELECT			0xF3
#define MIDI_TUNE_REQUEST			0xF6
#define MIDI_EOX					0xF7


// ******************************* //
// ** System Real-Time Messages ** //
// ******************************* //
#ifdef _DEBUG_C
	#pragma mark -
	#pragma mark System Real-Time Messages
#endif

// MIDI_CLOCK						248
#define MIDI_CLOCK					0xF8

// MIDI_START						250
// MIDI_CONTINUE					251
// MIDI_STOP						252
// MIDI_ACTIVE_SENSING				254
// MIDI_RESET						255
#define MIDI_START					0xFA
#define MIDI_CONTINUE				0xFB
#define MIDI_STOP					0xFC
#define MIDI_ACTIVE_SENSING			0xFE
#define MIDI_RESET					0xFF



// ****************** //
// ** NOTE LENGHTS ** //
// ****************** //
#ifdef _DEBUG_C
	#pragma mark -
	#pragma mark Note Lengths
#endif

#define MIDI_NOTE_LENGTH_0			0
#define MIDI_NOTE_LENGTH_1			96
#define MIDI_NOTE_LENGTH_2			48
#define MIDI_NOTE_LENGTH_3			36
#define MIDI_NOTE_LENGTH_4			24
#define MIDI_NOTE_LENGTH_6			18
#define MIDI_NOTE_LENGTH_8			12
#define MIDI_NOTE_LENGTH_12			9
#define MIDI_NOTE_LENGTH_16			6
#define MIDI_NOTE_LENGTH_32			3



// **************************** //
// ** BASE NOTES FOR HARMONY ** //
// **************************** //
#ifdef _DEBUG_C
	#pragma mark -
	#pragma mark Base Notes
#endif

#define MIDI_NOTE_C					0
#define MIDI_NOTE_CIS				1
#define MIDI_NOTE_D					2
#define MIDI_NOTE_DIS				3
#define MIDI_NOTE_E					4
#define MIDI_NOTE_F					5
#define MIDI_NOTE_FIS				6
#define MIDI_NOTE_G					7
#define MIDI_NOTE_GIS				8
#define MIDI_NOTE_A					9
#define MIDI_NOTE_AIS				10
#define MIDI_NOTE_H					11




// **************************** //
// ** NOTE 2 FREQUENCY TABLE ** //
// **************************** //
#ifdef _DEBUG_C
	#pragma mark -
	#pragma mark Note2Freq Table
#endif


// FREQUENCY (in Hertz) = 440 * 2 ^ ( (OCTAVE - 4) + ((TON - 10) / 12) )


									//	octave -5
#define MIDI_NOTE_0_FREQ	8		//	8.1757989156		-6900.00
#define MIDI_NOTE_1_FREQ	9		//	8.6619572180		-6800.00
#define MIDI_NOTE_2_FREQ	9		//	9.1770239974		-6700.00
#define MIDI_NOTE_3_FREQ	10		//	9.7227182413		-6600.00
#define MIDI_NOTE_4_FREQ	10		//	10.3008611535		-6500.00
#define MIDI_NOTE_5_FREQ	11		//	10.9133822323		-6400.00
#define MIDI_NOTE_6_FREQ	12		//	11.5623257097		-6300.00
#define MIDI_NOTE_7_FREQ	12		//	12.2498573744		-6200.00
#define MIDI_NOTE_8_FREQ	13		//	12.9782717994		-6100.00
#define MIDI_NOTE_9_FREQ	13		//	13.7500000000		-6000.00
#define MIDI_NOTE_10_FREQ	15		//	14.5676175474		-5900.00
#define MIDI_NOTE_11_FREQ	15		//	15.4338531643		-5800.00
									//		octave -4
#define MIDI_NOTE_12_FREQ	16		//	16.3515978313		-5700.00
#define MIDI_NOTE_13_FREQ	17		//	17.3239144361		-5600.00
#define MIDI_NOTE_14_FREQ	18		//	18.3540479948		-5500.00
#define MIDI_NOTE_15_FREQ	19		//	19.4454364826		-5400.00
#define MIDI_NOTE_16_FREQ	21		//	20.6017223071		-5300.00
#define MIDI_NOTE_17_FREQ	22		//	21.8267644646		-5200.00
#define MIDI_NOTE_18_FREQ	23		//	23.1246514195		-5100.00
#define MIDI_NOTE_19_FREQ	24		//	24.4997147489		-5000.00
#define MIDI_NOTE_20_FREQ	26		//	25.9565435987		-4900.00
#define MIDI_NOTE_21_FREQ	27		//	27.5000000000		-4800.00
#define MIDI_NOTE_22_FREQ	29		//	29.1352350949		-4700.00
#define MIDI_NOTE_23_FREQ	31		//	30.8677063285		-4600.00
									//		octave -3
#define MIDI_NOTE_24_FREQ	33		//	32.7031956626		-4500.00
#define MIDI_NOTE_25_FREQ	35		//	34.6478288721		-4400.00
#define MIDI_NOTE_26_FREQ	37		//	36.7080959897		-4300.00
#define MIDI_NOTE_27_FREQ	39		//	38.8908729653		-4200.00
#define MIDI_NOTE_28_FREQ	41		//	41.2034446141		-4100.00
#define MIDI_NOTE_29_FREQ	44		//	43.6535289291		-4000.00
#define MIDI_NOTE_30_FREQ	46		//	46.2493028390		-3900.00
#define MIDI_NOTE_31_FREQ	49		//	48.9994294977		-3800.00
#define MIDI_NOTE_32_FREQ	52		//	51.9130871975		-3700.00
#define MIDI_NOTE_33_FREQ	55		//	55.0000000000		-3600.00
#define MIDI_NOTE_34_FREQ	58		//	58.2704701898		-3500.00
#define MIDI_NOTE_35_FREQ	62		//	61.7354126570		-3400.00
									//		octave -2
#define MIDI_NOTE_36_FREQ	65		//	65.4063913251		-3300.00
#define MIDI_NOTE_37_FREQ	69		//	69.2956577442		-3200.00
#define MIDI_NOTE_38_FREQ	73		//	73.4161919794		-3100.00
#define MIDI_NOTE_39_FREQ	78		//	77.7817459305		-3000.00
#define MIDI_NOTE_40_FREQ	82		//	82.4068892282		-2900.00
#define MIDI_NOTE_41_FREQ	87		//	87.3070578583		-2800.00
#define MIDI_NOTE_42_FREQ	92		//	92.4986056779		-2700.00
#define MIDI_NOTE_43_FREQ	98		//	97.9988589954		-2600.00
#define MIDI_NOTE_44_FREQ	104		//	103.8261743950		-2500.00
#define MIDI_NOTE_45_FREQ	110		//	110.0000000000		-2400.00
#define MIDI_NOTE_46_FREQ	116		//	116.5409403795		-2300.00
#define MIDI_NOTE_47_FREQ	123		//	123.4708253140		-2200.00
									//		octave -1
#define MIDI_NOTE_48_FREQ	131		//	130.8127826503		-2100.00
#define MIDI_NOTE_49_FREQ	139		//	138.5913154884		-2000.00
#define MIDI_NOTE_50_FREQ	147		//	146.8323839587		-1900.00
#define MIDI_NOTE_51_FREQ	156		//	155.5634918610		-1800.00
#define MIDI_NOTE_52_FREQ	165		//	164.8137784564		-1700.00
#define MIDI_NOTE_53_FREQ	175		//	174.6141157165		-1600.00
#define MIDI_NOTE_54_FREQ	185		//	184.9972113558		-1500.00
#define MIDI_NOTE_55_FREQ	196		//	195.9977179909		-1400.00
#define MIDI_NOTE_56_FREQ	208		//	207.6523487900		-1300.00
#define MIDI_NOTE_57_FREQ	220		//	220.0000000000		-1200.00
#define MIDI_NOTE_58_FREQ	233		//	233.0818807590		-1100.00
#define MIDI_NOTE_59_FREQ	247		//	246.9416506281		-1000.00
									//		octave 0
#define MIDI_NOTE_60_FREQ	262		//	261.6255653006		-900.00
#define MIDI_NOTE_61_FREQ	277		//	277.1826309769		-800.00
#define MIDI_NOTE_62_FREQ	294		//	293.6647679174		-700.00
#define MIDI_NOTE_63_FREQ	311		//	311.1269837221		-600.00
#define MIDI_NOTE_64_FREQ	330		//	329.6275569129		-500.00
#define MIDI_NOTE_65_FREQ	349		//	349.2282314330		-400.00
#define MIDI_NOTE_66_FREQ	370		//	369.9944227116		-300.00
#define MIDI_NOTE_67_FREQ	392		//	391.9954359817		-200.00
#define MIDI_NOTE_68_FREQ	415		//	415.3046975799		-100.00
#define MIDI_NOTE_69_FREQ	440		//	440.0000000000		0.00
#define MIDI_NOTE_70_FREQ	466		//	466.1637615181		100.00
#define MIDI_NOTE_71_FREQ	494		//	493.8833012561		200.00
									//		octave 1
#define MIDI_NOTE_72_FREQ	523		//	523.2511306012		300.00
#define MIDI_NOTE_73_FREQ	554		//	554.3652619537		400.00
#define MIDI_NOTE_74_FREQ	587		//	587.3295358348		500.00
#define MIDI_NOTE_75_FREQ	622		//	622.2539674442		600.00
#define MIDI_NOTE_76_FREQ	659		//	659.2551138257		700.00
#define MIDI_NOTE_77_FREQ	698		//	698.4564628660		800.00
#define MIDI_NOTE_78_FREQ	740		//	739.9888454233		900.00
#define MIDI_NOTE_79_FREQ	784		//	783.9908719635		1000.00
#define MIDI_NOTE_80_FREQ	831		//	830.6093951599		1100.00
#define MIDI_NOTE_81_FREQ	880		//	880.0000000000		1200.00
#define MIDI_NOTE_82_FREQ	932		//	932.3275230362		1300.00
#define MIDI_NOTE_83_FREQ	988		//	987.7666025122		1400.00
									//		octave 2
#define MIDI_NOTE_84_FREQ	1046	//	1046.5022612024		1500.00
#define MIDI_NOTE_85_FREQ	1109	//	1108.7305239075		1600.00
#define MIDI_NOTE_86_FREQ	1175	//	1174.6590716696		1700.00
#define MIDI_NOTE_87_FREQ	1244	//	1244.5079348883		1800.00
#define MIDI_NOTE_88_FREQ	1318	//	1318.5102276515		1900.00
#define MIDI_NOTE_89_FREQ	1397	//	1396.9129257320		2000.00
#define MIDI_NOTE_90_FREQ	1480	//	1479.9776908465		2100.00
#define MIDI_NOTE_91_FREQ	1568	//	1567.9817439270		2200.00
#define MIDI_NOTE_92_FREQ	1661	//	1661.2187903198		2300.00
#define MIDI_NOTE_93_FREQ	1760	//	1760.0000000000		2400.00
#define MIDI_NOTE_94_FREQ	1865	//	1864.6550460724		2500.00
#define MIDI_NOTE_95_FREQ	1975	//	1975.5332050245		2600.00
									//		octave 3
#define MIDI_NOTE_96_FREQ	2093	//	2093.0045224048		2700.00
#define MIDI_NOTE_97_FREQ	2217	//	2217.4610478150		2800.00
#define MIDI_NOTE_98_FREQ	2349	//	2349.3181433393		2900.00
#define MIDI_NOTE_99_FREQ	2489	//	2489.0158697766		3000.00
#define MIDI_NOTE_100_FREQ	2637	//	2637.0204553030		3100.00
#define MIDI_NOTE_101_FREQ	2794	//	2793.8258514640		3200.00
#define MIDI_NOTE_102_FREQ	2960	//	2959.9553816931		3300.00
#define MIDI_NOTE_103_FREQ	3136	//	3135.9634878540		3400.00
#define MIDI_NOTE_104_FREQ	3322	//	3322.4375806396		3500.00
#define MIDI_NOTE_105_FREQ	3520	//	3520.0000000000		3600.00
#define MIDI_NOTE_106_FREQ	3729	//	3729.3100921447		3700.00
#define MIDI_NOTE_107_FREQ	3951	//	3951.0664100490		3800.00
									//		octave 4
#define MIDI_NOTE_108_FREQ	4186	//	4186.0090448096		3900.00
#define MIDI_NOTE_109_FREQ	4435	//	4434.9220956300		4000.00
#define MIDI_NOTE_110_FREQ	4699	//	4698.6362866785		4100.00
#define MIDI_NOTE_111_FREQ	4978	//	4978.0317395533		4200.00
#define MIDI_NOTE_112_FREQ	5274	//	5274.0409106059		4300.00
#define MIDI_NOTE_113_FREQ	5588	//	5587.6517029281		4400.00
#define MIDI_NOTE_114_FREQ	5920	//	5919.9107633862		4500.00
#define MIDI_NOTE_115_FREQ	6272	//	6271.9269757080		4600.00
#define MIDI_NOTE_116_FREQ	6645	//	6644.8751612791		4700.00
#define MIDI_NOTE_117_FREQ	7040	//	7040.0000000000		4800.00
#define MIDI_NOTE_118_FREQ	7459	//	7458.6201842894		4900.00
#define MIDI_NOTE_119_FREQ	7902	//	7902.1328200980		5000.00
									//		octave 5
#define MIDI_NOTE_120_FREQ	8372	//	8372.0180896192		5100.00
#define MIDI_NOTE_121_FREQ	8870	//	8869.8441912599		5200.00
#define MIDI_NOTE_122_FREQ	9397	//	9397.2725733570		5300.00
#define MIDI_NOTE_123_FREQ	9956	//	9956.0634791066		5400.00
#define MIDI_NOTE_124_FREQ	10548	//	10548.0818212118	5500.00
#define MIDI_NOTE_125_FREQ	11175	//	11175.3034058561	5600.00
#define MIDI_NOTE_126_FREQ	11840	//	11839.8215267723	5700.00
#define MIDI_NOTE_127_FREQ	12544	//	12543.8539514160	5800.00




#endif /* _ACMIDIDEFINES_H */
