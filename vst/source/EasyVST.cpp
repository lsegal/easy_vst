/*
 *  EasyVST - EasyVST.cpp
 *  Copyright (c) 2014 Loren Segal, All rights reserved
 */

#ifndef __EasyVST_H
#include "EasyVST.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {
    ruby_init();
    ruby_init_loadpath();
    rb_gc_disable();
    rb_require("drb/drb");
    rb_eval_string("DRb.start_service");

    return new EasyVST(audioMaster);
}

EasyVST::EasyVST(audioMasterCallback audioMaster)
: AudioEffectX(audioMaster, kNumPrograms, kNumParameters) {
    if(audioMaster != NULL) {
        setNumInputs(kNumInputs);
        setNumOutputs(kNumOutputs);
        setUniqueID(kUniqueId);
        canProcessReplacing();
        isSynth();
    }

    server = rb_eval_string("DRbObject.new_with_uri('druby://localhost:9090')");

    suspend();
}

EasyVST::~EasyVST() {
}

VstInt32 EasyVST::canDo(char *text) {
    // TODO: Fill in according to your plugin's capabilities
    if(!strcmp(text, "receiveVstEvents")) {
		return 1;
    }
    else if(!strcmp(text, "receiveVstMidiEvent")) {
        return 0;
    }
    else if(!strcmp(text, "midiProgramNames")) {
		return 1;
    }
  
    // -1 => explicitly can't do; 0 => don't know
    return 0;
}

bool EasyVST::copyProgram(long destination) {
    // TODO: Copy program to destination
    return false;
}

VstInt32 EasyVST::getCurrentMidiProgram(VstInt32 channel, MidiProgramName *mpn) {
	VstInt32 prg = 0;
    // TODO: Look up your current MIDI program and fill the MidiProgramName with it
	return prg;
}

bool EasyVST::getEffectName(char* name) {
    strncpy(name, "EasyVST", kVstMaxProductStrLen);
    return true;
}

bool EasyVST::getMidiKeyName(VstInt32 channel, MidiKeyName *key) {
	// TODO: Fill in this information
    // key->thisProgramIndex;		// >= 0. fill struct for this program index.
	// key->thisKeyNumber;			// 0 - 127. fill struct for this key number.
	key->keyName[0] = 0;
	key->reserved = 0;				// zero
	key->flags = 0;					// reserved, none defined yet, zero.
	return true;
}

VstInt32 EasyVST::getMidiProgramCategory(VstInt32 channel, MidiProgramCategory *category) {
    // TODO: Get the MIDI program category
	return 0;
}

VstInt32 EasyVST::getMidiProgramName(VstInt32 channel, MidiProgramName *mpn) {
    // TODO: Return the MIDI program name
    return 0;
}

VstInt32 EasyVST::getNumMidiInputChannels() {
    // TODO: Change this value for polyphonic synths
    return 1;
}

VstInt32 EasyVST::getNumMidiOutputChannels() {
    // TODO: Change this value if you are sending MIDI output back to the host
    return 0;
}

bool EasyVST::getOutputProperties(VstInt32 index, VstPinProperties *properties) {
  if(index < kNumOutputs) {
    sprintf(properties->label, "%1d Out", index + 1);
		properties->flags = kVstPinIsActive;
		if(index < 2) {
			properties->flags |= kVstPinIsStereo;
    }
    return true;
  }
  return false;
}

float EasyVST::getParameter(VstInt32 index) {
  // TODO: Get parameter value for index
  return 0.0;
}

void EasyVST::getParameterDisplay(VstInt32 index, char *text) {
  // TODO: Get parameter display for index
}

void EasyVST::getParameterLabel(VstInt32 index, char *text) {
  // TODO: Get parameter label for index
}

void EasyVST::getParameterName(VstInt32 index, char *text) {
  // TODO: Get parameter name for index
}

VstPlugCategory EasyVST::getPlugCategory() { 
  return kPlugCategSynth;
}

bool EasyVST::getProductString(char* text) {
  // TODO: Replace with actual description of your synth
  strncpy(text, "EasyVST", kVstMaxProductStrLen);
  return true;
}

void EasyVST::getProgramName(char *name) {
  // TODO: Copy active program name into "name" string
}

bool EasyVST::getProgramNameIndexed(VstInt32 category, VstInt32 index, char *text) {
  // TODO: Return program name for index
  return false;
}

bool EasyVST::getVendorString(char* text) {
  strncpy(text, "Gnuu.org", kVstMaxVendorStrLen);
  return true;
}

VstInt32 EasyVST::getVendorVersion() {
  // TODO: Return actual version of this plugin
  return 1000;
}

bool EasyVST::hasMidiProgramsChanged(VstInt32 channel) {
  // TODO: Return true/false if the MIDI programs have changed
  return false;
}

void EasyVST::setBlockSize(VstInt32 blockSize) {
  // TODO: Handle this call if necessary
	AudioEffectX::setBlockSize(blockSize);
}

void EasyVST::setParameter(VstInt32 index, float value) {
  // TODO: Set parameter value for index
}

void EasyVST::setProgram(VstInt32 index) {
  // TOOD: Set local variables based on program parameters
}

void EasyVST::setProgramName(char *name) {
  // TODO: Set program name
}

void EasyVST::setSampleRate(float sampleRate) {
  // TODO: Handle this call if necessary
	AudioEffectX::setSampleRate(sampleRate);
}