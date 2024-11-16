
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
