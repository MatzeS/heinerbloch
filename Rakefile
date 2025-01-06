# frozen_string_literal: true

desc 'devcontainer related commands'
namespace 'dev' do
  desc 'builds the dev container'
  task :build do
    sh 'devcontainer build   --workspace-folder .'
  end

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

linux_build_directory = 'platform/linux/build'
desc 'build for host'
task 'build:linux' do
  build_directory = linux_build_directory
  Dir.mkdir(build_directory) unless File.directory?(build_directory)

  Dir.chdir(build_directory) do
    sh 'cmake' \
      ' -D CMAKE_EXPORT_COMPILE_COMMANDS=ON' \
      ' -G Ninja ..'
    sh 'ninja'
  end
end

rp2350_build_directory = 'platform/rp2350/build'

desc 'build for rp2350'
task 'build:rp2350' do
  build_directory = rp2350_build_directory
  Dir.mkdir(build_directory) unless File.directory?(build_directory)

  Dir.chdir(build_directory) do
    sh 'cmake' \
      ' -D CMAKE_EXPORT_COMPILE_COMMANDS=ON' \
      ' -D PICO_SDK_PATH=/opt/pico-sdk/' \
      ' -D FREERTOS_KERNEL_PATH=/opt/FreeRTOS-Kernel/' \
      ' -G Ninja ..'
    sh 'ninja'
  end
end

desc 'run unit tests'
task 'test:unit' do
  Dir.chdir("#{linux_build_directory}/build-tests") do
    sh 'ctest'
  end
end

desc 'flash firmware to pico via debug probe'
task 'flash' do
  sh 'openocd' \
    ' -f interface/cmsis-dap.cfg' \
    ' -f target/rp2350.cfg' \
    ' -c "adapter speed 5000"' \
    " -c \"program #{rp2350_build_directory}/heinerbloch-rp2350.elf verify reset exit\""
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

namespace 'lint' do
  task :cpp do
    files = `git ls-tree HEAD -r | awk '{ print $4 }'`.split
    files = files.select { |f| f =~ /.*\.(cpp|hpp|c|h)$/ }

    rp2350_files = files.select { |f| f.start_with?('platform/rp2350') }
    standard_files = files - rp2350_files

    sh "clang-tidy -p=platform/linux/build #{standard_files.join(' ')}"
    sh "clang-tidy -p=platform/rp2350/build #{rp2350_files.join(' ')}"
  end
end
