task default: :server

desc "Start the server"
task :server => :build do
  sh "irb -r./music_server"
end

desc "Build the VST"
task :build do
  Dir.chdir("vst") do
    sh "make"
  end
end
