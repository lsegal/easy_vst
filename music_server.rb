require 'drb/drb'
srand

SINE     = -> { Math.sin(2 * Math::PI * @step) }
SQUARE   = -> { @step < 0.5 ? -1 : 1 }
TRIANGLE = -> { 1 - 4 * (@step.round - @step).abs }
SAW      = -> { 2 * (@step - @step.round) }
NOISE    = -> { rand - 0.5 }

EXP_FALLOFF  = -> { [(1 / (@note_frame * 0.002)), 1.0].min }
EXP_FALLOFF2 = -> { [(1 / (@note_frame * 0.005)), 1.0].min }
LIN_FALLOFF  = -> { (50000.0 - @note_frame) / 50000.0 }

SNARE = -> { e(EXP_FALLOFF) * e(NOISE) }
BASSDRUM = -> { e(EXP_FALLOFF2) * e(NOISE) * 0.1 + e(SINE) * 0.9 * e(EXP_FALLOFF) }

OCTAVE = -> { e(SINE) * 0.5 + f(SINE,note+12) * 0.5 }
TRIAD = -> { e(SINE) * 0.3 + f(SINE,note+3) * 0.3 + f(SINE,note+5) * 0.3 }

PHASED = -> { fr(SINE, Math.sin(@step / 4.0) * (freq / 2.0)) }

class MusicServer
  def initialize
    @file = $DEBUG ? File.open("out.log", "w") : nil
    @note_on = false

    @sample_rate = 48000
    @freq = 0
    @amp = 0.6

    @fn = SINE
    @step = 0
    @frame = 0
    @note_frame = 0
    @note = 0
  end

  attr_accessor :fn, :freq, :amp, :sample_rate, :step, :note_frame, :note

  def log(*args)
    return unless @file
    @file.puts "* #{Time.now}: #{args.join(" ")}"
  end

  def midi_note(status, note, velocity)
    log __method__, status, note, velocity
    @freq = freq_for_note(note)
    @note = note
    @note_frame = 0
    @note_on = status == 0x90 && velocity > 0
  end

  def freq_for_note(note)
    2.0 ** ((note-49.0)/12.0) * 440.0
  end

  def midi_all_off
    log __method__
    @note_on = false
  end

  def process(samples)
    return if !@fn || !@note_on

    result = Array.new(samples)
    samples.times do |i|
      calculate_step
      result[i] = @amp.to_f * instance_exec(&@fn)
      @frame += 1
      @note_frame += 1
    end

    log __method__, samples, result
    result
  end

  def calculate_step
    @step = (@frame * @freq / @sample_rate.to_f) % 1.0
  end

  def map(samples, &block)
    Array.new(samples).map(&block)
  end
end

def e(fn = nil, &block)
  $music.send(:instance_exec, &(fn || block))
end

def f(fn = nil, note, &block)
  orig_freq, orig_step = $music.freq, $music.step
  $music.freq = freq_for_note(note)
  $music.calculate_step
  result = e(fn || block)
  $music.freq = orig_freq
  $music.step = orig_step
  result
end

def fr(fn = nil, freq, &block)
  orig_freq, orig_step = $music.freq, $music.step
  $music.freq = freq
  $music.calculate_step
  result = e(fn || block)
  $music.freq = orig_freq
  $music.step = orig_step
  result
end

$music = MusicServer.new
DRb.start_service("druby://localhost:9090", $music)
#DRb.thread.join
