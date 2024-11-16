
desc "devcontainer related commands"
namespace "dev" do

  desc "starts the dev container"
  task :start do
    sh "devcontainer up --workspace-folder ."
  end

  desc "stops the dev container"
  task :stop do
    sh "docker stop $(docker ps | grep vsc-heinerbloch | awk '{print $1}')"
  end

  task :remove do
    sh "docker remove $(docker ps -a | grep vsc-heinerbloch | awk '{print $1}')"
  end

  desc "enters the dev container"
  task :enter do
    sh "devcontainer exec --workspace-folder . bash"
  end

end

build_directory = "build"

desc "build the firmware"
task "build" do
    if not File.directory?(build_directory)
        Dir.mkdir(build_directory)
    end

    Dir.chdir(build_directory) do
        sh "cmake -D PICO_SDK_PATH=/opt/pico-sdk/ -D PICO_PLATFORM=rp2350 -D PICO_BOARD=solderparty_rp2350_stamp_xl -G Ninja .."
        sh "ninja"
    end
end

desc "flash firmware to pico via debug probe"
task "flash" do
    sh "openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg -c \"adapter speed 5000\" -c \"program #{build_directory}/heinerbloch.elf verify reset exit\""
end
