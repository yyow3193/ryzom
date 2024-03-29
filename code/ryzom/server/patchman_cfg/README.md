
shard_ctrl_definitions.txt: Contains all macros for various shard services and shard configurations.

shard_ctrl_mini01.txt: Example configuration for a development domain with a single mainland and a single ring shard running on one machine.

terminal_mini01: Contains the terminal to control the patch managers of the mini01 domain. To deploy the shard configuration, install the patchman services on all services, run the terminal and hit Deploy. You may need to hit Deploy a second time if it gives an error. To install the patch version 1, run 'terminal.install mini01 1', this can be done while a previous version is still running. To launch the new version, stop the shard, then run 'terminal.launch mini01 1', this will swap the live version with the next version, and launch the shard immediately.

shard_ctrl_std01.txt: Example configuration for a full blown domain with multiple shards.

terminal_std01: Contains the terminal to control the patch managers of the mini01 domain.

default: Contains base configuration files of the services containing per-service non-domain non-shard specific values.

cfg: Contains base configuration files with domain and shard type specific values.

admin_install: Contains the scripts to launch the patch manager and the shard. This directory is built into admin_install.tgz by the build pipeline. Subdirectory patchman requires addition of the ryzom_patchman_service executable on the server, the build pipeline adds this file into the tgz archive automatically, do not add it manually. The patchman_service_local.cfg file must be installed manually per server to contain the hostname of the server. The contents of the admin_install.tgz must be installed manually to the server the first time a server is deployed. The working directory is assumed to be /srv/core, which will contain /srv/core/bin and /srv/core/patchman. The configurations under patchman must be modified to match your own domains. Launch /srv/core/bin/startup to launch the patchman services. Run '/srv/core/bin/admin stop' to stop the patchman services. There is one bridge server, which is tied to one domain, but is used by the other domains as well. The bridge server has a folder /srv/core/bridge_server, which is generated by the build pipeline when creating a new server patch.