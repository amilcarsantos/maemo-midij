/****************************************************************/
/*      Router-scripts MIDI controller script v1.00             */
/*          Copyright (C) 2011, Amilcar Santos                  */
/*      but feel free to tweak this to your heart's content!    */
/*      For MidiJ version 0.1.x                                 */
/****************************************************************/
function Router() {}

//----------   Customization variables ----------
Router.debug = false;


Router.channelDeckA = 0x7;	// channel 8
Router.channelDeckB = 0x8;	// channel 9


Router.loopModeControls = { 0x48: 0x24,  0x4A: 0x24,  0x4C: 0x26,  0x4E: 0x26,
                            0x4F: 0x25,  0x51: 0x25,  0x53: 0x27,  0x55: 0x27};

Router.trigModeControls = { 0x48: 0x34,  0x4A: 0x34,  0x4C: 0x36,  0x4E: 0x36,
                            0x4F: 0x35,  0x51: 0x35,  0x53: 0x37,  0x55: 0x37};


Router.init = function(device) {
	if (Router.debug) print("Router.init() - MidiJ Version", engine.getValue("[MidiJ]", "version"));
}

Router.noteOnOff = function (status, value, byte2, channel) {
	var byte1 = 0;
	if ((status & 0xF0) == 0x90)
		byte1 = 0x90 + channel;
	else
		byte1 = 0x80 + channel;
	// send to PC
	gateway.midi.sendShortMsg(byte1, byte2, value);
}

Router.noteOnOffDeck = function (status, value, byte2) {
	var channel = 0;
	if (StantonSCS3d.deck == 1) {
		if (Router.debug) print("Router.noteOnOffDeck: processing deck A");
		channel = Router.channelDeckA;
	} else {
		if (Router.debug) print("Router.noteOnOffDeck: processing deck B");
		channel = Router.channelDeckB;
	}
	var byte1 = 0;
	if ((status & 0xF0) == 0x90)
		byte1 = 0x90 + channel;
	else
		byte1 = 0x80 + channel;
	// send to PC
	gateway.midi.sendShortMsg(byte1, byte2, value);
}


Router.noteOnOffDeck = function (status, value, byte2DeckA, byte2DeckB) {
	var byte2 = 0;
	var channel = 0;
	if (StantonSCS3d.deck == 1) {
		if (Router.debug) print("Router.noteOnOffDeck: processing deck A");
		channel = Router.channelDeckA;
		byte2 = byte2DeckA;
	} else {
		if (Router.debug) print("Router.noteOnOffDeck: processing deck B");
		channel = Router.channelDeckB;
		byte2 = byte2DeckB;
	}
	var byte1 = 0;
	if ((status & 0xF0) == 0x90)
		byte1 = 0x90 + channel;
	else
		byte1 = 0x80 + channel;
	// send to PC
	gateway.midi.sendShortMsg(byte1, byte2, value);
}

Router.controlChangeDeck = function (validMode, value, byte2) {
	if (validMode != "") {
	    if (StantonSCS3d.currentDeckMode() != validMode)
	    	return;
	}
	var channel = 0;
	if (StantonSCS3d.deck == 1) {
		if (Router.debug) print("Router.controlChangeDeck: processing deck A");
		channel = Router.channelDeckA;
	} else {
		if (Router.debug) print("Router.controlChangeDeck: processing deck B");
		channel = Router.channelDeckB;
	}
	var byte1 = 0xB0 + channel;
	// send to PC
	gateway.midi.sendShortMsg(byte1, byte2, value);
}

Router.B11 = function (channel, control, value, status) {
	var byte2 = 0;
	switch (StantonSCS3d.currentDeckMode())
	{
	case "fx":
		byte2 = 0x0;
		break;
	case "eq":
		byte2 = 0x10;
		break;
	case "loop":
	case "loop2":
	case "loop3":
		byte2 = 0x20;
		break;
	case "trig":
	case "trig2":
	case "trig3":
		byte2 = 0x30;
		break;
	case "vinyl":
	case "vinyl2":
	case "vinyl3":
		byte2 = 0x40;
		break;
	default:
		return;
	}
	Router.noteOnOffDeck(status, value, byte2);
}

Router.B12 = function (channel, control, value, status) {
	var byte2 = 0;
	switch (StantonSCS3d.currentDeckMode())
	{
	case "fx":
		byte2 = 0x01;
		break;
	case "eq":
		byte2 = 0x11;
		break;
	case "loop":
	case "loop2":
	case "loop3":
		byte2 = 0x21;
		break;
	case "trig":
	case "trig2":
	case "trig3":
		byte2 = 0x31;
		break;
	case "vinyl":
	case "vinyl2":
	case "vinyl3":
		byte2 = 0x41;
		break;
	default:
		return;
	}
	Router.noteOnOffDeck(status, value, byte2);
}

