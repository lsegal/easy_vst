# EasyVST

A VST Instrument Plug-In that connects to a remote Ruby process via DRb to
process audio samples and MIDI messages.

## Requirements

This project has only been tested on Mac OS X. The `vst/Makefile` will likely
need modifications to build on other systems.

You will also need the [VST3 SDK][sdk] and a a compatible DAW that can use VSTs.

## Using

To build the VST and start the server, copy the VST SDK into `vst/vst3` and
type:

```sh
$ rake
```

This will launch you into a interactive Ruby console where you can control the
`$music` object that is exposed by DRb.

To change the music function, set `$music.fn`. Some examples:

```ruby
# Play a square wave
$music.fn = SQUARE

# Play a square + sine wave
$music.fn = -> { e(SINE) * 0.5 - e(SQUARE) * 0.5 }
```

See the `music_server.rb` file for more functions and examples defined as
constants at the top of the file.

## Remote Server Protocol

A sample server is provided in `music_server.rb`. To implement a custom server,
you must expose an object to the DRb service on port 9090 that supports the
following methods:

### `#process(num_samples) -> Array<Float>`

Synthesizes `num_samples` samples for audio playback. This function should
return an array of `num_samples` floating point values.

### `#midi_note(status, note, velocity) -> void`

Indicates that a MIDI note changed its status. The default server has logic
to handle a single MIDI note being pressed at a time.

### `#midi_all_off -> void`

Indicates that all MIDI messages have been stopped.

## License

EasyVST is licensed under an MIT license and was created by Loren Segal
in 2014. The files under `vst/source` were created from templates provided
by the [teragonaudio/XcodeVstTemplates][teragon] project on GitHub.

[sdk]: http://www.steinberg.net/en/company/3rd_party_developer.html
[teragon]: https://github.com/teragonaudio/XcodeVstTemplates
