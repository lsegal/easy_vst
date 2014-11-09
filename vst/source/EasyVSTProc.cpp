/*
 *  EasyVST - EasyVSTProc.cpp
 *  Copyright (c) 2014 Loren Segal, All rights reserved
 */

#ifndef __EasyVST_H
#include "EasyVST.h"
#endif

VstInt32 EasyVST::processEvents (VstEvents* ev)
{
    for (VstInt32 i = 0; i < ev->numEvents; i++)
    {
        if ((ev->events[i])->type != kVstMidiType)
            continue;
        
        VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
        char* midiData = event->midiData;
        VstInt32 status = midiData[0] & 0xF0;	// ignoring channel
        if (status == 0x90 || status == 0x80)	// we only look at notes
        {
            VstInt32 velocity = midiData[2];
            VstInt32 note = midiData[1];
            rb_funcall(server, rb_intern("midi_note"), 3,
                       INT2FIX(status), INT2FIX(note), INT2FIX(velocity));
        }
        else if (status == 0xB0)
        {
            rb_funcall(server, rb_intern("midi_all_off"), 0);
        }
        event++;
    }
    return 1;
}

struct callProcessStruct {
    VALUE object;
    VALUE arg;
};

static VALUE callProcess(VALUE payload)
{
    struct callProcessStruct *info = (struct callProcessStruct *)payload;
    return rb_funcall(info->object, rb_intern("process"), 1, info->arg);
}

void EasyVST::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames)
{
    float *left = outputs[0];
    float *right = outputs[1];
    int status = 0;
    
    struct callProcessStruct info;
    info.object = server;
    info.arg = INT2FIX(sampleFrames);
    VALUE result = rb_protect(callProcess, (VALUE)&info, &status);

    if (status == 0 && rb_type(result) == T_ARRAY)
    {
        int result_len = RARRAY_LEN(result);
        for (int i = 0; i < sampleFrames; i++)
        {
            float v = 0.0f;
            if (i < result_len) v = (float)NUM2DBL(rb_ary_entry(result, i));
            left[i] = right[i] = v;
        }
    }
    else
    {
        memset(left, 0, sampleFrames * sizeof(float));
        memset(right, 0, sampleFrames * sizeof(float));
    }
}