Router.B13 = function (channel, control, value, status) {
	var byte2 = 0;
	switch (StantonSCS3d.currentDeckMode())
	{
	case "fx":
		byte2 = 0x02;
		break;
	case "eq":
		byte2 = 0x12;
		break;
	case "loop":
	case "loop2":
	case "loop3":
		byte2 = 0x22;
		break;
	case "trig":
	case "trig2":
	case "trig3":
		byte2 = 0x32;
		break;
	case "vinyl":
	case "vinyl2":
	case "vinyl3":
		byte2 = 0x42;
		break;
	default:
		return;
	}
	Router.noteOnOffDeck(status, value, byte2);
}

Router.B14 = function (channel, control, value, status) {
	var byte2 = 0;
	switch (StantonSCS3d.currentDeckMode())
	{
	case "fx":
		byte2 = 0x03;
		break;
	case "eq":
		byte2 = 0x13;
		break;
	case "loop":
	case "loop2":
	case "loop3":
		byte2 = 0x23;
		break;
	case "trig":
	case "trig2":
	case "trig3":
		byte2 = 0x33;
		break;
	case "vinyl":
	case "vinyl2":
	case "vinyl3":
		byte2 = 0x43;
		break;
	default:
		return;
	}
	Router.noteOnOffDeck(status, value, byte2);
}



Router.playButton = function (channel, control, value, status) {
	Router.noteOnOffDeck(status, value, 0x6C);
}

Router.cueButton = function (channel, control, value, status) {
	Router.noteOnOffDeck(status, value, 0x6D);
}

Router.syncButton = function (channel, control, value, status) {
	Router.noteOnOffDeck(status, value, 0x6E);
}

Router.tapButton = function (channel, control, value, status) {
	Router.noteOnOffDeck(status, value, 0x6F);
}


Router.gain = function (channel, control, value, status) {
	Router.controlChangeDeck("", value, 0x60);
}

Router.pitch = function (channel, control, value, status) {
	Router.controlChangeDeck("", value, 0x61);
}



Router.S3absolute = function (channel, control, value, status) {
	var byte2 = 0;
	switch (StantonSCS3d.currentDeckMode())
	{
	case "fx":
		byte2 = 0x02;
		break;
	case "eq":
		byte2 = 0x12;
		break;
	default:
		return;
	}
	Router.controlChangeDeck("", value, byte2);
}

Router.S4absolute = function (channel, control, value, status) {
	var byte2 = 0;
	switch (StantonSCS3d.currentDeckMode())
	{
	case "fx":
		byte2 = 0x03;
		break;
	case "eq":
		byte2 = 0x13;
		break;
	case "vinyl":
	case "vinyl2":
	case "vinyl3":
		byte2 = 0x4D;
		break;
	default:
		return;
	}
	Router.controlChangeDeck("", value, byte2);
}

Router.S5absolute = function (channel, control, value, status) {
	var byte2 = 0;
	switch (StantonSCS3d.currentDeckMode())
	{
	case "fx":
		byte2 = 0x04;
		break;
	case "eq":
		byte2 = 0x14;
		break;
	default:
		return;
	}
	Router.controlChangeDeck("", value, byte2);
}


/* XXX removed
Router.B1touch = function (channel, control, value, status) {
	if (StantonSCS3d.currentDeckMode() == "eq") {
		// EQ mode
		Router.noteOnOffDeck(status, value, "Play Reverse A", "Play Reverse B");
	}
}
*/

Router.B2touch = function (channel, control, value, status) {
	var byte2 = 0;
	switch (StantonSCS3d.currentDeckMode()) {
	case "loop":
	case "loop2":
	case "loop3":
		byte2 = 0x28;
		break;
	case "trig":
	case "trig2":
	case "trig3":
		byte2 = 0x38;
		break;
	case "vinyl":
	case "vinyl2":
	case "vinyl3":
		byte2 = 0x4D;
		break;
	default:
		return;
	}
	Router.noteOnOffDeck(status, value, byte2);
}


/* XXX remove this
Router.B3touch = function (channel, control, value, status) {
	if (StantonSCS3d.currentDeckMode() == "eq") {
		// EQ mode
		Router.noteOnOffDeck(status, value, "Play Forward A", "Play Forward B");
	}
}
*/


Router.SurfaceButton = function (channel, control, value, status) {
	var currentMode = StantonSCS3d.currentDeckMode()
	
	if (currentMode.substring(0,4) != "loop" &&
		currentMode.substring(0,4) != "trig")
		return;

	var index = currentMode.charAt(currentMode.length-1);
	if (index != "2" && index != "3") index = "1";

	var byte2 = 0;
	if (currentMode.substring(0,4) == "loop")
		byte2 = Router.loopModeControls[control];
	else
		byte2 = Router.trigModeControls[control];

	// send to PC
	Router.noteOnOffDeck(status, value, byte2);
}


Router.C1touch = function (channel, control, value, status) {
	Router.noteOnOffDeck(status, value, 0x4c);
}

Router.C1Absolute = function (channel, control, value, status) {
	// TODO transform to RELATIVE
}

