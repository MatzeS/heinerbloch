# frozen_string_literal: true

desc 'devcontainer related commands'
namespace 'dev' do
  desc 'starts the dev container'
  task :start do
    sh 'devcontainer up --workspace-folder .'
  end

  desc 'stops the dev container'
  task :stop do
    sh "docker stop $(docker ps | grep vsc-heinerbloch | awk '{print $1}')"
  end

  task :remove do
    sh "docker remove $(docker ps -a | grep vsc-heinerbloch | awk '{print $1}')"
  end

  desc 'enters the dev container'
  task :enter do
    sh 'devcontainer exec --workspace-folder . bash'
  end
end

build_directory = 'build'

desc 'build the firmware'
task 'build' do
  Dir.mkdir(build_directory) unless File.directory?(build_directory)

  Dir.chdir(build_directory) do
    sh 'cmake' \
      ' -D PICO_SDK_PATH=/opt/pico-sdk/' \
      ' -D PICO_PLATFORM=rp2350' \
      ' -D PICO_BOARD=solderparty_rp2350_stamp_xl' \
      ' -D CMAKE_EXPORT_COMPILE_COMMANDS=ON' \
      ' -G Ninja ..'
    sh 'ninja'
  end
end

desc 'flash firmware to pico via debug probe'
task 'flash' do
  sh 'openocd' \
    ' -f interface/cmsis-dap.cfg' \
    ' -f target/rp2350.cfg' \
    ' -c "adapter speed 5000"' \
    " -c \"program #{build_directory}/heinerbloch.elf verify reset exit\""
end

namespace 'setup' do
  task :udev_rules do
    rules_file = '/etc/udev/rules.d/pico-debug-probe.rules'
    if File.exist?(rules_file)
      puts "#{rules_file} file already exists. Not changing any files."
    else
      File.open(rules_file, 'w') do |f|
        f.write('SUBSYSTEM=="usb", ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="000c", MODE="0666"')
      end
    end
    sh 'udevadm control --reload-rules'
    sh 'udevadm trigger'
  end
end
